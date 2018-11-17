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
#include <stdio.h>
#include <stdlib.h>

#include "FourCellIDAssignment.h"
#include "struct_nucleus.h"
#include "logfile.h"
#include "parameter_t.h"
#include "TimeToDivide.h"

/* TODO: Fix parameters */
int
FourCellIDAssignment(nucleus_t * const * const nuclei_record,
    const int * const nuc_ct_rec, parameter_t * const parameters,
    const int four_cell_time, const int num_timepts)
{
  nucleus_t *north = NULL;
  nucleus_t *south = NULL;
  nucleus_t *west = NULL;
  nucleus_t *east = NULL;

    {
      nucleus_t * const four_cell_nuclei = nuclei_record[four_cell_time];
      const int four_cell_nuc_ct = nuc_ct_rec[four_cell_time];

      for (int i = 0; i < four_cell_nuc_ct; i++)
        {
          if (four_cell_nuclei[i].id_tag == 'n')
            {
              north = &four_cell_nuclei[i];
            }
          else if (four_cell_nuclei[i].id_tag == 's')
            {
              south = &four_cell_nuclei[i];
            }
          else if (four_cell_nuclei[i].id_tag == 'e')
            {
              east = &four_cell_nuclei[i];
            }
          else if (four_cell_nuclei[i].id_tag == 'w')
            {
              west = &four_cell_nuclei[i];
            }
          four_cell_nuclei[i].id_tag = '\0';
        }
    }

  assert (north != NULL);
  assert (south != NULL);
  assert (east != NULL);
  assert (west != NULL);

  const int ntime = TimeToDivide((const nucleus_t **) nuclei_record,
      four_cell_time, north, num_timepts);
  const int stime = TimeToDivide((const nucleus_t **) nuclei_record,
      four_cell_time, south, num_timepts);
  const int etime = TimeToDivide((const nucleus_t **) nuclei_record,
      four_cell_time, east, num_timepts);
  const int wtime = TimeToDivide((const nucleus_t **) nuclei_record,
      four_cell_time, west, num_timepts);

  if (ntime == -1 || stime == -1 || etime == -1 || wtime == -1)
    {
      fprintf(sn_logfile, "Cell death at four cell stage.\n");
      return 1;
    }

  nucleus_t *ABa, *ABp, *EMS, *P2;

  if (wtime < etime)
    {
      ABa = west;
      P2 = east;
      parameters->ap = 1;
    }
  else if (wtime > etime)
    {
      ABa = east;
      P2 = west;
      parameters->ap = -1;
    }
  else
    {
      assert (wtime == etime);
      if (wtime == num_timepts - 1)
        fprintf(sn_logfile, "Movie too short to capture 6 cell stage\n");
      else
        fprintf(sn_logfile, "putative ABa and P2 divide simutaneously.\n");
      return 1;
    }

  if (ntime < stime)
    {
      ABp = north;
      EMS = south;
      parameters->dv = 1;
    }
  else if (ntime > stime)
    {
      ABp = south;
      EMS = north;
      parameters->dv = -1;
    }
  else
    {
      assert (ntime == stime);
      fprintf(sn_logfile, "putative ABp and EMS divide simutaneously.\n");
      return 1;
    }

  parameters->lr = parameters->ap * parameters->dv;

  /* TODO: Test return values */
  snprintf(ABa->identity, NUCLEUS_IDENTITY_SIZE, "ABa");
  snprintf(ABp->identity, NUCLEUS_IDENTITY_SIZE, "ABp");
  snprintf(EMS->identity, NUCLEUS_IDENTITY_SIZE, "EMS");
  snprintf(P2->identity, NUCLEUS_IDENTITY_SIZE, "P2");

  return 0;
}
