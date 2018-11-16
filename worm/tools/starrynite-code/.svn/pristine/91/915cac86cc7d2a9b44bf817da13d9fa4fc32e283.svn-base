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

#include "CalibrateNoise_3D.h"
#include <stdint.h>
#include "LocalDensityStat.h"
#include "noise-filter.h"
#include "LocalWeightScan_2D.h"
#include "struct_frame.h"

/** Sets noise cutoff for each plane in a z-stack
 */
void
CalibrateNoise_3D(int * const * const density_stack_2D, int * const density_2D,
    int * const * const noise_ori, int * const * const max_cutoff_ori,
    const int neighborhood_size, const float noise_fraction,
    const frame_t * const frame, const uint32_t image_width,
    const uint32_t image_height, const uint8_t * const * const image_stack,
    const int t)
{
  const int num_planes = frame->num_planes;
  const int length = 255 * neighborhood_size * neighborhood_size + 1;
  int i;

  int dist[length];
  int dist1[length];
  int dist2[length];

  for (i = 0; i < num_planes; i++)
    {
      LocalWeightScan_2D(image_stack[i], frame, neighborhood_size,
          density_stack_2D[i], density_2D);

      /* Calibrating noise */

      LocalDensityStat(neighborhood_size, noise_fraction, image_width,
          image_height, t, i, density_stack_2D, dist, dist1, dist2, noise_ori,
          max_cutoff_ori);
    }
}

