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

#include "UpdateBranchInfo.h"

#include "struct_nucleus.h"

void
UpdateBranchInfo(nucleus_t * const nuclei, const int nuc_ct,
    const nucleus_t * const nuclei_prev, const int nuc_ct_prev, const int t)
{
  for (int j = 0; j < nuc_ct; j++)
    {
      if (nuclei[j].is_valid == 0)
        continue;

      if (nuclei[j].has_predecessor == 0)
        {
          nuclei[j].time_start = t;
          continue;
        }

      const int pred = nuclei[j].predecessor;

      if (nuclei_prev[pred].sskip)
        {
          nuclei[j].time_start = t;
        }
      else if (nuclei_prev[pred].has_successor2)
        {
          /* sister will take care of herself */
          nuclei[j].time_start = t;
          nuclei[j].generation = nuclei_prev[pred].generation + 1;
        }
      else
        {
          nuclei[j].time_start = nuclei_prev[pred].time_start;
          nuclei[j].generation = nuclei_prev[pred].generation;
        }
    }
}
