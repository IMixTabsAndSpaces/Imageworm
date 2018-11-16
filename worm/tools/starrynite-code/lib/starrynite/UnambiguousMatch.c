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

#include "UnambiguousMatch.h"
#include <stdlib.h>

#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "error-codes.h"

#include "ExtendBranch.h"
#include "Bifurcation_2.h"
#include "Bifurcation_1.h"
#include "struct_nucleus.h"

void
UnambiguousMatch(const int time_start, const parameter_t * const parameters,
    const tool_t * const tools, const int t)
{
  /* mutual matches */
  int j;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  const nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
  nucleus_t * const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;

  for (j = 0; j < nuc_ct; j++)
    {
      const partner_t * const partners = nuclei[j].p_partners;
      int k;
      int pred = -1;

      /* find 1<-1 mapping */
      if (nuclei[j].is_valid == 0)
        continue;

      for (k = 0; k < nuc_ct_prev; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
            {
              pred = partners[k].index;
              nuclei[j].predecessor = partners[k].index;
              nuclei[j].distance_to_predecessor = partners[k].distance;
              break;
            }
        }

      if (nuclei[j].pskip)
        {
          continue;
        }
      else if (nuclei[j].potential_predecessor_count != 1)
        {
          continue;
        }
      /* error check */
      else if (pred == -1)
        {
          fprintf(sn_logfile, "predecessor not found, %d:%d\n", time_start - 1
              + t + 1, j + 1);
          exit(ERROR_UNAMBIGUOUSMATCH1);
        }
      else if (!nuclei_prev[pred].is_valid || !nuclei_prev[pred].s_ct)
        {
          fprintf(sn_logfile, "predecessor dead, %d:%d <- %d:%d\n", time_start
              - 1 + t + 1, j + 1, time_start - 1 + t, pred + 1);
          exit(ERROR_UNAMBIGUOUSMATCH2);
        }
      else if (nuclei_prev[pred].sskip)
        {
          fprintf(sn_logfile, "predecessor s_skipped, %d:%d <- %d:%d\n",
              time_start - 1 + t + 1, j + 1, time_start - 1 + t, pred + 1);
          exit(ERROR_UNAMBIGUOUSMATCH3);
        }
      /* recording unambiguous predecessors */
      else if (nuclei_prev[pred].s_ct == 1)
        {
          if (nuclei_prev[pred].s_ct2 == 1)
            ExtendBranch(tools->nuclei_record, t, pred, j); /* 1<->1 mapping */
        }
      else
        {
          if (nuclei_prev[pred].s_ct2 == 1)
            Bifurcation_1(tools->nuclei_record, t, pred, j);
          else
            Bifurcation_2(tools->nuclei_record, time_start, t, pred, j); /* if s_ct2 > 2, skip */
        }
    }
}
