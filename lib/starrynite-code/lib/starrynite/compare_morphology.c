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

#include "compare_morphology.h"
#include "partner_t.h"

int
compare_morphology(const void * const a, const void * const b)
{
  const partner_t * const par1 = a;
  const partner_t * const par2 = b;

  if (par1->availability)
    {
      if (!par2->availability)
        return -1;
      else
        return par2->score2m - par1->score2m;
    }
  else
    {
      if (par2->availability)
        return 1;
      else
        return -1;
    }
}
