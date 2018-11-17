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

#include "initialize_slice.h"
#include <math.h>
#include <string.h>
#include "xalloc.h"

void
initialize_slice(int ** const xrange, int * const radii, const int j,
    const int slices, const float zf2, const int r)
{
  int k;
  size_t xrange_size = sizeof(int) * (2 * r + 1);

  radii[slices - j] = r;
  xrange[slices - j] = (int *) xmalloc(xrange_size);

  for (k = r; k > 0; k--)
    {
      const int dist = (int) sqrtf((float) r * r - k * k);
      xrange[slices - j][r - k] = dist;
      xrange[slices - j][r + k] = dist;
    }
  xrange[slices - j][r] = r;

  if (j != 0)
    {
      radii[slices + j] = r;
      xrange[slices + j] = (int *) xmalloc(xrange_size);
      memcpy(xrange[slices + j], xrange[slices - j], xrange_size);
    }
}
