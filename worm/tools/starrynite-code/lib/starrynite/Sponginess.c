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

#include <stdint.h>

#include "Sponginess.h"

#include "struct_nucleus.h"
#include "tool_t.h"
#include "struct_frame.h"

sponginess_rval_t
Sponginess(const tool_t * const tools,
		const uint8_t * const * const image_stack, frame_t * frame,
		nucleus_t * nuc) {
	int num_planes = frame->num_planes;
	int my_image_width = frame->width;
	int my_image_height = frame->height;
	int nuc_size;
	int slices, r, *radii, **x_range, dist;
	int j, k, m;
	int x, y, z, base_line, base, x_start, x_end;
	const uint8_t *image;
	int average;
	int b_ct, db_ct, d_ct, bd_ct;

	x = nuc->x;
	y = nuc->y;
	z = (int) (nuc->z + 0.5);
	nuc_size = nuc->size;

	x_range = tools->spheres[nuc_size];
	radii = tools->s_radii[nuc_size];
	slices = tools->s_layers[nuc_size];

	average = nuc->weight / nuc->volume;
	b_ct = 0;
	db_ct = 0;
	d_ct = 0;
	bd_ct = 0;

	for (j = 0 - slices; j <= slices; j++) {
		if (z + j < 0 || z + j >= num_planes)
			continue;
		r = radii[slices + j];
		image = image_stack[z + j];
		base_line = y / my_image_width;
		for (k = 0 - r; k <= r; k++) {
			if (base_line + k < 0)
				continue;
			if (base_line + k >= my_image_height)
				break;
			base = y + k * my_image_width;
			dist = x_range[slices + j][r + k];
			x_start = x - dist > 1 ? x - dist : 1;
			x_end = x + dist < my_image_width - 1 ? x + dist : my_image_width
					- 1;
			for (m = x_start; m <= x_end; m++) {
				if ((int) image[base + m] >= average) {
					b_ct++;
					if (m && (int) image[base + m - 1] < average) {
						db_ct++;
					}
					if (m < my_image_width - 1 && (int) image[base + m + 1]
							< average) {
						db_ct++;
					}
				} else {
					d_ct++;
					if (m && (int) image[base + m - 1] >= average) {
						bd_ct++;
					}
					if (m < my_image_width - 1 && (int) image[base + m + 1]
							>= average) {
						bd_ct++;
					}
				}
			}
		}
	}

	{
		sponginess_rval_t rval;

		if (b_ct)
			rval.sponginess1 = 0.5 * db_ct / b_ct;
		else
			rval.sponginess1 = 0.5;

		if (d_ct)
			rval.sponginess2 = 0.5 * bd_ct / d_ct;
		else
			rval.sponginess2 = 0.5;

		return rval;
	}
}
