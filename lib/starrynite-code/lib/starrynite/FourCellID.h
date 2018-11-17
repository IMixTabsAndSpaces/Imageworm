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

#ifndef SN__FOURCELLID_H
#define SN__FOURCELLID_H 1

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "nucleus_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "SisterID.h"
#include "FourCellIDAssignment.h"
#include "AlignDiamond.h"

/* TODO: Consider extracting orientation data from parameter_t */
int
FourCellID(nucleus_t * const * const nuclei_record,
    const int * const nuc_ct_rec, const movie_t * const movie,
    parameter_t * const parameters, const int four_cell_time,
    int * const lineage_ct_p)
;

#endif /* ! SN__FOURCELLID_H */
