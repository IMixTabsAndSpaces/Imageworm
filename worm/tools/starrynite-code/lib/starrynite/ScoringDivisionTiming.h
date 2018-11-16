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

#ifndef SN__SCORINGDIVISIONTIMING_H
#define SN__SCORINGDIVISIONTIMING_H 1

#include "ScoringDivision.h"
#include "nucleus_t.h"
#include "parameter_t.h"

int
ScoringDivisionTiming(const parameter_t * const parameters, const int t,
    const nucleus_t * const mother, const nucleus_t * const dau1,
    const nucleus_t * const dau2)
;
#endif /* ! SN__SCORINGDIVISIONTIMING_H */
