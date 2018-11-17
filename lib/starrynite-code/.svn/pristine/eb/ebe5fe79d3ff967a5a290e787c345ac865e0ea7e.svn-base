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

#include "NewBornID.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "logfile.h"
#include "parameter_t.h"
#include "struct_nucleus.h"

void
NewBornID(const parameter_t * const parameters, const nucleus_t * const mother,
    nucleus_t * const restrict dau1, nucleus_t * const restrict dau2)
{
  if (strcmp(mother->identity, "polar") == 0)
    {
      fprintf(sn_logfile, "Dividing polar body.\n");
    }

  if (strcmp(mother->identity, "ABa") == 0)
    {
      if (isless((dau1->z - dau2->z) * parameters->lr, 0.) != 0)
        {
          dau1->id_tag = 'l';
          dau2->id_tag = 'r';
        }
      else
        {
          dau1->id_tag = 'r';
          dau2->id_tag = 'l';
        }
    }

  else if (strcmp(mother->identity, "ABp") == 0)
    {
      if (isless((dau1->z - dau2->z) * parameters->lr, 0.) != 0)
        {
          dau1->id_tag = 'l';
          dau2->id_tag = 'r';
        }
      else
        {
          dau1->id_tag = 'r';
          dau2->id_tag = 'l';
        }
    }

  else if (strcmp(mother->identity, "EMS") == 0)
    {
      if (dau1->id_tag == 'a')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "MS");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "E");
          return;
        }
      else if (dau1->id_tag == 'p')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "E");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "MS");
          return;
        }
    }

  else if (strcmp(mother->identity, "P2") == 0)
    {
      if ((dau1->y - dau2->y) * parameters->dv + dau1->size / 2 < 0)
        {
          dau1->id_tag = 'd';
          dau2->id_tag = 'v';
        }
      else if ((dau1->y - dau2->y) * parameters->dv > dau1->size / 2)
        {
          dau1->id_tag = 'v';
          dau2->id_tag = 'd';
        }

      if (dau1->id_tag == 'd')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "C");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "P3");
          return;
        }
      else if (dau1->id_tag == 'v')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "P3");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "C");
          return;
        }
    }

  else if (strcmp(mother->identity, "P3") == 0)
    {
      if ((dau1->y - dau2->y) * parameters->dv < 0 - dau1->size / 2)
        {
          dau1->id_tag = 'd';
          dau2->id_tag = 'v';
        }
      else if ((dau1->y - dau2->y) * parameters->dv > dau1->size / 2)
        {
          dau1->id_tag = 'v';
          dau2->id_tag = 'd';
        }

      if (dau1->id_tag == 'd')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "D");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "P4");
          return;
        }
      else if (dau1->id_tag == 'v')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "P4");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "D");
          return;
        }
    }

  else if (strcmp(mother->identity, "P4") == 0)
    {
      if (dau1->id_tag == 'p')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "Z2");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "Z3");
          return;
        }
      else if (dau1->id_tag == 'a')
        {
          (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "Z3");
          (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "Z2");
          return;
        }
    }

  (void) snprintf(dau1->identity, NUCLEUS_IDENTITY_SIZE, "%s%c",
      mother->identity, dau1->id_tag);
  (void) snprintf(dau2->identity, NUCLEUS_IDENTITY_SIZE, "%s%c",
      mother->identity, dau2->id_tag);
}
