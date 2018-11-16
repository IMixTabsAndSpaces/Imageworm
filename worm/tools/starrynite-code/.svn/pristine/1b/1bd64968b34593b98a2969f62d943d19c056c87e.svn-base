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

#include "CountUnambiguousMatches.h"
#include "UnambiguousMatch.h"
#include "partner_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "struct_nucleus.h"

/**************************************
 **                                  **
 **    RECORD UNAMBIGUOUS MATCHES    **
 **                                  **
 **************************************/

void
CountUnambiguousMatches(const parameter_t * const parameters,
    const tool_t * const tools, int t)
{
  int i, k;
  int nuc_ct_prev, nuc_ct;
  nucleus_t *nuclei_prev, *nuclei;
  int suc;
  partner_t *partners;
  float cutoff;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].is_valid)
        continue;
      if (!nuclei_prev[i].s_ct)
        continue;
      partners = nuclei_prev[i].s_partners;
      for (k = 0; k < nuc_ct; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
            {
              suc = partners[k].index;
              if (nuclei[suc].potential_predecessor_count == 1)
                nuclei_prev[i].s_ct2 = nuclei_prev[i].s_ct2 + 1;
            }
          if (nuclei_prev[i].s_ct2 == nuclei_prev[i].s_ct)
            break;
        }
    }
}
