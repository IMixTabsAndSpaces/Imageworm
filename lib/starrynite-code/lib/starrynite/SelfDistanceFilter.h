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

#ifndef SN__SELFDISTANCEFILTER_H
#define SN__SELFDISTANCEFILTER_H 1

#include "nucleus_t.h"
#include "frame_t.h"
#include "parameter_t.h"
#include "metric_calculate_distance.h"
#include "struct_metric.h"
#include "init_metric.h"

void
SelfDistanceFilter(const parameter_t * const parameters, nucleus_t * nuclei,
    int nuc_ct, frame_t * frame)
;

#endif /* ! SN__SELFDISTANCEFILTER_H */