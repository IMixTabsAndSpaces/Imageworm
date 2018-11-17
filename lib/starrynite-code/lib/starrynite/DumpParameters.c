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

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "DumpParameters.h"

#include "parameter_t.h"

void
DumpParameters(const parameter_t * const parameters)
{
  assert (parameters != NULL);
  assert (parameters->sizes != NULL);

  (void) printf("movie\n");
  (void) printf("\ttime_start = %d\n", parameters->movie.time_start);
  (void) printf("\ttime_end = %d\n", parameters->movie.time_end);
  (void) printf("\tnum_timepts = %d\n", parameters->movie.num_timepts);
  (void) printf("\tplane_start = %d\n", parameters->movie.plane_start);
  (void) printf("\tplane_end = %d\n", parameters->movie.plane_end);
  (void) printf("\txy_res = %f\n", parameters->movie.xy_res);
  (void) printf("\tz_res = %f\n", parameters->movie.z_res);
  (void) printf("\tt_interval = %d\n", parameters->movie.t_interval);

  (void) printf("graphic_output = %d\n", parameters->graphic_output);
  (void) printf("neighborhood_size = %d\n", parameters->neighborhood_size);
  (void) printf("nuc_size = %d\n", parameters->nuc_size);
  (void) printf("polar_size = %d\n", parameters->polar_size);
  (void) printf("max_nuc_size = %d\n", parameters->max_nuc_size);
  (void) printf("min_nuc_size = %d\n", parameters->min_nuc_size);
  (void) printf("nuc_size1 = %d\n", parameters->nuc_size1);
  (void) printf("nuc_size2 = %d\n", parameters->nuc_size2);
  (void) printf("nuc_size3 = %d\n", parameters->nuc_size3);
  (void) printf("nuc_size4 = %d\n", parameters->nuc_size4);
  (void) printf("nuc_size_factor1 = %f\n", parameters->nuc_size_factor1);
  (void) printf("nuc_size_factor2 = %f\n", parameters->nuc_size_factor2);
  (void) printf("nuc_size_factor3 = %f\n", parameters->nuc_size_factor3);
  (void) printf("nuc_size_factor4 = %f\n", parameters->nuc_size_factor4);
  (void) printf("z_res_fudge = %f\n", parameters->z_res_fudge);
  (void) printf("z_factor = %f\n", parameters->z_factor);
  (void) printf("z_size1 = %d\n", parameters->z_size1);
  (void) printf("cube_size = %d\n", parameters->cube_size);
  (void) printf("cen_ct_limit = %d\n", parameters->cen_ct_limit);
  (void) printf("t = %d\n", parameters->t);
  (void) printf("noise_time_switch1 = %d\n", parameters->noise_time_switch1);
  (void) printf("noise_time_switch2 = %d\n", parameters->noise_time_switch2);
  (void) printf("noise_fraction = %f\n", parameters->noise_fraction);
  (void) printf("max_weight_cutoff = %f\n", parameters->max_weight_cutoff);
  (void) printf("nuc_density_cutoff = %f\n", parameters->nuc_density_cutoff);
  (void) printf("noise_factor = %f\n", parameters->noise_factor);
  (void) printf("nuc_density_cutoff1 = %f\n", parameters->nuc_density_cutoff1);
  (void) printf("noise_factor1 = %f\n", parameters->noise_factor1);
  (void) printf("nuc_density_cutoff2 = %f\n", parameters->nuc_density_cutoff2);
  (void) printf("noise_factor2 = %f\n", parameters->noise_factor2);
  (void) printf("nuc_density_cutoff3 = %f\n", parameters->nuc_density_cutoff3);
  (void) printf("noise_factor3 = %f\n", parameters->noise_factor3);
  (void) printf("axis = %d\n", parameters->axis);
  (void) printf("ap = %d\n", parameters->ap);
  (void) printf("lr = %d\n", parameters->lr);
  (void) printf("dv = %d\n", parameters->dv);
  (void) printf("minimal_cell_cycle = %d\n", parameters->minimal_cell_cycle);
  (void) printf("ambiguity_cutoff = %f\n", parameters->ambiguity_cutoff);
  (void) printf("shrink_elastisity = %f\n", parameters->shrink_elastisity);
  (void) printf("expand_elastisity = %f\n", parameters->expand_elastisity);

}

