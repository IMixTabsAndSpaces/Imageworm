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

#include "AddAmbiguousSuccessors.h"
#include <assert.h>
#include <stdbool.h>

#include "struct_nucleus.h"
#include "partner_t.h"
#include "logfile.h"
#include "AddLink.h"

typedef struct get_cutoff1_rval
{
  bool cutoff_found;
  float cutoff;
} get_cutoff1_rval_t;

/** Returns a get_cutoff1_rval_t with cutoff_found = true and
 * cutoff = ambiguity_cutoff * partners[i].distance for the first
 * partners[i] meeting some conditions, or a get_cutoff1_rval_t
 * with cutoff_found = false if the conditions are not met.
 */
static get_cutoff1_rval_t
get_cutoff1(const partner_t * const partners, const int nuc_ct,
    const nucleus_t * const nuclei_prev, const int nuc_ct_prev, const int index,
    const float cutoff, const float ambiguity_cutoff)
{
  get_cutoff1_rval_t rval;

  for (int i = 0; i < nuc_ct; i++)
    {
      if (partners[i].distance > cutoff)
        {
          break;
        }

      if (partners[i].index != nuclei_prev[index].successor1
          && partners[i].index != nuclei_prev[index].successor2)
        {
          float cutoff = ambiguity_cutoff * partners[i].distance;

          assert (cutoff> 0.);

          rval.cutoff_found = true;
          rval.cutoff = cutoff;
          return rval;
        }
    }

  rval.cutoff_found = false;
  rval.cutoff = 0.;
  return rval;
}

static void
add_ambiguous_successors_sub1(partner_t * const partners,
    nucleus_t * const nuclei, const int nuc_ct, nucleus_t * const nuclei_prev,
    const int nuc_ct_prev, const int time_start, const int t, const int index,
    const float cutoff, const float cutoff1)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      if (partners[i].distance > cutoff)
        break;
      if (partners[i].availability == 0)
        {
          const int suc = partners[i].index;
          if (partners[i].distance > cutoff1)
            break;
          partners[i].availability = 1;
          fprintf(sn_logfile, ", adding %d:%d", time_start - 1 + t + 1, suc + 1);
          nuclei_prev[index].s_ct++;
          nuclei[suc].potential_predecessor_count++;
          AddLink(nuclei[suc].p_partners, index, nuc_ct_prev);
        }
    }
  fprintf(sn_logfile, "\n");
}

void
AddAmbiguousSuccessors(nucleus_t * const nuclei, const int nuc_ct,
    nucleus_t * const nuclei_prev, const int nuc_ct_prev, const int time_start,
    const int t, const int index, const float cutoff,
    const float ambiguity_cutoff)
{
  partner_t * const partners = nuclei_prev[index].s_partners;

  fprintf(sn_logfile, "Adding suc for %d:%d", time_start - 1 + t, index + 1);

  const get_cutoff1_rval_t get_cutoff1_rval = get_cutoff1(partners, nuc_ct,
      nuclei_prev, nuc_ct_prev, index, cutoff, ambiguity_cutoff);

  if (!get_cutoff1_rval.cutoff_found)
    {
      fprintf(sn_logfile, " nothing close enough\n");
    }
  else
    {
      add_ambiguous_successors_sub1(partners, nuclei, nuc_ct, nuclei_prev,
          nuc_ct_prev, time_start, t, index, cutoff, get_cutoff1_rval.cutoff);
    }
}
