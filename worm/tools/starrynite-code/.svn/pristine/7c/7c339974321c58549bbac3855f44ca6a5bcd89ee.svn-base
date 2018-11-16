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

#include "nucleus_list_init_s_partners.h"
#include "struct_nucleus.h"
#include "xalloc.h"
#include "partner_t.h"

void
nucleus_list_init_s_partners(nucleus_t * const nuclei,
	const int nuc_ct, const int partner_count)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      nuclei[i].s_partners = xmalloc(sizeof(partner_t) * partner_count);
    }
}
