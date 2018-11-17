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

#include "PizzaSlices.h"

#include "struct_frame.h"
#include "tool_t.h"
#include "struct_nucleus.h"

pizzaslices_rval_t PizzaSlices(const tool_t * const tools,
		const uint8_t * const * const image_stack, frame_t * frame,
		nucleus_t * nuc) {
	int num_planes = frame->num_planes;
	int my_image_width = frame->width;
	int my_image_height = frame->height;
	int nuc_size;
	int slices, r, *radii, **x_range, dist;
	int i, j, k, m;
	int x, y, z, base_line, base, x_start, x_end;
	const uint8_t *image;
	int xdiff, average, signal;

	int piece_weight[8];
	int piece_vol[8];
	int piece_brights[8];

	for (i = 0; i < 8; i++) {
		piece_weight[i] = 0;
		piece_vol[i] = 0;
		piece_brights[i] = 0;
	}

	x = nuc->x;
	y = nuc->y;
	z = (int) (nuc->z + 0.5);
	nuc_size = nuc->size;

	x_range = tools->spheres[nuc_size];
	radii = tools->s_radii[nuc_size];
	slices = tools->s_layers[nuc_size];
	average = nuc->weight / nuc->volume;

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
				signal = (int) image[base + m];
				xdiff = m - x;
				if (k >= 0) {
					if (xdiff >= 0) {
						if (k < xdiff) {
							piece_weight[0] += signal;
							piece_vol[0]++;
							if (signal >= average)
								piece_brights[0]++;
						} else {
							piece_weight[1] += signal;
							piece_vol[1]++;
							if (signal >= average)
								piece_brights[1]++;
						}
					} else {
						if (k > 0 - xdiff) {
							piece_weight[2] += signal;
							piece_vol[2]++;
							if (signal >= average)
								piece_brights[2]++;
						} else {
							piece_weight[3] += (int) image[base + m];
							piece_vol[3]++;
							if (signal >= average)
								piece_brights[3]++;
						}
					}
				} else {
					if (xdiff < 0) {
						if (0 - k < 0 - xdiff) {
							piece_weight[4] += signal;
							piece_vol[4]++;
							if (signal >= average)
								piece_brights[4]++;
						} else {
							piece_weight[5] += signal;
							piece_vol[5]++;
							if (signal >= average)
								piece_brights[5]++;
						}
					} else {
						if (0 - k > xdiff) {
							piece_weight[6] += signal;
							piece_vol[6]++;
							if (signal >= average)
								piece_brights[6]++;
						} else {
							piece_weight[7] += signal;
							piece_vol[7]++;
							if (signal >= average)
								piece_brights[7]++;
						}
					}
				}
			}
		}
	}

	pizzaslices_rval_t rval;

	/* TODO: extract the code repeated here into a subroutine */
	{
		float piece = 1.0 * (piece_weight[0] + piece_weight[4]) / (piece_vol[0]
				+ piece_vol[4]);
		float max = piece;
		float min = piece;
		for (i = 1; i < 4; i++) {
			piece = 1.0 * (piece_weight[i] + piece_weight[i + 4])
					/ (piece_vol[i] + piece_vol[i + 4]);
			if (piece > max)
				max = piece;
			else if (piece < min)
				min = piece;
		}
		rval.piece_ratio = min / max;
	}

	{
		float piece = 1.0 * (piece_brights[0] + piece_brights[4])
				/ (piece_vol[0] + piece_vol[4]);
		float max = piece;
		float min = piece;
		for (i = 1; i < 4; i++) {
			piece = 1.0 * (piece_brights[i] + piece_brights[i + 4])
					/ (piece_vol[i] + piece_vol[i + 4]);
			if (piece > max)
				max = piece;
			else if (piece < min)
				min = piece;
		}
		rval.piece_ratio2 = min / max;
	}

	return rval;
}
