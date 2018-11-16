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

#include "CleanUpTools.h"

#include "tool_t.h"

void
CleanUpTools(tool_t * const tools, const int num_planes, const int num_timepts,
    const int max_nuc_size, const int min_nuc_size)
{
  int i;

  const int upper = max_nuc_size * 2;

  for (i = 0; i < num_timepts; ++i)
    {
      free(tools->nuclei_record[i]);
    }

  for (i = 0; i < num_planes; ++i)
    {
      free(tools->image_stack[i]);
      free(tools->density_stack[i]);
      free(tools->density_stack_2D[i]);
      free(tools->max_stat[i]);
    }

  free(tools->nuclei_record);
  free(tools->nuc_ct_rec);
  free(tools->image_stack);
  free(tools->density_stack);
  free(tools->density_stack_2D);
  free(tools->density_2D);
  free(tools->max_stat);
  free(tools->centroids);
  free(tools->cen_tmp);

  for (i = min_nuc_size; i < upper; i += 2)
    {
      int j;
      const int slices = tools->s_layers[i];
      int **xrange = tools->spheres[i];
      for (j = 0; j <= 2 * slices; ++j)
        {
          free(xrange[j]);
        }
      free(xrange);
      free(tools->s_radii[i]);
    }

  free(tools->spheres);
  free(tools->s_layers);
  free(tools->s_radii);
}

