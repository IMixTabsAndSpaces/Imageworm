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

#include "MarkCentroid.h"
#include <stdint.h>

#include "SetPixel.h"
#include "struct_frame.h"
#include "get_pixel_index.h"

void
MarkCentroid(uint8_t * image, const frame_t * const frame, const int x,
    const int y, const int color, const int brightness)
{
  for (int i = -1; i <= 1; ++i)
    {
      for (int j = -1; j <= 1; ++j)
        {
          const int index = get_pixel_index(x + j, y + i, frame->width);
          SetPixel(image, index, color, brightness);
        }
    }
}
