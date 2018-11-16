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

#include "add_ambiguous_predecessors.h"

#include "Encroached.h"
#include "AddLink.h"
#include "logfile.h"
#include "nucleus_t.h"
#include "partner_t.h"
#include "AddLink.h"

void
add_ambiguous_predecessors(nucleus_t * const nuclei, const int nuc_ct,
    nucleus_t * const nuclei_prev, const int nuc_ct_prev, const int time_start,
    const int t, const int index, const int width, const float z_factor,
    const float cutoff, const float ambiguity_cutoff)
{

  int pred = nuclei[index].predecessor;
  partner_t * const partners = nuclei[index].p_partners;
  float cutoff1 = 0.0;
  int new_ct = 0;

  fprintf(sn_logfile, "Adding pred for %d:%d (%.2f to %d:%d), ", time_start - 1
      + t + 1, index + 1, nuclei[index].distance_to_predecessor, time_start - 1 + t, pred + 1);

  /* this loop sets cutoff1 once or not at all */
  for (int k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (partners[k].index != nuclei[index].predecessor)
        {
          cutoff1 = ambiguity_cutoff * partners[k].distance;
          break;
        }
    }

  /* TODO: fix this comparison between an int and a float */
  if (cutoff1 == 0)
    {
      fprintf(sn_logfile, "nothing close enough\n");
      return;
    }

  for (int k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;

      if (!partners[k].availability)
        {
          if (partners[k].distance > cutoff1)
            {
              break;
            }

          new_ct++;

          if (new_ct == 3)
            {
              break;
            }

          /* added this to check previous statement: what if new_ct == 4? */
          assert (new_ct < 3);

          pred = partners[k].index;
            {
              if (Encroached(nuclei, nuc_ct, nuclei_prev, time_start, t, pred,
                  index, width, z_factor, cutoff))
                {
                  continue;
                }
            }
          fprintf(sn_logfile, "adding %d:%d (%.2f), ", time_start - 1 + t, pred
              + 1, partners[k].distance);

          nuclei[index].potential_predecessor_count++;
          partners[k].availability = 1;
          nuclei_prev[pred].s_ct++;
          AddLink(nuclei_prev[pred].s_partners, index, nuc_ct);
        }
    }
  fprintf(sn_logfile, "\n");
}
