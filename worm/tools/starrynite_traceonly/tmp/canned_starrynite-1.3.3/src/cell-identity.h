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

#ifndef CELL_IDENTITY_H_
#define CELL_IDENTITY_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include "types.h"

void IdentityAssignment (
  const MOVIE_t *,
  const TOOL_t *,
  PARAMETER_t *,
  FILE *,
  int *lineage_ct_p
);

#endif /* CELL_IDENTITY_H_ */
