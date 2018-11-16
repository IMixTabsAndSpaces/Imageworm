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

#ifndef SN__REDUCEAMBIGUITY_H
#define SN__REDUCEAMBIGUITY_H 1

#include <assert.h>

#include "tool_t.h"
#include "parameter_t.h"

#include "UpdateCounts.h"
#include "ScoringDivision.h"
#include "RemoveLink.h"
#include "compare_morphology.h"
#include "compare_partner_distance.h"

void
ReduceAmbiguity(const int time_start, const parameter_t * const parameters,
    const tool_t * const tools, const int t)
;
#endif /* ! SN__REDUCEAMBIGUITY_H */
