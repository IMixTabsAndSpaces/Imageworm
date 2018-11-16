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

#ifndef SN__LOCALDENSITYSTAT_H
#define SN__LOCALDENSITYSTAT_H 1

#include <stdint.h>
#include "SetCutoff.h"
#include "Smoothing.h"
#include "CompileDensityDistrib.h"

/* LocalDensityStat uses these references from tools:
 * tools->density_stack_2D[plane]
 * tools->dist
 * tools->dist1
 * tools->dist2
 * tools->noise_ori
 * tools->max_cutoff_ori
 */
void
LocalDensityStat(const int neighborhood_size, const float noise_fraction,
    const uint32_t image_width, const uint32_t image_height, const int t,
    const int plane, int * const * const density_stack_2D, int * const dist,
    int * const dist1, int * const dist2, int * const * const noise_ori,
    int * const * const max_cutoff_ori)
;

#endif /* ! SN__LOCALDENSITYSTAT_H */
