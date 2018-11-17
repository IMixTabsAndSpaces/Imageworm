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

#include "BackwardBranchInfo.h"
#include "struct_nucleus.h"

void
BackwardBranchInfo(const nucleus_t * const nuclei, const int nuc_ct,
    nucleus_t * const nuclei_prev, const int nuc_ct_prev)
{
  /* branch info need further update upon decisions on ambiguous cases */

  for (int i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
        continue;
      if (nuclei_prev[i].has_successor1 && nuclei_prev[i].s_ct == 1)
        {
          const int suc = nuclei_prev[i].successor1;
          nuclei_prev[i].time_end = nuclei[suc].time_end;
          nuclei_prev[i].end_state = nuclei[suc].end_state;
          nuclei_prev[i].end_scorem = nuclei[suc].end_scorem;
        }
      else
        {
          if (!nuclei_prev[i].s_ct)
            nuclei_prev[i].end_state = nucleus_end_state_potential_death; /* potential death/fn */
          else if (nuclei_prev[i].s_ct == 1)
            nuclei_prev[i].end_state = nucleus_end_state_potential_death; /* potential death/fn */
          else
            {
              if (nuclei_prev[i].s_ct2 >= 2)
                nuclei_prev[i].end_state = nucleus_end_state_division; /* division */
              else
                nuclei_prev[i].end_state = nucleus_end_state_potential_division; /* potential division */
            }
        }
    }
}
