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

#include "CalculateMorphologyScore.h"
#include "partner_t.h"
#include "error-codes.h"

#include "ScoringExtension.h"
#include "ScoringDivision.h"
#include "ScoringDeath.h"
#include "struct_nucleus.h"

void
CalculateMorphologyScore(const int time_start, nucleus_t * const nuclei,
    const int nuc_ct, nucleus_t * const nuclei_prev, const int t,
    const int p_index, const int s_index, partner_t * const par,
    const float cutoff)
{

  int my_extension, sis_extension;

  nucleus_t * const pred = &nuclei_prev[p_index];
  nucleus_t * const suc = &nuclei[s_index];

  assert (par != NULL);

  if (pred->s_ct2 > 2)
    {
      fprintf(sn_logfile, "S_Skip missed: %d:%d\n", time_start - 1 + t, p_index
          + 1);
      exit(ERROR_CALCULATEMORPHOLOGYSCORE1);
    }
  else if (pred->s_ct2 == 2)
    {
      par->score1m = 100;
      par->score2m = -100;
    }
  else if (pred->s_ct2 == 1)
    {
      par->score1m = ScoringExtension(pred, nuclei + pred->successor1);
      par->score2m = ScoringDivision(pred, nuclei + pred->successor1, suc);
      if (par->score2m > pred->end_scorem)
        pred->end_scorem = par->score2m;
      if (par->score2m > suc->start_scorem)
        suc->start_scorem = par->score2m;
    }
  else
    {
      /* pred->s_ct2 == 0 */
      if (pred->s_ct == 1)
        {
          par->score1m = ScoringDeath(pred);
          par->score2m = ScoringExtension(pred, suc);
        }
      else if (pred->s_ct == 2)
        {
          const partner_t * const partners = pred->s_partners;
          nucleus_t *sis = NULL;
          for (int k = 0; k < nuc_ct; k++)
            {
              if (partners[k].distance > cutoff)
                break;
              if (!partners[k].availability)
                continue;
              if (partners[k].index != s_index)
                {
                  sis = &nuclei[partners[k].index];
                  break;
                }
            }
          assert (sis != NULL);
          my_extension = ScoringExtension(pred, suc);
          sis_extension = ScoringExtension(pred, sis);
          par->score1m = sis_extension;
          if (my_extension > sis_extension)
            par->score1m += 3;
          /*score = ScoringDeath(pred);
           if (score > par->score1m) par->score1m = score; */
          par->score2m = ScoringDivision(pred, suc, sis);
          if (par->score2m > pred->end_scorem)
            pred->end_scorem = par->score2m;
          if (par->score2m > suc->start_scorem)
            suc->start_scorem = par->score2m;
          /*score = ScoringExtension(pred, suc);
           if (score > par->score2m) par->score2m = score; */
        }
      else
        {
          /* s_ct > 2 */
          fprintf(sn_logfile, "S_Skip missed: %d:%d\n", time_start - 1 + t,
              p_index + 1);
          exit(ERROR_CALCULATEMORPHOLOGYSCORE2);
        }
    }
}
