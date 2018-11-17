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

#ifndef SN__ALLOCATE_SPHERE_MEMORY_H
#define SN__ALLOCATE_SPHERE_MEMORY_H 1

#include <math.h>
#include "initialize_slice.h"
#include "xalloc.h"

void
allocate_sphere_memory(int **** const spheres, int *** const s_radii,
    int ** const s_layers, const float z_factor, const int min_nuc_size,
    const int max_nuc_size)
;

#endif /* ! SN__ALLOCATE_SPHERE_MEMORY_H */
