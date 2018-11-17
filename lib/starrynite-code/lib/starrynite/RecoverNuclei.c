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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "RecoverNuclei.h"

#include "RefineNuclei.h"
#include "logfile.h"
#include "LocalMaxima_3D.h"
#include "nucleus_t.h"
#include "struct_frame.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "LocalWeightScan_3D.h"
#include "CalculateSizeParameters.h"
#include "DistanceFilter.h"
#include "xalloc.h"
#include "struct_metric.h"
#include "init_metric.h"

void
RecoverNuclei(const int time_start, parameter_t * const parameters,
    tool_t * const tools, uint8_t ** backup_stack,

    const uint8_t * const *image_stack, frame_t * frame, int t)
{
  int nuc_ct, nuc_ct_recov, nuc_left;
  nucleus_t *nuclei, *nuclei_recov;
  int j;

  /*while (parameters->nuc_size > parameters->min_nuc_size) { */
  nuclei = tools->nuclei_record[t];
  nuc_ct = tools->nuc_ct_rec[t];

  parameters->nuc_size = parameters->nuc_size * 2 / 3;
  if (parameters->nuc_size < parameters->min_nuc_size)
    parameters->nuc_size = parameters->min_nuc_size;
  CalculateSizeParameters(parameters);

  LocalWeightScan_3D(tools->density_stack, tools->density_stack_2D,
      tools->density_2D, image_stack, frame, parameters->nuc_size1,
      parameters->z_size1);
  LocalMaxima_3D(tools, parameters, backup_stack, image_stack, frame, 0, t);

  if (tools->cen_ct)
    {
      nuc_ct_recov = tools->cen_ct;
      nuclei_recov = (nucleus_t *) xmalloc(sizeof(nucleus_t) * nuc_ct_recov);
      RefineNuclei(tools, parameters, time_start, image_stack, frame,
          nuclei_recov, t);

      metric_t metric;
      init_metric(&metric, frame->width, parameters->z_factor);


      DistanceFilter(nuclei_recov, nuc_ct_recov, nuclei, nuc_ct, &metric, parameters->nuc_size_factor4);

      nuc_left = 0;
      for (j = 0; j < nuc_ct_recov; j++)
        {
          if (nuclei_recov[j].is_valid)
            nuc_left++;
        }

      if (nuc_left)
        {
          tools->nuclei_record[t] = (nucleus_t *) xmalloc(sizeof(nucleus_t)
              * (nuc_ct + nuc_left));
          memcpy(tools->nuclei_record[t], nuclei, sizeof(nucleus_t) * nuc_ct);
          free(nuclei);
          nuclei = tools->nuclei_record[t];
          tools->nuc_ct_rec[t] = nuc_ct + nuc_left;
          for (j = 0; j < nuc_ct_recov; j++)
            {
              if (!nuclei_recov[j].is_valid)
                continue;
              memcpy(nuclei + nuc_ct, nuclei_recov + j, sizeof(nucleus_t));
              nuc_ct++;
            }
        }
      free(nuclei_recov);
    }
  fprintf(sn_logfile, ", %d cells", nuc_ct);
  /*} */

  parameters->nuc_size = parameters->sizes[parameters->t];
  CalculateSizeParameters(parameters);
}

