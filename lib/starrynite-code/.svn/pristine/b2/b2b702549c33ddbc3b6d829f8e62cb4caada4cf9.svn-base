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

#include "MissingCells.h"
#include "RemoveCell.h"
#include "extend_nuclei.h"
#include "initialize_ghost.h"
#include "tool_t.h"
#include "struct_nucleus.h"

/* TODO: Fix parameter set; depends on AddGhostNucleus, RemoveCell */
int
MissingCells(const int time_start, const tool_t * const tools, const int t)
{
  nucleus_t ** const nuclei_record = tools->nuclei_record;
  int * const nuc_ct_rec = tools->nuc_ct_rec;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];

  int err_ct = 0;

  for (int i = 0; i < nuc_ct_prev; i++)
    {
      if (nuclei_prev[i].is_valid == 0)
        continue;
      if (nuclei_prev[i].sskip != 0)
        continue;
      if (nuclei_prev[i].has_successor1 != 0)
        {
          if (nuclei_prev[i].ghost == 2)
            {
              int ppred = nuclei_prev[i].predecessor;
              fprintf(sn_logfile,
                  "Cell resusicated, %d:%d to %d:%d to %d:%d\n", time_start - 1
                      + t - 1, ppred + 1, time_start - 1 + t, i + 1, time_start
                      - 1 + t + 1, nuclei_prev[i].successor1 + 1);
              nuclei_prev[i].ghost = 3;
            }
        }
      else
        {
          /* missing successor */
          if (!nuclei_prev[i].ghost)
            {
              if (!nuclei_prev[i].ghost_proof)
                {
                  fprintf(sn_logfile,
                      "Adding ghost cell %d:%d to succeed %d:%d\n", time_start
                          - 1 + t + 1, tools->nuc_ct_rec[t] + 1, time_start - 1
                          + t, i + 1);
                    {

                      extend_nuclei(nuclei_record + t, nuc_ct_rec + t);

                        {
                          nucleus_t * const nuclei = nuclei_record[t];
                          const int nuc_ct = nuc_ct_rec[t];

                          initialize_ghost(&(nuclei[nuc_ct - 1]), &(nuclei_prev[i]));
                        }
                      err_ct++;
                    }
                }
            }
          else if (nuclei_prev[i].ghost == 2)
            {
              RemoveCell(time_start, tools, t - 1, i, &err_ct);
            }
        }
    }

  return err_ct;
}
