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

#ifndef SN__SMOOTHINGNOISEMODEL_H
#define SN__SMOOTHINGNOISEMODEL_H 1

/************************************
 * Averaging noise model over time *
 ***********************************/

void
SmoothingNoiseModel(int * const * const noise, int * const * const noise_ori,
    int * const * const max_cutoff, int * const * const max_cutoff_ori,
    const int num_planes, const int num_timepts);

#endif /* ! SN__SMOOTHINGNOISEMODEL_H */
