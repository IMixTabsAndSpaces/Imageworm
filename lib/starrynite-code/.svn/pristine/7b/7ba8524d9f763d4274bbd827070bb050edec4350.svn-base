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

#ifndef SN__PARAMETER_T_H
#define SN__PARAMETER_T_H 1

typedef struct parameter_t parameter_t;

struct parameter_t
{
  struct
  {
    int time_start, time_end, num_timepts;
    int plane_start, plane_end;
    float xy_res, z_res;
    int t_interval;
  } movie;

  int graphic_output;
  int neighborhood_size;
  int *sizes;
  int nuc_size;
  int polar_size;
  int max_nuc_size;
  int min_nuc_size;
  int nuc_size1;
  int nuc_size2;
  int nuc_size3;
  int nuc_size4;
  float nuc_size_factor1;
  float nuc_size_factor2;
  float nuc_size_factor3;
  float nuc_size_factor4;
  float z_res_fudge;
  float z_factor;
  int z_size1, cube_size;
  int cen_ct_limit;
  int t;
  int noise_time_switch1;
  int noise_time_switch2;
  float noise_fraction;
  float max_weight_cutoff;
  float nuc_density_cutoff;
  float noise_factor;
  float nuc_density_cutoff1;
  float noise_factor1;
  float nuc_density_cutoff2;
  float noise_factor2;
  float nuc_density_cutoff3;
  float noise_factor3;
  int axis;
  int ap;
  int lr;
  int dv;
  int minimal_cell_cycle;
  float ambiguity_cutoff;
  float shrink_elastisity;
  float expand_elastisity;
};

#endif /* ! SN__PARAMETER_T_H */
