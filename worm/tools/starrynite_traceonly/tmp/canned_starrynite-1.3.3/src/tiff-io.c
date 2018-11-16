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

/*
 * functions for reading in a tiff image
 *
 * 2/6/08 - dblick
 * Replaced ReadTiff with Open Source code from:
 * http://www-128.ibm.com/developerworks/linux/library/l-libtiff/
 *
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <tiffio.h>

#include "tiff-io.h"
#include "error-codes.h"

uint32_t image_width = 0;
uint32_t image_height = 0;
uint32_t image_size = 0;

static int16_t bits_per_sample;

#if 0
static int16_t photo_interp;
static uint16_t resolution_unit;
static uint32_t resolutions[4];
static uint16_t RGB_bits[3];
#endif


/* BUG: WriteTiff probably does not work as desired! We need to keep better
 * track of properties. */
void
WriteTiff (const char *const fname, uint8_t * const buffer, int tifftype)
{
  TIFF *image;

  image = TIFFOpen (fname, "w");
  if (image == NULL)
  {
    fprintf (stderr, "Could not open %s for writing\n", fname);
    exit (ERROR_WRITETIFF);
  }

  /* We need to set some values for basic tags before we can add any data */

  TIFFSetField (image, TIFFTAG_IMAGEWIDTH, image_width);
  TIFFSetField (image, TIFFTAG_IMAGELENGTH, image_height);
  TIFFSetField (image, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
  TIFFSetField (image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField (image, TIFFTAG_ROWSPERSTRIP, image_height);

  TIFFSetField (image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField (image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField (image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

#if 0
  TIFFSetField (image, TIFFTAG_XRESOLUTION, 150.0);
  TIFFSetField (image, TIFFTAG_YRESOLUTION, 150.0);
  TIFFSetField (image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
#endif

  /* Write the information to the file */

  TIFFWriteEncodedStrip (image, 0, buffer, image_width * image_height);

  /* Close the file */
  TIFFClose (image);

}

void
ReadTiff (const char *const fname, uint8_t ** const image_p,
          uint32_t * const local_image_width,
          uint32_t * const local_image_height)
{
  TIFF *image;
  uint16 photo, bps, spp, fillorder;
  tsize_t stripSize;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  uint8_t *buffer, tempbyte;
  unsigned long bufferSize, count;

  /* Open the TIFF image */
  if ((image = TIFFOpen (fname, "r")) == NULL)
  {
    fprintf (stderr, "Could not open image %s\n", fname);
    exit (ERROR_READTIFF);
  }

  /* Check that it is of a type that we support */
  if ((TIFFGetField (image, TIFFTAG_BITSPERSAMPLE, &bps) == 0) || (bps != 8))
  {
    fprintf (stderr,
             "Either undefined or unsupported number of bits per sample\n");
    fprintf (stderr, "bps = %d\n", bps);
    exit (ERROR_READTIFF);
  }

  bits_per_sample = bps;

  if ((TIFFGetField (image, TIFFTAG_SAMPLESPERPIXEL, &spp) == 0)
      || (spp != 1))
  {
    fprintf (stderr,
             "Either undefined or unsupported number of samples per pixel\n");
    exit (ERROR_READTIFF);
  }

  /* Read in the possibly multiple strips */
  stripSize = TIFFStripSize (image);
  stripMax = TIFFNumberOfStrips (image);
  imageOffset = 0;

  bufferSize = stripMax * stripSize;
  if ((buffer = (uint8_t *) malloc (bufferSize)) == NULL)
  {
    fprintf (stderr,
             "Could not allocate enough memory for the uncompressed image\n");
    exit (ERROR_READTIFF);
  }


  for (stripCount = 0; stripCount < stripMax; stripCount++)
  {
    if ((result = TIFFReadEncodedStrip (image, stripCount,
                                        buffer + imageOffset,
                                        stripSize)) == -1)
    {
      fprintf (stderr, "Read error on input strip number %d\n", stripCount);
      exit (ERROR_READTIFF);
    }

    imageOffset += result;
  }

  /* Deal with photometric interpretations */
  if (TIFFGetField (image, TIFFTAG_PHOTOMETRIC, &photo) == 0)
  {
    fprintf (stderr, "Image has an undefined photometric interpretation\n");
    exit (ERROR_READTIFF);
  }

  if (photo != PHOTOMETRIC_MINISBLACK)
  {
    /* Flip bits */
    fprintf (stderr, "Fixing the photometric interpretation\n");

    for (count = 0; count < bufferSize; count++)
      buffer[count] = ~buffer[count];
  }

  /* Deal with fillorder */
  if (TIFFGetFieldDefaulted (image, TIFFTAG_FILLORDER, &fillorder) == 0)
  {
    fprintf (stderr, "Image has an undefined fillorder\n");
    exit (ERROR_READTIFF);
  }

  if (fillorder != FILLORDER_MSB2LSB)
  {
    /* We need to swap bits -- ABCDEFGH becomes HGFEDCBA */
    printf ("Fixing the fillorder\n");

    for (count = 0; count < bufferSize; count++)
    {
      tempbyte = 0;
      if (buffer[count] & 128)
        tempbyte += 1;
      if (buffer[count] & 64)
        tempbyte += 2;
      if (buffer[count] & 32)
        tempbyte += 4;
      if (buffer[count] & 16)
        tempbyte += 8;
      if (buffer[count] & 8)
        tempbyte += 16;
      if (buffer[count] & 4)
        tempbyte += 32;
      if (buffer[count] & 2)
        tempbyte += 64;
      if (buffer[count] & 1)
        tempbyte += 128;
      buffer[count] = tempbyte;
    }
  }

  /* Do whatever it is we do with the buffer -- we dump it in hex */
  if (TIFFGetField (image, TIFFTAG_IMAGEWIDTH, &image_width) == 0)
  {
    fprintf (stderr, "Image does not define its width\n");
    exit (ERROR_READTIFF);
  }

  *local_image_width = image_width;

  if (TIFFGetField (image, TIFFTAG_IMAGELENGTH, &image_height) == 0)
  {
    fprintf (stderr, "Image does not define its height\n");
    exit (ERROR_READTIFF);
  }

  *local_image_height = image_height;

  image_size = image_width * image_height;

  *image_p = buffer;

  TIFFClose (image);
}
