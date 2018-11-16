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

#include "RefineNuclei.h"
#include <stdint.h>
#include "NucInit.h"
#include "MeasureMorphology.h"
#include "nucleus_t.h"
#include "frame_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "NucWeight.h"
#include "NucReCenter.h"
#include "DensityFilter.h"
#include "SelfDistanceFilter.h"
#include "NucReSize.h"

/* TODO: Fix parameter set; depends on NucReCenter, NucWeight, NucReSize,
 * MeasureMorphology, DensityFilter, SelfDistanceFilter */
void
RefineNuclei(const tool_t * const tools, const parameter_t * const parameters,
    const int time_start, const uint8_t * const *image_stack, frame_t * frame,
    nucleus_t * nuclei, int t)
{
  int cen_ct, i;

  cen_ct = tools->cen_ct;
  for (i = 0; i < cen_ct; i++)
    {
      NucInit(tools->centroids, parameters, nuclei + i, t, i);
      if (!NucReCenter(tools, image_stack, frame, nuclei + i))
        continue;
      if (!NucWeight(tools, image_stack, frame, nuclei + i))
        continue;
      if (!NucReSize(parameters, time_start, tools, image_stack, frame, nuclei
          + i, i, t, 0))
        continue;
      nuclei[i].max_weight = nuclei[i].weight;
      nuclei[i].max_size = nuclei[i].size;
      MeasureMorphology(tools, image_stack, frame, nuclei + i);
    }
    {
      const float cutoff_factor = parameters->nuc_density_cutoff
          / (parameters->neighborhood_size * parameters->neighborhood_size);
      DensityFilter(nuclei, cen_ct, cutoff_factor, tools->noise[t]);
    }
  SelfDistanceFilter(parameters, nuclei, cen_ct, frame);
}
