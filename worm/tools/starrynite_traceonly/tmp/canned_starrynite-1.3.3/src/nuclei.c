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
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "nuclei.h"
#include "colors.h"

#include "types.h"

#include "parameters-tools.h"
#include "sliding-box.h"
#include "utility.h"
#include "image-manipulation.h"

/* BEGIN FUNCTION PROTOTYPES */

static void LocalMaxima_3D (
  TOOL_t * const tools,
  PARAMETER_t * const parameters,
  uint8_t * const *backup_stack,
  FILE * const logfile,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  int report_max,
  int t
);

static void NucInit (
  const TOOL_t *,
  const PARAMETER_t *,
  NUCLEUS_t * nuc,
  int t,
  int index
);

static int NucReCenter (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc
);

static int NucWeight (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc
);

static void PizzaSlices (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc
);

static void Sponginess (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc
);

static void MeasureMorphology (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc
);

static float Crust (
  const TOOL_t *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuc
);

static int NucReSize (
  const PARAMETER_t *,
  const MOVIE_t *,
  const TOOL_t *,
  FILE *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * const nuc,
  int index,
  const int t,
  const int last_change
);

/* static float NucDistance_square (
 *   NUCLEUS_t * nuc1, 
 *   NUCLEUS_t * nuc2,
 *   FRAME_t * frame
 * );
 */

static void DensityFilter (
  const PARAMETER_t *,
  const TOOL_t *,
  NUCLEUS_t * nuclei,
  int nuc_ct,
  int t
);

static void SelfDistanceFilter (
  const PARAMETER_t *,
  NUCLEUS_t * nuclei,
  int nuc_ct,
  FRAME_t * frame
);

static void RefineNuclei (
  const TOOL_t *,
  const PARAMETER_t *,
  const MOVIE_t *,
  FILE *,
  const uint8_t * const *image_stack,
  FRAME_t * frame,
  NUCLEUS_t * nuclei,
  int t
);

static void DistanceFilter (
  const PARAMETER_t *,
  NUCLEUS_t * nuclei_recov,
  int nuc_ct_recov,
  NUCLEUS_t * nuclei,
  int nuc_ct,
  FRAME_t * frame
);

static void record_max (
  uint8_t * const *const backup_stack,
  int *const stat_plane,
  const int base,
  const int k,
  const int *const plane,
  const int max,
  const int weight_max_cutoff,
  const int report_max,
  const int i,
  const FRAME_t * const frame
);

/* END FUNCTION PROTOTYPES */

static int
update_max (const int val, int *const max, int *const pos, const int in)
{
  int rval;
  if (val >= *max)
  {
    *max = val;
    *pos = in;
    rval = 1;
  }
  else
  {
    rval = 0;
  }
  return rval;
}


static void
reestablish_max (int *const pos, const int out, int *const max,
                 const int *const plane, const int base, const int in)
{
  int i, cur;

  *pos = out + 1;
  *max = plane[base + *pos];
  for (i = out + 2; i <= in; ++i)
  {

    cur = plane[base + i];

    if (cur >= *max)
    {
      *max = cur;
      *pos = i;
    }
  }
}



static void
record_max (uint8_t * const *const backup_stack, int *const stat_plane,
            const int base, const int k, const int *const plane,
            const int max, const int weight_max_cutoff, const int report_max,
            const int i, const FRAME_t * const frame)
{
  stat_plane[(base + k) * 2] = plane[base + k];

  if (max > weight_max_cutoff && plane[base + k] == max)
  {
    stat_plane[(base + k) * 2 + 1] = 1;
    if (report_max == 1)
    {
      MarkCentroid (backup_stack[i], frame, k, base, MONO, 191);
    }
  }
  else
  {
    stat_plane[(base + k) * 2 + 1] = 0;
  }
}


static void
max_scan_y (uint8_t * const *const backup_stack,
            const int i,
            const int my_image_width,
            const int j,
            const int step1,
            const int width_end1,
            int *const stat_plane,
            int *const centroids,
            const FRAME_t * const frame,
            const int in, const int out, const int report_max)
{
  int max_ct = 0;
  int base = my_image_width * j;
  int k, m;

  for (k = step1; k < width_end1; ++k)
  {
    if (stat_plane[(base + k) * 2 + 1] == 1)
    {
      centroids[5 * max_ct + 1] = k;
      centroids[5 * max_ct + 4] = stat_plane[(base + k) * 2];   /* 'weight' */
      ++max_ct;
    }
  }

  /* changed 'continue' in macro to 'return' in function */
  if (!max_ct)
    return;

  /* backup_stack is a global */

  if (report_max == 2)
  {
    for (k = 0; k < max_ct; k++)
    {
      MarkCentroid (backup_stack[i], frame, centroids[5 * k + 1], base,
                    MONO, 191);
    }
  }

  {
    int max_remained = max_ct;

    for (m = out; m <= in; m++)
    {
      if (m == j)
        continue;
      for (k = 0; k < max_ct; k++)
      {
        const int cur = centroids[5 * k + 4];
        if (!cur)
          continue;
        if (cur < stat_plane[(m * my_image_width + centroids[5 * k + 1]) * 2])
        {
          centroids[5 * k + 4] = 0;
          --max_remained;
          if (!max_remained)
            break;
        }
      }
      if (!max_remained)
        break;
    }

  }

  /* backup_stack is a global */

  for (k = 0; k < max_ct; k++)
  {
    if (!centroids[5 * k + 4])
    {
      stat_plane[(base + centroids[5 * k + 1]) * 2 + 1] = 0;
    }
    else if (report_max == 3)
    {
      MarkCentroid (backup_stack[i], frame, centroids[5 * k + 1], base,
                    MONO, 255);
    }
  }

}



/* REMEMBER TO IMPLEMENT PROOF-CHECK FOR max_ct/max_tmp_ct NOT EXCEEDING CEN_CT_LIMIT */
static void
LocalMaxima_3D (TOOL_t * const tools, PARAMETER_t * const parameters,
                uint8_t * const *backup_stack, FILE * const logfile,
                const uint8_t * const *image_stack, FRAME_t * frame,
                int report_max, int t)
{
  int **density_stack, **max_stat;
  int i, j, k, m;
  int step1, width_end1, height_end1;
  int step2, width_end2, height_end2;
  int z_size, z_step1, z_end1, z_step2;
  int step3, z_step3;
  int *plane, base, *stat_plane;
  int max, pos, in, out, tmp_end;
  int max_ct, max_tmp_ct, max_remained;
  int *centroids, *cen_tmp, x, y, z, weight, x_dist, y_dist, z_dist;
  float zf2;
  int weight_max_cutoff;

  int my_image_height = frame->height;
  int my_image_width = frame->width;
  int my_plane_number = frame->plane_number;

  density_stack = tools->density_stack;
  max_stat = tools->max_stat;
  centroids = tools->centroids;
  cen_tmp = tools->cen_tmp;

  zf2 = parameters->z_factor * parameters->z_factor;

  /* boundary effects */
  step1 = parameters->nuc_size1 / 2;
  width_end1 = my_image_width - step1;
  height_end1 = my_image_height - step1;

  step2 = parameters->nuc_size2 / 2;
  tmp_end = step1 + step2;
  width_end2 = my_image_width - tmp_end;
  height_end2 = my_image_height - tmp_end;

  z_step1 = parameters->z_size1 / 2;
  z_end1 = my_plane_number - z_step1;

  z_size = (int) (1.0 * parameters->nuc_size2 / parameters->z_factor);
  z_step2 = z_size / 2;
  if (z_step2 < 1)
    z_step2 = 1;

  step3 = parameters->nuc_size3;        /* not divided by 2 as step1 and 2; different nature */
  z_step3 = (int) (1.0 * parameters->nuc_size3 / parameters->z_factor);
  if (z_step3 < 1)
    z_step3 = 1;

  /* 2D max */
  for (i = z_step1; i < z_end1; i++)
  {
    weight_max_cutoff =
      (int) (parameters->max_weight_cutoff * tools->noise[t][i] *
             parameters->cube_size / (parameters->neighborhood_size *
                                      parameters->neighborhood_size));
    /*if (weight_max_cutoff < 5000) weight_max_cutoff = 5000; */
    plane = density_stack[i];
    stat_plane = max_stat[i];
    /* 1st dimension: along each row */
    for (j = step1; j < height_end1; j++)
    {
      base = my_image_width * j;
      /* first window in a row (only half a window, left half is all 0) */
      pos = step1;
      max = plane[base + pos];
      for (in = step1 + 1; in <= tmp_end; in++)
      {
        update_max (plane[base + in], &max, &pos, in);
      }
      k = step1;
      record_max (backup_stack, stat_plane, base, k, plane, max,
                  weight_max_cutoff, report_max, i, frame);

      /* rest of the row: sliding window
       * divided into three fragments to take into account of boundary effect 
       * one can easily unify code for the three fragments 
       * (by checking boundaries, see the first step in 3rd dimension),
       * but that wastes a lot of CPU actions, hence this messier implementation 
       * first fragment: nothing going out of the sliding window due to boundary effect, 
       * or pixels going out are '0's */

      for (k = step1 + 1, in = k + step2; k <= tmp_end; k++, in++)
      {
        update_max (plane[base + in], &max, &pos, in);
        record_max (backup_stack, stat_plane, base, k, plane, max,
                    weight_max_cutoff, report_max, i, frame);
      }
      /* second fragment: one side coming in, the other going out */
      for (k = tmp_end + 1, in = k + step2, out = step1; k < width_end2;
           k++, in++, out++)
      {
        /* theoretically, there's no need to update 'in' here or 'out' for later steps as they are synchronized with k.  This is just to remind me what values they are (for debugging) */
        int updated = update_max (plane[base + in], &max, &pos, in);
        if ((updated == 0) && (pos == out))
        {
          /* max is leaving; re-establishing window */
          reestablish_max (&pos, out, &max, plane, base, in);
        }
        record_max (backup_stack, stat_plane, base, k, plane, max,
                    weight_max_cutoff, report_max, i, frame);
      }
      /* third fragment: nothing coming in */
      for (k = width_end2, in = width_end1 - 1, out = k - 1 - step2;
           k < width_end1; k++, out++)
      {
        if (pos == out)
        {
          /* max is leaving; re-establishing window */
          reestablish_max (&pos, out, &max, plane, base, in);
        }
        record_max (backup_stack, stat_plane, base, k, plane, max,
                    weight_max_cutoff, report_max, i, frame);
      }
    }
    /*second dimension */
    /* notice two differences from the first dimension */
    /* 1. out is no longer the one going out of the sliding window, but the beginning of the scanning window */
    /* 2. we only eliminate non-max positions, but no longer update the local maxima for 2nd dimension, which leads to the two step elimination in the 3rd dimension */
    /* the code is structurally related to the first step in scanning in the 3rd dimension (right below).   See there for more explanations. */
    for (j = step1, in = tmp_end, out = step1; j <= tmp_end; j++, in++)
    {
      /* MAX_SCAN_Y */
      max_scan_y (backup_stack, i, my_image_width, j, step1, width_end1,
                  stat_plane, centroids, frame, in, out, report_max);

    }
    for (j = tmp_end + 1, in = j + step2, out = step1 + 1; j < height_end2;
         j++, in++, out++)
    {
      /* MAX_SCAN_Y */
      max_scan_y (backup_stack, i, my_image_width, j, step1, width_end1,
                  stat_plane, centroids, frame, in, out, report_max);


    }
    for (j = height_end2, in = height_end1 - 1, out = j - step2;
         j < height_end1; j++, out++)
    {
      /* MAX_SCAN_Y */
      max_scan_y (backup_stack, i, my_image_width, j, step1, width_end1,
                  stat_plane, centroids, frame, in, out, report_max);


    }
  }
  /* 3D max */
  /* two steps for 3rd dimension: first is to check the neighbor-window (along z-axis) like the 2nd dimension */
  /* second is to compare candidates from the first step that are w/i the neighborhood cube */
  /* reason for the second step is that we did not update local_max for the 2nd dimension */
  /* first step: neighborhood window */
  max_tmp_ct = 0;
  for (i = z_step1; i < z_end1; i++)
  {
    stat_plane = max_stat[i];
    max_ct = 0;
    /* recording potential local maxima (still with stat of 1 in max_stat) to 'centroids' */
    /* 'centroids' (and 'cen_tmp', too) devotes 5 (integer) elements to record one centroid: status (binary), x, y, z coordinates and weight.  Notice that the y value is recorded as the address of the first pixel in the row. */
    /* the scan for the 2nd dimension uses a watered-down version where only x and weight are used. */
    /* be careful to traverse max_stat as the boundary elements were not initialized (padded) */
    for (j = step1; j < height_end1; j++)
    {
      base = my_image_width * j;
      for (k = step1; k < width_end1; k++)
      {
        if (stat_plane[(base + k) * 2 + 1] == 1)
        {
          weight = stat_plane[(base + k) * 2];
          centroids[5 * max_ct] = 1;
          centroids[5 * max_ct + 1] = k;
          centroids[5 * max_ct + 2] = base;
          centroids[5 * max_ct + 3] = i;
          centroids[5 * max_ct + 4] = weight;
          ++max_ct;
        }
      }
    }                           /* for (j=step1; j<height_end1; j++) */

    if (!max_ct)
      continue;
    max_remained = max_ct;

    if (report_max == 4)
    {
      for (j = 0; j < max_ct; j++)
      {
        MarkCentroid (backup_stack[i], frame, centroids[5 * j + 1],
                      centroids[5 * j + 2], MONO, 225);
      }                         /* for (j=0; j<max_ct; j++) */
    }                           /* if (report_max == 4) */

    /* compared to the implementation of the 2nd dimension: I'm using the tighter/cleaner code because the wasted actions do not add to much */
    out = z_step1 > i - z_step2 ? z_step1 : i - z_step2;
    in = z_end1 < i + z_step2 ? z_end1 : i + z_step2;
    /* using the traversal of 'centroids' (k) as inner loop.  Could be the outer loop (outside traversal of pixels (m).  This structure might reduce the number of swaps b/t cache and memory when accessing 'max_stat+m'.  Just a guess, not tested.  If wrong, this implementation is more expensive as it repeatedly dereferences elements in 'centroids'.  Worse for the 2nd dimension as there would be more elements in 'centroids' there (see MAX_SCAN_Y). */
    for (m = out; m <= in; m++)
    {
      if (m == i)
        continue;
      plane = max_stat[m];
      for (k = 0; k < max_ct; k++)
      {
        if (!centroids[5 * k])
          continue;
        if (centroids[5 * k + 4] <
            plane[(centroids[5 * k + 2] + centroids[5 * k + 1]) * 2])
        {
          /* not a local maxima */
          centroids[5 * k] = 0;
          --max_remained;
          if (!max_remained)
            break;
        }
      }
      if (!max_remained)
        break;
    }
    if (!max_remained)
      continue;

    /* copy potential local maxima to 'cent_tmp' for second step */
    for (k = 0; k < max_ct; k++)
    {
      if (centroids[5 * k])
      {
        if (report_max == 5)
        {
          MarkCentroid (backup_stack[centroids[5 * k + 3]], frame,
                        centroids[5 * k + 1], centroids[5 * k + 2],
                        MONO, 255);
        }
        memcpy (cen_tmp + 5 * max_tmp_ct, centroids + 5 * k,
                sizeof (int) * 5);
        ++max_tmp_ct;
        if (max_tmp_ct == parameters->cen_ct_limit)
        {
          fprintf (logfile, "expanding tools->centroids and tools->cen_tmp");
          parameters->cen_ct_limit = max_tmp_ct * 2;
          tools->centroids =
            (int *) malloc_exit (sizeof (int) * parameters->cen_ct_limit * 5);
          tools->cen_tmp =
            (int *) malloc_exit (sizeof (int) * parameters->cen_ct_limit * 5);
          memcpy (tools->centroids, centroids, sizeof (int) * max_tmp_ct * 5);
          memcpy (tools->cen_tmp, cen_tmp, sizeof (int) * max_tmp_ct * 5);
          free (centroids);
          free (cen_tmp);
          centroids = tools->centroids;
          cen_tmp = tools->cen_tmp;
        }                       /* if (max_tmp_ct == parameters->cen_ct_limit) */
      }                         /* if (*(centroids+5*k)) */
    }                           /* for (k=0; k<max_ct; k++) */
  }                             /* for (i=z_step1; i<z_end1; i++) */

  if (report_max == 6)
  {
    for (i = 0; i < max_tmp_ct; i++)
    {
      MarkCentroid (backup_stack[cen_tmp[5 * i + 3]], frame,
                    cen_tmp[5 * i + 1], cen_tmp[5 * i + 2], MONO, 255);
    }
  }

  /* second step: weed out candidates in 'cen_tmp' that are too close to each other (w/i neighborhood cube) */
  for (i = 0; i < max_tmp_ct; i++)
  {
    if (!cen_tmp[5 * i])
      continue;
    x = cen_tmp[5 * i + 1];
    y = cen_tmp[5 * i + 2];
    z = cen_tmp[5 * i + 3];
    weight = cen_tmp[5 * i + 4];
    for (j = i + 1; j < max_tmp_ct; j++)
    {
      if (!cen_tmp[5 * j])
        continue;
      z_dist = cen_tmp[5 * j + 3] - z;
      /*if (z_dist > z_step3) break; */
      x_dist = abs (cen_tmp[5 * j + 1] - x);
      y_dist = abs (cen_tmp[5 * j + 2] - y) / my_image_width;
      /*if (x_dist>step3 || y_dist>step3) continue; */
      if (sqrt (x_dist * x_dist + y_dist * y_dist + z_dist * z_dist * zf2)
          > step3)
        continue;
      if (cen_tmp[5 * j + 4] > weight)
        cen_tmp[5 * i] = 0;
      else
        cen_tmp[5 * j] = 0;
    }
  }

  {
    /* copy identified local maxima into 'centroids' */
    int local_max_ct = 0;

    for (i = 0; i < max_tmp_ct; i++)
    {
      if (cen_tmp[5 * i])
      {
        memcpy (centroids + 5 * local_max_ct, cen_tmp + 5 * i,
                sizeof (int) * 5);
        ++local_max_ct;
      }
    }
    tools->cen_ct = local_max_ct;
  }

}

static void
NucInit (const TOOL_t * const tools, const PARAMETER_t * const parameters,
         NUCLEUS_t * const nuc, int t, int index)
{
  int i;
  const int *const centroids = tools->centroids;

  nuc->x = centroids[5 * index + 1];
  nuc->y = centroids[5 * index + 2];
  nuc->z = centroids[5 * index + 3];
  nuc->weight = centroids[5 * index + 4];
  nuc->volume = parameters->cube_size;

  nuc->x0 = centroids[5 * index + 1];
  nuc->y0 = centroids[5 * index + 2];
  nuc->z0 = centroids[5 * index + 3];
  nuc->wei0 = centroids[5 * index + 4];

  nuc->status = 1;
  *(nuc->identity) = '\0';
  nuc->birthday = t;
  nuc->generation = 0;
  nuc->end_time = t;
  nuc->start_state = 0;
  nuc->end_state = 0;
  nuc->start_scorem = -10000;
  nuc->end_scorem = -10000;
  nuc->max_size_time = t;
  nuc->max_weight_time = t;
  nuc->ghost = 0;
  nuc->ghost_proof = 0;
  nuc->condensed = 0;
  nuc->id_tag = '\0';
  nuc->size = parameters->nuc_size;
  nuc->sponginess1 = -1.0;
  nuc->sponginess2 = -1.0;
  for (i = 0; i < 8; i++)
    nuc->piece_weight[i] = 0;
  for (i = 0; i < 8; i++)
    nuc->piece_vol[i] = 0;
  for (i = 0; i < 8; i++)
    nuc->piece_brights[i] = 0;
  nuc->p_ambiguity = 0;
  nuc->s_ambiguity = 0;
  nuc->pskip = 0;
  nuc->sskip = 0;
  nuc->p_partners = NULL;
  nuc->s_partners = NULL;
  nuc->p_ct = 0;
  nuc->s_ct = 0;
  nuc->p_ct2 = 0;
  nuc->s_ct2 = 0;
  nuc->predecessor = -1;
  nuc->successor1 = -1;
  nuc->successor2 = -1;
  nuc->pdist = -1.0;
  nuc->sdist1 = -1.0;
  nuc->sdist2 = -1.0;
  nuc->pflag = 0;
  nuc->sflag1 = 0;
  nuc->sflag2 = 0;
}


static int
NucReCenter (const TOOL_t * const tools,
             const uint8_t * const *const image_stack, FRAME_t * frame,
             NUCLEUS_t * const nuc)
{
  const int plane_number = frame->plane_number;
  const int my_image_width = frame->width;
  const int my_image_height = frame->height;

  int j;
  int dist;
  int x, y, z;
  int x_sum, y_sum, z_sum;

  int base, x_start, x_end;
  int weight;
  int weight_y;
  int weight_z;
  int signal;
  int average;

  /* equivalent to parameters->nuc_size here */
  const int nuc_size = nuc->size;

  int *const *const x_range = tools->spheres[nuc_size];
  const int *const radii = tools->s_radii[nuc_size];
  const int slices = tools->s_layers[nuc_size];

  int base_line = 0;
  x = nuc->x;
  y = nuc->y;
  z = (int) nuc->z;
  average = nuc->weight / nuc->volume;

  weight = 0;
  x_sum = 0;
  y_sum = 0;
  z_sum = 0;

  /* base_line is set at least once */
  assert (slices > 0);

  for (j = 0 - slices; j <= slices; j++)
  {
    int k;
    if (z + j < 0 || z + j >= plane_number)
      continue;
    weight_z = 0;
    const int r = radii[slices + j];
    const uint8_t *const image = image_stack[z + j];
    base_line = y / my_image_width;
    for (k = 0 - r; k <= r; k++)
    {
      int m;
      if (base_line + k < 0)
        continue;
      if (base_line + k >= my_image_height)
        break;
      weight_y = 0;
      base = y + k * my_image_width;
      dist = x_range[slices + j][r + k];
      x_start = x - dist > 0 ? x - dist : 0;
      x_end = x + dist < my_image_width ? x + dist : my_image_width;
      for (m = x_start; m <= x_end; m++)
      {
        signal = (int) image[base + m];
        if (signal < average)
          continue;
        x_sum += (m - x) * signal;
        weight_y += signal;
      }
      y_sum += k * weight_y;
      weight_z += weight_y;
    }
    z_sum += j * weight_z;
    weight += weight_z;
  }

  if (weight == 0)
  {
    nuc->status = 0;
    return 0;
  }

  nuc->x = ((int) (1.0 * x_sum / weight + 0.5)) + x;
  nuc->y =
    (((int) (1.0 * y_sum / weight + 0.5)) + base_line) * my_image_width;
  nuc->z = 1.0 * z_sum / weight + z;

  return 1;
}



static int
NucWeight (const TOOL_t * const tools,
           const uint8_t * const *const image_stack, FRAME_t * frame,
           NUCLEUS_t * nuc)
{
  int plane_number = frame->plane_number;
  int my_image_width = frame->width;
  int my_image_height = frame->height;

  int nuc_size;
  int slices, r, *radii, **x_range, dist;
  int j, k, m;
  int x, y, z, base_line, base, x_start, x_end;
  const uint8_t *image;
  int volume, weight;

  x = nuc->x;
  y = nuc->y;
  z = (int) (nuc->z + 0.5);
  nuc_size = nuc->size;

  x_range = tools->spheres[nuc_size];
  radii = tools->s_radii[nuc_size];
  slices = tools->s_layers[nuc_size];

  weight = 0;
  volume = 0;
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
      x_start = x - dist > 1 ? x - dist : 1;
      x_end = x + dist < my_image_width - 1 ? x + dist : my_image_width - 1;
      for (m = x_start; m <= x_end; m++)
      {
        weight += (int) image[base + m];
        volume++;
      }
    }
  }

  nuc->weight = weight;
  nuc->volume = volume;

  if (!volume)
  {
    nuc->status = 0;
    return 0;
  }
  return 1;
}



static void
PizzaSlices (const TOOL_t * const tools,
             const uint8_t * const *const image_stack, FRAME_t * frame,
             NUCLEUS_t * nuc)
{
  int plane_number = frame->plane_number;
  int my_image_width = frame->width;
  int my_image_height = frame->height;
  int nuc_size;
  int slices, r, *radii, **x_range, dist;
  int i, j, k, m;
  int x, y, z, base_line, base, x_start, x_end;
  const uint8_t *image;
  int xdiff, average, signal;


  float max, min, piece;

  x = nuc->x;
  y = nuc->y;
  z = (int) (nuc->z + 0.5);
  nuc_size = nuc->size;

  x_range = tools->spheres[nuc_size];
  radii = tools->s_radii[nuc_size];
  slices = tools->s_layers[nuc_size];
  average = nuc->weight / nuc->volume;

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
      x_start = x - dist > 1 ? x - dist : 1;
      x_end = x + dist < my_image_width - 1 ? x + dist : my_image_width - 1;
      for (m = x_start; m <= x_end; m++)
      {
        signal = (int) image[base + m];
        xdiff = m - x;
        if (k >= 0)
        {
          if (xdiff >= 0)
          {
            if (k < xdiff)
            {
              nuc->piece_weight[0] += signal;
              nuc->piece_vol[0]++;
              if (signal >= average)
                nuc->piece_brights[0]++;
            }
            else
            {
              nuc->piece_weight[1] += signal;
              nuc->piece_vol[1]++;
              if (signal >= average)
                nuc->piece_brights[1]++;
            }
          }
          else
          {
            if (k > 0 - xdiff)
            {
              nuc->piece_weight[2] += signal;
              nuc->piece_vol[2]++;
              if (signal >= average)
                nuc->piece_brights[2]++;
            }
            else
            {
              nuc->piece_weight[3] += (int) image[base + m];
              nuc->piece_vol[3]++;
              if (signal >= average)
                nuc->piece_brights[3]++;
            }
          }
        }
        else
        {
          if (xdiff < 0)
          {
            if (0 - k < 0 - xdiff)
            {
              nuc->piece_weight[4] += signal;
              nuc->piece_vol[4]++;
              if (signal >= average)
                nuc->piece_brights[4]++;
            }
            else
            {
              nuc->piece_weight[5] += signal;
              nuc->piece_vol[5]++;
              if (signal >= average)
                nuc->piece_brights[5]++;
            }
          }
          else
          {
            if (0 - k > xdiff)
            {
              nuc->piece_weight[6] += signal;
              nuc->piece_vol[6]++;
              if (signal >= average)
                nuc->piece_brights[6]++;
            }
            else
            {
              nuc->piece_weight[7] += signal;
              nuc->piece_vol[7]++;
              if (signal >= average)
                nuc->piece_brights[7]++;
            }
          }
        }
      }
    }
  }

  piece =
    1.0 * (nuc->piece_weight[0] + nuc->piece_weight[4]) / (nuc->piece_vol[0] +
                                                           nuc->piece_vol[4]);
  max = piece;
  min = piece;
  for (i = 1; i < 4; i++)
  {
    piece =
      1.0 * (nuc->piece_weight[i] +
             nuc->piece_weight[i + 4]) / (nuc->piece_vol[i] +
                                          nuc->piece_vol[i + 4]);
    if (piece > max)
      max = piece;
    else if (piece < min)
      min = piece;
  }
  nuc->piece_ratio = min / max;

  piece =
    1.0 * (nuc->piece_brights[0] +
           nuc->piece_brights[4]) / (nuc->piece_vol[0] + nuc->piece_vol[4]);
  max = piece;
  min = piece;
  for (i = 1; i < 4; i++)
  {
    piece =
      1.0 * (nuc->piece_brights[i] +
             nuc->piece_brights[i + 4]) / (nuc->piece_vol[i] +
                                           nuc->piece_vol[i + 4]);
    if (piece > max)
      max = piece;
    else if (piece < min)
      min = piece;
  }
  nuc->piece_ratio2 = min / max;
}



static void
Sponginess (const TOOL_t * const tools,
            const uint8_t * const *const image_stack, FRAME_t * frame,
            NUCLEUS_t * nuc)
{
  int plane_number = frame->plane_number;
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
      x_start = x - dist > 1 ? x - dist : 1;
      x_end = x + dist < my_image_width - 1 ? x + dist : my_image_width - 1;
      for (m = x_start; m <= x_end; m++)
      {
        if ((int) image[base + m] >= average)
        {
          b_ct++;
          if (m && (int) image[base + m - 1] < average)
          {
            db_ct++;
          }
          if (m < my_image_width - 1 && (int) image[base + m + 1] < average)
          {
            db_ct++;
          }
        }
        else
        {
          d_ct++;
          if (m && (int) image[base + m - 1] >= average)
          {
            bd_ct++;
          }
          if (m < my_image_width - 1 && (int) image[base + m + 1] >= average)
          {
            bd_ct++;
          }
        }
      }
    }
  }

  if (b_ct)
    nuc->sponginess1 = 0.5 * db_ct / b_ct;
  else
    nuc->sponginess1 = 0.5;

  if (d_ct)
    nuc->sponginess2 = 0.5 * bd_ct / d_ct;
  else
    nuc->sponginess2 = 0.5;

  nuc->b_ct = b_ct;
}


static void
MeasureMorphology (const TOOL_t * const tools,
                   const uint8_t * const *const image_stack, FRAME_t * frame,
                   NUCLEUS_t * nuc)
{
  float sponge, slice;

  Sponginess (tools, image_stack, frame, nuc);
  PizzaSlices (tools, image_stack, frame, nuc);
  if ((nuc->sponginess1 <= 0.25 || nuc->sponginess2 <= 0.22)
      && (nuc->piece_ratio <= 0.69 || nuc->piece_ratio2 <= 0.52))
  {
    sponge =
      nuc->sponginess1 - 0.25 <
      nuc->sponginess2 - 0.22 ? nuc->sponginess1 - 0.25 : nuc->sponginess2 -
      0.22;
    slice =
      nuc->piece_ratio - 0.69 <
      nuc->piece_ratio2 - 0.52 ? nuc->piece_ratio -
      0.69 : nuc->piece_ratio2 - 0.52;
    if (sponge + slice < -0.05)
      nuc->condensed = 3;
  }
}



static float
Crust (const TOOL_t * const tools, const uint8_t * const *const image_stack,
       FRAME_t * frame, NUCLEUS_t * nuc)
{
  int slices, r, *radii, **x_range, dist;
  int j, k;
  int x, y, z, base_line, base, pos;
  const uint8_t *image;
  int volume, weight;

  int plane_number = frame->plane_number;
  int my_image_width = frame->width;
  int my_image_height = frame->height;

  x = nuc->x;
  y = nuc->y;
  z = (int) (nuc->z + 0.5);

  x_range = tools->spheres[nuc->size];
  radii = tools->s_radii[nuc->size];
  slices = tools->s_layers[nuc->size];

  weight = 0;
  volume = 0;
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
      pos = x - dist;
      if (pos > 0 && pos < my_image_width)
      {
        weight += (int) image[base + pos];
        volume++;
      }
      pos++;
      if (pos > 0 && pos < my_image_width && pos <= x + dist)
      {
        weight += (int) image[base + pos];
        volume++;
      }
      pos = x + dist;
      if (pos > 0 && pos < my_image_width && pos > x - dist + 1)
      {
        weight += (int) image[base + pos];
        volume++;
      }
      pos--;
      if (pos > 0 && pos < my_image_width && pos > x - dist + 1)
      {
        weight += (int) image[base + pos];
        volume++;
      }
    }
  }

  return 1.0 * weight / volume;
}



static int
NucReSize (const PARAMETER_t * const parameters,
           const MOVIE_t * const movie,
           const TOOL_t * const tools,
           FILE * const logfile,
           const uint8_t * const *const image_stack, FRAME_t * frame,
           NUCLEUS_t * const nuc, int index, const int t,
           const int last_change)
{

  int z = (int) (nuc->z + 0.5);

  float noise =
    parameters->nuc_density_cutoff * tools->noise[t][z]
    / (parameters->neighborhood_size * parameters->neighborhood_size);

  float nuc_ave = 1.0 * nuc->weight / nuc->volume;
  float crust_ave = Crust (tools, image_stack, frame, nuc);

  int change;
  int ori_nuc_size = 0;

  if (last_change == 0)
  {
    ori_nuc_size = nuc->size;
  }

  if (crust_ave <
      parameters->shrink_elastisity * nuc_ave /*|| crust_ave <= noise */ )
  {
    change = -2;
  }
  else if (crust_ave > parameters->expand_elastisity * nuc_ave
           && crust_ave > noise)
  {
    change = 2;
  }
  else
  {
    change = 0;
  }

  if (nuc->size + change > parameters->max_nuc_size
      || nuc->size + change < parameters->min_nuc_size)
  {
    change = 0;
  }

  if (change != 0)
  {
    nuc->size += change;

    if (NucReCenter (tools, image_stack, frame, nuc) == 0)
    {
      return 0;
    }
    if (NucWeight (tools, image_stack, frame, nuc) == 0)
    {
      return 0;
    }

    if (nuc->size < (int) (1.1 * parameters->nuc_size)
        && nuc->size > parameters->min_nuc_size && last_change + change)
    {
      NucReSize (parameters, movie, tools, logfile, image_stack, frame, nuc,
                 index, t, change);
    }
  }

  if (last_change == 0)
  {
    if (nuc->size > ori_nuc_size * 2)
    {
      fprintf (logfile, "Nuclear size over twice the "
               "average/expected size: tp %d, nuc %d",
               movie->time_start + parameters->t + 1, index + 1);
    }
    else if (nuc->size < ori_nuc_size / 3)
    {
      fprintf (logfile,
               "Nuclear size less than one third "
               "the average/expected size: tp %d, nuc %d",
               movie->time_start + parameters->t + 1, index + 1);
    }
  }

  return 1;
}



/*
static float
NucDistance_square (NUCLEUS_t * nuc1, NUCLEUS_t * nuc2, FRAME_t * frame)
{
  int xdist, ydist;
  float zdist;

  xdist = nuc1->x - nuc2->x;
  ydist = (nuc1->y - nuc2->y) / frame->width;
  zdist = (nuc1->z - nuc2->z) * Parameters.z_factor;

  return zdist * zdist + xdist * xdist + ydist * ydist;
}
*/

float
NucDistance (const PARAMETER_t * const parameters,
             const NUCLEUS_t * const nuc1, const NUCLEUS_t * const nuc2,
             const FRAME_t * const frame)
{
  int xdist, ydist;
  float zdist;

  xdist = nuc1->x - nuc2->x;
  ydist = (nuc1->y - nuc2->y) / frame->width;
  zdist = (nuc1->z - nuc2->z) * parameters->z_factor;

  return sqrt (zdist * zdist + xdist * xdist + ydist * ydist);
}



static void
DensityFilter (const PARAMETER_t * const parameters,
               const TOOL_t * const tools, NUCLEUS_t * nuclei, int nuc_ct,
               int t)
{
  int j, z;
  float cutoff;

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    z = (int) (nuclei[j].z + 0.5);
    cutoff =
      parameters->nuc_density_cutoff * tools->noise[t][z]
      / (parameters->neighborhood_size * parameters->neighborhood_size);
    if (1.0 * nuclei[j].weight / nuclei[j].volume < cutoff)
    {
      nuclei[j].status = 0;
    }
  }
}



static void
SelfDistanceFilter (const PARAMETER_t * const parameters, NUCLEUS_t * nuclei,
                    int nuc_ct, FRAME_t * frame)
{
  int i, j;
  int cutoff;
  float distance;

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    for (j = i + 1; j < nuc_ct; j++)
    {
      if (!nuclei[j].status)
        continue;
      distance = NucDistance (parameters, nuclei + j, nuclei + i, frame);
      cutoff =
        parameters->nuc_size_factor4 * (nuclei[j].size + nuclei[i].size) / 2;
      if (distance < cutoff)
      {
        if (nuclei[i].weight < nuclei[j].weight)
          nuclei[i].status = 0;
        else
          nuclei[j].status = 0;
      }
    }
  }
}



static void
RefineNuclei (const TOOL_t * const tools,
              const PARAMETER_t * const parameters,
              const MOVIE_t * const movie, FILE * logfile,
              const uint8_t * const *image_stack, FRAME_t * frame,
              NUCLEUS_t * nuclei, int t)
{
  int cen_ct, i;

  cen_ct = tools->cen_ct;
  for (i = 0; i < cen_ct; i++)
  {
    NucInit (tools, parameters, nuclei + i, t, i);
    if (!NucReCenter (tools, image_stack, frame, nuclei + i))
      continue;
    
	if (!NucWeight (tools, image_stack, frame, nuclei + i))
      continue;
    if (!NucReSize
        (parameters, movie, tools, logfile, image_stack, frame, nuclei + i,
         i, t, 0))
      continue;
    nuclei[i].max_weight = nuclei[i].weight;
    nuclei[i].max_size = nuclei[i].size;
    MeasureMorphology (tools, image_stack, frame, nuclei + i);
  }

  DensityFilter (parameters, tools, nuclei, cen_ct, t);
  SelfDistanceFilter (parameters, nuclei, cen_ct, frame);
}



void
LoadDiameters(const TOOL_t * const tools,const char *const filename, NUCLEUS_t * nuclei)
{
	//printf(filename);
  int total=0;
  int *centroids=tools->centroids;
  static const int line_size = 500;
  static const int head_size = 50;

  FILE *parameter_file = NULL;
  char line[line_size], head[head_size];
  int d;

  assert (filename != NULL);

  parameter_file = fopen (filename, "r");

  if (parameter_file == NULL)
  {
    const char *const str = "Could not open matlab diameter file";

    fprintf (stderr, "ERROR: %s %s\n", str, filename);
    exit (-1);
  }
 while (fgets (line, line_size, parameter_file))
  {
    sscanf (line, "%d ",&d);
	nuclei[total].size=d;
    total++;

	}
 fclose (parameter_file);

}




static void
RefineLoadedNuclei (const TOOL_t * const tools,
              const PARAMETER_t * const parameters,
              const MOVIE_t * const movie, FILE * logfile,
              const uint8_t * const *image_stack, FRAME_t * frame,
              NUCLEUS_t * nuclei, int t,const char *const filename)
{
  int cen_ct, i;

  cen_ct = tools->cen_ct;
  for (i = 0; i < cen_ct; i++)
  {
	 NucInit (tools, parameters, nuclei + i, t, i);
  }

  LoadDiameters(tools,filename,nuclei);
  
  for (i = 0; i < cen_ct; i++)
  {

    if (!NucWeight (tools, image_stack, frame, nuclei + i))
      continue;

    nuclei[i].max_weight = nuclei[i].weight;
    nuclei[i].max_size = nuclei[i].size;
    MeasureMorphology (tools, image_stack, frame, nuclei + i);
  }

 // DensityFilter (parameters, tools, nuclei, cen_ct, t);
 // SelfDistanceFilter (parameters, nuclei, cen_ct, frame);
}



void
DefineNuclei (const MOVIE_t * const movie,
              PARAMETER_t * const parameters,
              TOOL_t * const tools,
              uint8_t ** backup_stack,
              FILE * logfile,
              const uint8_t * const *image_stack, FRAME_t * frame,
              NUCLEUS_t ** nuc_rec, int *nuc_ct_p, int t)
{
  int cen_ct;
  NUCLEUS_t *nuclei, *nuclei2;
  int i, nuc_left;

  /* centroids */
  LocalWeightScan_3D (tools, image_stack, frame, parameters->nuc_size1,
                      parameters->z_size1);
  LocalMaxima_3D (tools, parameters, backup_stack, logfile, image_stack,
                  frame, 0, t);

  cen_ct = tools->cen_ct;
  if (!cen_ct)
  {
    *nuc_rec = NULL;
    *nuc_ct_p = 0;
    fprintf (logfile, ", 0 cells");
    return;
  }

  nuclei = (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * cen_ct);
  RefineNuclei (tools, parameters, movie, logfile, image_stack, frame,
                nuclei, t);

  nuc_left = 0;
  for (i = 0; i < cen_ct; i++)
  {
    if (nuclei[i].status)
      nuc_left++;
  }

  *nuc_ct_p = nuc_left;

  if (!nuc_left)
  {
    *nuc_rec = NULL;
    *nuc_ct_p = 0;
    free (nuclei);
  }
  else if (nuc_left == cen_ct)
  {
    *nuc_rec = nuclei;
  }
  else
  {
    nuclei2 = (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * nuc_left);
    nuc_left = 0;
    for (i = 0; i < cen_ct; i++)
    {
      if (nuclei[i].status)
      {
        nuclei2[nuc_left] = nuclei[i];
        nuc_left++;
      }
    }
    *nuc_rec = nuclei2;
    free (nuclei);
  }

  fprintf (logfile, ", %d cells", nuc_left);
}

static void
DistanceFilter (const PARAMETER_t * const parameters,
                NUCLEUS_t * nuclei_recov, int nuc_ct_recov,
                NUCLEUS_t * nuclei, int nuc_ct, FRAME_t * frame)
{
  int i, j;
  int cutoff;
  float distance;

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    for (j = 0; j < nuc_ct_recov; j++)
    {
      if (!nuclei_recov[j].status)
        continue;
      distance =
        NucDistance (parameters, nuclei_recov + j, nuclei + i, frame);
      cutoff =
        parameters->nuc_size_factor4 * (nuclei_recov[j].size +
                                        nuclei[i].size) / 2;
      if (distance < cutoff)
      {
        nuclei_recov[j].status = 0;
      }
    }
  }
}



void
RecoverNuclei (const MOVIE_t * const movie,
               PARAMETER_t * const parameters,
               TOOL_t * const tools,
               uint8_t ** backup_stack,
               FILE * logfile,
               const uint8_t * const *image_stack, FRAME_t * frame, int t)
{
  int nuc_ct, nuc_ct_recov, nuc_left;
  NUCLEUS_t *nuclei, *nuclei_recov;
  int j;

  /*while (parameters->nuc_size > parameters->min_nuc_size) { */
  nuclei = tools->nuclei_record[t];
  nuc_ct = tools->nuc_ct_rec[t];

  parameters->nuc_size = parameters->nuc_size * 2 / 3;
  if (parameters->nuc_size < parameters->min_nuc_size)
    parameters->nuc_size = parameters->min_nuc_size;
  CalculateSizeParameters (parameters);

  LocalWeightScan_3D (tools, image_stack, frame, parameters->nuc_size1,
                      parameters->z_size1);
  LocalMaxima_3D (tools, parameters, backup_stack, logfile, image_stack,
                  frame, 0, t);

  if (tools->cen_ct)
  {
    nuc_ct_recov = tools->cen_ct;
    nuclei_recov =
      (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * nuc_ct_recov);
    RefineNuclei (tools, parameters, movie, logfile, image_stack, frame,
                  nuclei_recov, t);
    DistanceFilter (parameters, nuclei_recov, nuc_ct_recov, nuclei, nuc_ct,
                    frame);

    nuc_left = 0;
    for (j = 0; j < nuc_ct_recov; j++)
    {
      if (nuclei_recov[j].status)
        nuc_left++;
    }

    if (nuc_left)
    {
      tools->nuclei_record[t] =
        (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * (nuc_ct + nuc_left));
      memcpy (tools->nuclei_record[t], nuclei, sizeof (NUCLEUS_t) * nuc_ct);
      free (nuclei);
      nuclei = tools->nuclei_record[t];
      tools->nuc_ct_rec[t] = nuc_ct + nuc_left;
      for (j = 0; j < nuc_ct_recov; j++)
      {
        if (!nuclei_recov[j].status)
          continue;
        memcpy (nuclei + nuc_ct, nuclei_recov + j, sizeof (NUCLEUS_t));
        nuc_ct++;
      }
    }
    free (nuclei_recov);
  }
  fprintf (logfile, ", %d cells", nuc_ct);
  /*} */

  parameters->nuc_size = parameters->sizes[parameters->t];
  CalculateSizeParameters (parameters);
}


void
LoadNuclei (const MOVIE_t * const movie,
              PARAMETER_t * const parameters,
              TOOL_t * const tools,
              uint8_t ** backup_stack,
              FILE * logfile,
              const uint8_t * const *image_stack, FRAME_t * frame,
              NUCLEUS_t ** nuc_rec, int *nuc_ct_p, int t,const char *const filename,const char *const filename2)
{
  int cen_ct;
  NUCLEUS_t *nuclei, *nuclei2;
  int i, nuc_left;
  int my_image_width=frame->width;
  /* centroids */
  //LocalWeightScan_3D (tools, image_stack, frame, parameters->nuc_size1,
  //                    parameters->z_size1);
 // LocalMaxima_3D (tools, parameters, backup_stack, logfile, image_stack,
  //                frame, 0, t);

	// load from file filename and initialize positions

  //initialize centroids to dummy values
  
 LoadCentroids (tools,filename,my_image_width);


  cen_ct = tools->cen_ct;
  if (!cen_ct)
  {
    *nuc_rec = NULL;
    *nuc_ct_p = 0;
    fprintf (logfile, ", 0 cells");
    return;
  }

  nuclei = (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * cen_ct);


  RefineLoadedNuclei (tools, parameters, movie, logfile, image_stack, frame,nuclei, t, filename2);





  nuc_left = 0;
  for (i = 0; i < cen_ct; i++)
  {
    if (nuclei[i].status)
      nuc_left++;
  }

  *nuc_ct_p = nuc_left;

  if (!nuc_left)
  {
    *nuc_rec = NULL;
    *nuc_ct_p = 0;
    free (nuclei);
  }
  else if (nuc_left == cen_ct)
  {
    *nuc_rec = nuclei;
  }
  else
  {
    nuclei2 = (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * nuc_left);
    nuc_left = 0;
    for (i = 0; i < cen_ct; i++)
    {
      if (nuclei[i].status)
      {
        nuclei2[nuc_left] = nuclei[i];
        nuc_left++;
      }
    }
    *nuc_rec = nuclei2;
    free (nuclei);
  }

  fprintf (logfile, ", %d cells", nuc_left);

  }

void
LoadCentroids (TOOL_t * const tools,const char *const filename,int my_image_width)
{
	//printf(filename);
  int total=0;
  int *centroids=tools->centroids;
  static const int line_size = 500;
  static const int head_size = 50;

  FILE *parameter_file = NULL;
  char line[line_size], head[head_size];
  int x,y,z;

  assert (filename != NULL);

  parameter_file = fopen (filename, "r");

  if (parameter_file == NULL)
  {
    const char *const str = "Could not open matlab nucleus file";

    fprintf (stderr, "ERROR: %s %s\n", str, filename);
    exit (-1);
  }
 while (fgets (line, line_size, parameter_file))
  {

    sscanf (line, "%d %d %d",&x, &y,&z);
	//printf("%d %d %d\n",x, y,z);
	centroids[5 * total] = 1;
    centroids[5 * total + 1] = x;
    centroids[5 * total + 2] = y*my_image_width;
    centroids[5 * total + 3] = z-1;
    centroids[5 * total + 4] =1;//dummy value for intensity
	total++;

	}

 fclose (parameter_file);
tools->cen_ct=total;

}
