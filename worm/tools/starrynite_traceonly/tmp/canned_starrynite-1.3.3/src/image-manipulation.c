/* Copyright (C) 2005-2008 University of Washington
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
#endif

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "image-manipulation.h"
#include "tiff-io.h"
#include "colors.h"
#include "utility.h"
#include "error-codes.h"
#include "types.h"

/* BEGIN FUNCTION PROTOTYPES */

static void DrawSphere (
  const TOOL_t *,
  uint8_t ** image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc,
  int thickness,
  int brightness,
  int color
);

static void SetPixel (
  uint8_t * image,
  int index,
  int color,
  int brightness
);

/* END FUNCTION PROTOTYPES */

void
ReadImageStack (const MOVIE_t * const movie, const char *const input_prefix,
                int tp, int plane_start, uint8_t ** image_stack,
                const bool is_first_tiff,
                uint32_t * const image_width_record_p,
                uint32_t * const image_height_record_p)
{
  int i;
  const size_t pathname_len = FILENAME_MAX;
  char pathname[pathname_len];
  uint32_t image_width;
  uint32_t image_height;

  const int plane_number = movie->frame.plane_number;

  for (i = 0; i < plane_number; i++)
  {
	 // if(plane_number<100)
	 // {
    (void) snprintf (pathname, pathname_len, "%s-t%03d-p%02d.tif",
                     input_prefix, tp, plane_start + i);
	 // }
	//  else
	//  {
	//(void) snprintf (pathname, pathname_len, "%s-t%03d-p%03d.tif",
     //                input_prefix, tp, plane_start + i);
	//  }
    ReadTiff (pathname, image_stack + i, &image_width, &image_height);
    if (is_first_tiff)
    {
      *image_width_record_p = image_width;
      *image_height_record_p = image_height;
    }
    else if (image_width != *image_width_record_p
             || image_height != *image_height_record_p)
    {
      const char *const str = "Different size of image for different planes";
      fprintf (stderr, "ERROR: %s\n", str);
      exit (ERROR_READIMAGESTACK);
    }
  }
}

void
GetImageStacks (const MOVIE_t * const movie,
                const PARAMETER_t * const parameters, TOOL_t * const tools,
                const char *const input_prefix, const bool is_first_tiff,
                uint32_t * const image_width_record_p,
                uint32_t * const image_height_record_p)
{
  int plane_number = movie->frame.plane_number;
  int i;

  if (parameters->t != 0)
  {
    if (parameters->t > 1)
    {
      for (i = 0; i < plane_number; i++)
        free (tools->image_stack_prev[i]);
      free (tools->image_stack_prev);
    }
    tools->image_stack_prev = tools->image_stack;
  }

  tools->image_stack =
    (uint8_t **) malloc_exit (sizeof (uint8_t *) * plane_number);

  /* have to allocate *(image_stack+i) in ReadTiff (or later) b/c we don't yet know the value of image_size */
  ReadImageStack (movie, input_prefix, movie->time_start + parameters->t,
                  movie->plane_start, tools->image_stack, is_first_tiff,
                  image_width_record_p, image_height_record_p);
}


/* output tools */
/* MarkCentroid() and DrawCircle() are called by DrawNuclei().
 * DrawNuclei() will probably change with data structure (eg, for CELL),
 * but MarkCentroid() and DrawCircle() are likely to remain the same. */

void
TurnGreen (const MOVIE_t * const movie, const uint32_t image_size,
           uint8_t ** image_stack)
{
  int plane_number = movie->frame.plane_number;
  int i, j, RGBsize, brightness, colormix;
  uint8_t *RGBimage, *Grayimage;

  RGBsize = image_size * 3;
  for (i = 0; i < plane_number; i++)
  {
    RGBimage = (uint8_t *) malloc_exit (sizeof (uint8_t) * image_size * 3);
    for (j = 0; j < RGBsize; j++)
      RGBimage[j] = 0;
    Grayimage = image_stack[i];
    for (j = 0; j < image_size; j++)
    {
      brightness = (int) Grayimage[j];
      colormix = (brightness - 32) / 4;
      if (colormix < 0)
        colormix = 0;
      RGBimage[j * 3] = (uint8_t) colormix;
      RGBimage[j * 3 + 1] = (uint8_t) brightness;
      RGBimage[j * 3 + 2] = (uint8_t) colormix;
    }
    image_stack[i] = RGBimage;
    free (Grayimage);
  }
}



static void
SetPixel (uint8_t * image, int index, int color, int brightness)
{
  if (color == MONO)
  {
    image[index] = (uint8_t) brightness;
  }
  else if (color == RED)
  {
    image[index * 3] = (uint8_t) brightness;
    image[index * 3 + 1] = (uint8_t) 63;
        /**(image+index*3+2) = 63;*/
  }
  else if (color == YELLOW)
  {
    image[index * 3] = (uint8_t) brightness;
    image[index * 3 + 1] = (uint8_t) brightness;
  }
}


void
MarkCentroid (uint8_t * image, const FRAME_t * const frame, const int x,
              const int y, const int color, const int brightness)
{
  int i;

  for (i = -1; i < 2; i++)
  {
    int j;
    for (j = -1; j < 2; j++)
    {
      const int index = y + i * frame->width + x + j;
      SetPixel (image, index, color, brightness);
    }
  }
}


static void
DrawSphere (const TOOL_t * const tools, uint8_t ** image_stack,
            FRAME_t * frame, NUCLEUS_t * nuc, int thickness, int brightness,
            int color)
{
  int slices, r, *radii, **x_range, dist;
  int i, j, k;
  int x, y, z, base_line, base, pos;
  uint8_t *image;

  int plane_number = frame->plane_number;
  int my_image_width = frame->width;
  int my_image_height = frame->height;

  x = nuc->x;
  y = nuc->y;
  z = (int) (nuc->z + 0.5);

  x_range = tools->spheres[nuc->size];
  radii = tools->s_radii[nuc->size];
  slices = tools->s_layers[nuc->size];

  for (j = 0 - slices; j <= slices; j++)
  {
    if (z + j < 0 || z + j >= plane_number)
      continue;
    r = radii[slices + j];
    image = image_stack[z + j];
    base_line = y / my_image_width;
    for (k = 0 - r; k <= r; k++)
    {
      if (base_line + k < 0)
        continue;
      if (base_line + k >= my_image_height)
        break;
      base = y + k * my_image_width;
      dist = x_range[slices + j][r + k];
      for (pos = x - dist, i = 0; i < thickness; i++, pos++)
      {
        if (pos > 0 && pos < my_image_width)
          SetPixel (image, base + pos, color, brightness);
      }
      for (pos = x + dist, i = 0; i < thickness; i++, pos--)
      {
        if (pos > 0 && pos < my_image_width)
          SetPixel (image, base + pos, color, brightness);
      }
    }
  }
}


/* drawing the model sphere for defined nuclei */
void
DrawNuclei (const MOVIE_t * const movie, const TOOL_t * const tools,
            uint8_t ** image_stack, FRAME_t * frame, int t, int thickness,
            int brightness, int color, int markcen)
{
  int i;
  NUCLEUS_t *nuclei = tools->nuclei_record[t];
  int nuc_ct = tools->nuc_ct_rec[t];

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    DrawSphere (tools, image_stack, frame, nuclei + i, thickness, brightness,
                color);
    if (markcen)
      MarkCentroid (image_stack[(int) (nuclei[i].z + 0.5)],
                    &movie->frame, nuclei[i].x, nuclei[i].y, YELLOW, 255);
  }
}
