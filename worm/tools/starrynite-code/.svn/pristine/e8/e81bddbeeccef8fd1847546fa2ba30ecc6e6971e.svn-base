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

#include "TimingScores.h"

#include "struct_nucleus.h"
#include "CalculateTimingScore.h"
#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"

void
TimingScores(const parameter_t * const parameters, const tool_t * const tools,
    int t)
{
  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  nucleus_t * const nuclei = tools->nuclei_record[t];
  const float cutoff = 2.0 * parameters->nuc_size;

  for (int j = 0; j < nuc_ct; j++)
    {
      if (!nuclei[j].is_valid)
        continue;
      if (nuclei[j].potential_predecessor_count <= 1)
        continue;
      if (nuclei[j].pskip)
        continue;

      partner_t * const partners = nuclei[j].p_partners;

      for (int k = 0; k < nuc_ct_prev; k++)
        {
          partner_t * const partner = partners + k;
          if (partner->distance > cutoff)
            break;
          if (partner->availability == 0)
            continue;
          CalculateTimingScore(parameters, tools->nuclei_record,
              tools->nuc_ct_rec, t, partner->index, j, partner);
        }
    }
}
