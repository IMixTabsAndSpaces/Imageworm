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

#include "DensityFilter.h"
#include "struct_nucleus.h"

void
DensityFilter(nucleus_t * nuclei, int nuc_ct, const float cutoff_factor,
    const int * const noise_at_time_by_plane)
{
  for (int j = 0; j < nuc_ct; j++)
    {
      if (nuclei[j].is_valid == 0)
        continue;

      /* TODO: Fix cast */
      const int z = (int) (nuclei[j].z + 0.5);

      const float cutoff = cutoff_factor * noise_at_time_by_plane[z];

      /* TODO: Fix cast */
      if (1.0 * nuclei[j].weight / nuclei[j].volume < cutoff)
        {
          nuclei[j].is_valid = 0;
        }
    }
}
