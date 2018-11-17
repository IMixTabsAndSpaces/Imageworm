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

#include "RemoveCell.h"

#include "struct_nucleus.h"
#include "error-codes.h"
#include "logfile.h"
#include "tool_t.h"

void
RemoveCell(const int time_start, const tool_t * const tools, int t, int index,
    int *err_ct_p)
{
  /* const int nuc_ct = tools->nuc_ct_rec[t]; */
  nucleus_t * const nuclei = tools->nuclei_record[t];

  if (nuclei[index].ghost != 0)
    {
      const int pred = nuclei[index].predecessor;
      if (t == 0)
        {
          fprintf(sn_logfile, "ghost cell at first time point: "
            "%d:%d\nExiting.", time_start - 1 + t + 1, index + 1);
          exit(ERROR_REMOVE_CELL1);
        }
      else
        {
          nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];

          if (pred == -1)
            {
              fprintf(sn_logfile, "ghost cell has no predecessor: "
                "%d:%d\nExiting.", time_start - 1 + t + 1, index + 1);
              exit(ERROR_REMOVE_CELL2);
            }

          if (nuclei_prev[pred].ghost)
            {
              fprintf(sn_logfile, "ghost cell has ghost predecessor: "
                "%d:%d <- %d:%d\nExiting\n", time_start - 1 + t + 1, index + 1,
                  time_start - 1 + t, pred + 1);
              exit(ERROR_REMOVE_CELL3);
            }

          nuclei[index].is_valid = 0;
          nuclei[index].ghost = 3;
          fprintf(sn_logfile, "ghost %d:%d removed\n", time_start - 1 + t + 1,
              index + 1);

          if (nuclei_prev[pred].has_successor2 == 0)
            {
              RemoveCell(time_start, tools, t - 1, pred, err_ct_p);
            }
          else
            {
              fprintf(sn_logfile,
                  "Cell division nullified, %d:%d -> %d:%d, %d:%d\n",
                  time_start - 1 + t, pred + 1, time_start - 1 + t + 1,
                  nuclei_prev[pred].successor1 + 1, time_start - 1 + t + 1,
                  nuclei_prev[pred].successor2 + 1);
              if (index == nuclei_prev[pred].successor1)
                {
                  nuclei_prev[pred].successor1 = nuclei_prev[pred].successor2;
                }
              nuclei_prev[pred].successor2 = -1;
              nuclei_prev[pred].has_successor2 = 0;
              *err_ct_p = *err_ct_p + 1;
              nuclei_prev[pred].ghost_proof = 1;
              fprintf(sn_logfile, "%d:%d becomes ghost proof\n", time_start - 1
                  + t, pred + 1);
            }
        }
    } /* nuclei[index].ghost != 0 */
  else
    {
      /* nuclei[index].ghost == 0 */
      const int pred = nuclei[index].predecessor;

      if (t == 0 || pred == -1 || nuclei[index].pskip)
        {
          nuclei[index].is_valid = 0;
          *err_ct_p = *err_ct_p + 1;
          if (t)
            fprintf(sn_logfile, "Emerging cell removed, %d:%d\n", time_start
                - 1 + t + 1, index + 1);
          fprintf(sn_logfile, "False positive removed, %d:%d\n", time_start - 1
              + t + 1, index + 1);
        }
      else
        {
          nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
          if (nuclei_prev[pred].has_successor2)
            {
              nuclei[index].is_valid = 0;
              *err_ct_p = *err_ct_p + 1;
              fprintf(sn_logfile, "False positive removed, %d:%d\n", time_start
                  - 1 + t + 1, index + 1);
              fprintf(sn_logfile,
                  "Cell division nullified, %d:%d -> %d:%d, %d:%d\n",
                  time_start - 1 + t, pred + 1, time_start - 1 + t + 1,
                  nuclei_prev[pred].successor1 + 1, time_start - 1 + t + 1,
                  nuclei_prev[pred].successor2 + 1);
              if (index == nuclei_prev[pred].successor1)
                {
                  nuclei_prev[pred].successor1 = nuclei_prev[pred].successor2;
                }
              nuclei_prev[pred].successor2 = -1;
              nuclei_prev[pred].has_successor2 = 0;
            }
          else if (nuclei_prev[pred].ghost)
            {
              nuclei[index].is_valid = 0;
              *err_ct_p = *err_ct_p + 1;
              fprintf(sn_logfile, "False positive removed, %d:%d\n", time_start
                  - 1 + t + 1, index + 1);
              RemoveCell(time_start, tools, t - 1, pred, err_ct_p);
            }
          else
            {
              nuclei[index].ghost_proof = 1;
              fprintf(sn_logfile, "%d:%d becomes ghost proof\n", time_start - 1
                  + t + 1, index + 1);
              fprintf(sn_logfile, "Cell death pronounced, %d:%d\n", time_start
                  - 1 + t + 1, index + 1);
            }
        }
    }
}
