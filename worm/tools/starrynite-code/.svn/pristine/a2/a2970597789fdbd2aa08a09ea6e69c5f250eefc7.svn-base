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
#include <stdio.h>
#include <stdlib.h>

#include "LoadParameterFile.h"

#include "param-file.h"
#include "parameter_t.h"
#include "error-codes.h"
#include "FinalizeParameters.h"

parameter_t *
LoadParameterFile(const char * const filename)
/*@globals: stderr, errno@*/
{
  parameter_t *p = NULL;
  ParamFile *filedata = NULL;

  assert (filename != NULL);

    {
      FILE *infile = fopen(filename, "r");
      if (infile == NULL)
        {
          const char str[] = "Could not open the parameter file";

          (void) fprintf(stderr, "ERROR: %s\n", str);
          exit(ERROR_SETPARAMETERS);
        }

      filedata = ReadParameterFile(infile);

      (void) fclose(infile);
    }

  assert (filedata != NULL);

  p = FinalizeParameters(filedata);

  free(filedata);

  assert (p != NULL);

  return p;
}

