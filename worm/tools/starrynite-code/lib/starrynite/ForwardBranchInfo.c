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

#include "ForwardBranchInfo.h"

#include "struct_nucleus.h"

/*************************************
 **                                 **
 **    RESOLVE AMBIGUOUS MATCHES    **
 **                                 **
 *************************************/

void
ForwardBranchInfo(nucleus_t * const nuclei, const int nuc_ct,
    const nucleus_t * const nuclei_prev, const int nuc_ct_prev)
{
  /* branch info need further update upon decisions on ambiguous cases */
  for (int j = 0; j < nuc_ct; j++)
    {
      if (!nuclei[j].is_valid)
        continue;
      if (nuclei[j].has_predecessor)
        {
          const int pred = nuclei[j].predecessor;
          if (nuclei_prev[pred].has_successor1 && nuclei_prev[pred].s_ct == 1)
            {
              nuclei[j].start_state = nuclei_prev[pred].start_state;
              nuclei[j].start_scorem = nuclei_prev[pred].start_scorem;
              nuclei[j].time_start = nuclei_prev[pred].time_start;
            }
          else if (nuclei_prev[pred].has_successor2)
            {
              nuclei[j].start_state = nucleus_start_state_division; /* division */
            }
          else
            {
              nuclei[j].start_state = nucleus_start_state_potential_division; /* potential division */
              nuclei[j].start_scorem = nuclei_prev[pred].end_scorem;
            }
        }
      else
        {
          if (!nuclei[j].potential_predecessor_count)
            nuclei[j].start_state = nucleus_start_state_emerging; /* emerging */
          if (nuclei[j].potential_predecessor_count == 1)
            nuclei[j].start_state = nucleus_start_state_emerging; /* emerging */
          else
            {
              nuclei[j].start_state = nucleus_start_state_potential_division; /* potential division */
            }
        }
    }
}
