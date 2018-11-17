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

#ifndef SN__NUCLEUS_LIST_INIT_PREDECESSOR_DISTANCES_H
#define SN__NUCLEUS_LIST_INIT_PREDECESSOR_DISTANCES_H 1

#include "nucleus_t.h"
#include "xalloc.h"
#include "metric_calculate_distance.h"
#include "nucleus_list_init_p_partners.h"
#include "nucleus_list_init_s_partners.h"

void
nucleus_list_init_predecessor_distances(nucleus_t * const nuclei,
    const int nuc_ct, nucleus_t * const nuclei_prev,
    const int nuc_ct_prev, const metric_t *const metric,
    const float max_distance)
;

#endif /* ! SN__NUCLEUS_LIST_INIT_PREDECESSOR_DISTANCES_H */
