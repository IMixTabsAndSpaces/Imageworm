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

#include "DistanceFilter.h"

#include "struct_nucleus.h"
#include "metric_calculate_distance.h"
#include "metric_t.h"

void
DistanceFilter(nucleus_t * nuclei_recov, int nuc_ct_recov,
	nucleus_t * nuclei, int nuc_ct, const metric_t *const metric,
	const float nuc_size_factor4)
{
  for (int i = 0; i < nuc_ct; i++)
    {
      if (!nuclei[i].is_valid)
        continue;

      for (int j = 0; j < nuc_ct_recov; j++)
        {
          if (!nuclei_recov[j].is_valid)
            continue;

          const float distance = metric_calculate_distance(metric, &nuclei_recov[j],
		  &nuclei[i]);

          const int cutoff = nuc_size_factor4 * 
	      (nuclei_recov[j].size + nuclei[i].size) / 2;

          if (distance < cutoff)
            {
              nuclei_recov[j].is_valid = 0;
            }
        }
    }
}
