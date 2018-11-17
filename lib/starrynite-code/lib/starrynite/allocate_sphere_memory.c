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

#include "allocate_sphere_memory.h"
#include <math.h>
#include "initialize_slice.h"
#include "xalloc.h"

void
allocate_sphere_memory(int **** const spheres, int *** const s_radii,
    int ** const s_layers, const float z_factor, const int min_nuc_size,
    const int max_nuc_size)
{
  /* even number of elements, means index for last element is odd;
   * reason for doubling is for MaskNuclei() to mask a little extra */
  const int upper = max_nuc_size * 2;

  int i;

  *spheres = (int ***) xmalloc(sizeof(int **) * upper);
  *s_radii = (int **) xmalloc(sizeof(int *) * upper);
  *s_layers = (int *) xmalloc(sizeof(int) * upper);

  for (i = min_nuc_size; i < upper; i += 2)
    {
      int j;
      const int R = i / 2;
      const float R2 = 1.0 * R * R;

      const int slices = (int) (1.0 * R / z_factor);

      int ** const xrange = (int **) xmalloc(sizeof(int *) * (2 * slices + 1));
      int * const radii = (int *) xmalloc(sizeof(int) * (2 * slices + 1));

      (*spheres)[i] = xrange;
      (*s_radii)[i] = radii;
      (*s_layers)[i] = slices;

      if (i % 2 == 0 && i < upper - 1)
        {
          /* index for last element is odd, so it's taken care of by the previous k */
          (*spheres)[i + 1] = xrange;
          (*s_radii)[i + 1] = radii;
          (*s_layers)[i + 1] = slices;
        }
      else if (i > min_nuc_size)
        {
          (*spheres)[i - 1] = xrange;
          (*s_radii)[i - 1] = radii;
          (*s_layers)[i - 1] = slices;
        }

      for (j = slices; j >= 0; --j)
        {
          const float zf2 = z_factor * z_factor;
          const int r = (int) sqrt(R2 - zf2 * j * j);
          initialize_slice(xrange, radii, j, slices, zf2, r);
        } /* done going through the slices */

    }

}
