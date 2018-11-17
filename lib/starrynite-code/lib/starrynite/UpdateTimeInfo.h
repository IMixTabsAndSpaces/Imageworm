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

#ifndef SN__UPDATETIMEINFO_H
#define SN__UPDATETIMEINFO_H 1

#include "parameter_t.h"

/* TODO: Extract variable component from parameter_t into a separate type */
void
UpdateTimeInfo(parameter_t * const parameters, const float z_xy_res_ratio,
    const int * const nuc_ct_rec, const int t);

#endif /* ! SN__UPDATETIMEINFO_H */
