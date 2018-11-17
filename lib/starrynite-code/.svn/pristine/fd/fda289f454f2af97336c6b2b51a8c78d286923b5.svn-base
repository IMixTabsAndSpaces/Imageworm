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

#include <stdlib.h>
#include <assert.h>

#include "CalculateTimingScore.h"

#include "partner_t.h"
#include "parameter_t.h"
#include "ScoringExtensionTiming.h"
#include "ScoringExtension.h"
#include "ScoringDivisionTiming.h"
#include "ScoringDivision.h"
#include "ScoringDeath.h"
#include "struct_nucleus.h"

void
CalculateTimingScore(const parameter_t * const parameters,
    nucleus_t * const * const nuclei_record, const int * const nuc_ct_rec,
    const int t, const int p_index, const int s_index, partner_t * const par)
{
  int k;

  const int nuc_ct = nuc_ct_rec[t];
  const nucleus_t * const pred = nuclei_record[t - 1] + p_index;
  const nucleus_t * const suc = nuclei_record[t] + s_index;

  const float cutoff = 2.0 * parameters->nuc_size;

  assert (par != NULL);

  if (pred->s_ct2 >= 2)
    {
      par->score1 = par->score1m;
      par->score2 = par->score2m;
    }
  else if (pred->s_ct2 == 1)
    {
      par->score1 = par->score1m + ScoringExtensionTiming(t, pred,
          nuclei_record[t] + pred->successor1);
      par->score2 = par->score2m + ScoringDivisionTiming(parameters, t, pred,
          nuclei_record[t] + pred->successor1, suc);
    }
  else if (pred->s_ct == 1)
    {
      par->score1 = par->score1m;
      par->score2 = par->score2m;
    }
  else
    {
      if (pred->s_ct == 2)
        {
          const partner_t * const partners = pred->s_partners;
          nucleus_t *sis = NULL;
          int score;
          for (k = 0; k < nuc_ct; k++)
            {
              if (partners[k].distance > cutoff)
                break;
              if (!partners[k].availability)
                continue;
              if (partners[k].index != s_index)
                {
                  sis = nuclei_record[t] + partners[k].index;
                  break;
                }
            }
          assert (sis != NULL);
          par->score1 = par->score1m + ScoringExtensionTiming(t, pred, sis);
          score = ScoringDeath(pred);

          if (score > par->score1)
            par->score1 = score;

          par->score2 = par->score2m + ScoringDivisionTiming(parameters, t,
              pred, suc, sis);

          score = ScoringExtension(pred, suc) + ScoringExtensionTiming(t, pred,
              suc);
          if (score > par->score2)
            par->score2 = score;
        }
    }
}
