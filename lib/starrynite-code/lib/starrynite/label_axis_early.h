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

#ifndef SN__LABEL_AXIS_EARLY_H
#define SN__LABEL_AXIS_EARLY_H 1

#include <assert.h>
#include "struct_nucleus.h"

/* Updates id_tag of nuc2 based on the values of lr, dv, and nuc1->id_tag. */
void
label_axis_early(const int lr, const int dv, nucleus_t * const nuc1,
    nucleus_t * const nuc2)
;

#endif /* ! SN__LABEL_AXIS_EARLY_H */