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

#ifndef SN__CALCULATETIMINGSCORE_H
#define SN__CALCULATETIMINGSCORE_H 1

#include <stdlib.h>
#include <assert.h>

#include "partner_t.h"
#include "nucleus_t.h"
#include "parameter_t.h"

#include "ScoringExtensionTiming.h"
#include "ScoringExtension.h"
#include "ScoringDivisionTiming.h"
#include "ScoringDivision.h"
#include "ScoringDeath.h"

void
CalculateTimingScore(const parameter_t * const parameters,
    nucleus_t * const * const nuclei_record, const int * const nuc_ct_rec,
    const int t, const int p_index, const int s_index, partner_t * const par)
;
#endif /* ! SN__CALCULATETIMINGSCORE_H */
