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

#ifndef SN__NUCRESIZE_H
#define SN__NUCRESIZE_H 1

#include <stdio.h>
#include <stdint.h>
#include "Crust.h"
#include "logfile.h"
#include "nucleus_t.h"
#include "frame_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "NucWeight.h"
#include "NucReCenter.h"

int
NucReSize(const parameter_t * const parameters, const int time_start,
    const tool_t * const tools,

    const uint8_t * const * const image_stack, frame_t * frame,
    nucleus_t * const nuc, int index, const int t, const int last_change)
;

#endif /* ! SN__NUCRESIZE_H */
