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

#include "DrawSphere.h"
#include <stdint.h>

#include "struct_frame.h"
#include "tool_t.h"
#include "struct_nucleus.h"
#include "SetPixel.h"

void
DrawSphere(const tool_t * const tools, uint8_t ** image_stack, frame_t * frame,
    nucleus_t * nuc, int thickness, int brightness, int color)
{
  int slices, r, *radii, **x_range, dist;
  int i, j, k;
  int x, y, z, base_line, base, pos;
  uint8_t *image;

  int num_planes = frame->num_planes;
  int my_image_width = frame->width;
  int my_image_height = frame->height;

  x = nuc->x;
  y = nuc->y;
  z = (int) (nuc->z + 0.5);

  x_range = tools->spheres[nuc->size];
  radii = tools->s_radii[nuc->size];
  slices = tools->s_layers[nuc->size];

  for (j = 0 - slices; j <= slices; j++)
    {
      if (z + j < 0 || z + j >= num_planes)
        continue;
      r = radii[slices + j];
      image = image_stack[z + j];
      base_line = y / my_image_width;
      for (k = 0 - r; k <= r; k++)
        {
          if (base_line + k < 0)
            continue;
          if (base_line + k >= my_image_height)
            break;
          base = y + k * my_image_width;
          dist = x_range[slices + j][r + k];
          for (pos = x - dist, i = 0; i < thickness; i++, pos++)
            {
              if (pos > 0 && pos < my_image_width)
                SetPixel(image, base + pos, color, brightness);
            }
          for (pos = x + dist, i = 0; i < thickness; i++, pos--)
            {
              if (pos > 0 && pos < my_image_width)
                SetPixel(image, base + pos, color, brightness);
            }
        }
    }
}
