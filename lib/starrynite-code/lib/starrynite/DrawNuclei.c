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

#include "movie_t.h"
#include "frame_t.h"
#include "tool_t.h"
#include "MarkCentroid.h"
#include "DrawSphere.h"
#include "DrawNuclei.h"
#include "struct_nucleus.h"

/* TODO: Fix parameter set, depends on DrawSphere */
/** Draws the model sphere for defined nuclei */
void
DrawNuclei(const movie_t * const movie, const tool_t * const tools,
    uint8_t ** image_stack, frame_t * frame, int t, int thickness,
    int brightness, int color, int markcen)
{
  nucleus_t *nuclei = tools->nuclei_record[t];
  int nuc_ct = tools->nuc_ct_rec[t];

  for (int i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].is_valid == 0)
        continue;

      DrawSphere(tools, image_stack, frame, nuclei + i, thickness, brightness,
          color);

      if (markcen != 0) {
        MarkCentroid(image_stack[(int) (nuclei[i].z + 0.5)], &movie->frame,
            nuclei[i].x, nuclei[i].y, YELLOW, 255);
      }
    }
}
