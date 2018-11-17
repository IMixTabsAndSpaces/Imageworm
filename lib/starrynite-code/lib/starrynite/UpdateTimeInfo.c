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

#include "UpdateTimeInfo.h"

#include "parameter_t.h"

/* TODO: Extract variable component from parameter_t into a separate type */
void
UpdateTimeInfo(parameter_t * const parameters, const float z_xy_res_ratio,
    const int * const nuc_ct_rec, const int t)
{
  parameters->nuc_size = parameters->sizes[t];

  /* z_factor */
  if (t == 0 || nuc_ct_rec[t - 1] < 100)
    {
      parameters->z_factor = z_xy_res_ratio;
    }
  else if (nuc_ct_rec[t - 1] < 170)
    {
      parameters->z_factor = 1.1 * z_xy_res_ratio;
    }
  else if (nuc_ct_rec[t - 1] < 200)
    {
      parameters->z_factor = 1.2 * z_xy_res_ratio;
    }
  else
    {
      parameters->z_factor = 1.3 * z_xy_res_ratio;
    }

  /* noise cutoff */
  if (t == 0 || nuc_ct_rec[t - 1] <= parameters->noise_time_switch1)
    {
      parameters->nuc_density_cutoff = parameters->nuc_density_cutoff1;
      parameters->noise_factor = parameters->noise_factor1;
    }
  else if (nuc_ct_rec[t - 1] <= parameters->noise_time_switch2)
    {
      parameters->nuc_density_cutoff = parameters->nuc_density_cutoff2;
      parameters->noise_factor = parameters->noise_factor2;
    }
  else
    {
      parameters->nuc_density_cutoff = parameters->nuc_density_cutoff3;
      parameters->noise_factor = parameters->noise_factor3;
    }
}
