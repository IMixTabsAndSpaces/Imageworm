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

#include "RecordDivision.h"

#include "struct_nucleus.h"

void
RecordDivision(nucleus_t * const * const nuclei_record, const int t,
    const int m_index, const int d_index2)
{
  nucleus_t * const mother = nuclei_record[t - 1] + m_index;
  nucleus_t * const daughter1 = nuclei_record[t] + mother->successor1;
  nucleus_t * const daughter2 = nuclei_record[t] + d_index2;

  mother->successor2 = d_index2;
  mother->distance_to_successor2 = daughter2->distance_to_predecessor;
  mother->has_successor2 = 1;
  mother->time_end = t - 1;

  daughter2->has_predecessor = 1;

  daughter2->generation = mother->generation + 1;
  daughter2->time_start = t;
  daughter1->generation = mother->generation + 1;
  daughter1->time_start = t;
}
