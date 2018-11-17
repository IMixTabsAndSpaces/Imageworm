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

#include "AlignDiamond.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logfile.h"
#include "struct_nucleus.h"
#include "struct_frame.h"

/* TODO: Add documentation */
int
AlignDiamond(nucleus_t * nuclei, int nuc_ct, int t,
    const frame_t * const frame, const int time_start)
{
  int xmin, xmax, ymin, ymax;
  nucleus_t *north = NULL, *south = NULL, *west = NULL, *east = NULL;
  int i;

  xmin = frame->width;
  xmax = 0;
  ymin = frame->height * frame->width;
  ymax = 0;

  for (i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].is_valid == 0)
        continue;

      if (strcmp(nuclei[i].identity, "polar") == 0)
        continue;

      if (nuclei[i].x < xmin)
        {
          xmin = nuclei[i].x;
          west = nuclei + i;
        }
      if (nuclei[i].x > xmax)
        {
          xmax = nuclei[i].x;
          east = nuclei + i;
        }
      if (nuclei[i].y < ymin)
        {
          ymin = nuclei[i].y;
          north = nuclei + i;
        }
      if (nuclei[i].y > ymax)
        {
          ymax = nuclei[i].y;
          south = nuclei + i;
        }
    }

  if (!north || !south || !west || !east)
    {
      fprintf(sn_logfile, "No diamond four cell stage, time %d\n", t
          + time_start);
      return 1;
    }
  if (north == south || north == west || north == east || south == west
      || south == east || west == east)
    {
      fprintf(sn_logfile, "No diamond four cell stage, time %d\n", t
          + time_start);
      return 1;
    }

  north->id_tag = 'n';
  south->id_tag = 's';
  east->id_tag = 'e';
  west->id_tag = 'w';

  return 0;
}
