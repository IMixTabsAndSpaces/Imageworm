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

#ifndef SN__RECORD_MAX_H
#define SN__RECORD_MAX_H 1

#include "colors.h"
#include "frame_t.h"
#include "MarkCentroid.h"

void
record_max(uint8_t * const * const backup_stack, int * const stat_plane,
    const int base, const int k, const int * const plane, const int max,
    const int weight_max_cutoff, const int report_max, const int i,
    const frame_t * const frame)
;

#endif /* ! SN__RECORD_MAX_H */
