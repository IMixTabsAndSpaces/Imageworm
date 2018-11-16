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

#include "SkipComplexity.h"

#include "struct_nucleus.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "logfile.h"
#include "partner_t.h"

void
SkipComplexity(const int time_start, const parameter_t * const parameters,
    const tool_t * const tools, int t)
{
  /* mark and skip 1 -> >2 matches */
  int i, j, k;
  int nuc_ct_prev, nuc_ct;
  nucleus_t *nuclei_prev, *nuclei;
  int suc, pred;
  partner_t *partners;
  int newct;
  const float cutoff = 2.0 * parameters->nuc_size;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  for (i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
        continue;
      if (!nuclei_prev[i].sskip)
        {
          if (nuclei_prev[i].s_ct2 > 2 || nuclei_prev[i].s_ct
              - nuclei_prev[i].s_ct2 > 2)
            {
              if (nuclei_prev[i].s_ct2 > 2)
                fprintf(sn_logfile, "S_Skipping %d:%d, >2 hard successors\n",
                    time_start - 1 + t, i + 1);
              else
                fprintf(sn_logfile, "S_Skipping %d:%d, >2 soft successors\n",
                    time_start - 1 + t, i + 1);
              nuclei_prev[i].sskip = 1;
            }
        }
      if (nuclei_prev[i].sskip)
        {
          fprintf(sn_logfile, "P_Skipping based on %d:%d:", time_start - 1 + t,
              i + 1);
          partners = nuclei_prev[i].s_partners;
          for (k = 0; k < nuc_ct; k++)
            {
              if (partners[k].distance > cutoff)
                break;
              if (partners[k].availability)
                {
                  suc = partners[k].index;
                  nuclei[suc].pskip = 1;
                  fprintf(sn_logfile, " %d:%d,", time_start - 1 + t + 1, suc
                      + 1);
                }
            }
          fprintf(sn_logfile, "\n");
        }
    }

  newct = 1;
  while (newct)
    {
      newct = 0;
      for (j = 0; j < nuc_ct; j++)
        {
          if (!nuclei[j].is_valid)
            continue;
          if (nuclei[j].pskip)
            {
              partners = nuclei[j].p_partners;
              for (k = 0; k < nuc_ct_prev; k++)
                {
                  if (partners[k].distance > cutoff)
                    break;
                  if (partners[k].availability)
                    {
                      pred = partners[k].index;
                      if (!nuclei_prev[pred].sskip)
                        {
                          newct++;
                          nuclei_prev[pred].sskip = 1;
                          fprintf(sn_logfile,
                              "S_skipping %d:%d due to %d:%d\n", time_start - 1
                                  + t, pred + 1, time_start - 1 + t + 1, j + 1);
                        }
                    }
                }
            }
        }

      for (i = 0; i < nuc_ct_prev; i++)
        {
          if (!nuclei_prev[i].is_valid)
            continue;
          if (nuclei_prev[i].sskip)
            {
              partners = nuclei_prev[i].s_partners;
              for (k = 0; k < nuc_ct; k++)
                {
                  if (partners[k].distance > cutoff)
                    break;
                  if (partners[k].availability)
                    {
                      suc = partners[k].index;
                      if (!nuclei[suc].pskip)
                        {
                          newct++;
                          nuclei[suc].pskip = 1;
                          fprintf(sn_logfile,
                              "P_skipping %d:%d due to %d:%d\n", time_start - 1
                                  + t + 1, suc + 1, time_start - 1 + t, i + 1);
                        }
                    }
                }
            }
        }
    }
}
