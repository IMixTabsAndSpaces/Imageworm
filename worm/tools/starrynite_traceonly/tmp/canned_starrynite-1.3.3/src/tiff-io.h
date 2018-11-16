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

#ifndef _TIFF_IO_H
#define _TIFF_IO_H 1

/* functions for reading in a tiff image */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

struct image_info
{
  uint32_t width;
  uint32_t height;
  uint32_t size;
};

void WriteTiff (
  const char *fname,
  uint8_t *,
  int tifftype
);

void ReadTiff (
  const char *fname,
  uint8_t ** image_p,
  /*@out @ */ uint32_t *,
  /*@out @ */ uint32_t *
);

#endif /* _TIFF_IO_H */
