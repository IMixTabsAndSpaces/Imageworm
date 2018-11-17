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

#include "MeasureMorphology.h"

#include <stdint.h>

#include "PizzaSlices.h"
#include "frame_t.h"
#include "tool_t.h"
#include "Sponginess.h"
#include "struct_nucleus.h"

void MeasureMorphology(const tool_t * const tools,
		const uint8_t * const * const image_stack, frame_t * frame,
		nucleus_t * nuc) {
	float sponge, slice;

	sponginess_rval_t sponginess_rval = Sponginess(tools, image_stack, frame,
			nuc);

	pizzaslices_rval_t pizzaslices_rval = PizzaSlices(tools, image_stack, frame, nuc);

	if ((sponginess_rval.sponginess1 <= 0.25 || sponginess_rval.sponginess2
			<= 0.22) && (pizzaslices_rval.piece_ratio <= 0.69 || pizzaslices_rval.piece_ratio2 <= 0.52)) {
		sponge
				= sponginess_rval.sponginess1 - 0.25
						< sponginess_rval.sponginess2 - 0.22 ? sponginess_rval.sponginess1
						- 0.25
						: sponginess_rval.sponginess2 - 0.22;
		slice
				= pizzaslices_rval.piece_ratio - 0.69 < pizzaslices_rval.piece_ratio2 - 0.52 ? pizzaslices_rval.piece_ratio
						- 0.69
						: pizzaslices_rval.piece_ratio2 - 0.52;
		if (sponge + slice < -0.05)
			nuc->condensed = 3;
	}
}
