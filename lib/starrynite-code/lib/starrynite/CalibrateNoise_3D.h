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

#ifndef SN__CALIBRATENOISE_3D_H
#define SN__CALIBRATENOISE_3D_H 1

#include <stdint.h>

#include "frame_t.h"

/* Sets noise cutoff for each plane in a z-stack
 * CalibrateNoise_3D uses these references from tools:
 * density_stack_2D
 * density_2D
 * max_cutoff_ori
 * noise_ori
 */
void
CalibrateNoise_3D(int * const * const density_stack_2D, int * const density_2D,
    int * const * const noise_ori, int * const * const max_cutoff_ori,
    const int neighborhood_size, const float noise_fraction,
    const frame_t * const frame, const uint32_t image_width,
    const uint32_t image_height, const uint8_t * const * const image_stack,
    const int t);

#endif /* ! SN__CALIBRATENOISE_3D_H */
