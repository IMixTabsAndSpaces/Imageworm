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

#include "nucleus_sort_predecessor_partners.h"
#include <stdlib.h>

#include "compare_partner_distance.h"
#include "struct_nucleus.h"
#include "struct_partner.h"

void
nucleus_sort_predecessor_partners(nucleus_t * const nuc,
        const int partner_ct)
{
  if (nuc->is_valid == 0)
    return;

  if (nuc->p_partners == 0)
    return;

  qsort(nuc->p_partners, partner_ct, sizeof(partner_t),
      compare_partner_distance);
}