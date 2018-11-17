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

#include <stdlib.h> /* for abs */

#include "ScoringDivision.h"

#include "struct_nucleus.h"
#include "logfile.h"

int
ScoringDivision(const nucleus_t * const mother, const nucleus_t * const dau1,
    const nucleus_t * const dau2)
{
  int score = 0;

  if (mother->condensed == 3)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    fprintf(sn_logfile, " 0");

  if (dau1->condensed == 3 || mother->max_size - dau1->size >= 4)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    {
      score -= 1;
      fprintf(sn_logfile, " 0");
    }
  if (dau2->condensed == 3 || mother->max_size - dau2->size >= 4)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    {
      score -= 1;
      fprintf(sn_logfile, " 0");
    }

  if (1.0 * (mother->max_weight - dau1->weight) / mother->max_weight > 0.3)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    {
      score -= 1;
      fprintf(sn_logfile, " 0");
    }
  if (1.0 * (mother->max_weight - dau2->weight) / mother->max_weight > 0.3)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    {
      score -= 1;
      fprintf(sn_logfile, " 0");
    }

  if (abs(dau1->size - dau2->size) <= 2)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    {
      score -= 1;
      fprintf(sn_logfile, " 0");
    }
  if (1.0 * abs(dau1->weight - dau2->weight) / dau2->weight < 0.1)
    {
      score += 1;
      fprintf(sn_logfile, " 1");
    }
  else
    {
      score -= 1;
      fprintf(sn_logfile, " 0");
    }

  return score;
}
