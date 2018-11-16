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
#include <assert.h>

#include "NucReCenter.h"

#include "struct_frame.h"
#include "tool_t.h"
#include "struct_nucleus.h"

int
NucReCenter(const tool_t * const tools,
    const uint8_t * const * const image_stack, frame_t * frame,
    nucleus_t * const nuc)
{
  const int num_planes = frame->num_planes;
  const int my_image_width = frame->width;
  const int my_image_height = frame->height;

  int j;
  int dist;
  int x, y, z;
  int x_sum, y_sum, z_sum;

  int base, x_start, x_end;
  int weight;
  int signal;
  int average;

  /* equivalent to parameters->nuc_size here */
  const int nuc_size = nuc->size;

  int * const * const x_range = tools->spheres[nuc_size];
  const int * const radii = tools->s_radii[nuc_size];
  const int slices = tools->s_layers[nuc_size];

  int base_line = 0;
  x = nuc->x;
  y = nuc->y;
  z = (int) nuc->z;
  average = nuc->weight / nuc->volume;

  weight = 0;
  x_sum = 0;
  y_sum = 0;
  z_sum = 0;

  /* base_line is set at least once */
  assert (slices> 0);

  for (j = 0 - slices; j <= slices; j++)
    {
      if (z + j < 0 || z + j >= num_planes)
        continue;
      else
        {
          const int r = radii[slices + j];
          const uint8_t * const image = image_stack[z + j];
          int k;
          int weight_z = 0;
          base_line = y / my_image_width;
          for (k = 0 - r; k <= r; k++)
            {
              int m;
              int weight_y;
              if (base_line + k < 0)
                continue;
              if (base_line + k >= my_image_height)
                break;
              weight_y = 0;
              base = y + k * my_image_width;
              dist = x_range[slices + j][r + k];
              x_start = x - dist > 0 ? x - dist : 0;
              x_end = x + dist < my_image_width ? x + dist : my_image_width;
              for (m = x_start; m <= x_end; m++)
                {
                  signal = (int) image[base + m];
                  if (signal < average)
                    continue;
                  x_sum += (m - x) * signal;
                  weight_y += signal;
                }
              y_sum += k * weight_y;
              weight_z += weight_y;
            }
          z_sum += j * weight_z;
          weight += weight_z;
        }
    }

  if (weight == 0)
    {
      nuc->is_valid = 0;
      return 0;
    }

  nuc->x = ((int) (1.0 * x_sum / weight + 0.5)) + x;
  nuc->y = (((int) (1.0 * y_sum / weight + 0.5)) + base_line) * my_image_width;
  nuc->z = 1.0 * z_sum / weight + z;

  return 1;
}
