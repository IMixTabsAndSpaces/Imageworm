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

#include "TimeToDivide.h"

#include "struct_nucleus.h"

/** Returns the time at which the nucleus nuc divides, or num_timepts if the
 * nucleus does not divide, or -1 if the nucleus disappears before dividing */
int
TimeToDivide(const nucleus_t * const * const nuclei_record, int current_time,
    const nucleus_t * nuc, const int num_timepts)
{
  while (current_time < num_timepts - 1)
    {
      if (nuc->has_successor1 == 0)
        {
          current_time = -1;
          break;
        }
      if (nuc->has_successor2 != 0)
        {
          break;
        }
      current_time++;
      nuc = nuclei_record[current_time] + nuc->successor1;
    }

  return current_time;
}
