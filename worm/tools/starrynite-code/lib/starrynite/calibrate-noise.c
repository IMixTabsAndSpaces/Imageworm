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

#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "MakeTools.h"
#include "SetupInitialFrame.h"
#include "FreeImageStack.h"
#include "GetImageStacks.h"
#include "noise-filter.h"
#include "cell-identity.h"
#include "report-lineage.h"
#include "error-codes.h"
#include "xalloc.h"
#include "logfile.h"
#include "identify-nuclei.h"
#include "calibrate-noise.h"

#define LOG_NOISE_CUTOFFS 1

/****************************************
 * processing images / set noise cutoff *
 ****************************************/

void
sn_calibrate_noise_model(parameter_t * const pParameters, tool_t * const tools,
    movie_t * const movie, uint8_t *** const backup_stack,
    const char * const input_prefix, uint32_t * const image_height,
    uint32_t * const image_width, uint32_t * const image_size)
{
  int t, i;
  bool is_first_tiff = true;

  /* Read an image stack for t = 0. Set up some memory using information
   from the first images. Then, read in the rest of the image stacks,
   and call CalibrateNoise_3D with each of them. */
  for (t = 0; t < movie->num_timepts; t++)
    {
      if (t > 0)
        {
          FreeImageStack(tools->image_stack, movie->frame.num_planes);
          tools->image_stack = NULL;
        }

      tools->image_stack = GetImageStacks(movie->frame.num_planes,
          movie->time_start, movie->plane_start, t, input_prefix,
          is_first_tiff, image_width, image_height);
      (*image_size) = (*image_width) * (*image_height);
      pParameters->t = t;

      if (is_first_tiff)
        {
          is_first_tiff = false;

          /* TODO: Split the remaining part of this block into a
           * sub-function */
          MakeTools(movie->frame.num_planes, movie->num_timepts, pParameters,
              tools, (*image_size));

          SetupInitialFrame(pParameters->z_factor, (*image_width),
              (*image_height), (*image_size), &movie->frame,
              &movie->max_distance);

          *backup_stack = (uint8_t **) xmalloc(sizeof(uint8_t *)
              * movie->frame.num_planes);

          for (i = 0; i < movie->frame.num_planes; i++)
            {
              (*backup_stack)[i] = (uint8_t *) xmalloc(sizeof(uint8_t)
                  * (*image_size));
            }
        }

      CalibrateNoise_3D(tools->density_stack_2D, tools->density_2D,
          tools->noise_ori, tools->max_cutoff_ori,
          pParameters->neighborhood_size, pParameters->noise_fraction,
          &movie->frame, (*image_width), (*image_height),
          (const uint8_t **) tools->image_stack, t);

    }

  SmoothingNoiseModel(tools->noise, tools->noise_ori, tools->max_cutoff,
      tools->max_cutoff_ori, movie->frame.num_planes, movie->num_timepts);

  if (LOG_NOISE_CUTOFFS)
    {
      report_noise_model(movie->num_timepts, movie->time_start,
          movie->frame.num_planes, (const int **) tools->max_cutoff,
          (const int **) tools->max_cutoff_ori, (const int **) tools->noise,
          (const int **) tools->noise_ori);
    }

}
