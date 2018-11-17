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

#include <string.h>
#include "LocalWeightScan_3D.h"
#include "vector_subtract_vector.h"
#include "LocalWeightScan_2D.h"
#include "AddRow.h"
#include "struct_frame.h"

void
LocalWeightScan_3D(int * const * const weight_stack,
    int * const * const weight_stack_2D, int * const weight_2D,
    const uint8_t * const * const image_stack, const frame_t * const frame,
    const int xy_size, const int z_size)
{
  const int image_size = frame->image_size;
  const int num_planes = frame->num_planes;

  int i, j;
  int step, z_end;

  /* 2D weight for the planes */
  for (i = 0; i < num_planes; i++)
    {
      LocalWeightScan_2D(image_stack[i], frame, xy_size, weight_stack_2D[i],
          weight_2D);
    }

  /* 3D weight for the planes */
  step = z_size / 2;
  /* padding for the first "step" planes */
  for (i = 0; i < step; i++)
    {
      for (j = 0; j < image_size; j++)
        weight_stack[i][j] = 0;
    }
  /* first used plane */
  memcpy(weight_stack[step], *weight_stack_2D, sizeof(int) * image_size);
  for (i = 1; i < z_size; i++)
    {
      AddRow(weight_stack[step], weight_stack_2D[i], image_size);
    }
  /* middle planes */
  z_end = num_planes - step;
  for (i = step + 1; i < z_end; i++)
    {
      memcpy(weight_stack[i], weight_stack[i - 1], sizeof(int) * image_size);
      vector_subtract_vector(weight_stack[i], weight_stack_2D[i - 1 - step], image_size);
      AddRow(weight_stack[i], weight_stack_2D[i - 1 - step + z_size],
          image_size);
    }
  /*last "step" plane */
  for (i = z_end; i < num_planes; i++)
    {
      for (j = 0; j < image_size; j++)
        weight_stack[i][j] = 0;
    }
}

