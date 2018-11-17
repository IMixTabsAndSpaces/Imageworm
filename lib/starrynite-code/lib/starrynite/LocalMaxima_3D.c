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

#include "LocalMaxima_3D.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "logfile.h"
#include "struct_frame.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "MarkCentroid.h"
#include "max_scan_y.h"
#include "reestablish_max.h"
#include "update_max.h"
#include "record_max.h"
#include "xalloc.h"

static float
get_weight_max_cutoff_factor(const parameter_t * const parameters)
{
  const float neighborhood_size_squared = parameters->neighborhood_size
      * parameters->neighborhood_size;

  return parameters->max_weight_cutoff * parameters->cube_size
      / neighborhood_size_squared;
}

/* TODO: Split this into multiple procedures */
static void
loop11(const int i, int * const centroids, uint8_t * const *backup_stack,
    int * const plane, const frame_t * const frame, const int height_end1,
    const int height_end2, int * const stat_plane, const int report_max,
    const int step1, const int step2, const int t, const int sum_of_step_sizes,
    const int weight_max_cutoff, const int width_end1, const int width_end2)
{
  for (int j = step1; j < height_end1; j++)
    {
      const int base = frame->width * j;

      int max_offset = step1;
      int max_value = plane[base + step1];

      for (int offset = step1 + 1; offset <= sum_of_step_sizes; offset++)
        {
          const int value = plane[base + offset];
          if (value > max_value)
            {
              max_value = value;
              max_offset = offset;
            }
        }

      record_max(backup_stack, stat_plane, base, step1, plane, max_value,
          weight_max_cutoff, report_max, i, frame);

      for (int k = step1 + 1, offset = k + step2; k <= sum_of_step_sizes; k++, offset++)
        {
          update_max(plane[base + offset], &max_value, &max_offset, offset);
          record_max(backup_stack, stat_plane, base, k, plane, max_value,
              weight_max_cutoff, report_max, i, frame);
        }

      for (int k = sum_of_step_sizes + 1, offset = k + step2, out = step1; k < width_end2; k++, offset++, out++)
        {
          int updated = update_max(plane[base + offset], &max_value,
              &max_offset, offset);
          if ((updated == 0) && (max_offset == out))
            {
              reestablish_max(&max_offset, out, &max_value, plane, base, offset);
            }
          record_max(backup_stack, stat_plane, base, k, plane, max_value,
              weight_max_cutoff, report_max, i, frame);
        }

      for (int k = width_end2, offset = width_end1 - 1, out = k - 1 - step2; k
          < width_end1; k++, out++)
        {
          if (max_offset == out)
            {
              reestablish_max(&max_offset, out, &max_value, plane, base, offset);
            }
          record_max(backup_stack, stat_plane, base, k, plane, max_value,
              weight_max_cutoff, report_max, i, frame);
        }

    }
}

static void
loop12(const int i, int * const centroids, uint8_t * const *backup_stack,
    int * const plane, const frame_t * const frame, const int height_end1,
    const int height_end2, int * const stat_plane, const int report_max,
    const int step1, const int step2, const int t, const int sum_of_step_sizes,
    const int weight_max_cutoff, const int width_end1, const int width_end2)
{
  for (int l = 0; l <= sum_of_step_sizes - step1; l++)
    {
      const int j = step1 + l;
      const int in = sum_of_step_sizes + l;
      const int out = step1;

      max_scan_y(backup_stack, i, frame->width, j, step1, width_end1,
          stat_plane, centroids, frame, in, out, report_max);
    }

  for (int l = 0; l <= height_end2 - sum_of_step_sizes; l++)
    {
      const int j = sum_of_step_sizes + 1 + l;
      const int in = sum_of_step_sizes + 1 + step2 + l;
      const int out = step1 + 1 + l;

      max_scan_y(backup_stack, i, frame->width, j, step1, width_end1,
          stat_plane, centroids, frame, in, out, report_max);
    }

  for (int l = 0; l < height_end1 - height_end2; l++)
    {
      const int j = height_end2 + l;
      const int in = height_end1 - 1;
      const int out = height_end2 - step2 + l;

      max_scan_y(backup_stack, i, frame->width, j, step1, width_end1,
          stat_plane, centroids, frame, in, out, report_max);
    }
}

/* REMEMBER TO IMPLEMENT PROOF-CHECK FOR max_ct/max_tmp_ct NOT EXCEEDING CEN_CT_LIMIT */
/* TODO: Split this into multiple procedures */
void
LocalMaxima_3D(tool_t * const tools, parameter_t * const parameters,
    uint8_t * const *backup_stack, const uint8_t * const *image_stack,
    const frame_t * const frame, int report_max, int t)
{
  int max_ct, max_tmp_ct, max_remained;
  int x, y, z, weight, x_dist, y_dist, z_dist;

  int * const * const density_stack = tools->density_stack;
  int * const * const max_stat = tools->max_stat;
  int *cen_tmp = tools->cen_tmp;

  const float z_factor_squared = parameters->z_factor * parameters->z_factor;

  /* boundary effects */
  const int step1 = parameters->nuc_size1 / 2;
  const int width_end1 = frame->width - step1;
  const int height_end1 = frame->height - step1;

  const int step2 = parameters->nuc_size2 / 2;
  const int sum_of_step_sizes = step1 + step2;
  const int width_end2 = frame->width - sum_of_step_sizes;
  const int height_end2 = frame->height - sum_of_step_sizes;

  const int z_step1 = parameters->z_size1 / 2;
  const int z_end1 = frame->num_planes - z_step1;

  const int z_size = (int) (1.0 * parameters->nuc_size2 / parameters->z_factor);
  const int z_step2 = z_size / 2 < 1 ? 1 : z_size / 2;

  const int step3 = parameters->nuc_size3; /* not divided by 2 as step1 and 2; different nature */

  const float weight_max_cutoff_factor = get_weight_max_cutoff_factor(
      parameters);

  /* 2D max */
  for (int i = z_step1; i < z_end1; i++)
    {
      int * const plane = density_stack[i];
      int * const stat_plane = max_stat[i];
      const int weight_max_cutoff = (int) tools->noise[t][i]
          * weight_max_cutoff_factor;

      loop11(i, tools->centroids, backup_stack, plane, frame, height_end1, height_end2,
          stat_plane, report_max, step1, step2, t, sum_of_step_sizes, weight_max_cutoff,
          width_end1, width_end2);

      loop12(i, tools->centroids, backup_stack, plane, frame, height_end1, height_end2,
               stat_plane, report_max, step1, step2, t, sum_of_step_sizes, weight_max_cutoff,
               width_end1, width_end2);
    }

  /* 3D max */
  /* two steps for 3rd dimension: first is to check the neighbor-window (along z-axis) like the 2nd dimension */
  /* second is to compare candidates from the first step that are w/i the neighborhood cube */
  /* reason for the second step is that we did not update local_max for the 2nd dimension */
  /* first step: neighborhood window */
  max_tmp_ct = 0;
  for (int i = z_step1; i < z_end1; i++)
    {
      int * const stat_plane = max_stat[i];
      max_ct = 0;
      /* recording potential local maxima (still with stat of 1 in max_stat) to 'centroids' */
      /* 'centroids' (and 'cen_tmp', too) devotes 5 (integer) elements to record one centroid: is_valid (binary), x, y, z coordinates and weight.  Notice that the y value is recorded as the address of the first pixel in the row. */
      /* the scan for the 2nd dimension uses a watered-down version where only x and weight are used. */
      /* be careful to traverse max_stat as the boundary elements were not initialized (padded) */
      for (int j = step1; j < height_end1; j++)
        {
          const int base = frame->width * j;
          for (int k = step1; k < width_end1; k++)
            {
              if (stat_plane[(base + k) * 2 + 1] == 1)
                {
                  weight = stat_plane[(base + k) * 2];
                  tools->centroids[5 * max_ct] = 1;
                  tools->centroids[5 * max_ct + 1] = k;
                  tools->centroids[5 * max_ct + 2] = base;
                  tools->centroids[5 * max_ct + 3] = i;
                  tools->centroids[5 * max_ct + 4] = weight;
                  ++max_ct;
                }
            }
        } /* for (j=step1; j<height_end1; j++) */

      if (!max_ct)
        continue;
      max_remained = max_ct;

      if (report_max == 4)
        {
          for (int j = 0; j < max_ct; j++)
            {
              MarkCentroid(backup_stack[i], frame, tools->centroids[5 * j + 1],
                  tools->centroids[5 * j + 2], MONO, 225);
            } /* for (j=0; j<max_ct; j++) */
        } /* if (report_max == 4) */

      /* compared to the implementation of the 2nd dimension: I'm using
       * the tighter/cleaner code because the wasted actions do not add to much */
      const int out = z_step1 > i - z_step2 ? z_step1 : i - z_step2;
      const int in = z_end1 < i + z_step2 ? z_end1 : i + z_step2;

      /* using the traversal of 'centroids' (k) as inner loop.  Could be
       * the outer loop (outside traversal of pixels (m).  This structure
       * might reduce the number of swaps b/t cache and memory when
       * accessing 'max_stat+m'.  Just a guess, not tested.  If wrong,
       * this implementation is more expensive as it repeatedly dereferences
       * elements in 'centroids'.  Worse for the 2nd dimension as there would
       * be more elements in 'centroids' there (see MAX_SCAN_Y). */
      for (int m = out; m <= in; m++)
        {
          if (m == i)
            continue;
          const int * const plane = max_stat[m];
          for (int k = 0; k < max_ct; k++)
            {
              if (!tools->centroids[5 * k])
                continue;

              /* TODO: BUG: this test uses uninitialized values */
              if (tools->centroids[5 * k + 4] < plane[(tools->centroids[5 * k + 2]
                  + tools->centroids[5 * k + 1]) * 2])
                {
                  /* not a local maxima */
                  tools->centroids[5 * k] = 0;
                  --max_remained;
                  if (max_remained == 0)
                    break;
                }
            }
          if (max_remained == 0)
            break;
        }
      if (max_remained == 0)
        continue;

      /* copy potential local maxima to 'cent_tmp' for second step */
      for (int k = 0; k < max_ct; k++)
        {
          if (tools->centroids[5 * k])
            {
              if (report_max == 5)
                {
                  MarkCentroid(backup_stack[tools->centroids[5 * k + 3]], frame,
                      tools->centroids[5 * k + 1], tools->centroids[5 * k + 2], MONO, 255);
                }
              memcpy(cen_tmp + 5 * max_tmp_ct, tools->centroids + 5 * k, sizeof(int)
                  * 5);
              ++max_tmp_ct;
              if (max_tmp_ct == parameters->cen_ct_limit)
                {
                  fprintf(sn_logfile,
                      "expanding tools->centroids and tools->cen_tmp");
                  parameters->cen_ct_limit = max_tmp_ct * 2;
                  tools->centroids = realloc (tools->centroids, sizeof(int) * parameters->cen_ct_limit * 5);
                  assert (tools->centroids != NULL);
                  tools->cen_tmp = realloc (tools->cen_tmp, sizeof(int) * parameters->cen_ct_limit * 5);
                  assert (tools->cen_tmp != NULL);
                } /* if (max_tmp_ct == parameters->cen_ct_limit) */
            } /* if (*(centroids+5*k)) */
        } /* for (k=0; k<max_ct; k++) */
    } /* for (i=z_step1; i<z_end1; i++) */

  if (report_max == 6)
    {
      for (int i = 0; i < max_tmp_ct; i++)
        {
          MarkCentroid(backup_stack[cen_tmp[5 * i + 3]], frame, cen_tmp[5 * i
              + 1], cen_tmp[5 * i + 2], MONO, 255);
        }
    }

  /* second step: weed out candidates in 'cen_tmp' that are too close to each other (w/i neighborhood cube) */
  for (int i = 0; i < max_tmp_ct; i++)
    {
      if (!cen_tmp[5 * i])
        continue;
      x = cen_tmp[5 * i + 1];
      y = cen_tmp[5 * i + 2];
      z = cen_tmp[5 * i + 3];
      weight = cen_tmp[5 * i + 4];
      for (int j = i + 1; j < max_tmp_ct; j++)
        {
          if (!cen_tmp[5 * j])
            continue;
          z_dist = cen_tmp[5 * j + 3] - z;
          /*if (z_dist > z_step3) break; */
          x_dist = abs(cen_tmp[5 * j + 1] - x);
          y_dist = abs(cen_tmp[5 * j + 2] - y) / frame->width;
          /*if (x_dist>step3 || y_dist>step3) continue; */
          if (sqrt(x_dist * x_dist + y_dist * y_dist + z_dist * z_dist
              * z_factor_squared) > step3)
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

      for (int i = 0; i < max_tmp_ct; i++)
        {
          if (cen_tmp[5 * i])
            {
              memcpy(tools->centroids + 5 * local_max_ct, cen_tmp + 5 * i, sizeof(int)
                  * 5);
              ++local_max_ct;
            }
        }
      tools->cen_ct = local_max_ct;
    }

}
