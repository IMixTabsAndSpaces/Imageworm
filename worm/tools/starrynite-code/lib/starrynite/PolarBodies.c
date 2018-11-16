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

#include "PolarBodies.h"
#include <stdio.h>
#include <string.h>
#include "logfile.h"
#include "struct_nucleus.h"

/* TODO: Fix parameter set; depends on PolarBodies */
void
PolarBodies(nucleus_t * const * const nuclei_record,
    const int * const nuc_ct_rec, const int start_time, const int num_timepts,
    const int polar_size)
{
  int polar_ct = 0;
    {
      nucleus_t * const nuclei = nuclei_record[start_time];
      const int nuc_ct = nuc_ct_rec[start_time];

      for (int i = 0; i < nuc_ct; i++)
        {
          if (nuclei[i].is_valid == 0)
            continue;
          if (strcmp(nuclei[i].identity, "polar") == 0)
            continue;
          if (nuclei[i].size < polar_size)
            {
              (void) snprintf(nuclei[i].identity, NUCLEUS_IDENTITY_SIZE,
              "polar");
              polar_ct++;
            }
        }
    }

  if (polar_ct == 0)
    return;

  for (int t = start_time; t < num_timepts - 1; t++)
    {
      nucleus_t * const nuclei = nuclei_record[t];
      const int nuc_ct = nuc_ct_rec[t];

      for (int i = 0; i < nuc_ct; i++)
        {
          if (strcmp(nuclei[i].identity, "polar") != 0)
            continue;

          if (nuclei[i].has_successor1 == 0)
            polar_ct--;

          if (polar_ct == 0)
            break;

          if (nuclei[i].has_successor2 != 0)
            {
              fprintf(sn_logfile,
                  "Polar body divided: %d:%d -> %d:%d and %d:%d\n", t + 1, i
                      + 1, t + 2, nuclei[i].successor1 + 1, t + 2,
                  nuclei[i].successor2 + 1);
            }
          else
            {
              nucleus_t * const nuclei_next = nuclei_record[t + 1];
              const int suc = nuclei[i].successor1;
              (void) snprintf(nuclei_next[suc].identity, NUCLEUS_IDENTITY_SIZE,
              "polar");
            }
        }

      if (polar_ct == 0)
        break;
    }
}
