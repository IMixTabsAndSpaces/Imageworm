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

#include "nucleus_list_try_joining_predecessors.h"
#include "struct_nucleus.h"
#include "nucleus_try_joining_predecessor.h"

void
nucleus_list_try_joining_predecessors(nucleus_t * const nuclei,
	const int nuc_ct, nucleus_t * const nuclei_prev)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      nucleus_t * const nuc = &nuclei[i];
      nucleus_t * const prev_nuc = &nuclei_prev[nuc->predecessor];
      nucleus_try_joining_predecessor(nuc, prev_nuc);
    }
}
