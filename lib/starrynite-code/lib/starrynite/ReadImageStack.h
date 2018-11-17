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

#ifndef SN__READIMAGESTACK_H
#define SN__READIMAGESTACK_H 1

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
;

#endif /* ! SN__READIMAGESTACK_H */
