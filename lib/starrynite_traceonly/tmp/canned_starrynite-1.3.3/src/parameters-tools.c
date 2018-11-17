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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "parameters-tools.h"
#include "parameters-hash.h"
#include "utility.h"
#include "types.h"
#include "error-codes.h"

/* BEGIN FUNCTION PROTOTYPES */

/*
 * static void CleanUpFrame (U8 ** image_stack, FRAME_t * frame);
 */

/* END FUNCTION PROTOTYPES */

void
SetParameters (MOVIE_t * const movie, PARAMETER_t * const parameters,
               const char *const filename)
{
  static const int line_size = 500;
  static const int head_size = 50;

  FILE *parameter_file = NULL;
char line[line_size], head[head_size];

  float tmp;

  assert (filename != NULL);

  parameter_file = fopen (filename, "r");

  if (parameter_file == NULL)
  {
    const char *const str = "Could not open the parameter file";

    fprintf (stderr, "ERROR: %s\n", str);
    exit (ERROR_SETPARAMETERS);
  }

  /* TODO: Replace with a more stable implementation of getline, perhaps from:
   * http://git.sv.gnu.org/gitweb/?p=gnulib.git;a=tree;f=lib;hb=master */
  while (fgets (line, line_size, parameter_file))
  {
    if (line[0] == '#')
    {
      continue;
    }

    sscanf (line, "%s %f", head, &tmp);

    const struct parameter *const param = in_word_set (head, strlen (head));

    /* TODO: Finish implementing this switch */
    switch (param->code)
    {
    case TIME_START:
      movie->time_start = (int) tmp;
      break;

    case TIME_END:
      movie->time_end = (int) tmp;
      break;

    case PLANE_START:
      movie->plane_start = (int) tmp;
      break;

    case PLANE_END:
      movie->plane_end = (int) tmp;
      break;

    case XY_RES:
      movie->xy_res = tmp;
      break;

    case Z_RES:
      movie->z_res = tmp;
      break;
    case TIME_INTERVAL:
      movie->t_interval = (int) tmp;
      break;

    case GRAPHIC_OUTPUT:
      parameters->graphic_output = (int) tmp;
      break;

    case NEIGHBORHOOD_SIZE:
      parameters->neighborhood_size = (int) tmp;
      break;

    case CELL_CT_LIMIT:
      parameters->cen_ct_limit = (int) tmp;
      break;

    case NUC_SIZE:
      parameters->nuc_size = (int) tmp;
      break;

    case POLAR_SIZE:
      parameters->polar_size = (int) tmp;
      break;

    case MAX_NUC_SIZE:
      parameters->max_nuc_size = (int) tmp;
      break;

    case NUC_SIZE_FACTOR1:
      parameters->nuc_size_factor1 = tmp;
      break;

    case NUC_SIZE_FACTOR2:
      parameters->nuc_size_factor2 = tmp;
      break;

    case NUC_SIZE_FACTOR3:
      parameters->nuc_size_factor3 = tmp;
      break;

    case NUC_SIZE_FACTOR4:
      parameters->nuc_size_factor4 = tmp;
      break;

    case NOISE_FRACTION:
      parameters->noise_fraction = tmp;
      break;

    case MAX_WEIGHT_CUTOFF:
      parameters->max_weight_cutoff = tmp;
      break;

    case NUC_DENSITY_CUTOFF:
      parameters->nuc_density_cutoff = tmp;
      break;

    case NUC_DENSITY_CUTOFF1:
      parameters->nuc_density_cutoff1 = tmp;
      break;

    case NUC_DENSITY_CUTOFF2:
      parameters->nuc_density_cutoff2 = tmp;
      break;

    case NUC_DENSITY_CUTOFF3:
      parameters->nuc_density_cutoff3 = tmp;
      break;

    case NOISE_FACTOR:
      parameters->noise_factor = tmp;
      break;

    case NOISE_FACTOR1:
      parameters->noise_factor1 = tmp;
      break;

    case NOISE_FACTOR2:
      parameters->noise_factor2 = tmp;
      break;

    case NOISE_FACTOR3:
      parameters->noise_factor3 = tmp;
      break;

    case NOISE_TIME_SWITCH1:
      parameters->noise_time_switch1 = (int) tmp;
      break;

    case NOISE_TIME_SWITCH2:
      parameters->noise_time_switch2 = (int) tmp;
      break;

    case MINIMAL_CELL_CYCLE:
      parameters->minimal_cell_cycle = (int) tmp;
      break;

    case AMBIGUITY_CUTOFF:
      parameters->ambiguity_cutoff = tmp;
      break;

    case SHRINK_ELASTISITY:
      parameters->shrink_elastisity = tmp;
      break;

    case EXPAND_ELASTISITY:
      parameters->expand_elastisity = tmp;
      break;
    }
  }
  fclose (parameter_file);

  movie->tp_number = movie->time_end - movie->time_start + 1;
  movie->frame.plane_number = movie->plane_end - movie->plane_start + 1;

  parameters->z_factor = movie->z_res / movie->xy_res;
 // parameters->min_nuc_size = (int) (parameters->z_factor * 2.5);
   parameters->min_nuc_size = 3;

  parameters->sizes = (int *) malloc_exit (sizeof (int) * movie->tp_number);
  assert (parameters->sizes != NULL);

  parameters->sizes[0] = parameters->nuc_size;

  parameters->minimal_cell_cycle /= movie->t_interval;
}



void
CalculateSizeParameters (PARAMETER_t * const parameters)
{
  parameters->nuc_size1 =
    (int) (parameters->nuc_size_factor1 * parameters->nuc_size);
  parameters->nuc_size2 =
    (int) (parameters->nuc_size_factor2 * parameters->nuc_size);
  parameters->nuc_size3 =
    (int) (parameters->nuc_size_factor3 * parameters->nuc_size);
  parameters->nuc_size4 =
    (int) (parameters->nuc_size_factor4 * parameters->nuc_size);
  parameters->z_size1 =
    (int) (1.0 * parameters->nuc_size1 / parameters->z_factor);
  if (parameters->z_size1 < 3)
    parameters->z_size1 = 3;
  parameters->cube_size =
    parameters->nuc_size1 * parameters->nuc_size1 * parameters->z_size1;
}


void
UpdateNucSize (const TOOL_t * const tools, PARAMETER_t * const parameters,
               FILE * const logfile, const int t)
{
  int i;
  int total, ct;

  const NUCLEUS_t *const nuclei = tools->nuclei_record[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t - 1];

  int cutoff1 = (int) (0.7 * parameters->nuc_size);
 int cutoff2 = (nuc_ct < 150) ?
    (int) (1.2 * parameters->nuc_size) : (int) (1.1 * parameters->nuc_size);

  //as modified because canned diams are always odd numbers and this excludes them 
  if(cutoff1>parameters->nuc_size-3)
	  cutoff1=parameters->nuc_size-3;


  if(cutoff2<parameters->nuc_size+3)
	  cutoff2=parameters->nuc_size+3;

  total = 0;

  /* ct will hold the number of nuclei in the size range */
  ct = 0;

  for (i = 0; i < nuc_ct; ++i)
  {
    const int size = nuclei[i].size;
    if (size >= cutoff1 && size <= cutoff2)
    {
      total += size;
      ++ct;
    }
  }

  if (ct < 30 && ct < nuc_ct / 3 + 1)
  {
    const char fmt[] = "%s: time %d, %d out of %d total\n";
    const char msg[] = "Not enough nuclei within size range";

    fprintf (logfile, fmt, msg, t, ct, nuc_ct);
  }
  else
  {
    parameters->nuc_size = total / ct;
  }

  parameters->sizes[t] = parameters->nuc_size;
}



void
UpdateTimeInfo (PARAMETER_t * const parameters, const MOVIE_t * const movie,
                const TOOL_t * const tools, const int t)
{
  /* z_factor */
  parameters->nuc_size = parameters->sizes[t];
  if (t == 0)
  {
    parameters->z_factor = movie->z_res / movie->xy_res;
  }
  else
  {
    if (tools->nuc_ct_rec[t - 1] < 100)
    {
      parameters->z_factor = movie->z_res / movie->xy_res;
    }
    else if (tools->nuc_ct_rec[t - 1] < 170)
    {
      parameters->z_factor = 1.1 * movie->z_res / movie->xy_res;
    }
    else if (tools->nuc_ct_rec[t - 1] < 200)
    {
      parameters->z_factor = 1.2 * movie->z_res / movie->xy_res;
    }
    else
    {
      parameters->z_factor = 1.3 * movie->z_res / movie->xy_res;
    }
  }

  /* noise cutoff */
  if (t == 0)
  {
    parameters->nuc_density_cutoff = parameters->nuc_density_cutoff1;
    parameters->noise_factor = parameters->noise_factor1;
  }
  else
  {
    if (tools->nuc_ct_rec[t - 1] <= parameters->noise_time_switch1)
    {
      parameters->nuc_density_cutoff = parameters->nuc_density_cutoff1;
      parameters->noise_factor = parameters->noise_factor1;
    }
    else if (tools->nuc_ct_rec[t - 1] <= parameters->noise_time_switch2)
    {
      parameters->nuc_density_cutoff = parameters->nuc_density_cutoff2;
      parameters->noise_factor = parameters->noise_factor2;
    }
    else
    {
      parameters->nuc_density_cutoff = parameters->nuc_density_cutoff3;
      parameters->noise_factor = parameters->noise_factor3;
    }
  }
}



void
SetOriginalFrame (MOVIE_t * const movie, const PARAMETER_t * const parameters,
                  const int image_width, const int image_height,
                  const int image_size)
{
  movie->frame.x_start = 0;
  movie->frame.y_start = 0;
  movie->frame.z_start = 0;
  movie->frame.width = image_width;
  movie->frame.height = image_height;
  movie->frame.image_size = image_size;

  {
    const int w_squared = image_width * image_width;
    const int h_squared = image_height * image_height;

    const float z_height = (movie->frame.plane_number - 1) *
      parameters->z_factor;
    const float z_squared = z_height * z_height;

    const float sum_squares = w_squared + h_squared + z_squared;

    movie->max_distance = sqrtf (sum_squares);
  }
}


void
MakeTools (const MOVIE_t * const movie, const PARAMETER_t * const parameters,
           TOOL_t * const tools, const int image_size)
{
  int i, j, k;
  int length;

  int r, slices, dist;
  float zf2, r2;
  int *radii, **xrange;
  int upper;

  int plane_number = movie->frame.plane_number;
  int tp_number = movie->tp_number;

  tools->nuclei_record =
    (NUCLEUS_t **) malloc_exit (sizeof (NUCLEUS_t *) * tp_number);
  tools->nuc_ct_rec = (int *) malloc_exit (sizeof (int) * tp_number);

  tools->density_stack = (int **) malloc_exit (sizeof (int *) * plane_number);
  tools->density_stack_2D =
    (int **) malloc_exit (sizeof (int *) * plane_number);

  for (i = 0; i < plane_number; ++i)
  {
    tools->density_stack_2D[i] =
      (int *) malloc_exit (sizeof (int) * image_size);
    tools->density_stack[i] = (int *) malloc_exit (sizeof (int) * image_size);
  }

  tools->density_2D = (int *) malloc_exit (sizeof (int) * image_size);
  tools->max_stat = (int **) malloc_exit (sizeof (int *) * plane_number);

  for (i = 0; i < plane_number; ++i)
  {
    tools->max_stat[i] = (int *) malloc_exit (sizeof (int) * image_size * 2);
  }

  tools->centroids =
    (int *) malloc_exit (sizeof (int) * parameters->cen_ct_limit * 5);
  tools->cen_tmp =
    (int *) malloc_exit (sizeof (int) * parameters->cen_ct_limit * 5);

  tools->noise = (int **) malloc_exit (sizeof (int *) * tp_number);
  tools->noise_ori = (int **) malloc_exit (sizeof (int *) * tp_number);
  tools->max_cutoff = (int **) malloc_exit (sizeof (int *) * tp_number);
  tools->max_cutoff_ori = (int **) malloc_exit (sizeof (int *) * tp_number);

  for (i = 0; i < tp_number; i++)
  {
    tools->noise[i] = (int *) malloc_exit (sizeof (int) * plane_number);
    tools->noise_ori[i] = (int *) malloc_exit (sizeof (int) * plane_number);
    tools->max_cutoff[i] = (int *) malloc_exit (sizeof (int) * plane_number);
    tools->max_cutoff_ori[i] =
      (int *) malloc_exit (sizeof (int) * plane_number);
  }

  length =
    255 * parameters->neighborhood_size * parameters->neighborhood_size + 1;
  tools->dist = (int *) malloc_exit (sizeof (int) * length);
  tools->dist1 = (int *) malloc_exit (sizeof (int) * length);
  tools->dist2 = (int *) malloc_exit (sizeof (int) * length);
  tools->distx = (int *) malloc_exit (sizeof (int) * image_size);

  /* even number of elements, means index for last element is odd;
   * reason for doubling is for MaskNuclei() to mask a little extra */
  upper = parameters->max_nuc_size * 2;

  tools->spheres = (int ***) malloc_exit (sizeof (int **) * upper);
  tools->s_layers = (int *) malloc_exit (sizeof (int) * upper);
  tools->s_radii = (int **) malloc_exit (sizeof (int *) * upper);
  zf2 = parameters->z_factor * parameters->z_factor;

  for (k = parameters->min_nuc_size; k < upper; k += 2)
  //for (k = 3; k < upper; k += 2)
  {
    const int R = k / 2;
    const float R2 = 1.0 * R * R;

    slices = (int) (1.0 * R / parameters->z_factor);
    radii = (int *) malloc_exit (sizeof (int) * (2 * slices + 1));
    xrange = (int **) malloc_exit (sizeof (int *) * (2 * slices + 1));

    for (i = slices; i >= 0; --i)
    {
      r = (int) sqrt (R2 - zf2 * i * i);
	  if (r<0) r=0; //mod anthony

      radii[slices - i] = r;
      xrange[slices - i] = (int *) malloc_exit (sizeof (int) * (2 * r + 1));
      r2 = 1.0 * r * r;

      for (j = r; j > 0; j--)
      {
        dist = (int) sqrt (r2 - j * j);
        xrange[slices - i][r - j] = dist;
        xrange[slices - i][r + j] = dist;
      }
      xrange[slices - i][r] = r;

      if (i != 0)
      {
        radii[slices + i] = r;
        xrange[slices + i] = (int *) malloc_exit (sizeof (int) * (2 * r + 1));
        memcpy (xrange[slices + i], xrange[slices - i],
                sizeof (int) * (2 * r + 1));
      }
    }                           /* done going through the slices */

    tools->spheres[k] = xrange;
    tools->s_layers[k] = slices;
    tools->s_radii[k] = radii;

    if (k == (k / 2) * 2 && k < upper - 1)
    {
      /* index for last element is odd, so it's taken care of by the previous k */
      tools->spheres[k + 1] = xrange;
      tools->s_layers[k + 1] = slices;
      tools->s_radii[k + 1] = radii;
    }
    else if (k > parameters->min_nuc_size)
    {
      tools->spheres[k - 1] = xrange;
      tools->s_layers[k - 1] = slices;
      tools->s_radii[k - 1] = radii;
    }
  }
}


/*
static void
CleanUpFrame (U8 ** image_stack, FRAME_t * frame)
{
  int i;

  for (i = 0; i < frame->plane_number; i++)
    free (image_stack[i]);
  free (image_stack);
}
*/


void
CleanUpTools (const MOVIE_t * const movie,
              const PARAMETER_t * const parameters, TOOL_t * const tools)
{
  int i;
  const int plane_number = movie->frame.plane_number;
  const int upper = parameters->max_nuc_size * 2;

  for (i = 0; i < plane_number; i++)
  {
    free (tools->density_stack[i]);
    free (tools->density_stack_2D[i]);
    free (tools->max_stat[i]);
  }
  free (tools->density_stack);
  free (tools->density_stack_2D);
  free (tools->density_2D);
  free (tools->max_stat);
  free (tools->centroids);
  free (tools->cen_tmp);

  for (i = parameters->min_nuc_size; i < upper; i += 2)
  {
    int j;
    const int slices = tools->s_layers[i];
    int **xrange = tools->spheres[i];
    for (j = slices; j >= 0; j--)
    {
      free (xrange[slices - j]);
      if (j)
        free (xrange[slices + j]);
    }
    free (xrange);
    free (tools->s_radii[i]);
  }
  free (tools->spheres);
  free (tools->s_layers);
  free (tools->s_radii);
}
