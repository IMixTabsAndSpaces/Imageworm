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

#include "MakeTools.h"

#include "allocate_sphere_memory.h"
#include "nucleus_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "xalloc.h"

void
MakeTools(const int num_planes, const int num_timepts,
    const parameter_t * const parameters, tool_t * const tools,
    const int image_size)
{
  tools->nuclei_record = (nucleus_t **) xmalloc(sizeof(nucleus_t *)
      * num_timepts);
  tools->nuc_ct_rec = (int *) xmalloc(sizeof(int) * num_timepts);

  tools->density_stack = (int **) xmalloc(sizeof(int *) * num_planes);
  tools->density_stack_2D = (int **) xmalloc(sizeof(int *) * num_planes);

  for (int i = 0; i < num_planes; ++i)
    {
      tools->density_stack_2D[i] = (int *) xmalloc(sizeof(int) * image_size);
      tools->density_stack[i] = (int *) xmalloc(sizeof(int) * image_size);
    }

  tools->density_2D = (int *) xmalloc(sizeof(int) * image_size);
  tools->max_stat = (int **) xmalloc(sizeof(int *) * num_planes);

  for (int i = 0; i < num_planes; ++i)
    {
      tools->max_stat[i] = (int *) xmalloc(sizeof(int) * image_size * 2);
    }

  tools->centroids
      = (int *) xmalloc(sizeof(int) * parameters->cen_ct_limit * 5);
  tools->cen_tmp = (int *) xmalloc(sizeof(int) * parameters->cen_ct_limit * 5);

  tools->noise = (int **) xmalloc(sizeof(int *) * num_timepts);
  tools->noise_ori = (int **) xmalloc(sizeof(int *) * num_timepts);
  tools->max_cutoff = (int **) xmalloc(sizeof(int *) * num_timepts);
  tools->max_cutoff_ori = (int **) xmalloc(sizeof(int *) * num_timepts);

  for (int i = 0; i < num_timepts; i++)
    {
      tools->noise[i] = (int *) xmalloc(sizeof(int) * num_planes);
      tools->noise_ori[i] = (int *) xmalloc(sizeof(int) * num_planes);
      tools->max_cutoff[i] = (int *) xmalloc(sizeof(int) * num_planes);
      tools->max_cutoff_ori[i] = (int *) xmalloc(sizeof(int) * num_planes);
    }

    {
      const int length = 255 * parameters->neighborhood_size
          * parameters->neighborhood_size + 1;
      tools->dist = (int *) xmalloc(sizeof(int) * length);
      tools->dist1 = (int *) xmalloc(sizeof(int) * length);
      tools->dist2 = (int *) xmalloc(sizeof(int) * length);
    }

  allocate_sphere_memory(&tools->spheres, &tools->s_radii, &tools->s_layers,
      parameters->z_factor, parameters->min_nuc_size, parameters->max_nuc_size);

}

