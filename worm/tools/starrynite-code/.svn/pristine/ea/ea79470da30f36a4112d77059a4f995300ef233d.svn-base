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

#include "IntroduceAmbiguity.h"
#include "AddAmbiguousSuccessors.h"
#include "add_ambiguous_predecessors.h"
#include "nucleus_t.h"

void
IntroduceAmbiguity(nucleus_t * const nuclei, const int nuc_ct,
    nucleus_t * const nuclei_prev, const int nuc_ct_prev,
    const int time_start, const int t, const int width,
    const float z_factor, const float cutoff, const float ambiguity_cutoff)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      if (!nuclei[i].is_valid)
        continue;
      if (!nuclei[i].p_ambiguity)
        continue;
      if (nuclei[i].ghost)
        continue;
      if (nuclei[i].potential_predecessor_count == 1)
        {
          add_ambiguous_predecessors(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev,
              time_start, t, i, width, z_factor, cutoff,
              ambiguity_cutoff);
        }
      else
        {
          fprintf(sn_logfile,
              "Daughter cell already ambiguous: %d:%d -> %d:%d.  Exiting.\n",
              time_start - 1 + t, nuclei[i].predecessor + 1, time_start - 1 + t
                  + 1, i + 1);
          exit(ERROR_INTRODUCEAMBIGUITY);
        }
    }

  for (int i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
        continue;
      if (nuclei_prev[i].s_ambiguity)
        {

          AddAmbiguousSuccessors(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev,
              time_start, t, i, cutoff, ambiguity_cutoff);
        }
    }
}
