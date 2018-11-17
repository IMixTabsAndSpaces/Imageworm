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

#include "ReadImageStack.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "get_image_pathname.h"
#include "error-codes.h"
#include "tiff-io.h"
#include "struct_image_info.h"

void
ReadImageStack(const int num_planes, const char * const input_prefix,
    const int tp, const int plane_start, uint8_t ** const image_stack,
    const bool is_first_tiff, uint32_t * const image_width_record_p,
    uint32_t * const image_height_record_p)
{
  assert(input_prefix != NULL);
  assert(image_stack != NULL);
  assert(image_width_record_p != NULL);
  assert(image_height_record_p != NULL);

  for (int i = 0; i < num_planes; i++)
    {
      char *const pathname = get_image_pathname(input_prefix, tp, 
	      plane_start + i);

      assert (pathname != NULL);

      image_info_t image_info = ReadTiff(pathname);

      image_stack[i] = image_info.bytes;

      free(pathname);

      if (is_first_tiff)
        {
          *image_width_record_p = image_info.width;
          *image_height_record_p = image_info.height;
        }
      else if (image_info.width != *image_width_record_p
	      || image_info.height != *image_height_record_p)
        {
          const char str[] = "Different size of image for different planes";
          (void) fprintf(stderr, "ERROR: %s\n", str);
          exit(ERROR_READIMAGESTACK);
        }
    }
}
