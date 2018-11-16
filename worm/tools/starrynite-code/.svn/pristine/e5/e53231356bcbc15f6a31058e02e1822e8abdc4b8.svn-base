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

#include "CalculateSizeParameters.h"

#include "parameter_t.h"

void
CalculateSizeParameters(parameter_t * const parameters)
{
  parameters->nuc_size1 = (int) (parameters->nuc_size_factor1
      * parameters->nuc_size);
  parameters->nuc_size2 = (int) (parameters->nuc_size_factor2
      * parameters->nuc_size);
  parameters->nuc_size3 = (int) (parameters->nuc_size_factor3
      * parameters->nuc_size);
  parameters->nuc_size4 = (int) (parameters->nuc_size_factor4
      * parameters->nuc_size);
  parameters->z_size1 = (int) (1.0 * parameters->nuc_size1
      / parameters->z_factor);
  if (parameters->z_size1 < 3)
    parameters->z_size1 = 3;
  parameters->cube_size = parameters->nuc_size1 * parameters->nuc_size1
      * parameters->z_size1;
}

