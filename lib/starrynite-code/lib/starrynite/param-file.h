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

#ifndef SN__PARAM_FILE_H
#define SN__PARAM_FILE_H

#include <stdio.h>

typedef struct ParamFile ParamFile;

ParamFile *
ReadParameterFile(FILE *);

struct ParamFile
{
  float ambiguity_cutoff;
  int cell_ct_limit;
  float expand_elastisity;
  int graphic_output;
  int max_nuc_size;
  float max_weight_cutoff;
  int minimal_cell_cycle;
  int neighborhood_size;
  float noise_factor;
  float noise_factor1;
  float noise_factor2;
  float noise_factor3;
  float noise_fraction;
  int noise_time_switch1;
  int noise_time_switch2;
  float nuc_density_cutoff;
  float nuc_density_cutoff1;
  float nuc_density_cutoff2;
  float nuc_density_cutoff3;
  int nuc_size;
  float nuc_size_factor1;
  float nuc_size_factor2;
  float nuc_size_factor3;
  float nuc_size_factor4;
  int plane_end;
  int plane_start;
  int polar_size;
  float shrink_elastisity;
  int time_end;
  int time_interval;
  int time_start;
  float xy_res;
  float z_res;
};

#endif /* ! SN__PARAM_FILE_H */
