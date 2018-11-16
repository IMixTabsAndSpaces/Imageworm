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

#ifndef SN__EXTEND_NUCLEI_H
#define SN__EXTEND_NUCLEI_H 1

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "struct_nucleus.h"
#include "xalloc.h"

/** Extends the length of a (nucleus_t *) array by one.
 *
 * Extend the length of the (nucleus_t *) array pointed to by nuclei_ptr
 * by one, storing the new length in the location pointed to by
 * nuc_ct_ptr. */
void
extend_nuclei(nucleus_t ** const nuclei_ptr, int * const nuc_ct_ptr)
;

#endif /* ! SN__EXTEND_NUCLEI_H */
