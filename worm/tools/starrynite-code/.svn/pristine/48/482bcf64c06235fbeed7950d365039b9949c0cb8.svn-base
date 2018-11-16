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

#include "SelfDistanceFilter.h"

#include "struct_frame.h"
#include "parameter_t.h"
#include "metric_calculate_distance.h"
#include "struct_metric.h"
#include "init_metric.h"
#include "struct_nucleus.h"

void
SelfDistanceFilter(const parameter_t * const parameters, nucleus_t * nuclei,
    int nuc_ct, frame_t * frame)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].is_valid == 0)
        {
          continue;
        }

      for (int j = i + 1; j < nuc_ct; j++)
        {
          if (nuclei[j].is_valid == 0)
            {
              continue;
            }

	  metric_t metric;
	  init_metric(&metric, frame->width, parameters->z_factor);

          const float distance = metric_calculate_distance(&metric,
              &nuclei [j], &nuclei [i]);

          const int cutoff = parameters->nuc_size_factor4 * (nuclei[j].size
              + nuclei[i].size) / 2;
          if (distance < cutoff)
            {
              if (nuclei[i].weight < nuclei[j].weight)
                nuclei[i].is_valid = 0;
              else
                nuclei[j].is_valid = 0;
            }
        }
    }
}
