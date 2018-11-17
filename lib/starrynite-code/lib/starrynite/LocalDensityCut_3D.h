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

#ifndef SN__LOCALDENSITYCUT_3D_H
#define SN__LOCALDENSITYCUT_3D_H 1

#include <stdint.h>
#include "frame_t.h"
#include "parameter_t.h"

/** TODO: Add documentation.
 * LocalDensityCut_3D uses these references from tools:
 * density_stack_2D
 * density_2D
 * noise
 */
void
LocalDensityCut_3D(const frame_t * const frame, const parameter_t * parameters,
    const uint32_t image_width, const uint32_t image_height,
    const uint32_t image_size, uint8_t ** image_stack, const int t,
    int * const * const density_stack_2D, int * const density_2D,
    const int * const * const noise);

#endif /* ! SN__LOCALDENSITYCUT_3D_H */
