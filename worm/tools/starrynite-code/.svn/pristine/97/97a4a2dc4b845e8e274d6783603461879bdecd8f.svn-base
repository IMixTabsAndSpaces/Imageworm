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

#include "ReduceAmbiguity.h"

#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "UpdateCounts.h"
#include "ScoringDivision.h"
#include "RemoveLink.h"
#include "compare_morphology.h"
#include "compare_partner_distance.h"
#include "struct_nucleus.h"

void
ReduceAmbiguity(const int time_start, const parameter_t * const parameters,
    const tool_t * const tools, const int t)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
  nucleus_t * const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
    {
      if (nuclei_prev[i].is_valid == 0)
        continue;
      if (nuclei_prev[i].s_ct2 == 2)
        {
          if (nuclei_prev[i].s_ct > 2)
            {
              int k;
              partner_t * const partners = nuclei_prev[i].s_partners;
              fprintf(sn_logfile, "Removing successors for %d:%d :", time_start
                  - 1 + t, i + 1);
              for (k = 0; k < nuc_ct; k++)
                {
                  if (partners[k].distance > cutoff)
                    break;
                  if (partners[k].availability)
                    {
                      int suc = partners[k].index;
                      if (nuclei[suc].potential_predecessor_count > 1)
                        {
                          fprintf(sn_logfile, " %d:%d,",
                              time_start - 1 + t + 1, suc + 1);
                          nuclei_prev[i].s_ct--;
                          partners[k].availability = 0;
                          nuclei[suc].potential_predecessor_count--;
                          RemoveLink(nuclei[suc].p_partners, i, nuc_ct_prev);
                          if (nuclei[suc].potential_predecessor_count == 1)
                            UpdateCounts(parameters, tools, t, suc);
                        }
                    }
                }
              fprintf(sn_logfile, "\n");
            }
        }
      else if (nuclei_prev[i].s_ct2 == 1)
        {
          if (nuclei_prev[i].s_ct > 2)
            {
              int sis = -1;
              int k;
              partner_t * const partners = nuclei_prev[i].s_partners;
              fprintf(sn_logfile, "Removing successors for %d:%d :\n",
                  time_start - 1 + t, i + 1);
              for (k = 0; k < nuc_ct; k++)
                {
                  if (partners[k].distance > cutoff)
                    break;
                  if (partners[k].availability)
                    {
                      int suc = partners[k].index;
                      if (nuclei[suc].potential_predecessor_count != 1)
                        continue;
                      sis = suc;
                      partners[k].score2m = -10000;
                      break;
                    }
                }
              assert (sis != -1);
              fprintf(sn_logfile, "    hard suc %d:%d\n", time_start - 1 + t
                  + 1, sis + 1);
              for (k = 0; k < nuc_ct; k++)
                {
                  if (partners[k].distance > cutoff)
                    break;
                  if (partners[k].availability)
                    {
                      int suc = partners[k].index;
                      if (nuclei[suc].potential_predecessor_count == 1)
                        continue;
                      fprintf(sn_logfile, "    scoring soft suc %d:%d, ",
                          time_start - 1 + t + 1, suc + 1);
                      partners[k].score2m = ScoringDivision(nuclei_prev + i,
                          nuclei + sis, nuclei + suc);
                      fprintf(sn_logfile,
                          ", with score %d and distance %.2f\n",
                          partners[k].score2m, partners[k].distance);
                    }
                }
              qsort(nuclei_prev[i].s_partners, nuc_ct, sizeof(partner_t),
                  compare_morphology);
              for (k = 0; k < nuc_ct; k++)
                {
                  int suc = partners[k].index;
                  if (!partners[k].availability)
                    break;
                  if (nuclei[suc].potential_predecessor_count == 1)
                    break;
                  if (k < 1)
                    continue;
                  fprintf(sn_logfile, "    %d:%d removed", time_start - 1 + t
                      + 1, suc + 1);
                  nuclei_prev[i].s_ct--;
                  partners[k].availability = 0;
                  nuclei[suc].potential_predecessor_count--;
                  RemoveLink(nuclei[suc].p_partners, i, nuc_ct_prev);
                  if (nuclei[suc].potential_predecessor_count == 1)
                    UpdateCounts(parameters, tools, t, suc);
                }
              fprintf(sn_logfile, "\n");
              qsort(nuclei_prev[i].s_partners, nuc_ct, sizeof(partner_t),
                  compare_partner_distance);
            }
        }
      else if (nuclei_prev[i].s_ct2 == 0)
        {
          if (nuclei_prev[i].s_ct > 2)
            {
              partner_t * const partners = nuclei_prev[i].s_partners;
              int s_ct = 0;
              int k;
              fprintf(sn_logfile, "Removing successors for %d:%d :\n",
                  time_start - 1 + t, i + 1);
              for (k = 0; k < nuc_ct; k++)
                {
                  int suc = partners[k].index;
                  if (partners[k].distance > cutoff)
                    break;
                  if (!partners[k].availability)
                    continue;
                  s_ct++;
                  if (s_ct < 3)
                    continue;
                  fprintf(sn_logfile, "    %d:%d removed", time_start - 1 + t
                      + 1, suc + 1);
                  nuclei_prev[i].s_ct--;
                  partners[k].availability = 0;
                  nuclei[suc].potential_predecessor_count--;
                  RemoveLink(nuclei[suc].p_partners, i, nuc_ct_prev);
                  if (nuclei[suc].potential_predecessor_count == 1)
                    UpdateCounts(parameters, tools, t, suc);
                }
              fprintf(sn_logfile, "\n");
            }
        }
    }
}
