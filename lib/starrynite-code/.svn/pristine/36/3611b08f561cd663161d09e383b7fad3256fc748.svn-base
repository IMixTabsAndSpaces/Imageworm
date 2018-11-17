/* Copyright (C) 2005-2009 University of Washington
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "run_starrynite.h"

#include "calibrate-noise.h"
#include "cell-identity.h"
#include "CleanUpTools.h"
#include "error-codes.h"
#include "FreeParameters.h"
#include "identify-nuclei.h"
#include "LoadParameterFile.h"
#include "logfile.h"
#include "noise-filter.h"
#include "RemoveGhostNuclei.h"
#include "report-lineage.h"
#include "SetupMovie.h"
#include "Trace.h"
#include "run_starrynite_continuation_t.h"
#include "xalloc.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "movie_t.h"

#include "svm_classifier.h"

int
run_starrynite_before_tracing(const char * const input_prefix,
    const char * const param_file_name, bool skip_svm_classifier,
    run_starrynite_continuation_t continuation)
{
  movie_t *movie;
  parameter_t *pParameters;
  tool_t Tools;
  uint8_t **backup_stack = NULL;
  int t;
  int cell_ct = 0;
  int new_lineage_ct;
  uint32_t image_width = 0;
  uint32_t image_height = 0;
  uint32_t image_size = 0;

  if ((sn_logfile = fopen("log", "w")) == NULL)
    {
      (void) fprintf(stderr, "Could not open the log file\n");
      exit(ERROR_LOGFILE);
    }

  /**************************
   * read in the parameters *
   **************************/

  pParameters = LoadParameterFile(param_file_name);
  assert (pParameters != NULL);

  movie = SetupMovie(pParameters);
  assert(movie != NULL);

  /****************************************
   * processing images / set noise cutoff *
   ****************************************/

  sn_calibrate_noise_model(pParameters, &Tools, movie, &backup_stack,
      input_prefix, &image_height, &image_width, &image_size);

  /***************************************
   * processing images / identify nuclei *
   ***************************************/

  sn_identify_nuclei(pParameters, movie, &Tools, &movie->num_timepts,
      input_prefix, &image_width, &image_height, &image_size,
      movie->frame.num_planes, backup_stack, &cell_ct);

  return continuation(input_prefix, skip_svm_classifier, movie,
      pParameters, &Tools, t, cell_ct, new_lineage_ct,
      image_width, image_height, image_size);
}

int
run_starrynite_from_tracing(const char * const input_prefix,
    bool skip_svm_classifier, movie_t *movie, parameter_t *pParameters,
    tool_t *tools, int t, int cell_ct,
    int new_lineage_ct, uint32_t image_width, uint32_t image_height,
    uint32_t image_size)
{
  FILE *incorrect_division_calls_of_sn_file;
  FILE *correct_division_calls_of_sn_file;

  /*****************
   *  trace cells  *
   *****************/

  fprintf(sn_logfile, "\n\nStart cell tracing:\n");
  Trace(movie, pParameters, tools, 1);
  RemoveGhostNuclei(movie->time_start, movie->num_timepts, pParameters, tools);

  /***************************
   *   identity assignment   *
   ***************************/

  new_lineage_ct = 0;
  fprintf(sn_logfile, "\n\nStart cell ID assignment:\n");
  IdentityAssignment(movie, tools, pParameters, &new_lineage_ct,
      movie->num_timepts);

  if (!skip_svm_classifier)
    {
      /*******************************************************************************
       *   SVM classifier (currently operates on division calls made by StarryNite)   *
       ********************************************************************************/

	incorrect_division_calls_of_sn_file = fopen("svm_output_movements_called_as_divisions.txt", "r");
  	correct_division_calls_of_sn_file = fopen("svm_output_correct_division_calls.txt", "r");

	if (incorrect_division_calls_of_sn_file != NULL)
	{
		fclose(incorrect_division_calls_of_sn_file);
		incorrect_division_calls_of_sn_file = fopen("svm_output_movements_called_as_divisions.txt", "w");
		fclose(incorrect_division_calls_of_sn_file);
	}
	else
	{
		incorrect_division_calls_of_sn_file = fopen("svm_output_movements_called_as_divisions.txt", "w");
                fclose(incorrect_division_calls_of_sn_file);
	}

	if (correct_division_calls_of_sn_file != NULL)
	{
		fclose(correct_division_calls_of_sn_file);
		correct_division_calls_of_sn_file = fopen("svm_output_correct_division_calls.txt", "w");
		fclose(correct_division_calls_of_sn_file);
	}
	else
	{
		correct_division_calls_of_sn_file = fopen("svm_output_correct_division_calls.txt", "w");
                fclose(correct_division_calls_of_sn_file);
	}

        (void) svm_classifier(movie, tools, input_prefix, &image_width,
          &image_height);
    }

  /**************
   *   output   *
   **************/

  fprintf(sn_logfile, "\n\n");

  for (t = 0; t < movie->num_timepts; t++)
    {
      ReportNuclei(movie, tools, t, skip_svm_classifier);
    }

  fprintf(sn_logfile, "Total lineages:  %d\n", new_lineage_ct);

  /* clean up */

  if (fclose(sn_logfile) != 0)
    {
      (void) fprintf(stderr, "Error closing logfile\n");
    }

  CleanUpTools(tools, movie->frame.num_planes, movie->num_timepts,
      pParameters->max_nuc_size, pParameters->min_nuc_size);

  FreeParameters(pParameters);
  free(movie);

   return 0;
}

int
run_starrynite(const char * const input_prefix,
    const char * const param_file_name, bool skip_svm_classifier)
/*@globals: stdout, stderr, errno @*/
{
  run_starrynite_continuation_t continuation = run_starrynite_from_tracing;

  return run_starrynite_before_tracing(input_prefix, param_file_name,
      skip_svm_classifier, continuation);
}
