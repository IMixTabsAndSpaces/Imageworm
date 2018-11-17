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

#include "SetCutoff.h"
#include <assert.h>

void
SetCutoff(int * const area, const int * const dist,
    int * const * const noise_ori, int * const * const max_cutoff_ori,
    const float noise_fraction, int dmin, const int dmax, const int t,
    const int plane)
{
  int max = 0, i;
  int peak;
  int pos_ct;

  peak = 0;
  area[dmin - 1] = 0;

  for (i = dmin; i <= dmax; i++)
    {
      int x = dist[i];
      area[i] = area[i - 1] + x;
      if (x > peak)
        {
          peak = x;
          max = i;
        }
    }
  max_cutoff_ori[t][plane] = max;

  assert (max> 0);

  peak = area[max];

  for (i = dmin; i < max; i++)
    {
      int x = area[i];

      if (1.0 * x / peak > noise_fraction)
        break;
    }

    {
      const int start = (i > dmin + 1) ? i - 2 : dmin;
      const int end = (i < dmax) ? i + 1 : dmax;
      const int cut = (area[end] - area[start]) / (end - start);

      pos_ct = 0;

      for (i = 0; i < 10; i++)
        {
          if (dist[dmax - i] > cut)
            pos_ct++;
        }

      if (dmin < 9)
        dmin = 9;

      for (i = dmax - 1; i > dmin && pos_ct < 5; i--)
        {
          if (dist[i + 1] > cut)
            pos_ct--;
          if (dist[i - 9] > cut)
            pos_ct++;
        }
      noise_ori[t][plane] = i;
    }
}
