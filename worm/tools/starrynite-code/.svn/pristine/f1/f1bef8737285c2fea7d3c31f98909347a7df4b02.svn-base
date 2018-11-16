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

#include "nucleus_can_join_predecessor.h"
#include <stdbool.h>

#include "struct_nucleus.h"

bool
nucleus_can_join_predecessor(const nucleus_t * const nuc,
    const nucleus_t * const prev_nuc)
{
  if (!nuc->is_valid)
    return false;

  if (!nuc->potential_predecessor_count)
    return false;

  if (prev_nuc->s_ct != 1)
    return false;

  return true;
}
