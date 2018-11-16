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

#ifndef SN__MOVIE_T_H
#define SN__MOVIE_T_H 1

/* TODO: Separate typedef from implementation */
typedef struct movie_t movie_t;

#include "frame_t.h"
#include "struct_frame.h"

struct movie_t
{
  frame_t frame;
  float max_distance;
  float xy_res;
  float z_res;
  int plane_start;
  int plane_end;
  int t_interval;
  int time_start;
  int time_end;
  int num_timepts;
};

#endif /* ! SN__MOVIE_T_H */
