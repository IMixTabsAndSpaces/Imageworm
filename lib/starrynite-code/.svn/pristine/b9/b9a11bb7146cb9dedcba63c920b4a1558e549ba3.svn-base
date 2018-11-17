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

#include "IdentityAssignment.h"
#include <stdio.h>
#include <string.h>
#include "logfile.h"
#include "nucleus_t.h"
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "SisterID.h"
#include "RotateAxis.h"
#include "InitialID.h"
#include "cell-identity.h"
#include "NewBornID.h"

/* TODO: Fix parameter set; depends on InitialID */
void
IdentityAssignment(const movie_t * const movie, const tool_t * const tools,
    parameter_t * const parameters, int * const lineage_ct_p,
    const int num_timepts)
{
  int t;
  int start;
  int lin_ct;
  int rotate_axis = 1;

  /* initiation */
  if (InitialID(movie, tools, parameters, &start, lineage_ct_p, num_timepts)
      != 0)
    {
      fprintf(sn_logfile, "InitialID failed");
    }

  lin_ct = *lineage_ct_p;
  for (t = start; t < num_timepts; t++)
    {
      nucleus_t * const nuclei = tools->nuclei_record[t];
      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct = tools->nuc_ct_rec[t];
      int i;

      if (rotate_axis != 0 && nuc_ct > 40)
        {
          RotateAxis(parameters);
          rotate_axis = 0;
        }

      for (i = 0; i < nuc_ct; i++)
        {
          if (nuclei[i].is_valid == 0)
            continue;

          if (nuclei[i].identity[0] == '\0')
            {
              if (nuclei[i].has_predecessor == 0)
                {
                  lin_ct++;
                  (void) snprintf(nuclei[i].identity, NUCLEUS_IDENTITY_SIZE,
                  "Nuc%d", lin_ct);
                }
              else
                {
                  nucleus_t * const pred = nuclei_prev + nuclei[i].predecessor;
                  if (nuclei[i].id_tag == '\0')
                    {
                      memcpy(nuclei[i].identity, pred->identity, sizeof(char)
                          * 50);
                    }
                  else
                    NewBornID(parameters, pred, nuclei + pred->successor1,
                        nuclei + pred->successor2);
                }
            }

          if (nuclei[i].has_successor2 != 0)
            {
              nucleus_t * const nuclei_next = tools->nuclei_record[t + 1];
              nucleus_t * const nuc1 = &nuclei_next[nuclei[i].successor1];
              nucleus_t * const nuc2 = &nuclei_next[nuclei[i].successor2];
              SisterID(parameters, nuc1, nuc2, nuc_ct, movie->frame.width);
            }
        }

      /* keep track of rotation */
    }
  *lineage_ct_p = lin_ct;
}

