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

#include <stdio.h>

#include "UpdateNucSize.h"

#include "parameter_t.h"
#include "logfile.h"
#include "struct_nucleus.h"

void
UpdateNucSize(const nucleus_t * const nuclei, const int nuc_ct,
    parameter_t * const parameters, const int t)
{
  int i;
  int total, ct;

  const int cutoff1 = (int) (0.7 * parameters->nuc_size);
  const int cutoff2 = (nuc_ct < 150) ? (int) (1.2 * parameters->nuc_size)
      : (int) (1.1 * parameters->nuc_size);

  total = 0;

  /* ct will hold the number of nuclei in the size range */
  ct = 0;

  for (i = 0; i < nuc_ct; ++i)
    {
      const int size = nuclei[i].size;
      if (size >= cutoff1 && size <= cutoff2)
        {
          total += size;
          ++ct;
        }
    }

  if (ct < 30 && ct < nuc_ct / 3 + 1)
    {
      const char fmt[] = "%s: time %d, %d out of %d total\n";
      const char msg[] = "Not enough nuclei within size range";

      fprintf(sn_logfile, fmt, msg, t, ct, nuc_ct);
    }
  else
    {
      parameters->nuc_size = total / ct;
    }

  parameters->sizes[t] = parameters->nuc_size;
}

