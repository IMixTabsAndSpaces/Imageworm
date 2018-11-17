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

#include "ScoringExtension.h"

#include "struct_nucleus.h"

int
ScoringExtension(const nucleus_t * const pred, const nucleus_t * const suc)
{
  int score = 0;

  if (abs(pred->size - suc->size) <= 2)
    score += 1;
  else
    score -= 1;

  if (1.0 * abs(pred->weight - suc->weight) / suc->weight < 0.3)
    score += 3;
  else
    score -= 1;

  if (!pred->condensed && !suc->condensed)
    score += 3;

  return score;
}
