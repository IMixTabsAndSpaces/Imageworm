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

#ifndef SN__IMAGE_STACKS_H
#define SN__IMAGE_STACKS_H 1

#include <stdint.h>
#include <stdbool.h>


uint8_t **
GetImageStacks(int num_planes, int time_start, int plane_start, int t,
    const char *input_prefix, bool is_first_tiff,
    /*@out@*/uint32_t * p_image_width,
    /*@out@*/uint32_t * p_image_height);

typedef/*@only@*/uint8_t *oui8p;

void
FreeImageStack(
/*@only@*//*@out@*/oui8p * image_stack, int num_planes);

#endif /* ! SN__IMAGE_STACKS_H */
