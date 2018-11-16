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
#include <assert.h>
#include "vector_subtract_vector.h"
#include "LocalWeightScan_2D.h"
#include "AddRow.h"
#include "struct_frame.h"

void
LocalWeightScan_2D(const uint8_t * const image, const frame_t * const frame,
    const int w, int * const sum, int * const sum1)
{

  const int image_width = frame->width;
  const int image_height = frame->height;
  const int image_size = frame->image_size;

  const int step = w / 2;
  const int width_end = image_width - step;
  const int height_end = image_height - step;

  assert (sum != NULL);
  assert (sum1 != NULL);

    {
      /* pad the first "step" rows and initialize the first used row */
      const int pad_length = (step + 1) * image_width;

      for (int i = 0; i < pad_length; i++)
        sum[i] = 0;
    }

  for (int i = 0; i < image_height; i++)
    {
      /* head of the row */
      const int base = i * image_width;
      int sum_tmp;

      /* pad for the left end of each row */
      for (int j = 0; j < step; j++)
        {
          sum1[base + j] = 0;
        }

      /* first window in each row */
      sum_tmp = 0;
      for (int j = 0; j < w; j++)
        {
          sum_tmp += image[base + j];
        }
      sum1[base + step] = sum_tmp;

      /* rest of the row */
      for (int j = step + 1; j < width_end; j++)
        {
          sum_tmp -= image[base + j - 1 - step];
          sum_tmp += image[base + j - 1 - step + w];
          sum1[base + j] = (int) sum_tmp;
        }

      /* pad the right end of the row */
      for (int j = width_end; j < image_width; j++)
        {
          sum1[base + j] = 0;
        }

      /* second dimension */
      if (i == 0)
        {
          memcpy(sum + step * image_width, sum1, sizeof(int) * image_width);
        }
      else if (i < w)
        {
          AddRow(sum + step * image_width, sum1 + base, image_width);
        }
      else
        {
          const int target_base = (i - w + step + 1) * image_width;
          memcpy(sum + target_base, sum + target_base - image_width,
              sizeof(int) * image_width);
          vector_subtract_vector(sum + target_base, sum1 + base - w * image_width,
              image_width);
          AddRow(sum + target_base, sum1 + base, image_width);
        }
    }

  /* pad the last rows */
  for (int i = height_end * image_width; i < image_size; i++)
    {
      sum[i] = 0;
    }
}
