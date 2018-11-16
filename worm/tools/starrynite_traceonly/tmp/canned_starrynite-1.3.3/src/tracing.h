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

#ifndef TRACING_H_
#define TRACING_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "types.h"

void Trace (
  const MOVIE_t *,
  PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int
);

void RemoveGhostNuclei (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *
);

#endif /* !TRACING_H_ */
