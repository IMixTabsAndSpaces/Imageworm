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

#ifndef SLIDING_BOX_H_
#define SLIDING_BOX_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include "types.h"

/* TODO: Can we add restrict keyword to AddRow and SubtractRow? */
void AddRow (
  int *const a,
  const int *const b,
  const int end
);
void SubtractRow (
  int *const a,
  const int *const b,
  const int end
);

void LocalWeightScan_2D (
  const uint8_t *,
  int *sum,
  int *sum1,
  const FRAME_t *,
  int w
);

/* Pointers in the TOOL_t are updated to return weights. */
void LocalWeightScan_3D (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  const FRAME_t *,
  int xy_size,
  int z_size
);

#endif /* SLIDING_BOX_H_ */
