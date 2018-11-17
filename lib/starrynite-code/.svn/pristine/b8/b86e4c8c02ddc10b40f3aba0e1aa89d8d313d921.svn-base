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
#include <stdint.h>

#include "NucReSize.h"

#include "Crust.h"
#include "logfile.h"
#include "frame_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "NucWeight.h"
#include "NucReCenter.h"
#include "struct_nucleus.h"

int
NucReSize(const parameter_t * const parameters, const int time_start,
    const tool_t * const tools,

    const uint8_t * const * const image_stack, frame_t * frame,
    nucleus_t * const nuc, int index, const int t, const int last_change)
{

  int z = (int) (nuc->z + 0.5);

  float noise = parameters->nuc_density_cutoff * tools->noise[t][z]
      / (parameters->neighborhood_size * parameters->neighborhood_size);

  float nuc_ave = 1.0 * nuc->weight / nuc->volume;
  float crust_ave = Crust(tools, image_stack, frame, nuc);

  int change;
  int ori_nuc_size = 0;

  if (last_change == 0)
    {
      ori_nuc_size = nuc->size;
    }

  if (crust_ave < parameters->shrink_elastisity * nuc_ave /*|| crust_ave <= noise */)
    {
      change = -2;
    }
  else if (crust_ave > parameters->expand_elastisity * nuc_ave && crust_ave
      > noise)
    {
      change = 2;
    }
  else
    {
      change = 0;
    }

  if (nuc->size + change > parameters->max_nuc_size || nuc->size + change
      < parameters->min_nuc_size)
    {
      change = 0;
    }

  if (change != 0)
    {
      nuc->size += change;

      if (NucReCenter(tools, image_stack, frame, nuc) == 0)
        {
          return 0;
        }
      if (NucWeight(tools, image_stack, frame, nuc) == 0)
        {
          return 0;
        }

      if (nuc->size < (int) (1.1 * parameters->nuc_size) && nuc->size
          > parameters->min_nuc_size && last_change + change)
        {
          NucReSize(parameters, time_start, tools, image_stack, frame, nuc,
              index, t, change);
        }
    }

  if (last_change == 0)
    {
      if (nuc->size > ori_nuc_size * 2)
        {
          fprintf(sn_logfile, "Nuclear size over twice the "
            "average/expected size: tp %d, nuc %d", time_start + parameters->t
              + 1, index + 1);
        }
      else if (nuc->size < ori_nuc_size / 3)
        {
          fprintf(sn_logfile, "Nuclear size less than one third "
            "the average/expected size: tp %d, nuc %d", time_start
              + parameters->t + 1, index + 1);
        }
    }

  return 1;
}
