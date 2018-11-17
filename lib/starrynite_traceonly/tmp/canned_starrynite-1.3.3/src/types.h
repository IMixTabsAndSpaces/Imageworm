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

#ifndef TYPES_H_
#define TYPES_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

typedef struct PARTNER_t PARTNER_t;
typedef struct NUCLEUS_t NUCLEUS_t;
typedef struct FRAME_t FRAME_t;
typedef struct TOOL_t TOOL_t;
typedef struct MOVIE_t MOVIE_t;
typedef struct PARAMETER_t PARAMETER_t;

struct PARTNER_t
{
  int availability;
  int index;
  float distance;
  int score1, score2, score1m, score2m;
};

#define NUCLEUS_IDENTITY_SIZE 50

struct NUCLEUS_t
{
  short status, condensed, ghost, ghost_proof;
  char identity[NUCLEUS_IDENTITY_SIZE], id_tag;
  int birthday, generation, end_time;
  int start_state, end_state;
  int start_scorem, end_scorem;
  int x, y;                     /* measured in number of pixels */
  float z;                      /* position of centroid */
  int size, max_size, max_size_time;    /* diameter, measured in pixels */
  int volume, weight, max_weight, max_weight_time;
  int x0, y0, z0, wei0;
  int b_ct;
  float sponginess1, sponginess2;
  int piece_weight[8], piece_vol[8], piece_brights[8];
  float piece_ratio, piece_ratio2;
  int p_ambiguity, s_ambiguity;
  int pskip, sskip;
  PARTNER_t *p_partners, *s_partners;
  int p_ct, s_ct, p_ct2, s_ct2;
  int predecessor, successor1, successor2;
  float pdist, sdist1, sdist2;
  short pflag, sflag1, sflag2;
};

struct FRAME_t
{
  int x_start, y_start, z_start;
  int width, height, plane_number;
  int image_size;
};

struct MOVIE_t
{
  char output_prefix[500];
  FRAME_t frame;
  float max_distance;
  float xy_res, z_res;
  int plane_start, plane_end;
  int t_interval;
  int time_start, time_end, tp_number;
};

struct PARAMETER_t
{
  int graphic_output;
  int neighborhood_size;
  int *sizes;
  int nuc_size;
  int polar_size;
  int max_nuc_size;
  int min_nuc_size;
  int nuc_size1;
  int nuc_size2;
  int nuc_size3;
  int nuc_size4;
  float nuc_size_factor1;
  float nuc_size_factor2;
  float nuc_size_factor3;
  float nuc_size_factor4;
  float z_res_fudge;
  float z_factor;
  int z_size1, cube_size;
  int cen_ct_limit;
  int t;
  int noise_time_switch1;
  int noise_time_switch2;
  float noise_fraction;
  float max_weight_cutoff;
  float nuc_density_cutoff;
  float noise_factor;
  float nuc_density_cutoff1;
  float noise_factor1;
  float nuc_density_cutoff2;
  float noise_factor2;
  float nuc_density_cutoff3;
  float noise_factor3;
  int axis;
  int ap;
  int lr;
  int dv;
  int minimal_cell_cycle;
  float ambiguity_cutoff;
  float shrink_elastisity;
  float expand_elastisity;
};

struct TOOL_t
{
  uint8_t **image_stack;
  uint8_t **image_stack_prev;
  NUCLEUS_t **nuclei_record;
  int *nuc_ct_rec;
  /* for computing local density of signals */
  int **density_stack;
  int **density_stack_2D;
  int *density_2D;
  /* for finding local maxima/centroids */
  int **max_stat;
  int *centroids;
  int *cen_tmp;
  int cen_ct;
  /* for setting noise cutoff */
  int **noise;
  int **noise_ori;
  int **max_cutoff;
  int **max_cutoff_ori;
  int *dist;
  int *dist1;
  int *dist2;
  int *distx;
  /* for calculating the sphere */
  int ***spheres;
  int **s_radii;
  int *s_layers;
};

#endif /* !TYPES_H_ */
