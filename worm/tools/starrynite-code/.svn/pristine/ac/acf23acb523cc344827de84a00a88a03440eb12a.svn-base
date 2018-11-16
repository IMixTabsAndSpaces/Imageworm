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

#include <math.h>

#include "SetupInitialFrame.h"
#include "struct_frame.h"

void
SetupInitialFrame(const float z_factor, const int image_width,
    const int image_height, const int image_size, frame_t * const frame,
    float * const max_distance)
{
  /* Unused:
   * movie->frame.x_start = 0;
   * movie->frame.y_start = 0;
   * movie->frame.z_start = 0; */
  frame->width = image_width;
  frame->height = image_height;
  frame->image_size = image_size;

    {
      const int w_squared = image_width * image_width;
      const int h_squared = image_height * image_height;

      const float z_height = (frame->num_planes - 1) * z_factor;
      const float z_squared = z_height * z_height;

      const float sum_squares = w_squared + h_squared + z_squared;

      *max_distance = sqrtf(sum_squares);
    }
}

