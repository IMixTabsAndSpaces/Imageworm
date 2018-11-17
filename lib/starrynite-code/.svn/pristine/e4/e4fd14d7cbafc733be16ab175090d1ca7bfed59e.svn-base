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
#include "parameter_t.h"
#include "DensityCut_2D.h"

/********************
 * filtering images *
 ********************/
/* TODO: Add documentation */
void
DensityCut_2D(const parameter_t * const parameters, const uint32_t image_width,
    const uint32_t image_height, const uint32_t image_size,
    uint8_t * const image, int *sum, const int cutoff, const int t)
{
  const int step = parameters->neighborhood_size / 2;
  const int width_end = image_width - step;
  const int height_end = image_height - step;
  const int area = parameters->neighborhood_size
      * parameters->neighborhood_size;

  int i;

  /* cleaning up the first "step" rows */
  const int pad_length = step * image_width;
  for (i = 0; i < pad_length; i++)
    {
      image[i] = (uint8_t) 0;
    }

  /* the middle rows */
  for (i = step; i < height_end; i++)
    {
      const int base = i * image_width;
      int j;
      /* cleaning up the first "step" pixels in the row */
      for (j = 0; j < step; j++)
        image[base + j] = (uint8_t) 0;
      /* processing the middle of the row */
      for (j = step; j < width_end; j++)
        {
          if (sum[base + j] <= cutoff)
            image[base + j] = (uint8_t) 0;
          else if (t < 30)
            image[base + j] = (uint8_t) (sum[base + j] / area); /* smoothing! */
        }
      /* cleaning up the last "step" pixels in the row */
      for (j = width_end; j < image_width; j++)
        image[base + j] = (uint8_t) 0;
    }

  /* cleaning up the last "step" rows */
    {
      const int base = height_end * image_width;
      for (i = base; i < image_size; i++)
        {
          image[i] = (uint8_t) 0;
        }
    }
}

