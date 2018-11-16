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

#ifndef SN__REPORT_NOISE_MODEL_H
#define SN__REPORT_NOISE_MODEL_H 1

void
report_noise_model(const int num_timepts, const int time_start,
    const int num_planes, const int * const * const max_cutoff,
    const int * const * const max_cutoff_ori, const int * const * const noise,
    const int * const * const noise_ori);

#endif /* ! SN__REPORT_NOISE_MODEL_H */
