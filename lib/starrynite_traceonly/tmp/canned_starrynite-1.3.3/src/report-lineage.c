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

#include <stdio.h>

#include "report-lineage.h"
#include "types.h"

void
ReportNuclei (const MOVIE_t * const movie, const TOOL_t * const tools,
              FILE * const logfile, const int t)
{
  const size_t filename_len = 50;
  char filename[filename_len];
  FILE *map_file;
  int i;

  int division = 0;
  int death = 0;
  int emerge = 0;
  int pskip = 0;
  int sskip = 0;
  int total = 0;

  const NUCLEUS_t *const nuclei = tools->nuclei_record[t];
  const NUCLEUS_t *nuclei_next = NULL;
  const int nuc_ct = tools->nuc_ct_rec[t];

  if (t < movie->tp_number - 1)
    nuclei_next = tools->nuclei_record[t + 1];

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    total++;
    if (!nuclei[i].pflag)
    {
      if (nuclei[i].pskip)
        pskip++;
      else
        emerge++;
    }
    if (!nuclei[i].sflag1)
    {
      if (nuclei[i].sskip)
        sskip++;
      else
        death++;
    }
    else if (nuclei_next && !nuclei_next[nuclei[i].successor1].status)
      death++;
    if (nuclei[i].sflag2)
      division++;
  }

  if (!t)
    emerge = 0;
  if (!nuclei_next)
    death = 0;
  fprintf (logfile, "tp %03d:  %5d  %3d  %3d  %3d  %3d  %3d  %3d\n",
           movie->time_start + t, total, nuc_ct - total, division, death,
           emerge, pskip, sskip);

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    if (t && !nuclei[i].pflag)
    {
      if (nuclei[i].pskip)
        fprintf (logfile, "          p_skip: %s, %d\n",
                 nuclei[i].identity, i + 1);
      else
        fprintf (logfile, "          emerge: %s, %d\n",
                 nuclei[i].identity, i + 1);
    }
    if (nuclei_next)
    {
      if (!nuclei[i].sflag1 || !nuclei_next[nuclei[i].successor1].status)
      {
        if (nuclei[i].sskip)
          fprintf (logfile, "          s_skip: %s, %d\n",
                   nuclei[i].identity, i + 1);
        else
          fprintf (logfile, "          death: %s, %d\n",
                   nuclei[i].identity, i + 1);
      }
    }
    if (nuclei[i].sflag2)
    {
      fprintf (logfile, "          division: %s %d -> %s %d ",
               nuclei[i].identity, i + 1,
               nuclei_next[nuclei[i].successor1].identity,
               nuclei[i].successor1 + 1);
      if (nuclei_next[nuclei[i].successor1].p_ct > 1)
        fprintf (logfile, "(s), ");
      else
        fprintf (logfile, "(h), ");
      fprintf (logfile, "%s %d",
               nuclei_next[nuclei[i].successor2].identity,
               nuclei[i].successor2 + 1);
      if (nuclei_next[nuclei[i].successor2].p_ct > 1)
        fprintf (logfile, "(s), ");
      else
        fprintf (logfile, "(h), ");
      fprintf (logfile, "\n");
    }
  }

  (void) snprintf (filename, filename_len, "t%03d-nuclei",
                   movie->time_start + t);
  map_file = fopen (filename, "w");
  for (i = 0; i < nuc_ct; i++)
  {
    fprintf (map_file, "%4d,   ", i + 1);
    if (!nuclei[i].status)
      fprintf (map_file, "0,  ");
    else
      fprintf (map_file, "1,  ");
    if (nuclei[i].pflag)
      fprintf (map_file, "%4d,  ", nuclei[i].predecessor + 1);
    else
      fprintf (map_file, "%4d,  ", -1);
    if (!nuclei[i].sflag1)
      fprintf (map_file, "%4d,  ", -1);
    else
      fprintf (map_file, "%4d,  ", nuclei[i].successor1 + 1);
    if (nuclei[i].sflag2)
      fprintf (map_file, "%4d,  ", nuclei[i].successor2 + 1);
    else
      fprintf (map_file, "%4d,  ", -1);
    fprintf (map_file, "%4d,  %4d,  %4.1f,  ", nuclei[i].x,
             nuclei[i].y / movie->frame.width, nuclei[i].z + 1);
    fprintf (map_file, "%3d,  ", nuclei[i].size);
    if (nuclei[i].status)
      fprintf (map_file, "%-12s,", nuclei[i].identity);
    else
      fprintf (map_file, "nill        ,");
    fprintf (map_file, "  %8d,", nuclei[i].weight);
    fprintf (map_file, "\n");
  }
  fclose (map_file);
}
