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

#include "CompileDensityDistrib.h"
#include <stdint.h>

void
CompileDensityDistrib(int * const dist, const int ns,
    const uint32_t image_width, const uint32_t image_height,
    const int *weights, const int length)
{
  const int step = ns / 2;

  const int pix_begin = step * image_width;
  const int pix_end = (image_height - step) * image_width;

  int i;

  for (i = 0; i < length; i++)
    dist[i] = 0;

  for (i = pix_begin; i < pix_end; i++)
    {
      const int brightness = weights[i];
      if (brightness != 0)
        dist[brightness] = dist[brightness] + 1;
    }

    {
      const int left = 200 * ns * ns;
      for (i = left; i < length; i++)
        dist[i] = 0;
    }

  dist[0] = 0;
}
