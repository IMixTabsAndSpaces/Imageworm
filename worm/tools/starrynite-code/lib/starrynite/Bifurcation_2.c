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

#include "Bifurcation_2.h"
#include "logfile.h"
#include "RecordDivision.h"
#include "Bifurcation_1.h"
#include "struct_nucleus.h"

void
Bifurcation_2(nucleus_t * const * const nuclei_record, const int time_start,
    const int t, const int p_index, const int s_index)
{
  nucleus_t * const pred = nuclei_record[t - 1] + p_index;
  /*  nucleus_t *suc= *(tools->nuclei_record+t) + s_index; */

  if (!pred->has_successor1)
    {
      Bifurcation_1(nuclei_record, t, p_index, s_index);
    }
  else if (!pred->has_successor2)
    {
      RecordDivision(nuclei_record, t, p_index, s_index);
    }
  else
    {
      pred->sskip = 1;
      fprintf(sn_logfile, "Seeding skip: >2 successors assigned to %d:%d\n",
          time_start - 1 + t, p_index + 1);
    }
}
