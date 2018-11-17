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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "metric_calculate_distance_squared.h"
#include "struct_metric.h"
#include "struct_nucleus.h"

float
metric_calculate_distance_squared(const metric_t *const metric,
	const nucleus_t * const nuc1, const nucleus_t * const nuc2)
{
  int xdist, ydist;
  float zdist;

  xdist = nuc1->x - nuc2->x;
  ydist = (nuc1->y - nuc2->y) / metric->width;
  zdist = (nuc1->z - nuc2->z) * metric->z_factor;

  return zdist * zdist + xdist * xdist + ydist * ydist;
}
