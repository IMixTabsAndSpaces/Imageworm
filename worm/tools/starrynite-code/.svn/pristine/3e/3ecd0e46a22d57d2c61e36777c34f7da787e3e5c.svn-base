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

#include "label_axis_early.h"
#include <assert.h>
#include "struct_nucleus.h"

/* Updates id_tag of nuc2 based on the values of lr, dv, and nuc1->id_tag. */
void
label_axis_early(const int lr, const int dv, nucleus_t * const nuc1,
    nucleus_t * const nuc2)
{
  char left, right, dorsal, ventral;

  if (lr == 1)
    {
      left = 't';
      right = 'b';
    }
  else
    {
      left = 'b';
      right = 't';
    }

  if (dv == 1)
    {
      dorsal = 'u';
      ventral = 'd';
    }
  else
    {
      dorsal = 'd';
      ventral = 'u';
    }

  if (nuc1->id_tag == left)
    {
      nuc1->id_tag = 'l';
      nuc2->id_tag = 'r';
    }
  else if (nuc1->id_tag == right)
    {
      nuc1->id_tag = 'r';
      nuc2->id_tag = 'l';
    }
  else if (nuc1->id_tag == dorsal)
    {
      nuc1->id_tag = 'd';
      nuc2->id_tag = 'v';
    }
  else
    {
      nuc1->id_tag = 'v';
      nuc2->id_tag = 'd';
    }
}
