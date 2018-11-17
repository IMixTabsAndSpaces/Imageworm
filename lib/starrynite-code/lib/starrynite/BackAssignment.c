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

#include "BackAssignment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logfile.h"
#include "nucleus_t.h"
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "SisterID.h"
#include "NewBornID.h"

/* TODO: Split this into multiple procedures */
/* TODO: Fix parameter set; depends on SisterID */
int
BackAssignment(const movie_t * const movie, const tool_t * const tools,
    const parameter_t * const parameters, const int four_cells,
    int * const lineage_ct_p)
{
  for (int i = four_cells - 1; i >= 0; --i)
    {
      nucleus_t * const nuclei = tools->nuclei_record[i];
      nucleus_t * const nuclei_next = tools->nuclei_record[i + 1];

      const int nuc_ct = tools->nuc_ct_rec[i];

      for (int j = 0; j < nuc_ct; ++j)
        {
          if (strcmp(nuclei[j].identity, "polar") == 0)
            {
              continue;
            }

          if (nuclei[j].has_successor1 != 0)
            {
              const nucleus_t * const suc1 = nuclei_next + nuclei[j].successor1;

              if (nuclei[j].has_successor2 == 0)
                {
                  memcpy(nuclei[j].identity, suc1->identity, sizeof(char) * 50);
                }
              else
                {
                  const nucleus_t * const suc2 = nuclei_next
                      + nuclei[j].successor2;

                  if (strcmp(suc1->identity, "P2") == 0 || strcmp(
                      suc1->identity, "EMS") == 0)
                    {
                      if (strcmp(suc2->identity, "P2") == 0 || strcmp(
                          suc2->identity, "EMS") == 0)
                        {
                          (void) snprintf(nuclei[j].identity,
                              NUCLEUS_IDENTITY_SIZE, "P1");
                        }
                      else
                        {
                          fprintf(sn_logfile,
                              "%s and %s are considered sisters.\n",
                              suc1->identity, suc2->identity);
                          return 0;
                        }
                    }
                  else if (strcmp(suc1->identity, "ABa") == 0 || strcmp(
                      suc1->identity, "ABp") == 0)
                    {
                      if (strcmp(suc2->identity, "ABa") == 0 || strcmp(
                          suc2->identity, "ABp") == 0)
                        {
                          (void) snprintf(nuclei[j].identity,
                              NUCLEUS_IDENTITY_SIZE, "AB");
                        }
                      else
                        {
                          fprintf(sn_logfile,
                              "%s and %s are considered sisters.\n",
                              suc1->identity, suc2->identity);
                          return 0;
                        }
                    }
                  else if (strcmp(suc1->identity, "AB") == 0 || strcmp(
                      suc1->identity, "P1") == 0)
                    {
                      if (strcmp(suc1->identity, "AB") == 0 || strcmp(
                          suc1->identity, "P1") == 0)
                        {
                          (void) snprintf(nuclei[j].identity,
                              NUCLEUS_IDENTITY_SIZE, "P0");
                        }
                      else
                        {
                          fprintf(sn_logfile,
                              "%s and %s are considered sisters.\n",
                              suc1->identity, suc2->identity);
                          return 0;
                        }
                    }
                  else
                    {
                      fprintf(sn_logfile,
                          "%s and %s are considered sisters.\n",
                          suc1->identity, suc2->identity);
                      return 0;
                    }
                }
            }
        }
    }

    {
      nucleus_t * const nuclei = tools->nuclei_record[0];
      const int nuc_ct = tools->nuc_ct_rec[0];

      for (int i = 0; i < nuc_ct; ++i)
        {
          if (nuclei[i].is_valid == 0)
            continue;

          if (strcmp(nuclei[i].identity, "polar") == 0)
            continue;

          ++*lineage_ct_p;

          if (nuclei[i].identity == NULL)
            {
              (void) snprintf(nuclei[i].identity, NUCLEUS_IDENTITY_SIZE,
              "Nuc%d", *lineage_ct_p);
            }
        }
    }

  for (int i = 1; i < four_cells; i++)
    {
      nucleus_t * const nuclei = tools->nuclei_record[i];
      nucleus_t * const nuclei_prev = tools->nuclei_record[i - 1];
      nucleus_t * const nuclei_next = tools->nuclei_record[i + 1];

      const int nuc_ct = tools->nuc_ct_rec[i];

      for (int j = 0; j < nuc_ct; j++)
        {
          if (nuclei[j].is_valid == 0)
            continue;

          if (strcmp(nuclei[j].identity, "polar") == 0)
            continue;

          if (nuclei[j].has_predecessor == 0)
            {
              ++*lineage_ct_p;
              (void) snprintf(nuclei[j].identity, NUCLEUS_IDENTITY_SIZE,
              "Nuc%d", *lineage_ct_p);
            }
          else if (nuclei[j].identity == NULL)
            {
              const nucleus_t * const pred = nuclei_prev
                  + nuclei[j].predecessor;
              if (pred->has_successor2 == 0)
                {
                  memcpy(nuclei[j].identity, pred->identity, sizeof(char) * 50);
                }
              else
                {
                  NewBornID(parameters, pred, nuclei + pred->successor1, nuclei
                      + pred->successor2);
                }
            }

          if (nuclei[j].has_successor2 != 0)
            {
              nucleus_t * const nuc1 = &nuclei_next[nuclei[j].successor1];
              nucleus_t * const nuc2 = &nuclei_next[nuclei[j].successor2];
              SisterID(parameters, nuc1, nuc2, nuc_ct, movie->frame.width);
            }
        }
    }

  return 1;
}
