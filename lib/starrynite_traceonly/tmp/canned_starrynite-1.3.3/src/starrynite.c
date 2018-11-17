/* Copyright (C) 2005-2008 University of Washington
   Written by Zhirong Bao and Dan Blick
   This file is part of starrynite.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */


//  ./run_starrynite_loaderversion.prl C:/images/081505/image/tif/081505_L1  ../parameters-file c:/images/081505/matlabnuclei
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>             /* for basename */
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>

#include "types.h"

#include "parameters-tools.h"
#include "image-manipulation.h"
#include "sliding-box.h"
#include "noise-filter.h"
#include "nuclei.h"
#include "tracing.h"
#include "cell-identity.h"
#include "report-lineage.h"
#include "utility.h"
#include "colors.h"
#include "error-codes.h"

#define LOG_NOISE_CUTOFFS 1

MOVIE_t Movie;
PARAMETER_t Parameters;
TOOL_t Tools;
/*@dependent @*/ FILE *logfile;
uint8_t **backup_stack;
const char *program_name;

void
show_time (const char *const msg)
{
  time_t rawtime;
  struct tm *timeinfo;
  char *time_string;

  rawtime = time (&rawtime);
  assert (rawtime != -1);

  timeinfo = localtime (&rawtime);
  assert (timeinfo != NULL);

  time_string = asctime (timeinfo);
  assert (time_string != NULL);

  (void) printf ("%s: %s", msg, time_string);
}

int
main (const int argc, char *const argv[])
/*@globals: stdout, stderr, errno @*/
{
  /* TODO: modify to remove exit() statements and use a single return
   * statement */
  int time_start, tp_number, t;
  int plane_start, plane_number;
  bool is_first_tiff;
  int cell_ct = 0;
  int i;
  int new_lineage_ct;
  uint32_t image_width;
  uint32_t image_height;
  uint32_t image_size;

  assert (argv[0] != NULL);

  program_name = basename (argv[0]);

  if (argc == 2 && strcmp ("--version", argv[1]) == 0)
  {
    (void) printf ("%s\n", PACKAGE_STRING);
    exit (EXIT_FAILURE);
  }
  else if (argc != 5)
  {
    (void) fprintf (stderr, "Usage: %s <input tiff file prefix> "
                    "<output tiff file prefix> <parameter file> <pre-existing nucleus prefix>\n",
                    program_name);
    exit (ERROR_USAGE);
  }

  show_time ("Start time");

  logfile = fopen ("log", "w");
  if (logfile == NULL)
  {
    (void) fprintf (stderr, "Could not open the log file\n");
    exit (ERROR_LOGFILE);
  }


    /**************************
     * read in the parameters *
     **************************/

  SetParameters (&Movie, &Parameters, argv[3]);
  strncpy (Movie.output_prefix, argv[2], 500);

  time_start = Movie.time_start;
  tp_number = Movie.tp_number;
  plane_start = Movie.plane_start;
  plane_number = Movie.frame.plane_number;

    /****************************************
     * processing images / set noise cutoff *
     ****************************************/

  is_first_tiff = true;

  for (t = 0; t < tp_number; t++)
  {
    Parameters.t = t;
    GetImageStacks (&Movie, &Parameters, &Tools, argv[1], is_first_tiff,
                    &image_width, &image_height);
    image_size = image_width * image_height;

    if (is_first_tiff)
    {
      is_first_tiff = false;
      MakeTools (&Movie, &Parameters, &Tools, image_size);
      SetOriginalFrame (&Movie, &Parameters, image_width, image_height,
                        image_size);

      backup_stack =
        (uint8_t **) malloc_exit (sizeof (uint8_t *) * plane_number);
      for (i = 0; i < plane_number; i++)
      {
        backup_stack[i] =
          (uint8_t *) malloc_exit (sizeof (uint8_t) * image_size);
      }
    }
    CalibrateNoise_3D (&Tools, &Parameters, &Movie, image_width, image_height,
                       image_size, Tools.image_stack, t);

  }
  SmoothingNoiseModel (&Tools, &Parameters, &Movie);


#if LOG_NOISE_CUTOFFS
  (void) fprintf (logfile, "Noise Cutoffs\n\n");
  for (t = 0; t < tp_number; t++)
  {
    (void) fprintf (logfile, "time %2d: \n", Movie.time_start + t);
#if 0
    (void) fprintf (logfile, "max_cutoff: ");
    for (i = 0; i < plane_number; i++)
      (void) fprintf (logfile, " %4d", *(*(Tools.max_cutoff + t) + i));
    (void) fprintf (logfile, "\n");
    (void) fprintf (logfile, "max_ori:    ");
    for (i = 0; i < plane_number; i++)
      (void) fprintf (logfile, " %4d", *(*(Tools.max_cutoff_ori + t) + i));
    (void) fprintf (logfile, "\n");
#endif /* 0 */

    (void) fprintf (logfile, "noise:      ");
    for (i = 0; i < plane_number; i++)
      (void) fprintf (logfile, " %4d", Tools.noise[t][i]);
    (void) fprintf (logfile, "\n");
    (void) fprintf (logfile, "noise_ori:  ");
    for (i = 0; i < plane_number; i++)
      (void) fprintf (logfile, " %4d", Tools.noise_ori[t][i]);
    (void) fprintf (logfile, "\n\n");
  }
  (void) fprintf (logfile, "\n");
  (void) fflush (logfile);
#endif /* LOG_NOISE_CUTOFFS */

    /***************************************
     * processing images / identify nuclei *
     ***************************************/


  for (t = 0; t < tp_number; t++)
  {
    Parameters.t = t;

#if 0
    if (Movie.time_start + t == 150)
      Parameters.nuc_size = 45;
#endif

    if (t != 0)
    {
      UpdateNucSize (&Tools, &Parameters, logfile, t);
    }

    if (Parameters.nuc_size <= Parameters.min_nuc_size)
    {
      (void) fprintf (logfile,
                      "\ntime %d, nuclear size %d less than min nuclear size %d\n",
                      Movie.time_start + t, Parameters.nuc_size,
                      Parameters.min_nuc_size);
      tp_number = t;
      break;
    }

    UpdateTimeInfo (&Parameters, &Movie, &Tools, t);

    CalculateSizeParameters (&Parameters);
    UpdateNoise (&Tools, &Parameters, &Movie, t);

    (void) fprintf (logfile, "time %3d, nuclear size %d",
                    Movie.time_start + t, Parameters.nuc_size);
    (void) fprintf (logfile, ", z_factor %.2f", Parameters.z_factor);
    (void) fprintf (logfile, ", %.2f, %.2f", Parameters.noise_factor,
                    Parameters.nuc_density_cutoff);
    GetImageStacks (&Movie, &Parameters, &Tools, argv[1], is_first_tiff,
                    &image_width, &image_height);

    assert (image_size == image_width * image_height);

    for (i = 0; i < plane_number; i++)
    {
      memcpy (backup_stack[i], Tools.image_stack[i],
              sizeof (uint8_t) * image_size);
    }

    /* filtering noise */
    LocalDensityCut_3D (&Movie, &Parameters, &Tools, image_width,
                        image_height, image_size, Tools.image_stack, t);

  	/*
  DefineNuclei (&Movie, &Parameters, &Tools, backup_stack, logfile,
                  (const uint8_t **) Tools.image_stack, &(Movie.frame),
                  Tools.nuclei_record + t, Tools.nuc_ct_rec + t, t);
    RecoverNuclei (&Movie, &Parameters, &Tools, backup_stack, logfile,
                   (const uint8_t **) Tools.image_stack, &(Movie.frame), t);
	*/


    char filename [ FILENAME_MAX ];
    char filename2 [ FILENAME_MAX ];
    sprintf(filename, "%smatlabnuclei%d", argv[4], t+time_start);
	sprintf(filename2, "%smatlabdiams%d", argv[4], t+time_start);
	


   LoadNuclei (&Movie, &Parameters, &Tools, backup_stack, logfile,
                  (const uint8_t **) Tools.image_stack, &(Movie.frame),
                  Tools.nuclei_record + t, Tools.nuc_ct_rec + t, t,filename,filename2);



    cell_ct = Tools.nuc_ct_rec[t];

    (void) fprintf (logfile, "\n");
    (void) fflush (logfile);
/*
    if (Tools.nuc_ct_rec[t] > 700) //540
    {
      (void) fprintf (logfile,
                      "\ntime %d, %d cells identified.  Approaching second embryonic rotation.  Ending.\n",
                      Movie.time_start + t, Tools.nuc_ct_rec[t]);
      tp_number = t;
      break;
    }
*/
}

  (void) fprintf (logfile, "\nTotal time points %d\n", Movie.tp_number);
  (void) fprintf (logfile, "Last time point processed: %d\n",
                  Movie.time_start + tp_number - 1);
  if (tp_number == 0)
  {
    const char *const msg = "fatal error - tp_number == 0";

    (void) fprintf (stderr, "%s line %d: %s\n", __FILE__, __LINE__, msg);
    exit (ERROR_MAIN1);
  }
  Movie.tp_number = tp_number;




   //release earlier trying to avoid memory issues
  if (Tools.image_stack_prev != NULL)
  {
    for (i = 0; i < plane_number; i++)
      free (Tools.image_stack_prev[i]);
    free (Tools.image_stack_prev);
  }

  for (i = 0; i < plane_number; i++)
    free (Tools.image_stack[i]);
  free (Tools.image_stack);
//added this
  if (backup_stack != NULL)
  {
    for (i = 0; i < plane_number; i++)
      free (backup_stack[i]);
    free (backup_stack);
  }

  CleanUpTools (&Movie, &Parameters, &Tools);


    /*****************
     *  trace cells  *
     *****************/

  (void) fprintf (logfile, "\n\nStart cell tracing:\n");
  Trace (&Movie, &Parameters, &Tools, logfile, 1);
  RemoveGhostNuclei (&Movie, &Parameters, &Tools, logfile);
	(void) fprintf (logfile, "Done Tracing \n");
 

    /***************************
     *   identity assignment   *
     ***************************/

  new_lineage_ct = 0;
  (void) fprintf (logfile, "\n\nStart cell ID assignment:\n");
  IdentityAssignment (&Movie, &Tools, &Parameters, logfile, &new_lineage_ct);


    /**************
     *   output   *
     **************/

  (void) fprintf (logfile, "\n\n");

  for (t = 0; t < tp_number; t++)
  {
    Parameters.t = t;
    Parameters.nuc_size = Parameters.sizes[t];

    /* text output */
    ReportNuclei (&Movie, &Tools, logfile, t);
  }

  (void) fprintf (logfile, "Total lineages:  %d\n", new_lineage_ct);
  (void) fclose (logfile);

  /* clean up */
  for (i = 0; i < tp_number; i++)
  {
    free (Tools.nuclei_record[i]);
  }

  free (Tools.nuclei_record);
  free (Tools.nuc_ct_rec);

//took release of stacks from here



  show_time ("End time");

  exit (EXIT_SUCCESS);
}
