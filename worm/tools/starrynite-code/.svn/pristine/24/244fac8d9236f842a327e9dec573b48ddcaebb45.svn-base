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

#include "Encroached.h"
#include "logfile.h"
#include "metric_calculate_distance.h"
#include "struct_nucleus.h"
#include "struct_metric.h"
#include "init_metric.h"
#include "partner_t.h"

/* TODO: Break into smaller functions */
/* TODO: Move init_metric into caller */
/* TODO: Get nuclei_prev[pred] from caller, remove nuclei_prev from parameters */
int
Encroached(nucleus_t * const nuclei, const int nuc_ct,
    nucleus_t * const nuclei_prev, const int time_start, const int t,
    const int pred, const int index, const int width, const float z_factor,
    const float cutoff)
{
  const partner_t * const partners = nuclei_prev[pred].s_partners;

  metric_t metric;
  init_metric(&metric, width, z_factor);

  const float dist1 = metric_calculate_distance(&metric,
      &nuclei_prev[pred], &nuclei[index]);


  for (int i = 0; i < nuc_ct; i++)
    {
      if (partners[i].index == index)
        {
          if (i > 2)
            {
              fprintf(sn_logfile, "%d th suc to %d:%d (%.2f), ", i + 1,
                  time_start - 1 + t, pred + 1, dist1);
              return 1;
            }
          break;
        }
    }

  for (int i = 0; i < nuc_ct; i++)
    {
      const int suc = partners[i].index;

      if (partners[i].distance > cutoff)
        {
          break;
        }

      if (suc == index)
        {
          break;
        }

        {
          const float dist2 = metric_calculate_distance(&metric, &nuclei_prev[pred], &nuclei[suc]);
          const float dist3 = metric_calculate_distance(&metric, &nuclei[index], &nuclei[suc]);

          /* if (dist1 + dist2 > 1.3 * dist3) { */
          if (dist3 < dist1 && dist2 + dist3 < 2.0 * dist1)
            {
              fprintf(sn_logfile, " %d:%d (%.2f) encroached by %d:%d (%.2f), ",
                  time_start - 1 + t, pred + 1, dist1, time_start - 1 + t + 1,
                  suc + 1, dist2);
              return 1;
            }
        }
    }

  return 0;
}
