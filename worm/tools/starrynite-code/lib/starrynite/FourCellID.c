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

#include "FourCellID.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "nucleus_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "SisterID.h"
#include "FourCellIDAssignment.h"
#include "AlignDiamond.h"

/* TODO: Consider extracting orientation data from parameter_t */
int
FourCellID(nucleus_t * const * const nuclei_record,
    const int * const nuc_ct_rec, const movie_t * const movie,
    parameter_t * const parameters, const int four_cell_time,
    int * const lineage_ct_p)
{
  nucleus_t * const four_cell_nuclei = nuclei_record[four_cell_time];
  const int four_cell_nuc_ct = nuc_ct_rec[four_cell_time];

  nucleus_t *nuclei_next = NULL;

  if (AlignDiamond(four_cell_nuclei, four_cell_nuc_ct, four_cell_time,
      &movie->frame, movie->time_start))
    return 0;

  if (FourCellIDAssignment(nuclei_record, nuc_ct_rec, parameters, four_cell_time,
      movie->num_timepts))
    return 0;

  if (four_cell_time < movie->num_timepts - 1)
    {
      nuclei_next = nuclei_record[four_cell_time + 1];
    }

  for (int i = 0; i < four_cell_nuc_ct; ++i)
    {
      if (strcmp(four_cell_nuclei[i].identity, "polar") == 0)
        continue;

      if (four_cell_nuclei[i].has_predecessor == 0)
        ++*lineage_ct_p;

      if (four_cell_nuclei[i].has_successor2 != 0)
        {
          assert (nuclei_next != NULL);
          nucleus_t * const nuc1 = &nuclei_next[four_cell_nuclei[i].successor1];
          nucleus_t * const nuc2 = &nuclei_next[four_cell_nuclei[i].successor2];
          SisterID(parameters, nuc1, nuc2, four_cell_nuc_ct, movie->frame.width);
        }
    }

  return 1;
}
