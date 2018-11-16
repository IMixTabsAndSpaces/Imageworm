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

#include "ExtendBranch.h"
#include "struct_nucleus.h"

void
ExtendBranch(nucleus_t * const * const nuclei_record, const int t,
    const int p_index, const int s_index)
{
  nucleus_t * const pred = nuclei_record[t - 1] + p_index;
  nucleus_t * const suc = nuclei_record[t] + s_index;

  pred->successor1 = s_index;
  pred->distance_to_successor1 = suc->distance_to_predecessor;
  pred->has_successor1 = 1;
  if (!pred->condensed && suc->condensed)
    {
      pred->condensed = suc->condensed - 1;
    }

  suc->has_predecessor = 1;
  suc->time_start = pred->time_start;
  suc->generation = pred->generation;
  if (!suc->condensed && pred->condensed)
    {
      suc->condensed = pred->condensed - 1;
    }

  if (suc->max_weight < pred->max_weight)
    {
      suc->max_weight = pred->max_weight;
      suc->max_weight_time = pred->max_weight_time;
    }
  if (suc->max_size < pred->max_size)
    {
      suc->max_size = pred->max_size;
      suc->max_size_time = pred->max_size_time;
    }
}
