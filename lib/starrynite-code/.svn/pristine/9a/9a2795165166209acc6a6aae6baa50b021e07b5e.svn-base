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

#include "Smoothing.h"
#include <string.h>
#include <assert.h>

/** Returns the largest 0 < n < len such that dist[n] is nonzero,
 * or 0 otherwise.
 */
static int
find_last_nonzero(const int * const dist, const int len)
{
  int i = len - 1;
  int j = 0;

  while (j == 0 && i > 0)
    {
      if (dist[i] != 0)
        {
          j = i;
        }
      --i;
    }

  return j;
}

void
Smoothing(const int * const dist, int *dist1, int *dist2, const int length,
    const int xlen, int * const dmin_p, int * const dmax_p, const int rounds)
{
  int dmax;
  int dmin = 1;

  memcpy(dist1, dist, sizeof(int) * length);

  /* TODO: Why does this stop at 1? */

  dmax = find_last_nonzero(dist1, length);

  assert (dmax> 0);

  for (int i = 0; i < rounds; i++)
    {
      const int xw = xlen / 2;
      int *dist_tmp;
      int sum = 0;

      for (int j = dmin; j < dmin + xlen; j++)
        {
          sum += dist1[j];
        }

      if (dmax > length - xw - 1)
        {
          dmax = length - xw - 1;
        }

      for (int j = dmin + xw; j <= dmax; j++)
        {
          sum = sum - dist1[j - xw] + dist1[j + xw - 1];
          dist2[j] = sum / xlen;
        }

      dist_tmp = dist1;
      dist1 = dist2;
      dist2 = dist_tmp;
      dmin += xw;
      dmax -= xw;
    }

  /* TODO: Eliminate this use of local duplicate variables, either by
   * returning values or working directly with the pointers */
  *dmin_p = dmin;
  *dmax_p = dmax;
}
