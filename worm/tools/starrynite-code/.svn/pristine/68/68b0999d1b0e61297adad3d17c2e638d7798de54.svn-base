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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "param-file.h"
#include "lookup_parameter.h"
#include "xalloc.h"
#include "error-codes.h"

/* BEGIN FUNCTION PROTOTYPES */

/* END FUNCTION PROTOTYPES */

ParamFile *
ReadParameterFile(FILE * infile)
{
  ParamFile *p;
  size_t line_size;
  char *line = NULL;
  bool isset_mcc = false;

  p = xmalloc(sizeof(ParamFile));
  assert (p != NULL);

  /* Loop through the lines of the file. */
  while (getline(&line, &line_size, infile) != -1)
    {
      char head[50];
      float tmp;

      if (line[0] == '#')
        {
          continue;
        }

      /* TODO: Use strtok to fix this */

      sscanf(line, "%s %f", head, &tmp);

        {
          const struct parameter * const param = lookup_parameter(head, strlen(
              head));

          /*
           * TODO: Low priority, might be fun to fix.
           * cf. http://c2.com/cgi/wiki?SwitchStatementsSmell
           * one "object oriented" solution would define a Parameter type,
           * which would have a string (the parameter name)
           * and a function, which would take a ParamFile * and
           * the parameter argument (tmp) as arguments.
           */
          /*
           * TODO: reading in a number as a float, then casting
           * it to an int is not safe.
           */
          switch (param->code)
            {
          case TIME_START:
            p->time_start = (int) tmp;
            break;

          case TIME_END:
            p->time_end = (int) tmp;
            break;

          case PLANE_START:
            p->plane_start = (int) tmp;
            break;

          case PLANE_END:
            p->plane_end = (int) tmp;
            break;

          case XY_RES:
            p->xy_res = tmp;
            break;

          case Z_RES:
            p->z_res = tmp;
            break;

          case TIME_INTERVAL:
            p->time_interval = (int) tmp;
            break;

          case GRAPHIC_OUTPUT:
            p->graphic_output = (int) tmp;
            break;

          case NEIGHBORHOOD_SIZE:
            p->neighborhood_size = (int) tmp;
            break;

          case CELL_CT_LIMIT:
            p->cell_ct_limit = (int) tmp;
            break;

          case NUC_SIZE:
            p->nuc_size = (int) tmp;
            break;

          case POLAR_SIZE:
            p->polar_size = (int) tmp;
            break;

          case MAX_NUC_SIZE:
            p->max_nuc_size = (int) tmp;
            break;

          case NUC_SIZE_FACTOR1:
            p->nuc_size_factor1 = tmp;
            break;

          case NUC_SIZE_FACTOR2:
            p->nuc_size_factor2 = tmp;
            break;

          case NUC_SIZE_FACTOR3:
            p->nuc_size_factor3 = tmp;
            break;

          case NUC_SIZE_FACTOR4:
            p->nuc_size_factor4 = tmp;
            break;

          case NOISE_FRACTION:
            p->noise_fraction = tmp;
            break;

          case MAX_WEIGHT_CUTOFF:
            p->max_weight_cutoff = tmp;
            break;

          case NUC_DENSITY_CUTOFF:
            p->nuc_density_cutoff = tmp;
            break;

          case NUC_DENSITY_CUTOFF1:
            p->nuc_density_cutoff1 = tmp;
            break;

          case NUC_DENSITY_CUTOFF2:
            p->nuc_density_cutoff2 = tmp;
            break;

          case NUC_DENSITY_CUTOFF3:
            p->nuc_density_cutoff3 = tmp;
            break;

          case NOISE_FACTOR:
            p->noise_factor = tmp;
            break;

          case NOISE_FACTOR1:
            p->noise_factor1 = tmp;
            break;

          case NOISE_FACTOR2:
            p->noise_factor2 = tmp;
            break;

          case NOISE_FACTOR3:
            p->noise_factor3 = tmp;
            break;

          case NOISE_TIME_SWITCH1:
            p->noise_time_switch1 = (int) tmp;
            break;

          case NOISE_TIME_SWITCH2:
            p->noise_time_switch2 = (int) tmp;
            break;

          case MINIMAL_CELL_CYCLE:
            p->minimal_cell_cycle = (int) tmp;
            isset_mcc = true;
            break;

          case AMBIGUITY_CUTOFF:
            p->ambiguity_cutoff = tmp;
            break;

          case SHRINK_ELASTISITY:
            p->shrink_elastisity = tmp;
            break;

          case EXPAND_ELASTISITY:
            p->expand_elastisity = tmp;
            break;
            }
        }
    }

  if (line != NULL)
    {
      free(line);
    }

  if (!isset_mcc)
    {
      const char str[] = "parameter 'minimal_cell_cycle' was not set";

      (void) fprintf(stderr, "ERROR: %s\n", str);
      return NULL;
    }

  return p;
}
