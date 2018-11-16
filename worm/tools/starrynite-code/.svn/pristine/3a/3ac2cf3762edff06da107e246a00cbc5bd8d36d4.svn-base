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

#include "UpdateCounts.h"

#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "struct_nucleus.h"

void
UpdateCounts(const parameter_t * const parameters, const tool_t * const tools,
    const int t, const int index)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
  nucleus_t * const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;
  const partner_t * const partners = nuclei[index].p_partners;

  for (i = 0; i < nuc_ct_prev; i++)
    {
      if (partners[i].distance > cutoff)
        break;
      if (partners[i].availability)
        {
          const int pred = partners[i].index;
          nuclei[index].predecessor = pred;
          nuclei[index].distance_to_predecessor = partners[i].distance;
          nuclei_prev[pred].s_ct2++;
          break;
        }
    }
}
