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

#include "ResetBranch.h"

#include "struct_nucleus.h"
#include "tool_t.h"

void
ResetBranch(const tool_t * const tools, int t)
{
  nucleus_t *nuclei, *nuc;
  int nuc_ct;
  int j;

  nuc_ct = tools->nuc_ct_rec[t];
  nuclei = tools->nuclei_record[t];

  for (j = 0; j < nuc_ct; j++)
    {
      nuc = nuclei + j;
      if (!nuc->is_valid)
        continue;

      nuc->time_start = t;
      nuc->time_end = t;

      nuc->max_size = nuc->size;
      nuc->max_size_time = t;
      nuc->max_weight = nuc->weight;
      nuc->max_weight_time = t;

      nuc->s_ct2 = 0;
      nuc->predecessor = -1;
      nuc->successor1 = -1;
      nuc->successor2 = -1;
      nuc->distance_to_predecessor = -1.0;
      nuc->distance_to_successor1 = -1.0;
      nuc->distance_to_successor2 = -1.0;
      nuc->has_predecessor = 0;
      nuc->has_successor1 = 0;
      nuc->has_successor2 = 0;
    }
}
