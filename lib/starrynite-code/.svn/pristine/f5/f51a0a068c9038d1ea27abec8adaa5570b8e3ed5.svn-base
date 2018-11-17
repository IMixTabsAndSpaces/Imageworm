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

#include <assert.h>
#include <stdlib.h>

#include "SetupMovie.h"

#include "movie_t.h"
#include "parameter_t.h"
#include "xalloc.h"

movie_t *
SetupMovie(const parameter_t * const p)
{
  movie_t *m;
  assert(p != NULL);

  m = xmalloc(sizeof(movie_t));
  assert(m != NULL);

  m->time_start = p->movie.time_start;
  m->time_end = p->movie.time_end;
  m->plane_start = p->movie.plane_start;
  m->plane_end = p->movie.plane_end;
  m->xy_res = p->movie.xy_res;
  m->z_res = p->movie.z_res;
  m->t_interval = p->movie.t_interval;
  m->num_timepts = p->movie.num_timepts;
  m->frame.num_planes = p->movie.plane_end - p->movie.plane_start + 1;

  return m;
}

