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
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <tiffio.h>

#include "struct_image_info.h"
#include "tiff-io.h"
#include "error-codes.h"

static void
check_bitspersample(TIFF * const image)
{
  uint16 bps;
  /* Check that it is of a type that we support */
  if ((TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &bps) == 0) || (bps != 8))
    {
      (void) fprintf(stderr,
      "Either undefined or unsupported number of bits per sample\n");
      (void) fprintf(stderr, "bps = %d\n", bps);
      exit(ERROR_READTIFF);
    }
}

static image_info_t
read_tiff_image(TIFF * const image)
{
  uint16 photo, fillorder;
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  unsigned long bufferSize, count;
  image_info_t image_info;

  check_bitspersample(image);

  /* Read in the possibly multiple strips */
  stripSize = TIFFStripSize(image);
  stripMax = TIFFNumberOfStrips(image);
  imageOffset = 0;

  bufferSize = stripMax * stripSize;
  if ((image_info.bytes = (uint8_t *) malloc(bufferSize)) == NULL)
    {
      (void) fprintf(stderr,
      "Could not allocate enough memory for the uncompressed image\n");
      exit(ERROR_READTIFF);
    }

  for (stripCount = 0; stripCount < stripMax; stripCount++)
    {
      if ((result = TIFFReadEncodedStrip(image, stripCount,
          &image_info.bytes[imageOffset], stripSize)) == -1)
        {
          (void) fprintf(stderr, "Read error on input strip number %d\n",
              stripCount);
          exit(ERROR_READTIFF);
        }

      imageOffset += result;
    }

  /* Deal with photometric interpretations */
  if (TIFFGetField(image, TIFFTAG_PHOTOMETRIC, &photo) == 0)
    {
      (void) fprintf(stderr,
      "Image has an undefined photometric interpretation\n");
      exit(ERROR_READTIFF);
    }

  if (photo != PHOTOMETRIC_MINISBLACK)
    {
      /* Flip bits */
      (void) fprintf(stderr, "Fixing the photometric interpretation\n");

      for (count = 0; count < bufferSize; count++)
        image_info.bytes[count] = ~image_info.bytes[count];
    }

  /* Deal with fillorder */
  if (TIFFGetFieldDefaulted(image, TIFFTAG_FILLORDER, &fillorder) == 0)
    {
      (void) fprintf(stderr, "Image has an undefined fillorder\n");
      exit(ERROR_READTIFF);
    }

  if (fillorder != FILLORDER_MSB2LSB)
    {
      /* We need to swap bits -- ABCDEFGH becomes HGFEDCBA */
      printf("Fixing the fillorder\n");
      TIFFReverseBits(image_info.bytes, bufferSize);
    }

  /* Do whatever it is we do with the buffer -- we dump it in hex */
  if (TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &image_info.width) == 0)
    {
      (void) fprintf(stderr, "Image does not define its width\n");
      exit(ERROR_READTIFF);
    }

  if (TIFFGetField(image, TIFFTAG_IMAGELENGTH, &image_info.height) == 0)
    {
      (void) fprintf(stderr, "Image does not define its height\n");
      exit(ERROR_READTIFF);
    }

  TIFFClose(image);
  return image_info;
}

image_info_t
ReadTiff(const char * const fname)
{
  TIFF *image;

  /* Open the TIFF image */
  if ((image = TIFFOpen(fname, "r")) == NULL)
    {
      (void) fprintf(stderr, "Could not open image %s\n", fname);
      exit(ERROR_READTIFF);
    }

  return read_tiff_image(image);
}
