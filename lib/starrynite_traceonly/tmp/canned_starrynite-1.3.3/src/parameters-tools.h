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

#ifndef PARAMETERS_TOOLS_H_
#define PARAMETERS_TOOLS_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "types.h"

void SetParameters (
  MOVIE_t *,
  PARAMETER_t *,
  const char *filename
);
void CalculateSizeParameters (
  PARAMETER_t *
);
void UpdateNucSize (
  const TOOL_t *,
  PARAMETER_t *,
  FILE *,
  int t
);
void UpdateTimeInfo (
  PARAMETER_t *,
  const MOVIE_t *,
  const TOOL_t *,
  int t
);
void SetOriginalFrame (
  MOVIE_t *,
  const PARAMETER_t *,
  int,
  int,
  int
);
void MakeTools (
  const MOVIE_t *,
  const PARAMETER_t *,
  TOOL_t *,
  int image_size
);
void CleanUpTools (
  const MOVIE_t *,
  const PARAMETER_t *,
  TOOL_t *
);

#endif /* PARAMETERS_TOOLS_H_ */
