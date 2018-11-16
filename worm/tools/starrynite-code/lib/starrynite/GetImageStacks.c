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

#include "GetImageStacks.h"
#include "ReadImageStack.h"
#include "xalloc.h"

uint8_t **
GetImageStacks(const int num_planes, const int time_start,
    const int plane_start, const int t, const char * const input_prefix,
    const bool is_first_tiff, uint32_t * const image_width_record_p,
    uint32_t * const image_height_record_p)
{
  uint8_t ** const image_stack = (uint8_t **) xmalloc(sizeof(uint8_t *)
      * num_planes);

  /* have to allocate *(image_stack+i) in ReadTiff (or later)
   * because we don't yet know the value of image_size */
  ReadImageStack(num_planes, input_prefix, time_start + t, plane_start,
      image_stack, is_first_tiff, image_width_record_p, image_height_record_p);

  return image_stack;
}
