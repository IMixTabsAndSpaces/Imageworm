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

#include "ResolveAmbiguity.h"
#include "ExtendBranch.h"
#include "Bifurcation_2.h"
#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "struct_nucleus.h"

void
ResolveAmbiguity(const int time_start, const parameter_t * const parameters,
    const tool_t * const tools, int t)
{
  int j, k;

  int nuc_ct_prev, nuc_ct;
  nucleus_t *nuclei_prev, *nuclei;
  partner_t *partners;
  float cutoff;
  int pred, match;
  int max_score, total, score;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (j = 0; j < nuc_ct; j++)
    {
      if (!nuclei[j].is_valid)
        continue;
      if (nuclei[j].potential_predecessor_count <= 1)
        continue;
      if (nuclei[j].pskip)
        continue;
      pred = -1;
      partners = nuclei[j].p_partners;
      total = 0;
      for (k = 0; k < nuc_ct_prev; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (!partners[k].availability)
            continue;
          total += partners[k].score1;
        }
      max_score = -10000;
      match = -1;
      for (k = 0; k < nuc_ct_prev; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (!partners[k].availability)
            continue;
          score = partners[k].score2 + (total - partners[k].score1);
          if (score > max_score)
            {
              match = k;
              max_score = score;
            }
        }
      /* recording resolved predecessors */
      pred = partners[match].index;
      nuclei[j].predecessor = pred;
      nuclei[j].distance_to_predecessor = partners[match].distance;
      if (nuclei_prev[pred].s_ct == 1)
        {
          ExtendBranch(tools->nuclei_record, t, pred, j);
        }
      else
        {
          Bifurcation_2(tools->nuclei_record, time_start, t, pred, j);
        }
    }
}
