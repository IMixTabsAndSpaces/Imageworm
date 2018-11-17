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

#include "nucleus_list_init_predecessor_distances.h"

#include "partner_t.h"
#include "nucleus_t.h"
#include "xalloc.h"
#include "metric_calculate_distance.h"
#include "nucleus_list_init_p_partners.h"
#include "nucleus_list_init_s_partners.h"

void
nucleus_list_init_predecessor_distances(nucleus_t * const nuclei,
    const int nuc_ct, nucleus_t * const nuclei_prev,
    const int nuc_ct_prev, const metric_t *const metric,
    const float max_distance)
{
  if (!nuc_ct || !nuc_ct_prev)
    return;

  nucleus_list_init_s_partners(nuclei_prev, nuc_ct_prev, nuc_ct);
  nucleus_list_init_p_partners(nuclei, nuc_ct, nuc_ct_prev);

  for (int i = 0; i < nuc_ct_prev; i++)
    {
      partner_t * const succ_partners = nuclei_prev[i].s_partners;
      for (int j = 0; j < nuc_ct; j++)
        {
          partner_t * const pred_partners = nuclei[j].p_partners;

          float distance;

          if (!nuclei_prev[i].is_valid || !nuclei[j].is_valid)
            {
              distance = max_distance;
            }
          else
            {
              distance = metric_calculate_distance(metric,
                  &nuclei_prev[i], &nuclei[j]);
            }
          succ_partners[j].index = j;
          succ_partners[j].distance = distance;
          succ_partners[j].availability = 0;

          pred_partners[i].index = i;
          pred_partners[i].distance = distance;
          pred_partners[i].availability = 0;
        }
    }
}
