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

/*
 * struct_nucleus.h
 *
 *  Created on: Mar 2, 2009
 *      Author: blick
 */

#ifndef SN__STRUCT_NUCLEUS_H
#define SN__STRUCT_NUCLEUS_H 1

#include "nucleus_t.h"

#define NUCLEUS_IDENTITY_SIZE 50

#include "partner_t.h"

#include "enum_nucleus_start_states.h"
#include "enum_nucleus_end_states.h"

struct nucleus
{
  short is_valid, condensed, ghost, ghost_proof;
  char identity[NUCLEUS_IDENTITY_SIZE], id_tag;
  int time_start, generation, time_end;
  enum nucleus_start_states start_state;
  enum nucleus_end_states end_state;
  int start_scorem, end_scorem;
  int x, y; /* measured in number of pixels */
  float z; /* position of centroid */
  int size, max_size, max_size_time; /* diameter, measured in pixels */
  int volume, weight, max_weight, max_weight_time;

  int p_ambiguity, s_ambiguity;
  int pskip, sskip;
  partner_t *p_partners, *s_partners;
  int potential_predecessor_count, s_ct, s_ct2;
  int predecessor, successor1, successor2;
  float distance_to_predecessor, distance_to_successor1, distance_to_successor2;
  short has_predecessor, has_successor1, has_successor2;
  int is_dividing_call_correct;
  float svm_score_mov_vs_div;
};

#endif /* ! SN__STRUCT_NUCLEUS_H */
