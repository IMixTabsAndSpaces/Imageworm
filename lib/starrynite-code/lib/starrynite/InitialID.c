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

#include "InitialID.h"
#include <stdio.h>
#include <string.h>
#include "PolarBodies.h"
#include "logfile.h"
#include "BackAssignment.h"
#include "nucleus_t.h"
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "CountCells.h"
#include "FourCellID.h"

/* TODO: Fix parameter set; depends on PolarBodies, FourCellID, BackAssignment */
int
InitialID(const movie_t * const movie, const tool_t * const tools,
    parameter_t * const parameters, int * const start_p,
    int * const lineage_ct_p, const int num_timepts)
{
  nucleus_t *nuclei;
  int nuc_ct;
  int cell_ct;

  int t, i, j;
  int first_four = -1, last_four = -1, four_cells;
  int lin_ct = *lineage_ct_p;

  nuclei = tools->nuclei_record[0];
  nuc_ct = tools->nuc_ct_rec[0];
  cell_ct = CountCells(nuclei, nuc_ct);

  if (cell_ct <= 6)
    {
      PolarBodies(tools->nuclei_record, tools->nuc_ct_rec, 0, num_timepts,
          parameters->polar_size);
      cell_ct = CountCells(nuclei, nuc_ct);
    }

  if (cell_ct > 4)
    {
      for (i = 0; i < nuc_ct; i++)
        {
          if (nuclei[i].is_valid == 0)
            continue;
          if (strcmp(nuclei[i].identity, "polar") == 0)
            continue;

          lin_ct++;
          (void) snprintf(nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
              lin_ct);
        }
      parameters->axis = 0;
      *start_p = 0;
      fprintf(sn_logfile,
          "Starting with more than 4 cells.  No canonical ID assigned.\n\n");
      *lineage_ct_p = lin_ct;
      return 1;
    }

  parameters->axis = 1;
  for (t = 0; t < num_timepts; t++)
    {
      nuclei = tools->nuclei_record[t];
      nuc_ct = tools->nuc_ct_rec[t];
      cell_ct = CountCells(nuclei, nuc_ct);
      if (cell_ct > 4)
        break;
      if (cell_ct == 4)
        {
          if (first_four == -1)
            first_four = t;
          last_four = t;
        }
    }

  if (first_four == -1)
    {
      nuclei = tools->nuclei_record[0];
      nuc_ct = tools->nuc_ct_rec[0];
      for (i = 0; i < nuc_ct; i++)
        {
          if (nuclei[i].is_valid == 0)
            continue;
          if (strcmp(nuclei[i].identity, "polar") == 0)
            continue;
          lin_ct++;
          (void) snprintf(nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
              lin_ct);
        }
      parameters->axis = 0;
      *start_p = 0;
      *lineage_ct_p = lin_ct;
      fprintf(sn_logfile, "Movie too short to see four cells; t = %d\n",
          parameters->t);
      return 1;
    }

  four_cells = (first_four + last_four) / 2;
  *start_p = four_cells;

  if (FourCellID(tools->nuclei_record, tools->nuc_ct_rec, movie, parameters,
      four_cells, lineage_ct_p) == 0 || BackAssignment(movie, tools,
      parameters, four_cells, lineage_ct_p) == 0)
    {
      for (j = 0; j <= four_cells; j++)
        {
          nuclei = tools->nuclei_record[j];
          nuc_ct = tools->nuc_ct_rec[j];
          for (i = 0; i < nuc_ct; i++)
            {
              if (nuclei[i].is_valid != 0)
                {
                  nuclei[i].identity[0] = '\0';
                }
            }
        }
      nuclei = tools->nuclei_record[0];
      nuc_ct = tools->nuc_ct_rec[0];
      for (i = 0; i < nuc_ct; i++)
        {
          if (nuclei[i].is_valid == 0)
            continue;
          if (strcmp(nuclei[i].identity, "polar") == 0)
            continue;
          lin_ct++;
          (void) snprintf(nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
              lin_ct);
        }
      parameters->axis = 0;
      *start_p = 0;
      *lineage_ct_p = lin_ct;

      return 1;
    }

  return 0;
}
