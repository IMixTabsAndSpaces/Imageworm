/*
 * starrynite_main.c
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>
#include <stdlib.h>             /* for exit */
#include <libgen.h>             /* for basename */
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>             /* for getopt */

#include "error-codes.h"
#include "run_starrynite.h"
#include "starrynite_main.h"

static const char *program_name;

static void
show_time(const char * const msg)
{
  time_t rawtime;
  struct tm *timeinfo;
  char *time_string;

  rawtime = time(&rawtime);
  assert (rawtime != -1);

  timeinfo = localtime(&rawtime);
  assert (timeinfo != NULL);

  time_string = asctime(timeinfo);
  assert (time_string != NULL);

  (void) printf("%s: %s", msg, time_string);
}

static void
show_usage_exit()
{
  (void) fprintf(stderr, "Usage: "
    "%s [options] {input tiff file prefix} "
    "{parameter file}\n\nOptions:\n", program_name);

  (void) fprintf(stderr, "  %-22s%s\n", "-v",
      "show program's version number and exit");

  (void) fprintf(stderr, "  %-22s%s\n", "-h",
        "show this help message and exit");

  (void) fprintf(stderr, "  %-22s%s\n", "-s",
        "don't run the svm classifier");

  exit(ERROR_USAGE);
}

static void
deprecated_warning(void)
{
  (void) fprintf(stderr,
  "*WARNING*: Running starrynite with three command-line arguments is\n"
    "deprecated; support for this feature may be removed in a future "
    "version.\n\n");
}

int
starrynite_main(const int argc, char * const * const argv, run_starrynite_t callback)
/*@globals: stdout, stderr, errno @*/
{
  assert (argv[0] != NULL);
  program_name = basename(argv[0]);
  extern int optind;
  bool sflg = false;

    {
      int c;
      int hflg = 0;
      int vflg = 0;
      int errflg = 0;
      extern int optopt;

      while ((c = getopt(argc, argv, "hvs")) != -1)
        {
          switch (c)
            {
          case 'h':
            if (hflg != 0)
              {
                ++errflg;
              }
            else
              {
                ++hflg;
              }
            break;
          case 's':
            if (sflg != 0)
              {
                ++errflg;
              }
            else
              {
                sflg = true;
              }
            break;
          case 'v':
            if (vflg != 0)
              {
                ++errflg;
              }
            else
              {
                ++vflg;
              }
            break;
          case '?':
            (void) fprintf(stderr, "Unrecognized option: -%c\n", optopt);
            ++errflg;
            }
        }

      if (errflg != 0)
        {
          show_usage_exit();
        }

      if (hflg != 0)
        {
          show_usage_exit();
        }

      if (vflg != 0)
        {
          (void) printf("%s\n", PACKAGE_STRING);
        }

      if (argc - optind != 2 && argc - optind != 3)
        {
          show_usage_exit();
        }
    }

    {
      int retcode;

      assert(2 <= argc - optind);
      assert(3 >= argc - optind);

      const char * const input_prefix = argv[optind];
      const char * param_file_name;

      if (optind + 2 == argc)
        {
          param_file_name = argv[optind + 1];
        }
      else
        {
          assert(optind + 3 == argc);
          deprecated_warning();
          param_file_name = argv[optind + 2];
        }

      show_time("Start time");

      retcode = callback(input_prefix, param_file_name, sflg);
      if (retcode != 0)
        {
          (void) fprintf(stderr, "starrynite returned error code %d\n.",
              retcode);
          return retcode;
        }
    }

  show_time("End time");

  exit(EXIT_SUCCESS);
}
