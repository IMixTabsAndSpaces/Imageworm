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

#include <stdint.h>

#include "NucWeight.h"

#include "struct_frame.h"
#include "tool_t.h"
#include "struct_nucleus.h"

int
NucWeight(const tool_t * const tools,
    const uint8_t * const * const image_stack, frame_t * frame, nucleus_t * nuc)
{
  const int num_planes = frame->num_planes;
  const int my_image_width = frame->width;
  const int my_image_height = frame->height;

  const int x = nuc->x;
  const int y = nuc->y;
  const int z = (int) (nuc->z + 0.5);
  const int nuc_size = nuc->size;

  int * const * const x_range = tools->spheres[nuc_size];
  const int * const radii = tools->s_radii[nuc_size];
  const int slices = tools->s_layers[nuc_size];

  int weight = 0;
  int volume = 0;

  for (int j = 0 - slices; j <= slices; j++)
    {
      if (z + j < 0 || z + j >= num_planes)
        continue;
      const int r = radii[slices + j];
      const uint8_t * const image = image_stack[z + j];
      const int base_line = y / my_image_width;
      for (int k = 0 - r; k <= r; k++)
        {
          if (base_line + k < 0)
            continue;
          if (base_line + k >= my_image_height)
            break;
          const int base = y + k * my_image_width;
          const int dist = x_range[slices + j][r + k];
          const int x_start = x - dist > 1 ? x - dist : 1;
          const int x_end = x + dist < my_image_width - 1 ? x + dist
              : my_image_width - 1;
          for (int m = x_start; m <= x_end; m++)
            {
              weight += (int) image[base + m];
              volume++;
            }
        }
    }

  nuc->weight = weight;
  nuc->volume = volume;

  if (volume == 0)
    {
      nuc->is_valid = 0;
      return 0;
    }
  return 1;
}
