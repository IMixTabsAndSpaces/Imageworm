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

#include "logfile.h"
#include "RemoveGhostNuclei.h"
#include "struct_nucleus.h"
#include "tool_t.h"
#include "parameter_t.h"

void
RemoveGhostNuclei(const int time_start, const int num_timepts,
    const parameter_t * const parameters, const tool_t * const tools)
{
  int t;

  for (t = 0; t < num_timepts - 1; t++)
    {
      int j;
      const int nuc_ct = tools->nuc_ct_rec[t];
      nucleus_t * const nuclei = tools->nuclei_record[t];
      for (j = 0; j < nuc_ct; j++)
        {
          if (nuclei[j].is_valid == 0)
            continue;
          if (nuclei[j].ghost == 0)
            continue;
          if (nuclei[j].has_successor1 == 0)
            {
              nuclei[j].is_valid = 0;
              fprintf(sn_logfile, "hanging ghost, %d:%d\n", time_start - 1 + t,
                  j + 1);
            }
        }
    }
}
