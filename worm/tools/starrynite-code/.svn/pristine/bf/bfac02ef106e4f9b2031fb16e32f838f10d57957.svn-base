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

#include "SisterID.h"
#include <math.h>
#include "parameter_t.h"
#include "label_axis_early.h"
#include "label_axis_late.h"
#include "struct_nucleus.h"

/* TODO: Consider extracting orientation from parameter_t */
void
SisterID(const parameter_t * const parameters, nucleus_t * const nuc1,
    nucleus_t * const nuc2, const int nuc_ct, const int width)
{
  int xdiff = nuc1->x - nuc2->x;
  int ydiff = (nuc1->y - nuc2->y) / width;
  int cutoff = (nuc1->size + nuc2->size) / 4;

  if (cutoff < 10)
    cutoff = 10;

  if (parameters->axis == 0)
    {
      if (xdiff < 0 - cutoff)
        {
          nuc1->id_tag = 'w';
          nuc2->id_tag = 'e';
        }
      else if (xdiff > cutoff)
        {
          nuc1->id_tag = 'e';
          nuc2->id_tag = 'w';
        }
      else if (ydiff < 0 - cutoff)
        {
          nuc1->id_tag = 'u';
          nuc2->id_tag = 'd';
        }
      else if (ydiff > cutoff)
        {
          nuc1->id_tag = 'd';
          nuc2->id_tag = 'u';
        }
      else if (isless(nuc1->z, nuc2->z) != 0)
        {
          nuc1->id_tag = 't';
          nuc2->id_tag = 'b';
        }
      else
        {
          nuc1->id_tag = 'b';
          nuc2->id_tag = 't';
        }
      return;
    }

  if (xdiff * parameters->ap < -cutoff)
    {
      nuc1->id_tag = 'a';
      nuc2->id_tag = 'p';
      return;
    }
  else if (xdiff * parameters->ap > cutoff)
    {
      nuc1->id_tag = 'p';
      nuc2->id_tag = 'a';
      return;
    }

  if (ydiff * parameters->ap < -cutoff)
    {
      nuc1->id_tag = 'u';
      nuc2->id_tag = 'd';
    }
  else if (ydiff * parameters->ap > cutoff)
    {
      nuc1->id_tag = 'd';
      nuc2->id_tag = 'u';
    }
  else if (isless(nuc1->z, nuc2->z) != 0)
    {
      nuc1->id_tag = 't';
      nuc2->id_tag = 'b';
    }
  else
    {
      nuc1->id_tag = 'b';
      nuc2->id_tag = 't';
    }
  if (nuc_ct < 100)
    {
      label_axis_early(parameters->lr, parameters->dv, nuc1, nuc2);
    }
  else if (nuc_ct < 450)
    {
      label_axis_late(parameters->lr, parameters->dv, nuc1, nuc2);
    }
}
