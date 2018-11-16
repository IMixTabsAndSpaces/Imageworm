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

#include "SmoothingNoiseModel.h"
#include <string.h>
#include "noise-filter.h"
#include "AddRow.h"
#include "vector_subtract_vector.h"

/************************************
 * Averaging noise model over time *
 ***********************************/
/* TODO: Add documentation */
void
SmoothingNoiseModel(int * const * const noise, int * const * const noise_ori,
    int * const * const max_cutoff, int * const * const max_cutoff_ori,
    const int num_planes, const int num_timepts)
{
  int in;
  int *target;

  int sum[num_planes];
  const size_t sum_size = sizeof(int) * num_planes;

  if (num_timepts < 10)
    {
      /* These two steps add the noise_ori vectors into the vector sum: */

      /* First, copy the values on noise_ori[0] to the vector sum. */
      memcpy(sum, noise_ori[0], sum_size);

      /* Second, add the remaining noise_ori[i] vectors to the vector sum. */
      for (int i = 1; i < num_timepts; i++)
        AddRow(sum, noise_ori[i], num_planes);

      target = noise[0];
      for (int i = 0; i < num_planes; i++)
        target[i] = sum[i] / num_timepts;

      /* Now, write 'target' back to the individual noise[i] vectors. */
      for (int i = 1; i < num_timepts; i++)
        memcpy(noise[i], target, sum_size);

      /* Now, do the same with max_cutoff_ori and max_cutoff */

      memcpy(sum, max_cutoff_ori[0], sum_size);

      for (int i = 1; i < num_timepts; i++)
        AddRow(sum, max_cutoff_ori[i], num_planes);

      target = max_cutoff[0];

      for (int i = 0; i < num_planes; i++)
        target[i] = sum[i] / num_timepts;

      for (int i = 1; i < num_timepts; i++)
        memcpy(max_cutoff[i], target, sum_size);

      return;
    }

  /* Here, num_timepts >= 10 */

  memcpy(sum, noise_ori[0], sum_size);

  for (int i = 1; i < 9; i++)
    AddRow(sum, noise_ori[i], num_planes);

  target = noise[4];
  for (int i = 0; i < num_planes; i++)
    target[i] = sum[i] / 9;

  for (int i = 0; i < 4; i++)
    memcpy(noise[i], target, sum_size);

  for (in = 9; in < num_timepts; in++)
    {
      vector_subtract_vector(sum, noise_ori[in - 9], num_planes);
      AddRow(sum, noise_ori[in], num_planes);
      target = noise[in - 4];
      for (int i = 0; i < num_planes; i++)
        target[i] = sum[i] / 9;
    }

  for (int i = num_timepts - 4; i < num_timepts; i++)
    memcpy(noise[i], target, sum_size);

  memcpy(sum, max_cutoff_ori[0], sum_size);
  for (int i = 1; i < 9; i++)
    AddRow(sum, max_cutoff_ori[i], num_planes);

  target = max_cutoff[4];
  for (int i = 0; i < num_planes; i++)
    target[i] = sum[i] / 9;

  for (int i = 0; i < 4; i++)
    memcpy(max_cutoff[i], target, sum_size);

  for (in = 9; in < num_timepts; in++)
    {
      vector_subtract_vector(sum, max_cutoff_ori[in - 9], num_planes);
      AddRow(sum, max_cutoff_ori[in], num_planes);

      target = max_cutoff[in - 4];
      for (int i = 0; i < num_planes; i++)
        target[i] = sum[i] / 9;
    }

  for (int i = num_timepts - 4; i < num_timepts; i++)
    memcpy(max_cutoff[i], target, sum_size);

}

