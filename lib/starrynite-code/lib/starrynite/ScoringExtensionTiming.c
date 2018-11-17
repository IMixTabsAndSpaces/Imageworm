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

#include "ScoringExtensionTiming.h"

#include "struct_nucleus.h"
#include "ScoringExtension.h"

int
ScoringExtensionTiming(const int t, const nucleus_t * const pred,
    const nucleus_t * const suc)
{
  int score = 0;

  if (pred->condensed == 0 && suc->condensed == 0)
    /* noop */;

  else if (pred->time_start == 0)
    /* noop */;

  else if (t - pred->time_start <= 2)
    /* noop */;

  else if (suc->time_end - t > 2)
    score = -3;

  else if (suc->end_state < nucleus_end_state_potential_division)
    score = -3;

  return score;
}
