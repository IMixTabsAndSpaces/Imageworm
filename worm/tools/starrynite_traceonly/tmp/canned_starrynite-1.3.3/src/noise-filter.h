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

#ifndef NOISE_FILTER_H_
#define NOISE_FILTER_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include "types.h"

/* set noise cutoff for each plane in a z-stack */
void CalibrateNoise_3D (
  const TOOL_t *,
  const PARAMETER_t *,
  const MOVIE_t *,
  uint32_t,
  uint32_t,
  uint32_t,
  uint8_t ** image_stack,
  int t
);

void SmoothingNoiseModel (
  const TOOL_t *,
  const PARAMETER_t *,
  const MOVIE_t *
);

void UpdateNoise (
  const TOOL_t *,
  const PARAMETER_t *,
  const MOVIE_t *,
  int t
);

void LocalDensityCut_3D (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  uint32_t image_width,
  uint32_t image_height,
  uint32_t image_size,
  uint8_t ** image_stack,
  int t
);

#endif /* NOISE_FILTER_H_ */
