/* Copyright (C) 2005-2008 University of Washington
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
#endif

#include <string.h>
#include <stdint.h>

#include "sliding-box.h"
#include "types.h"

void
AddRow (int *const a, const int *const b, const int end)
{
  int i;

  for (i = 0; i < end; ++i)
  {
    a[i] += b[i];
  }
}


void
SubtractRow (int *const a, const int *const b, const int end)
{
  int i;

  for (i = 0; i < end; ++i)
  {
    a[i] -= b[i];
  }
}

/*
 * Called by LocalWeightScan_3D.  calculating the weight of each pixel in a 2D image,
 * where weight is the sum of brightness values of neighboring pixels
 * (within a square of size 'w' for 2D, and a cube for 3D).
 *
 * The weight can also be understood as the weight of a square/cube
 * centered at a given pixel.
 *
 * The most straightforward implementation would be that for each pixel in
 * the image, add up the brightness value of the neighboring pixels one by
 * one, which is O(w*w*w*N) where N is # of total pixels).
 *
 * Current implementation uses a sliding window, which is O(6N)), with 2N for each dimension
 * Similar idea also used to define centroids */

void
LocalWeightScan_2D (const uint8_t * const image, int *const sum,
                    int *const sum1, const FRAME_t * const frame, const int w)
{
  int i, j, base, target_base;
  int sum_tmp;

  const int my_image_width = frame->width;
  const int my_image_height = frame->height;
  const int my_image_size = frame->image_size;

  const int step = w / 2;
  const int width_end = my_image_width - step;
  const int height_end = my_image_height - step;


  {
    /* padding the first "step" rows and initializing the first used row */

    const int pad_length = (step + 1) * my_image_width;

    for (i = 0; i < pad_length; i++)
      sum[i] = 0;
  }

  for (i = 0; i < my_image_height; i++)
  {
    base = i * my_image_width;  /* head of the row */
    /* padding for the left end of each row */
    for (j = 0; j < step; j++)
      sum1[base + j] = 0;

    /* first window in each row */
    sum_tmp = 0;
    for (j = 0; j < w; j++)
    {
      sum_tmp += image[base + j];
    }
    sum1[base + step] = sum_tmp;
    /* rest of the row */
    for (j = step + 1; j < width_end; j++)
    {
      sum_tmp -= image[base + j - 1 - step];
      sum_tmp += image[base + j - 1 - step + w];
      sum1[base + j] = (int) sum_tmp;
    }
    /* padding for the right end of the row */
    for (j = width_end; j < my_image_width; j++)
      sum1[base + j] = 0;

    /* second dimension */
    if (!i)
      memcpy (sum + step * my_image_width, sum1,
              sizeof (int) * my_image_width);
    else if (i < w)
      AddRow (sum + step * my_image_width, sum1 + base, my_image_width);
    else
    {
      target_base = (i - w + step + 1) * my_image_width;
      memcpy (sum + target_base, sum + target_base - my_image_width,
              sizeof (int) * my_image_width);
      SubtractRow (sum + target_base, sum1 + base - w * my_image_width,
                   my_image_width);
      AddRow (sum + target_base, sum1 + base, my_image_width);
    }
  }
  /* padding the last rows */
  base = height_end * my_image_width;
  for (i = base; i < my_image_size; i++)
    sum[i] = 0;
}



void
LocalWeightScan_3D (const TOOL_t * const tools,
                    const uint8_t * const *const image_stack,
                    const FRAME_t * const frame, const int xy_size,
                    const int z_size)
{
  int *const *const weight_stack = tools->density_stack;
  int *const *const weight_stack_2D = tools->density_stack_2D;
  int *const weight_2D = tools->density_2D;

  const int my_image_size = frame->image_size;
  const int my_plane_number = frame->plane_number;

  int i, j;
  int step, z_end;

  /* 2D weight for the planes */
  for (i = 0; i < my_plane_number; i++)
  {
    LocalWeightScan_2D (image_stack[i], weight_stack_2D[i],
                        weight_2D, frame, xy_size);
  }

  /* 3D weight for the planes */
  step = z_size / 2;
  /* padding for the first "step" planes */
  for (i = 0; i < step; i++)
  {
    for (j = 0; j < my_image_size; j++)
      weight_stack[i][j] = 0;
  }
  /* first used plane */
  memcpy (weight_stack[step], *weight_stack_2D, sizeof (int) * my_image_size);
  for (i = 1; i < z_size; i++)
  {
    AddRow (weight_stack[step], weight_stack_2D[i], my_image_size);
  }
  /* middle planes */
  z_end = my_plane_number - step;
  for (i = step + 1; i < z_end; i++)
  {
    memcpy (weight_stack[i], weight_stack[i - 1],
            sizeof (int) * my_image_size);
    SubtractRow (weight_stack[i], weight_stack_2D[i - 1 - step],
                 my_image_size);
    AddRow (weight_stack[i], weight_stack_2D[i - 1 - step + z_size],
            my_image_size);
  }
  /*last "step" plane */
  for (i = z_end; i < my_plane_number; i++)
  {
    for (j = 0; j < my_image_size; j++)
      weight_stack[i][j] = 0;
  }
}
