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

#include "AddLink.h"
#include <stdlib.h>

#include "partner_t.h"
#include "logfile.h"
#include "error-codes.h"

/** Post-condition: item->availability = 1 for the first item in partners
 * where item->index = index */
void
AddLink(partner_t * const partners, const int index, const int nuclei_ct)
{

  for (int i = 0; i < nuclei_ct; i++)
    {
      if (partners[i].index != index)
        continue;

      if (partners[i].availability != 0)
        {
          fprintf(sn_logfile, "Wrong label for links.\n");
          exit(ERROR_ADDLINK);
        }
      partners[i].availability = 1;
      break;
    }
}
