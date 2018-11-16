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

#ifndef SN__TOOL_T_H
#define SN__TOOL_T_H 1

#include <stdint.h>

/* TODO: Separate typedef from implementation */
typedef struct tool_t tool_t;

#include "nucleus_t.h"

struct tool_t
{
  uint8_t **image_stack;
  nucleus_t **nuclei_record;
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
  /* for calculating the sphere */
  int ***spheres;
  int **s_radii;
  int *s_layers;
};

#endif /* ! SN__TOOL_T_H */
