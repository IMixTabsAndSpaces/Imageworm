/* Copyright (C) 2005-2008 University of Washington
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

#ifndef SN__LOOKUP_PARAMETER_H
#define SN__LOOKUP_PARAMETER_H

enum parameter_code
{
  TIME_START,
  TIME_END,
  PLANE_START,
  PLANE_END,
  XY_RES,
  Z_RES,
  TIME_INTERVAL,
  GRAPHIC_OUTPUT,
  NEIGHBORHOOD_SIZE,
  CELL_CT_LIMIT,
  NUC_SIZE,
  POLAR_SIZE,
  MAX_NUC_SIZE,
  NUC_SIZE_FACTOR1,
  NUC_SIZE_FACTOR2,
  NUC_SIZE_FACTOR3,
  NUC_SIZE_FACTOR4,
  NOISE_FRACTION,
  MAX_WEIGHT_CUTOFF,
  NUC_DENSITY_CUTOFF,
  NUC_DENSITY_CUTOFF1,
  NUC_DENSITY_CUTOFF2,
  NUC_DENSITY_CUTOFF3,
  NOISE_FACTOR,
  NOISE_FACTOR1,
  NOISE_FACTOR2,
  NOISE_FACTOR3,
  NOISE_TIME_SWITCH1,
  NOISE_TIME_SWITCH2,
  MINIMAL_CELL_CYCLE,
  AMBIGUITY_CUTOFF,
  SHRINK_ELASTISITY,
  EXPAND_ELASTISITY
};

struct parameter
{
  const char *name;
  enum parameter_code code;
};

const struct parameter *
lookup_parameter(const char *str, unsigned int len);

#endif /* ! SN__LOOKUP_PARAMETER_H */
