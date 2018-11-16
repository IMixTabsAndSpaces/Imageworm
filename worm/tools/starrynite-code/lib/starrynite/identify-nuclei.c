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

#include "identify-nuclei.h"

#include "DefineNuclei.h"
#include "RecoverNuclei.h"
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "FreeImageStack.h"
#include "GetImageStacks.h"
#include "noise-filter.h"
#include "error-codes.h"
#include "logfile.h"
#include "CalculateSizeParameters.h"
#include "UpdateNucSize.h"
#include "UpdateTimeInfo.h"

/***************************************
 * processing images / identify nuclei *
 ***************************************/

void
sn_identify_nuclei(parameter_t * const pParameters, movie_t * const movie,
    tool_t * const tools, int * const num_timepts,
    const char * const input_prefix, uint32_t *image_width,
    uint32_t *image_height, uint32_t *image_size, const int num_planes,
    uint8_t ** const backup_stack, int * const cell_ct

)
{
  int t;

  for (t = 0; t < *num_timepts; t++)
    {
      int i;
      pParameters->t = t;

      if (0)
        {
          /* TODO: Parameters should be constant during execution! */
          if (movie->time_start + t == 150)
            pParameters->nuc_size = 45;
        }

      if (t != 0)
        {
          UpdateNucSize(tools->nuclei_record[t - 1], tools->nuc_ct_rec[t - 1],
              pParameters, t);
        }

      if (pParameters->nuc_size <= pParameters->min_nuc_size)
        {
          fprintf(sn_logfile,
              "\ntime %d, nuclear size %d less than min nuclear size %d\n",
              movie->time_start + t, pParameters->nuc_size,
              pParameters->min_nuc_size);
          *num_timepts = t;
          break;
        }

      UpdateTimeInfo(pParameters, movie->z_res / movie->xy_res,
          tools->nuc_ct_rec, t);

      CalculateSizeParameters(pParameters);

      UpdateNoise(tools->noise, pParameters->noise_factor,
          movie->frame.num_planes, t);

      fprintf(sn_logfile, "time %3d, nuclear size %d", movie->time_start + t,
          pParameters->nuc_size);

      fprintf(sn_logfile, ", z_factor %.2f", pParameters->z_factor);

      fprintf(sn_logfile, ", %.2f, %.2f", pParameters->noise_factor,
          pParameters->nuc_density_cutoff);

      if (t > 0)
        {
          FreeImageStack(tools->image_stack, movie->frame.num_planes);
          tools->image_stack = NULL;
        }

      tools->image_stack = GetImageStacks(movie->frame.num_planes,
          movie->time_start, movie->plane_start, t, input_prefix, false,
          image_width, image_height);

      assert (*image_size == *image_width * *image_height);

      for (i = 0; i < num_planes; i++)
        {
          memcpy(backup_stack[i], tools->image_stack[i], sizeof(uint8_t)
              * *image_size);
        }

      /* filtering noise */
      LocalDensityCut_3D(&movie->frame, pParameters, *image_width,
          *image_height, *image_size, tools->image_stack, t,
          tools->density_stack_2D, tools->density_2D,
          (const int **) tools->noise);

      DefineNuclei(movie->time_start, pParameters, tools, backup_stack,
          (const uint8_t **) tools->image_stack, &(movie->frame),
          tools->nuclei_record + t, tools->nuc_ct_rec + t, t);

      RecoverNuclei(movie->time_start, pParameters, tools, backup_stack,
          (const uint8_t **) tools->image_stack, &(movie->frame), t);

      *cell_ct = tools->nuc_ct_rec[t];

      fprintf(sn_logfile, "\n");
      (void) fflush(sn_logfile);

      if (tools->nuc_ct_rec[t] > 540)
        {
          fprintf(sn_logfile, "\ntime %d, %d cells identified.  "
            "Approaching second embryonic rotation.  Ending.\n",
              movie->time_start + t, tools->nuc_ct_rec[t]);
          *num_timepts = t;
          break;
        }
    }

  fprintf(sn_logfile, "\nTotal time points %d\n", movie->num_timepts);
  fprintf(sn_logfile, "Last time point processed: %d\n", movie->time_start
      + *num_timepts - 1);

  if (*num_timepts == 0)
    {
      const char * const msg = "fatal error - num_timepts == 0";

      (void) fprintf(stderr, "%s line %d: %s\n", __FILE__, __LINE__, msg);
      exit(ERROR_MAIN1);
    }

}
