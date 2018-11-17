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

#ifndef SN__MAX_SCAN_Y_H
#define SN__MAX_SCAN_Y_H 1

#include "colors.h"
#include "frame_t.h"
#include "MarkCentroid.h"

void
max_scan_y(uint8_t * const * const backup_stack, const int i,
    const int my_image_width, const int j, const int step1,
    const int width_end1, int * const stat_plane, int * const centroids,
    const frame_t * const frame, const int in, const int out,
    const int report_max)
;

#endif /* ! SN__MAX_SCAN_Y_H */
