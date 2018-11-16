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

#include "LocalDensityCut_3D.h"
#include <stdint.h>
#include "struct_frame.h"
#include "parameter_t.h"
#include "DensityCut_2D.h"
#include "noise-filter.h"
#include "LocalWeightScan_2D.h"

/** TODO: Add documentation.
 * LocalDensityCut_3D uses these references from tools:
 * density_stack_2D
 * density_2D
 * noise
 */
void
LocalDensityCut_3D(const frame_t * const frame, const parameter_t * parameters,
    const uint32_t image_width, const uint32_t image_height,
    const uint32_t image_size, uint8_t ** image_stack, const int t,
    int * const * const density_stack_2D, int * const density_2D,
    const int * const * const noise)
{
  for (int i = 0; i < frame->num_planes; ++i)
    {
      LocalWeightScan_2D(image_stack[i], frame, parameters->neighborhood_size,
          density_stack_2D[i], density_2D);
      /* filtering images */
      DensityCut_2D(parameters, image_width, image_height, image_size,
          image_stack[i], density_stack_2D[i], noise[t][i], t);
    }
}

