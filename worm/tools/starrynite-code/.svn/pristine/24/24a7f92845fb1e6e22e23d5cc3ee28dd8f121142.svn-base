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

#include "ScoringDivisionTiming.h"

#include "struct_nucleus.h"
#include "ScoringDivision.h"
#include "parameter_t.h"

int
ScoringDivisionTiming(const parameter_t * const parameters, const int t,
    const nucleus_t * const mother, const nucleus_t * const dau1,
    const nucleus_t * const dau2)
{
  int score = 0;

  if (dau1->time_end - t < parameters->minimal_cell_cycle)
    {
      if (dau1->end_state == nucleus_end_state_potential_division)
        {
          if (mother->end_scorem <= dau1->end_scorem)
            score -= 1;
          else
            score += 1;
        }
      else if (dau1->end_state == nucleus_end_state_division)
        score -= 20;
      else
        score += 1;
    }
  else
    score += 1;

  if (dau2->time_end - t < parameters->minimal_cell_cycle)
    {
      if (dau2->end_state == nucleus_end_state_potential_division)
        {
          if (mother->end_scorem <= dau2->end_scorem)
            score -= 1;
          else
            score += 1;
        }
      else if (dau2->end_state == nucleus_end_state_division)
        score -= 20;
      else
        score += 1;
    }
  else
    score += 1;

  if (t - mother->time_start < parameters->minimal_cell_cycle)
    {
      if (mother->start_state == nucleus_start_state_division)
        score -= 20;
      else if (mother->start_state == nucleus_start_state_potential_division)
        {
          if (mother->end_scorem <= mother->start_scorem)
            score -= 1;
          else
            score += 1;
        }
    }

  return score;
}
