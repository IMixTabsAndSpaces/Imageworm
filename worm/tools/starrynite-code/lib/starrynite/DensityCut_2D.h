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

#ifndef SN__DENSITYCUT_2D_H
#define SN__DENSITYCUT_2D_H 1

#include "parameter_t.h"

void
DensityCut_2D(const parameter_t * const parameters, const uint32_t image_width,
    const uint32_t image_height, const uint32_t image_size,
    uint8_t * const image, int *sum, const int cutoff, const int t);

#endif /* ! SN__DENSITYCUT_2D_H */
