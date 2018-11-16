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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "RefineNuclei.h"
#include "logfile.h"
#include "LocalMaxima_3D.h"
#include "DefineNuclei.h"
#include "nucleus_t.h"
#include "frame_t.h"
#include "tool_t.h"
#include "parameter_t.h"
#include "LocalWeightScan_3D.h"
#include "xalloc.h"

void
DefineNuclei(const int time_start, parameter_t * const parameters,
    tool_t * const tools, uint8_t ** backup_stack,

    const uint8_t * const *image_stack, frame_t * frame, nucleus_t ** nuc_rec,
    int *nuc_ct_p, int t)
{
  int cen_ct;
  nucleus_t *nuclei, *nuclei2;
  int i, nuc_left;

  /* centroids */
  LocalWeightScan_3D(tools->density_stack, tools->density_stack_2D,
      tools->density_2D, image_stack, frame, parameters->nuc_size1,
      parameters->z_size1);
  LocalMaxima_3D(tools, parameters, backup_stack, image_stack, frame, 0, t);

  cen_ct = tools->cen_ct;
  if (!cen_ct)
    {
      *nuc_rec = NULL;
      *nuc_ct_p = 0;
      fprintf(sn_logfile, ", 0 cells");
      return;
    }

  nuclei = (nucleus_t *) xmalloc(sizeof(nucleus_t) * cen_ct);
  RefineNuclei(tools, parameters, time_start, image_stack, frame, nuclei, t);

  nuc_left = 0;
  for (i = 0; i < cen_ct; i++)
    {
      if (nuclei[i].is_valid)
        nuc_left++;
    }

  *nuc_ct_p = nuc_left;

  if (!nuc_left)
    {
      *nuc_rec = NULL;
      *nuc_ct_p = 0;
      free(nuclei);
    }
  else if (nuc_left == cen_ct)
    {
      *nuc_rec = nuclei;
    }
  else
    {
      nuclei2 = (nucleus_t *) xmalloc(sizeof(nucleus_t) * nuc_left);
      nuc_left = 0;
      for (i = 0; i < cen_ct; i++)
        {
          if (nuclei[i].is_valid)
            {
              nuclei2[nuc_left] = nuclei[i];
              nuc_left++;
            }
        }
      *nuc_rec = nuclei2;
      free(nuclei);
    }

  fprintf(sn_logfile, ", %d cells", nuc_left);
}

