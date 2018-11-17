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

#ifndef IMAGE_MANIPULATION_H_
#define IMAGE_MANIPULATION_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <stdbool.h>

#include "types.h"

void ReadImageStack (
  const MOVIE_t *,
  const char *input_prefix,
  int tp,
  int plane_start,
  uint8_t ** image_stack,
  bool is_first_tiff,
  uint32_t * p_image_width,
  uint32_t * p_image_height
);

void GetImageStacks (
  const MOVIE_t *,
  const PARAMETER_t *,
  TOOL_t *,
  const char *input_prefix,
  bool is_first_tiff,
  /*@out@ */ uint32_t * p_image_width,
  /*@out@ */ uint32_t * p_image_height
);

/* Output tools */

/* MarkCentroid() and DrawCircle() are called by DrawNuclei().
 *
 * DrawNuclei() will probably change with data structure (eg, for CELL),
 * but MarkCentroid() and DrawCircle() are likely to remain the same.
 */

void TurnGreen (
  const MOVIE_t *,
  uint32_t,
  uint8_t ** image_stack
);

void MarkCentroid (
  uint8_t * image,
  const FRAME_t * const frame,
  int x,
  int y,
  int color,
  int brightness
);

/* Draws the model sphere for defined nuclei */

void DrawNuclei (
  const MOVIE_t *,
  const TOOL_t *,
  uint8_t ** image_stack,
  FRAME_t * frame,
  int t,
  int thickness,
  int brightness,
  int color,
  int markcen
);

#endif /* IMAGE_MANIPULATION_H_ */
