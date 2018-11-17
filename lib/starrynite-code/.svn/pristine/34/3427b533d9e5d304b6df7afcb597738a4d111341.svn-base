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

#include <stdlib.h>

#include "ResetNuclei.h"

#include "struct_nucleus.h"

void
ResetNuclei(nucleus_t * const nuclei, const int nuc_ct, const int t)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      nucleus_t * const nuc = nuclei + i;
      if (nuc->p_partners)
        {
          free(nuc->p_partners);
          nuc->p_partners = NULL;
        }
      if (nuc->s_partners)
        {
          free(nuc->s_partners);
          nuc->s_partners = NULL;
        }
      if (nuc->is_valid == 0)
        continue;

      if (nuc->ghost == 1)
        nuc->ghost = 2;

      nuc->time_start = t;
      nuc->generation = 0;
      nuc->time_end = t;
      nuc->start_state = nucleus_start_state_emerging;
      nuc->end_state = nucleus_end_state_none;
      nuc->start_scorem = -10000;
      nuc->end_scorem = -10000;
      nuc->max_size = nuc->size;
      nuc->max_size_time = t;
      nuc->max_weight = nuc->weight;
      nuc->max_weight_time = t;

      nuc->p_ambiguity = 0;
      nuc->s_ambiguity = 0;
      nuc->pskip = 0;
      nuc->sskip = 0;
      nuc->potential_predecessor_count = 0;
      nuc->s_ct = 0;
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
