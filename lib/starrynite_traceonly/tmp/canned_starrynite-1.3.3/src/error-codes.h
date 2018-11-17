/* Copyright (C) 2005-2008 University of Washington
   Written by Dan Blick
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

#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_ 1

#if HAVE_CONFIG_H
#include <config.h>
#endif

enum
{
  ERROR_MALLOC = 2,
  ERROR_REMOVE_CELL1,           /* 3 */
  ERROR_REMOVE_CELL2,           /* 4 */
  ERROR_REMOVE_CELL3,           /* 5 */
  ERROR_ADDLINK,                /* 6 */
  ERROR_REMOVELINK,             /* 7 */
  ERROR_EVALUATELINKS,          /* 8 */
  ERROR_INTRODUCEAMBIGUITY,     /* 9 */
  ERROR_UNAMBIGUOUSMATCH1,      /* 10 */
  ERROR_UNAMBIGUOUSMATCH2,      /* 11 */
  ERROR_UNAMBIGUOUSMATCH3,      /* 12 */
  ERROR_CALCULATEMORPHOLOGYSCORE1,      /* 13 */
  ERROR_CALCULATEMORPHOLOGYSCORE2,      /* 14 */
  ERROR_READIMAGESTACK,         /* 15 */
  ERROR_SETPARAMETERS,          /* 16 */
  ERROR_WRITETIFF,              /* 17 */
  ERROR_READTIFF,               /* 18 */
  ERROR_USAGE,                  /* 19 */
  ERROR_LOGFILE = 21,           /* 21 */
  ERROR_MAIN1                   /* 22 */
};

#endif /* ! ERROR_CODES_H_ */
