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

#include "max_scan_y.h"
#include "colors.h"
#include "frame_t.h"
#include "MarkCentroid.h"

void
max_scan_y(uint8_t * const * const backup_stack, const int i,
    const int my_image_width, const int j, const int step1,
    const int width_end1, int * const stat_plane, int * const centroids,
    const frame_t * const frame, const int in, const int out,
    const int report_max)
{
  int max_ct = 0;
  int base = my_image_width * j;
  int k, m;

  for (k = step1; k < width_end1; ++k)
    {
      if (stat_plane[(base + k) * 2 + 1] == 1)
        {
          centroids[5 * max_ct + 1] = k;
          centroids[5 * max_ct + 4] = stat_plane[(base + k) * 2]; /* 'weight' */
          ++max_ct;
        }
    }

  /* changed 'continue' in macro to 'return' in function */
  if (!max_ct)
    return;

  /* backup_stack is a global */

  if (report_max == 2)
    {
      for (k = 0; k < max_ct; k++)
        {
          MarkCentroid(backup_stack[i], frame, centroids[5 * k + 1], base,
              MONO, 191);
        }
    }

    {
      int max_remained = max_ct;

      for (m = out; m <= in; m++)
        {
          if (m == j)
            continue;
          for (k = 0; k < max_ct; k++)
            {
              const int cur = centroids[5 * k + 4];
              if (!cur)
                continue;
              if (cur < stat_plane[(m * my_image_width + centroids[5 * k + 1])
                  * 2])
                {
                  centroids[5 * k + 4] = 0;
                  --max_remained;
                  if (!max_remained)
                    break;
                }
            }
          if (!max_remained)
            break;
        }

    }

  /* backup_stack is a global */

  for (k = 0; k < max_ct; k++)
    {
      if (!centroids[5 * k + 4])
        {
          stat_plane[(base + centroids[5 * k + 1]) * 2 + 1] = 0;
        }
      else if (report_max == 3)
        {
          MarkCentroid(backup_stack[i], frame, centroids[5 * k + 1], base,
              MONO, 255);
        }
    }

}
