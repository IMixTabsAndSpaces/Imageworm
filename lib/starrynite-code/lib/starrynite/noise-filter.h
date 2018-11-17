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

#ifndef SN__NOISE_FILTER_H
#define SN__NOISE_FILTER_H 1

#include <stdio.h>
#include <stdint.h>

#include "frame_t.h"
#include "parameter_t.h"

/* set noise cutoff for each plane in a z-stack */
void
CalibrateNoise_3D(int * const *, int *, int * const *, int * const *, int,
    float, const frame_t *, uint32_t, uint32_t,
    const uint8_t * const *image_stack, int t);

void
SmoothingNoiseModel(int * const *, int * const *, int * const *, int * const *,
    int, int);

void
UpdateNoise(int * const *, float, int, int t);

void
    LocalDensityCut_3D(const frame_t *, const parameter_t *,
        uint32_t image_width, uint32_t image_height, uint32_t image_size,
        uint8_t ** image_stack, int t, int * const * density_stack_2D,
        int * density_2D, const int * const * noise);

void
report_noise_model(

int num_timepts, int time_start, int num_planes, const int * const *max_cutoff,
    const int * const *max_cutoff_ori, const int * const *noise,
    const int * const *noise_ori);

#endif /* ! SN__NOISE_FILTER_H */
