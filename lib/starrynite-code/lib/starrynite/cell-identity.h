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

#ifndef SN__CELL_IDENTITY_H
#define SN__CELL_IDENTITY_H 1

#include <stdio.h>
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"

void
IdentityAssignment(const movie_t *, const tool_t *, parameter_t *,
    int *lineage_ct_p, int num_timepts);

#endif /* ! SN__CELL_IDENTITY_H */
