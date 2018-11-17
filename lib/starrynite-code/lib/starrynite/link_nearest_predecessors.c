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

#include "link_nearest_predecessors.h"
#include "AddLink.h"
#include "partner_t.h"
#include "struct_nucleus.h"

void
link_nearest_predecessors(nucleus_t * const nuclei, const int nuc_ct,
    nucleus_t * const nuclei_prev, const float cutoff)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      partner_t * const partners = nuclei[i].p_partners;

      if (nuclei[i].is_valid == 0)
        continue;
      if (partners == NULL)
        continue;
      if (partners->distance > cutoff)
        continue;

      partners->availability = 1;

        {
          const int pred = partners->index;
          nuclei[i].potential_predecessor_count = 1;
          nuclei[i].predecessor = pred;
          nuclei[i].distance_to_predecessor = partners->distance;
          nuclei_prev[pred].s_ct++;
          AddLink(nuclei_prev[pred].s_partners, i, nuc_ct);
          if (nuclei_prev[pred].s_ct > 2)
            continue;
          if (nuclei_prev[pred].successor1 == -1)
            {
              nuclei_prev[pred].successor1 = i;
              nuclei_prev[pred].distance_to_successor1 = partners->distance;
            }
          else
            {
              if (nuclei_prev[pred].distance_to_successor1 <= partners->distance)
                {
                  nuclei_prev[pred].successor2 = i;
                  nuclei_prev[pred].distance_to_successor2 = partners->distance;
                }
              else
                {
                  nuclei_prev[pred].successor2 = nuclei_prev[pred].successor1;
                  nuclei_prev[pred].distance_to_successor2 = nuclei_prev[pred].distance_to_successor1;
                  nuclei_prev[pred].successor1 = i;
                  nuclei_prev[pred].distance_to_successor1 = partners->distance;
                }
            }
        }
    }
}
