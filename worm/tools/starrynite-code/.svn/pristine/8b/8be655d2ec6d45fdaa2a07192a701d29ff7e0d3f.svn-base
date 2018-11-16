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

#include <assert.h>
#include <stdlib.h>

#include "FinalizeParameters.h"

#include "param-file.h"
#include "parameter_t.h"
#include "xalloc.h"

parameter_t *
FinalizeParameters(const ParamFile * const input)
{
  parameter_t *p;

  assert (input != NULL);

  p = xmalloc(sizeof(parameter_t));
  assert (p != NULL);

  /* Map unmodified parameter values */

  p->ambiguity_cutoff = input->ambiguity_cutoff;
  p->cen_ct_limit = input->cell_ct_limit;
  p->expand_elastisity = input->expand_elastisity;
  p->graphic_output = input->graphic_output;
  p->max_nuc_size = input->max_nuc_size;
  p->max_weight_cutoff = input->max_weight_cutoff;
  p->neighborhood_size = input->neighborhood_size;
  p->noise_factor = input->noise_factor;
  p->noise_factor1 = input->noise_factor1;
  p->noise_factor2 = input->noise_factor2;
  p->noise_factor3 = input->noise_factor3;
  p->noise_fraction = input->noise_fraction;
  p->noise_time_switch1 = input->noise_time_switch1;
  p->noise_time_switch2 = input->noise_time_switch2;
  p->nuc_density_cutoff = input->nuc_density_cutoff;
  p->nuc_density_cutoff1 = input->nuc_density_cutoff1;
  p->nuc_density_cutoff2 = input->nuc_density_cutoff2;
  p->nuc_density_cutoff3 = input->nuc_density_cutoff3;
  p->nuc_size = input->nuc_size;
  p->nuc_size_factor1 = input->nuc_size_factor1;
  p->nuc_size_factor2 = input->nuc_size_factor2;
  p->nuc_size_factor3 = input->nuc_size_factor3;
  p->nuc_size_factor4 = input->nuc_size_factor4;
  p->movie.plane_end = input->plane_end;
  p->movie.plane_start = input->plane_start;
  p->polar_size = input->polar_size;
  p->shrink_elastisity = input->shrink_elastisity;
  p->movie.time_end = input->time_end;
  p->movie.t_interval = input->time_interval;
  p->movie.time_start = input->time_start;
  p->movie.xy_res = input->xy_res;
  p->movie.z_res = input->z_res;

  /* Calculate some parameter values */

  p->z_factor = input->z_res / input->xy_res;
  p->min_nuc_size = (int) (p->z_factor * 2.2);

  p->minimal_cell_cycle = input->minimal_cell_cycle / input->time_interval;
  p->movie.num_timepts = input->time_end - input->time_start + 1;

  /*@-mustfreeonly@ */
  p->sizes = (int *) xmalloc(sizeof(int) * p->movie.num_timepts);
  /*@+mustfreeonly@ */
  assert (p->sizes != NULL);

  p->sizes[0] = input->nuc_size;

  return p;
}

