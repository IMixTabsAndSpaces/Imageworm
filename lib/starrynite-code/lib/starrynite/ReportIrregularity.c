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

#include "ReportIrregularity.h"

#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "struct_nucleus.h"
#include "logfile.h"

void
ReportIrregularity(const int time_start, const parameter_t * const parameters,
    const tool_t * const tools, int t)
{
  /* 1->0, 0->1 or 1 -> >2 matches */
  int i, j, k;
  int nuc_ct_prev, nuc_ct;
  nucleus_t *nuclei_prev, *nuclei;
  int suc;
  partner_t *partners;
  float cutoff;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
        continue;
      if (nuclei_prev[i].s_ct == 0)
        {
          if (nuclei_prev[i].ghost < 2)
            fprintf(sn_logfile, "Putative cell death (no successor), %d:%d\n",
                time_start - 1 + t, i + 1);
          continue;
        }
      if (nuclei_prev[i].s_ct > 2)
        {
          fprintf(sn_logfile,
              "More than 2 successors (%d of %d hard), %d:%d -> ",
              nuclei_prev[i].s_ct2, nuclei_prev[i].s_ct, time_start - 1 + t, i
                  + 1);
          partners = nuclei_prev[i].s_partners;
          for (k = 0; k < nuc_ct; k++)
            {
              if (partners[k].distance > cutoff)
                break;
              if (partners[k].availability)
                {
                  suc = partners[k].index;
                  fprintf(sn_logfile, "%d:%d with ", time_start - 1 + t + 1,
                      suc + 1);
                  if (nuclei[suc].potential_predecessor_count == 1)
                    fprintf(sn_logfile, "hard ");
                  else
                    fprintf(sn_logfile, "soft ");
                  fprintf(sn_logfile, "distance %.2f; ", partners[k].distance);
                }
            }
          fprintf(sn_logfile, "\n");
          continue;
        }
    }

  for (j = 0; j < nuc_ct; j++)
    {
      if (!nuclei[j].is_valid)
        continue;
      if (!nuclei[j].potential_predecessor_count)
        {
          fprintf(sn_logfile,
              "Emerging cell (no candidate predecessor), %d:%d\n", time_start
                  - 1 + t + 1, j + 1);
          continue;
        }
#if 0
      if (nuclei[j].potential_predecessor_count> 1)
        {
          fprintf(sn_logfile, "Multiple predecessors, %d:%d <- ",
              time_start - 1 + t + 1, j + 1);
          partners = nuclei[j].p_partners;
          for (k = 0; k < nuc_ct_prev; k++)
            {
              if (partners[k].distance> cutoff)
              break;
              if (partners[k].availability)
                {
                  fprintf(sn_logfile, "%d:%d with distance %.2f; ",
                      time_start - 1 + t,
                      partners[k].index + 1, partners[k].distance);
                }
            }
          fprintf(sn_logfile, "\n");
          continue;
        }
#endif
    }
}
