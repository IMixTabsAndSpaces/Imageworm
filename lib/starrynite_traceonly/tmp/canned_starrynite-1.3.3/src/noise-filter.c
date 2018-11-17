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
#include <stdint.h>
#include <assert.h>

#include "noise-filter.h"
#include "sliding-box.h"
#include "utility.h"
#include "types.h"

/* BEGIN FUNCTION PROTOTYPES */

/* Setting noise cutoff */

static void CompileDensityDistrib (
  const TOOL_t *,
  const PARAMETER_t *,
  uint32_t,
  uint32_t,
  const int *,
  int
);

static void Smoothing (
  const TOOL_t *,
  int length,
  int xlen,
  int *dmin_p,
  int *dmax_p,
  int rounds
);

static void SetCutoff (
  const TOOL_t *,
  const PARAMETER_t *,
  const int *dist,
  int dmin,
  int dmax,
  int t,
  int plane
);

static void LocalDensityStat (
  const TOOL_t *,
  const PARAMETER_t *,
  uint32_t,
  uint32_t,
  int t,
  int plane
);

static void ResetTools (
  const TOOL_t *,
  const PARAMETER_t *,
  uint32_t
);

/* Filtering images */

static void DensityCut_2D (
  const PARAMETER_t *,
  const TOOL_t *,
  const uint32_t image_width,
  const uint32_t image_height,
  const uint32_t image_size,
  uint8_t * image,
  int *sum,
  int cutoff,
  int t
);

/* END FUNCTION PROTOTYPES */


/************************
 * Setting noise cutoff *
 ************************/

/* Core functions are CalibrateNoise_3D() and LocalDensityStat() */


static void
CompileDensityDistrib (const TOOL_t * const tools,
                       const PARAMETER_t * const parameters,
                       const uint32_t image_width,
                       const uint32_t image_height,
                       const int *weights, const int length)
{
  int *const dist = tools->dist;
  const int step = parameters->neighborhood_size / 2;

  const int pix_begin = step * image_width;
  const int pix_end = (image_height - step) * image_width;

  int i;

  for (i = 0; i < length; i++)
    dist[i] = 0;

  for (i = pix_begin; i < pix_end; i++)
  {
    const int brightness = weights[i];
    if (brightness != 0)
      dist[brightness] = dist[brightness] + 1;
  }

  {
    const int ns = parameters->neighborhood_size;
    const int left = 200 * ns * ns;
    for (i = left; i < length; i++)
      dist[i] = 0;
  }

  dist[0] = 0;
}

/*
 * find_last_nonzero returns the largest 0 < n < len
 * such that dist[n] is nonzero, or 0 otherwise.
 */
static int
find_last_nonzero (const int *const dist, const int len)
{
  int i = len - 1;
  int j = 0;

  while (j == 0 && i > 0)
  {
    if (dist[i] != 0)
    {
      j = i;
    }
    --i;
  }

  return j;
}

static void
Smoothing (const TOOL_t * const tools, const int length, const int xlen,
           int *const dmin_p, int *const dmax_p, const int rounds)
{
  int i;

  int *dist1 = tools->dist1;

  int dmax = 0;
  int dmin = 1;

  memcpy (tools->dist1, tools->dist, sizeof (int) * length);

  /* TODO: Why does this stop at 1? */

  dmax = find_last_nonzero (dist1, length);

  assert (dmax > 0);

  int *dist2 = tools->dist2;

  for (i = 0; i < rounds; i++)
  {
    const int xw = xlen / 2;
    int j, *dist_tmp;
    int sum = 0;

    for (j = dmin; j < dmin + xlen; j++)
    {
      sum += dist1[j];
    }

    if (dmax > length - xw - 1)
    {
      dmax = length - xw - 1;
    }

    for (j = dmin + xw; j <= dmax; j++)
    {
      sum = sum - dist1[j - xw] + dist1[j + xw - 1];
      dist2[j] = sum / xlen;
    }

    dist_tmp = dist1;
    dist1 = dist2;
    dist2 = dist_tmp;
    dmin += xw;
    dmax -= xw;
  }

  *dmin_p = dmin;
  *dmax_p = dmax;
}


static void
SetCutoff (const TOOL_t * const tools, const PARAMETER_t * const parameters,
           const int *const dist, int dmin, const int dmax, const int t,
           const int plane)
{
  int max = 0, cut, i, start, end;
  int peak, *area;
  int pos_ct;

  peak = 0;
  area = tools->dist2;
  area[dmin - 1] = 0;

  for (i = dmin; i <= dmax; i++)
  {
    int x = dist[i];
    area[i] = area[i - 1] + x;
    if (x > peak)
    {
      peak = x;
      max = i;
    }
  }

  assert (max > 0);

  peak = area[max];

  for (i = dmin; i < max; i++)
  {
    int x = area[i];

    if (1.0 * x / peak > parameters->noise_fraction)
      break;
  }

  start = (i > dmin + 1) ? i - 2 : dmin;
  end = (i < dmax) ? i + 1 : dmax;

  cut = (area[end] - area[start]) / (end - start);

  pos_ct = 0;

  for (i = 0; i < 10; i++)
  {
    if (dist[dmax - i] > cut)
      pos_ct++;
  }

  if (dmin < 9)
    dmin = 9;

  for (i = dmax - 1; i > dmin && pos_ct < 5; i--)
  {
    if (dist[i + 1] > cut)
      pos_ct--;
    if (dist[i - 9] > cut)
      pos_ct++;
  }

  tools->noise_ori[t][plane] = i;
  tools->max_cutoff_ori[t][plane] = max;
}


static void
LocalDensityStat (const TOOL_t * const tools,
                  const PARAMETER_t * const parameters,
                  const uint32_t image_width, const uint32_t image_height,
                  const int t, const int plane)
{
  const int ns = parameters->neighborhood_size;
  const int length = 255 * ns * ns + 1;

  CompileDensityDistrib (tools, parameters, image_width, image_height,
                         tools->density_stack_2D[plane], length);

  const int xlen = 11;

  {
    int dmin, dmax;
    Smoothing (tools, length, xlen, &dmin, &dmax, 2);
    SetCutoff (tools, parameters, tools->dist1, dmin, dmax, t, plane);
  }
}



static void
ResetTools (const TOOL_t * const tools, const PARAMETER_t * const parameters,
            const uint32_t image_size)
{
  const int ns = parameters->neighborhood_size;
  const int length = 255 * ns * ns + 1;

  int *dist, i;

  dist = tools->dist;
  for (i = 0; i < length; i++)
    dist[i] = 0;

  dist = tools->dist1;
  for (i = 0; i < length; i++)
    dist[i] = 0;

  dist = tools->dist2;
  for (i = 0; i < length; i++)
    dist[i] = 0;

  dist = tools->distx;
  for (i = 0; i < image_size; i++)
    dist[i] = 0;
}


/* set noise cutoff for each plane in a z-stack */

void
CalibrateNoise_3D (const TOOL_t * const tools,
                   const PARAMETER_t * const parameters,
                   const MOVIE_t * const movie, const uint32_t image_width,
                   const uint32_t image_height, const uint32_t image_size,
                   uint8_t ** image_stack, const int t)
{
  const int plane_number = movie->frame.plane_number;
  int i;

  ResetTools (tools, parameters, image_size);
  for (i = 0; i < plane_number; i++)
  {
    LocalWeightScan_2D (image_stack[i], tools->density_stack_2D[i],
                        tools->density_2D, &(movie->frame),
                        parameters->neighborhood_size);
    /* calibrating noise */
    LocalDensityStat (tools, parameters, image_width, image_height, t, i);
  }
}





/***********************************
 * Averaging noise model over time *
 ***********************************/


void
SmoothingNoiseModel (const TOOL_t * const tools,
                     const PARAMETER_t * const parameters,
                     const MOVIE_t * const movie)
{
  int i, in;
  int *sum, *target;

  int plane_number = movie->frame.plane_number;
  int tp_number = movie->tp_number;

  sum = (int *) malloc_exit (sizeof (int *) * plane_number);

  if (tp_number < 10)
  {
    memcpy (sum, tools->noise_ori[0], sizeof (int) * plane_number);
    for (i = 1; i < tp_number; i++)
      AddRow (sum, tools->noise_ori[i], plane_number);
    target = tools->noise[0];
    for (i = 0; i < plane_number; i++)
      target[i] = sum[i] / tp_number;
    for (i = 1; i < tp_number; i++)
      memcpy (tools->noise[i], target, sizeof (int) * plane_number);

    memcpy (sum, tools->max_cutoff_ori[0], sizeof (int) * plane_number);
    for (i = 1; i < tp_number; i++)
      AddRow (sum, tools->max_cutoff_ori[i], plane_number);
    target = tools->max_cutoff[0];
    for (i = 0; i < plane_number; i++)
      target[i] = sum[i] / tp_number;
    for (i = 1; i < tp_number; i++)
      memcpy (tools->max_cutoff[i], target, sizeof (int) * plane_number);

    free (sum);
    return;
  }

  memcpy (sum, tools->noise_ori[0], sizeof (int) * plane_number);
  for (i = 1; i < 9; i++)
    AddRow (sum, tools->noise_ori[i], plane_number);
  target = tools->noise[4];
  for (i = 0; i < plane_number; i++)
    target[i] = sum[i] / 9;
  for (i = 0; i < 4; i++)
    memcpy (tools->noise[i], target, sizeof (int) * plane_number);
  for (in = 9; in < tp_number; in++)
  {
    SubtractRow (sum, tools->noise_ori[in - 9], plane_number);
    AddRow (sum, tools->noise_ori[in], plane_number);
    target = tools->noise[in - 4];
    for (i = 0; i < plane_number; i++)
      target[i] = sum[i] / 9;
  }
  for (i = tp_number - 4; i < tp_number; i++)
    memcpy (tools->noise[i], target, sizeof (int) * plane_number);

  memcpy (sum, tools->max_cutoff_ori[0], sizeof (int) * plane_number);
  for (i = 1; i < 9; i++)
    AddRow (sum, tools->max_cutoff_ori[i], plane_number);
  target = tools->max_cutoff[4];
  for (i = 0; i < plane_number; i++)
    target[i] = sum[i] / 9;
  for (i = 0; i < 4; i++)
    memcpy (tools->max_cutoff[i], target, sizeof (int) * plane_number);
  for (in = 9; in < tp_number; in++)
  {
    SubtractRow (sum, tools->max_cutoff_ori[in - 9], plane_number);
    AddRow (sum, tools->max_cutoff_ori[in], plane_number);
    target = tools->max_cutoff[in - 4];
    for (i = 0; i < plane_number; i++)
      target[i] = sum[i] / 9;
  }
  for (i = tp_number - 4; i < tp_number; i++)
    memcpy (tools->max_cutoff[i], target, sizeof (int) * plane_number);

  free (sum);
}



/* TODO: Pass simpler parameters. */

void
UpdateNoise (const TOOL_t * const tools,
             const PARAMETER_t * const parameters,
             const MOVIE_t * const movie, int t)
{
  int i;

  for (i = 0; i < movie->frame.plane_number; i++)
  {
    tools->noise[t][i] =
      (int) (tools->noise[t][i] * parameters->noise_factor);
  }
}



/********************
 * filtering images *
 ********************/


void
DensityCut_2D (const PARAMETER_t * const parameters,
               const TOOL_t * const tools,
               const uint32_t image_width,
               const uint32_t image_height,
               const uint32_t image_size,
               uint8_t * const image, int *sum, int cutoff, int t)
{
  const int step = parameters->neighborhood_size / 2;
  const int width_end = image_width - step;
  const int height_end = image_height - step;
  const int area =
    parameters->neighborhood_size * parameters->neighborhood_size;

  int i;

  /* cleaning up the first "step" rows */
  const int pad_length = step * image_width;
  for (i = 0; i < pad_length; i++)
  {
    image[i] = (uint8_t) 0;
  }

  /* the middle rows */
  for (i = step; i < height_end; i++)
  {
    const int base = i * image_width;
    int j;
    /* cleaning up the first "step" pixels in the row */
    for (j = 0; j < step; j++)
      image[base + j] = (uint8_t) 0;
    /* processing the middle of the row */
    for (j = step; j < width_end; j++)
    {
      if (sum[base + j] <= cutoff)
        image[base + j] = (uint8_t) 0;
      else if (t < 30)
        image[base + j] = (uint8_t) (sum[base + j] / area);     /* smoothing! */
    }
    /* cleaning up the last "step" pixels in the row */
    for (j = width_end; j < image_width; j++)
      image[base + j] = (uint8_t) 0;
  }

  /* cleaning up the last "step" rows */
  {
    const int base = height_end * image_width;
    for (i = base; i < image_size; i++)
    {
      image[i] = (uint8_t) 0;
    }
  }
}


void
LocalDensityCut_3D (const MOVIE_t * movie, const PARAMETER_t * parameters,
                    const TOOL_t * tools, const uint32_t image_width,
                    const uint32_t image_height, const uint32_t image_size,
                    uint8_t ** image_stack, int t)
{
  int plane_number = movie->frame.plane_number;
  int i;

  for (i = 0; i < plane_number; i++)
  {
    LocalWeightScan_2D (image_stack[i], tools->density_stack_2D[i],
                        tools->density_2D, &(movie->frame),
                        parameters->neighborhood_size);
    /* filtering images */
    DensityCut_2D (parameters, tools, image_width, image_height, image_size,
                   image_stack[i], tools->density_stack_2D[i],
                   tools->noise[t][i], t);
  }
}
