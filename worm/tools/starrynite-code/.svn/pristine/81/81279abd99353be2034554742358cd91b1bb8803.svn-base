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

#ifndef SN__IMAGE_INFO_T_H
#define SN__IMAGE_INFO_T_H 1

#include <stdint.h>

typedef struct image_info image_info_t;

typedef struct image_info image_t;

void
image_init(image_t *, uint32_t width, uint32_t height);

uint32_t
image_width_get(const image_t *);

uint32_t
image_height_get(const image_t *);

uint32_t
image_size_get(const image_t *);

uint8_t *
image_bytes_get(const image_t *);

#endif /* ! SN__IMAGE_INFO_T_H */
