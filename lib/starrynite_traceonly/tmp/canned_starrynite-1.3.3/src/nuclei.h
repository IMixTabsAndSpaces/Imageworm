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

#ifndef NUCLEI_H_
#define NUCLEI_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include "types.h"

float NucDistance (
  const PARAMETER_t *,
  const NUCLEUS_t *,
  const NUCLEUS_t *,
  const FRAME_t *
);

void DefineNuclei (
  const MOVIE_t *,
  PARAMETER_t *,
  TOOL_t *,
  uint8_t **,
  FILE *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t ** nuc_rec,
  int *nuc_ct_p,
  int t
);

void RecoverNuclei (
  const MOVIE_t *,
  PARAMETER_t *,
  TOOL_t *,
  uint8_t ** backup_stack,
  FILE * logfile,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  int t
);

void LoadNuclei (
  const MOVIE_t *,
  PARAMETER_t *,
  TOOL_t *,
  uint8_t **,
  FILE *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t ** nuc_rec,
  int *nuc_ct_p,
  int t,
  const char *const filename,
    const char *const filename2
);

void
LoadCentroids (
	TOOL_t * const tools,
	const char *const filename,
	int my_image_width
	);

#endif /* NUCLEI_H_ */
