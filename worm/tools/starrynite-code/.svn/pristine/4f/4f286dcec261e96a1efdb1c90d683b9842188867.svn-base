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

#include "EvaluateLinks.h"

#include "struct_nucleus.h"
#include "partner_t.h"
#include "error-codes.h"

#include "EvaluateDivision.h"

void
EvaluateLinks(nucleus_t * const nuclei, const int nuc_ct,
    const nucleus_t * const nuclei_prev, const int nuc_ct_prev,
    const float cutoff, const int time_start, const int t)
{
  /* evaluate divisions */
  for (int i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
        continue;
      if (nuclei_prev[i].s_ct < 2)
        continue;
      if (nuclei_prev[i].s_ct > 2)
        {
          fprintf(sn_logfile, "Breaking multiple hard suc's for %d:%d",
              time_start - 1 + t, i + 1);
          partner_t * const partners = nuclei_prev[i].s_partners;
          for (int k = 0; k < nuc_ct; k++)
            {
              if (partners[k].distance > cutoff)
                break;
              if (partners[k].availability)
                {
                  const int suc = partners[k].index;
                  nuclei[suc].p_ambiguity = 1;
                  fprintf(sn_logfile, ", %d:%d", time_start - 1 + t + 1, suc
                      + 1);
                }
            }
          fprintf(sn_logfile, "\n");
        }
      else
        {
          const int dau1_index = nuclei_prev[i].successor1;
          const int dau2_index = nuclei_prev[i].successor2;
          if (nuclei[dau1_index].potential_predecessor_count > 1 || nuclei[dau2_index].potential_predecessor_count > 1)
            {
              fprintf(sn_logfile,
                  "Daughter cell already ambiguous for %d:%d.  Exiting.\n",
                  time_start - 1 + t, i + 1);
              exit(ERROR_EVALUATELINKS);
            }
          fprintf(sn_logfile,
              "Evaluating division: %2d:%-3d -> %2d:%-3d, %2d:%-3d,",
              time_start - 1 + t, i + 1, time_start - 1 + t + 1,
              dau1_index + 1, time_start - 1 + t + 1, dau2_index + 1);

          if (0)
            {
              fprintf(sn_logfile, " %2d %2d", t - 1
                  - (nuclei_prev + i)->max_weight_time, t - 1 - (nuclei_prev
                  + i)->max_size_time);
            }

            {
              const nucleus_t * const n_mother = nuclei_prev + i;
              const nucleus_t * const n_dau1 = nuclei + dau1_index;
              const nucleus_t * const n_dau2 = nuclei + dau2_index;

              const int score = EvaluateDivision(n_mother, n_dau1, n_dau2,
                  nuc_ct);

              fprintf(sn_logfile, "   %2d", score);
              if (score < 7)
                {
                  fprintf(sn_logfile, "    open");
                  nuclei[dau1_index].p_ambiguity = 1;
                  nuclei[dau2_index].p_ambiguity = 1;
                }
              fprintf(sn_logfile, " \n");
            }
        }
    }

#if 0
  /* evaluate missing successors, first or second */
  for (i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
      continue;
      if (nuclei_prev[i].s_ct >= 2)
      continue;
      if (!nuclei_prev[i].s_ct)
        {
          /* apparent death */
          fprintf(sn_logfile, "opening ambiguity for cell death: %d:%d\n",
              time_start - 1 + t, i + 1);
          nuclei_prev[i].s_ambiguity = 1;
        }
      else
        {
          /* apparent non-dividing cell */
          partners = nuclei_prev[i].s_partners;
          for (k = 0; k < nuc_ct; k++)
            {
              if (partners[k].distance> cutoff)
              break;
              if (partners[k].availability)
                {
                  suc = partners[k].index;
                  /* apparent division without second successor */
                  if ((nuclei_prev[i].weight -
                          nuclei[suc].weight) / nuclei_prev[i].weight> 0.3)
                    {
                      if (t - nuclei_prev[i].time_start> 3
                          && nuclei_prev[i].time_end - t> 3)
                        {
                          fprintf(sn_logfile,
                              "opening ambiguity for second daughter: %d:%d -> %d:%d\n",
                              time_start - 1 + t, i + 1,
                              time_start - 1 + t + 1, suc + 1);
                          nuclei_prev[i].s_ambiguity = 1;
                        }
                    }
                  break;
                }
            }
        }
    }
#endif
}
