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

#include "MorphologyScores.h"
#include "struct_nucleus.h"
#include "CalculateMorphologyScore.h"
#include "partner_t.h"

void
MorphologyScores(const int time_start, nucleus_t * const nuclei,
    const int nuc_ct, nucleus_t * const nuclei_prev, const int nuc_ct_prev,
    int t, const float cutoff)
{

  for (int j = 0; j < nuc_ct; j++)
    {
      nucleus_t * const nuc = nuclei + j;

      if (nuc->is_valid == 0)
        continue;

      if (nuc->potential_predecessor_count <= 1)
        continue;

      if (nuc->pskip)
        continue;

      partner_t * const partners = nuc->p_partners;

      for (int k = 0; k < nuc_ct_prev; k++)
        {
          partner_t * const par = partners + k;

          if (par->distance > cutoff)
            break;

          if (par->availability == 0)
            continue;

          CalculateMorphologyScore(time_start, nuclei, nuc_ct, nuclei_prev, t,
              par->index, j, par, cutoff);
        }
    }
}
