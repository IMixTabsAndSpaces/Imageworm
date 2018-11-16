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

#ifndef SN__MAKE_MESSAGE_H
#define SN__MAKE_MESSAGE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static char *
make_message(const char *fmt, ...)
{
  /* Guess we need no more than 100 bytes. */
  int n, size = 100;
  char *p, *np;
  va_list ap;

  if ((p = malloc(size)) == NULL)
    return NULL;

  while (1)
    {
      /* Try to print in the allocated space. */
      va_start (ap, fmt);
      n = vsnprintf(p, size, fmt, ap);
      va_end (ap);
      /* If that worked, return the string. */
      if (n > -1 && n < size)
        return p;
      /* Else try again with more space. */
      if (n > -1)
        size = n + 1; /* precisely what is needed */
      else
        size *= 2; /* twice the old size */
      if ((np = realloc(p, size)) == NULL)
        {
          free(p);
          return NULL;
        }
      else
        {
          p = np;
        }
    }
}

#endif /* ! SN__MAKE_MESSAGE_H */
