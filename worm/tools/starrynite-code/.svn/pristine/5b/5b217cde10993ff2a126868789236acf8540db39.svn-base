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
#include <stdlib.h>

#include "movie_t.h"
#include "TurnGreen.h"
#include "xalloc.h"

void
TurnGreen(const movie_t * const movie, const uint32_t image_size,
    uint8_t ** image_stack)
{
  const int num_planes = movie->frame.num_planes;
  const size_t RGBsize = image_size * 3;

  for (int i = 0; i < num_planes; i++)
    {
      uint8_t * const RGBimage = calloc(RGBsize, sizeof(uint8_t));
      uint8_t * const Grayimage = image_stack[i];

      for (uint32_t j = 0; j < image_size; j++)
        {
          const uint8_t brightness = Grayimage[j];
          uint8_t colormix;

          if (brightness > 32)
            {
              colormix = (brightness - 32) / 4;
            }
          else
            {
              colormix = 0;
            }

          RGBimage[j * 3] = colormix;
          RGBimage[j * 3 + 1] = brightness;
          RGBimage[j * 3 + 2] = colormix;
        }
      image_stack[i] = RGBimage;
      free(Grayimage);
    }
}
