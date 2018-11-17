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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "cell-identity.h"
#include "types.h"

/* BEGIN FUNCTION PROTOTYPES */

static void EarlyAxis (
  const PARAMETER_t *,
  NUCLEUS_t *,
  NUCLEUS_t *
);

static void MidAxis (
  const PARAMETER_t *,
  NUCLEUS_t *,
  NUCLEUS_t *
);

static void SisterID (
  const MOVIE_t *,
  const PARAMETER_t *,
  NUCLEUS_t *,
  NUCLEUS_t *,
  int
);

static void RotateAxis (
  PARAMETER_t *
);

static int AlignDiamond (
  const MOVIE_t *,
  FILE *,
  NUCLEUS_t * nuclei,
  int nuc_ct,
  int t
);

static int TimeToDivide (
  const MOVIE_t *,
  const TOOL_t *,
  int current_time,
  NUCLEUS_t * nuc
);

static int FourCellIDAssignment (
  const MOVIE_t *,
  const TOOL_t *,
  PARAMETER_t *,
  FILE *,
  int four_cells
);

static int FourCellID (
  const MOVIE_t *,
  const TOOL_t *,
  PARAMETER_t *,
  FILE *,
  int four_cells,
  int *lineage_ct_p
);

static void NewBornID (
  const PARAMETER_t *,
  FILE *,
  const NUCLEUS_t * const mother,
  NUCLEUS_t * const restrict dau1,
  NUCLEUS_t * const restrict dau2
);

static int BackAssignment (
  const MOVIE_t *,
  const TOOL_t *,
  const PARAMETER_t *,
  FILE *,
  int four_cells,
  int *lineage_ct_p
);

static int CountCells (
  const NUCLEUS_t *,
  int
);

static void PolarBodies (
  const MOVIE_t *,
  const TOOL_t *,
  const PARAMETER_t *,
  FILE *,
  int tx,
  int *polar_ct_p
);

static int InitialID (
/*@out@ */ const MOVIE_t *,
  const TOOL_t *,
  PARAMETER_t *,
  FILE *,
  int *start_p,
  int *lineage_ct_p
);

/* END FUNCTION PROTOTYPES */

/* Function bodies */

static void
EarlyAxis (const PARAMETER_t * const parameters, NUCLEUS_t * const nuc1,
           NUCLEUS_t * const nuc2)
{
  char left, right, dorsal, ventral;

  if (parameters->lr == 1)
  {
    left = 't';
    right = 'b';
  }
  else
  {
    left = 'b';
    right = 't';
  }

  if (parameters->dv == 1)
  {
    dorsal = 'u';
    ventral = 'd';
  }
  else
  {
    dorsal = 'd';
    ventral = 'u';
  }

  if (nuc1->id_tag == left)
  {
    nuc1->id_tag = 'l';
    nuc2->id_tag = 'r';
  }
  else if (nuc1->id_tag == right)
  {
    nuc1->id_tag = 'r';
    nuc2->id_tag = 'l';
  }
  else if (nuc1->id_tag == dorsal)
  {
    nuc1->id_tag = 'd';
    nuc2->id_tag = 'v';
  }
  else
  {
    nuc1->id_tag = 'v';
    nuc2->id_tag = 'd';
  }
}



static void
MidAxis (const PARAMETER_t * const parameters, NUCLEUS_t * const nuc1,
         NUCLEUS_t * const nuc2)
{
  char left, right, dorsal, ventral;

  if (parameters->lr == 1)
  {
    left = 'u';
    right = 'd';
  }
  else
  {
    left = 'd';
    right = 'u';
  }

  if (parameters->dv == 1)
  {
    dorsal = 't';
    ventral = 'b';
  }
  else
  {
    dorsal = 'b';
    ventral = 't';
  }

  if (nuc1->id_tag == left)
  {
    nuc1->id_tag = 'l';
    nuc2->id_tag = 'r';
  }
  else if (nuc1->id_tag == right)
  {
    nuc1->id_tag = 'r';
    nuc2->id_tag = 'l';
  }
  else if (nuc1->id_tag == dorsal)
  {
    nuc1->id_tag = 'd';
    nuc2->id_tag = 'v';
  }
  else
  {
    nuc1->id_tag = 'v';
    nuc2->id_tag = 'd';
  }
}



static void
SisterID (const MOVIE_t * const movie, const PARAMETER_t * const parameters,
          NUCLEUS_t * const nuc1, NUCLEUS_t * const nuc2, const int nuc_ct)
{
  int xdiff = nuc1->x - nuc2->x;
  int ydiff = (nuc1->y - nuc2->y) / (int) movie->frame.width;
  int cutoff = (nuc1->size + nuc2->size) / 4;

  if (cutoff < 10)
    cutoff = 10;

  if (parameters->axis == 0)
  {
    if (xdiff < 0 - cutoff)
    {
      nuc1->id_tag = 'w';
      nuc2->id_tag = 'e';
    }
    else if (xdiff > cutoff)
    {
      nuc1->id_tag = 'e';
      nuc2->id_tag = 'w';
    }
    else if (ydiff < 0 - cutoff)
    {
      nuc1->id_tag = 'u';
      nuc2->id_tag = 'd';
    }
    else if (ydiff > cutoff)
    {
      nuc1->id_tag = 'd';
      nuc2->id_tag = 'u';
    }
    else if (isless (nuc1->z, nuc2->z) != 0)
    {
      nuc1->id_tag = 't';
      nuc2->id_tag = 'b';
    }
    else
    {
      nuc1->id_tag = 'b';
      nuc2->id_tag = 't';
    }
    return;
  }

  if (xdiff * parameters->ap < -cutoff)
  {
    nuc1->id_tag = 'a';
    nuc2->id_tag = 'p';
    return;
  }
  else if (xdiff * parameters->ap > cutoff)
  {
    nuc1->id_tag = 'p';
    nuc2->id_tag = 'a';
    return;
  }

  if (ydiff * parameters->ap < -cutoff)
  {
    nuc1->id_tag = 'u';
    nuc2->id_tag = 'd';
  }
  else if (ydiff * parameters->ap > cutoff)
  {
    nuc1->id_tag = 'd';
    nuc2->id_tag = 'u';
  }
  else if (isless (nuc1->z, nuc2->z) != 0)
  {
    nuc1->id_tag = 't';
    nuc2->id_tag = 'b';
  }
  else
  {
    nuc1->id_tag = 'b';
    nuc2->id_tag = 't';
  }
  if (nuc_ct < 100)
  {
    EarlyAxis (parameters, nuc1, nuc2);
  }
  else if (nuc_ct < 450)
  {
    MidAxis (parameters, nuc1, nuc2);
  }
}

static void
RotateAxis (PARAMETER_t * const parameters)
{
  parameters->lr *= parameters->ap * (-1);
  parameters->dv *= parameters->ap;
}



static int
AlignDiamond (const MOVIE_t * const movie, FILE * const logfile,
              NUCLEUS_t * nuclei, int nuc_ct, int t)
{
  int xmin, xmax, ymin, ymax;
  NUCLEUS_t *north = NULL, *south = NULL, *west = NULL, *east = NULL;
  int i;

  xmin = movie->frame.width;
  xmax = 0;
  ymin = movie->frame.height * movie->frame.width;
  ymax = 0;

  for (i = 0; i < nuc_ct; i++)
  {
    if (nuclei[i].status == 0)
      continue;

    if (strcmp (nuclei[i].identity, "polar") == 0)
      continue;

    if (nuclei[i].x < xmin)
    {
      xmin = nuclei[i].x;
      west = nuclei + i;
    }
    if (nuclei[i].x > xmax)
    {
      xmax = nuclei[i].x;
      east = nuclei + i;
    }
    if (nuclei[i].y < ymin)
    {
      ymin = nuclei[i].y;
      north = nuclei + i;
    }
    if (nuclei[i].y > ymax)
    {
      ymax = nuclei[i].y;
      south = nuclei + i;
    }
  }

  if (!north || !south || !west || !east)
  {
    (void) fprintf (logfile, "No diamond four cell stage, time %d\n",
                    t + movie->time_start);
    return 0;
  }
  if (north == south || north == west || north == east || south == west
      || south == east || west == east)
  {
    (void) fprintf (logfile, "No diamond four cell stage, time %d\n",
                    t + movie->time_start);
    return 0;
  }

  north->id_tag = 'n';
  south->id_tag = 's';
  east->id_tag = 'e';
  west->id_tag = 'w';

  return 1;
}

static int
TimeToDivide (const MOVIE_t * const movie, const TOOL_t * const tools,
              int current_time, NUCLEUS_t * nuc)
{
  int tp_number = movie->tp_number;

  while (current_time < tp_number - 1)
  {
    if (nuc->sflag1 == 0)
    {
      current_time = -1;
      break;
    }
    if (nuc->sflag2 != 0)
    {
      break;
    }
    current_time++;
    nuc = tools->nuclei_record[current_time] + nuc->successor1;
  }

  return current_time;
}



static int
FourCellIDAssignment (const MOVIE_t * const movie, const TOOL_t * const tools,
                      PARAMETER_t * const parameters, FILE * logfile,
                      const int four_cells)
{
  /* TODO: Make this function use only one return statement. */
  /* TODO: Apply the Anna Karenina principle:
   * make this function return '0' for success, '1' for errors */
  NUCLEUS_t *north, *south, *west, *east;

  NUCLEUS_t *ABa, *ABp, *EMS, *P2;
  int ntime, stime, etime, wtime;
  int i;
  const int tp_number = movie->tp_number;

  NUCLEUS_t *const nuclei = tools->nuclei_record[four_cells];
  const int nuc_ct = tools->nuc_ct_rec[four_cells];

  north = NULL;
  south = NULL;
  west = NULL;
  east = NULL;

  for (i = 0; i < nuc_ct; i++)
  {
    if (nuclei[i].id_tag == 'n')
    {
      north = nuclei + i;
    }
    else if (nuclei[i].id_tag == 's')
    {
      south = nuclei + i;
    }
    else if (nuclei[i].id_tag == 'e')
    {
      east = nuclei + i;
    }
    else if (nuclei[i].id_tag == 'w')
    {
      west = nuclei + i;
    }
    nuclei[i].id_tag = '\0';
  }

  assert (north != NULL);
  assert (south != NULL);
  assert (east != NULL);
  assert (west != NULL);

  ntime = TimeToDivide (movie, tools, four_cells, north);
  stime = TimeToDivide (movie, tools, four_cells, south);
  etime = TimeToDivide (movie, tools, four_cells, east);
  wtime = TimeToDivide (movie, tools, four_cells, west);

  if (ntime == -1 || stime == -1 || etime == -1 || wtime == -1)
  {
    (void) fprintf (logfile, "Cell death at four cell stage.\n");
    return 0;
  }

  if (wtime < etime)
  {
    ABa = west;
    P2 = east;
    parameters->ap = 1;
  }
  else if (wtime > etime)
  {
    ABa = east;
    P2 = west;
    parameters->ap = -1;
  }
  else
  {
    if (wtime == tp_number - 1)
      (void) fprintf (logfile, "Movie too short to capture 6 cell stage\n");
    else
      (void) fprintf (logfile, "putative ABa and P2 divide simutaneously.\n");
    return 0;
  }

  if (ntime < stime)
  {
    ABp = north;
    EMS = south;
    parameters->dv = 1;
  }
  else if (ntime > stime)
  {
    ABp = south;
    EMS = north;
    parameters->dv = -1;
  }
  else
  {
    (void) fprintf (logfile, "putative ABp and EMS divide simutaneously.\n");
    return 0;
  }

  parameters->lr = parameters->ap * parameters->dv;

  (void) snprintf (ABa->identity, NUCLEUS_IDENTITY_SIZE, "ABa");
  (void) snprintf (ABp->identity, NUCLEUS_IDENTITY_SIZE, "ABp");
  (void) snprintf (EMS->identity, NUCLEUS_IDENTITY_SIZE, "EMS");
  (void) snprintf (P2->identity, NUCLEUS_IDENTITY_SIZE, "P2");

  return 1;
}


static int
FourCellID (const MOVIE_t * const movie, const TOOL_t * const tools,
            PARAMETER_t * const parameters, FILE * const logfile,
            const int four_cells, int *const lineage_ct_p)
{
  NUCLEUS_t *const nuclei = tools->nuclei_record[four_cells];
  const int nuc_ct = tools->nuc_ct_rec[four_cells];
  const int tp_number = movie->tp_number;

  NUCLEUS_t *nuclei_next = NULL;
  int i;
  int lin_ct = *lineage_ct_p;

  if (AlignDiamond (movie, logfile, nuclei, nuc_ct, four_cells) == 0)
    return 0;
  if (FourCellIDAssignment (movie, tools, parameters, logfile, four_cells) ==
      0)
    return 0;

  if (four_cells < tp_number - 1)
  {
    nuclei_next = tools->nuclei_record[four_cells + 1];
  }

  for (i = 0; i < nuc_ct; ++i)
  {
    if (strcmp (nuclei[i].identity, "polar") == 0)
      continue;
    if (nuclei[i].pflag == 0)
      ++lin_ct;
    if (nuclei[i].sflag2 != 0)
    {
      assert (nuclei_next != NULL);
      SisterID (movie, parameters, nuclei_next + nuclei[i].successor1,
                nuclei_next + nuclei[i].successor2, nuc_ct);
    }
  }

  *lineage_ct_p = lin_ct;
  return 1;
}



static void
NewBornID (const PARAMETER_t * const parameters, FILE * const logfile,
           const NUCLEUS_t * const mother, NUCLEUS_t * const restrict dau1,
           NUCLEUS_t * const restrict dau2)
{
  if (strcmp (mother->identity, "polar") == 0)
  {
    (void) fprintf (logfile, "Dividing polar body.\n");
  }

  if (strcmp (mother->identity, "ABa") == 0)
  {
    if (isless ((dau1->z - dau2->z) * parameters->lr, 0.) != 0)
    {
      dau1->id_tag = 'l';
      dau2->id_tag = 'r';
    }
    else
    {
      dau1->id_tag = 'r';
      dau2->id_tag = 'l';
    }
  }

  else if (strcmp (mother->identity, "ABp") == 0)
  {
    if (isless ((dau1->z - dau2->z) * parameters->lr, 0.) != 0)
    {
      dau1->id_tag = 'l';
      dau2->id_tag = 'r';
    }
    else
    {
      dau1->id_tag = 'r';
      dau2->id_tag = 'l';
    }
  }

  else if (strcmp (mother->identity, "EMS") == 0)
  {
    if (dau1->id_tag == 'a')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "MS");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "E");
      return;
    }
    else if (dau1->id_tag == 'p')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "E");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "MS");
      return;
    }
  }

  else if (strcmp (mother->identity, "P2") == 0)
  {
    if ((dau1->y - dau2->y) * parameters->dv + dau1->size / 2 < 0)
    {
      dau1->id_tag = 'd';
      dau2->id_tag = 'v';
    }
    else if ((dau1->y - dau2->y) * parameters->dv > dau1->size / 2)
    {
      dau1->id_tag = 'v';
      dau2->id_tag = 'd';
    }

    if (dau1->id_tag == 'd')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "C");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "P3");
      return;
    }
    else if (dau1->id_tag == 'v')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "P3");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "C");
      return;
    }
  }

  else if (strcmp (mother->identity, "P3") == 0)
  {
    if ((dau1->y - dau2->y) * parameters->dv < 0 - dau1->size / 2)
    {
      dau1->id_tag = 'd';
      dau2->id_tag = 'v';
    }
    else if ((dau1->y - dau2->y) * parameters->dv > dau1->size / 2)
    {
      dau1->id_tag = 'v';
      dau2->id_tag = 'd';
    }

    if (dau1->id_tag == 'd')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "D");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "P4");
      return;
    }
    else if (dau1->id_tag == 'v')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "P4");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "D");
      return;
    }
  }

  else if (strcmp (mother->identity, "P4") == 0)
  {
    if (dau1->id_tag == 'p')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "Z2");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "Z3");
      return;
    }
    else if (dau1->id_tag == 'a')
    {
      (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "Z3");
      (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "Z2");
      return;
    }
  }

  (void) snprintf (dau1->identity, NUCLEUS_IDENTITY_SIZE, "%s%c",
                   mother->identity, dau1->id_tag);
  (void) snprintf (dau2->identity, NUCLEUS_IDENTITY_SIZE, "%s%c",
                   mother->identity, dau2->id_tag);
}


static int
BackAssignment (const MOVIE_t * const movie, const TOOL_t * const tools,
                const PARAMETER_t * const parameters, FILE * logfile,
                const int four_cells, int *const lineage_ct_p)
{
  int i;

  int lin_ct = *lineage_ct_p;

  for (i = four_cells - 1; i >= 0; --i)
  {
    NUCLEUS_t *const nuclei = tools->nuclei_record[i];
    NUCLEUS_t *const nuclei_next = tools->nuclei_record[i + 1];

    const int nuc_ct = tools->nuc_ct_rec[i];
    int j;

    for (j = 0; j < nuc_ct; ++j)
    {
      if (strcmp (nuclei[j].identity, "polar") == 0)
      {
        continue;
      }

      if (nuclei[j].sflag1 != 0)
      {
        const NUCLEUS_t *const suc1 = nuclei_next + nuclei[j].successor1;

        if (nuclei[j].sflag2 == 0)
        {
          memcpy (nuclei[j].identity, suc1->identity, sizeof (char) * 50);
        }
        else
        {
          const NUCLEUS_t *const suc2 = nuclei_next + nuclei[j].successor2;

          if (strcmp (suc1->identity, "P2") == 0
              || strcmp (suc1->identity, "EMS") == 0)
          {
            if (strcmp (suc2->identity, "P2") == 0
                || strcmp (suc2->identity, "EMS") == 0)
            {
              (void) snprintf (nuclei[j].identity, NUCLEUS_IDENTITY_SIZE,
                               "P1");
            }
            else
            {
              (void) fprintf (logfile,
                              "%s and %s are considered sisters.\n",
                              suc1->identity, suc2->identity);
              return 0;
            }
          }
          else if (strcmp (suc1->identity, "ABa") == 0
                   || strcmp (suc1->identity, "ABp") == 0)
          {
            if (strcmp (suc2->identity, "ABa") == 0
                || strcmp (suc2->identity, "ABp") == 0)
            {
              (void) snprintf (nuclei[j].identity, NUCLEUS_IDENTITY_SIZE,
                               "AB");
            }
            else
            {
              (void) fprintf (logfile,
                              "%s and %s are considered sisters.\n",
                              suc1->identity, suc2->identity);
              return 0;
            }
          }
          else if (strcmp (suc1->identity, "AB") == 0
                   || strcmp (suc1->identity, "P1") == 0)
          {
            if (strcmp (suc1->identity, "AB") == 0
                || strcmp (suc1->identity, "P1") == 0)
            {
              (void) snprintf (nuclei[j].identity, NUCLEUS_IDENTITY_SIZE,
                               "P0");
            }
            else
            {
              (void) fprintf (logfile,
                              "%s and %s are considered sisters.\n",
                              suc1->identity, suc2->identity);
              return 0;
            }
          }
          else
          {
            (void) fprintf (logfile, "%s and %s are considered sisters.\n",
                            suc1->identity, suc2->identity);
            return 0;
          }
        }
      }
    }
  }

  {
    NUCLEUS_t *const nuclei = tools->nuclei_record[0];
    const int nuc_ct = tools->nuc_ct_rec[0];

    for (i = 0; i < nuc_ct; ++i)
    {
      if (nuclei[i].status == 0)
        continue;

      if (strcmp (nuclei[i].identity, "polar") == 0)
        continue;

      ++lin_ct;

      if (nuclei[i].identity == NULL)
      {
        (void) snprintf (nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
                         lin_ct);
      }
    }
  }


  for (i = 1; i < four_cells; i++)
  {
    NUCLEUS_t *const nuclei = tools->nuclei_record[i];
    NUCLEUS_t *const nuclei_prev = tools->nuclei_record[i - 1];
    NUCLEUS_t *const nuclei_next = tools->nuclei_record[i + 1];

    const int nuc_ct = tools->nuc_ct_rec[i];
    int j;

    for (j = 0; j < nuc_ct; j++)
    {
      if (nuclei[j].status == 0)
        continue;

      if (strcmp (nuclei[j].identity, "polar") == 0)
        continue;

      if (nuclei[j].pflag == 0)
      {
        ++lin_ct;
        (void) snprintf (nuclei[j].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
                         lin_ct);
      }
      else if (nuclei[j].identity == NULL)
      {
        const NUCLEUS_t *const pred = nuclei_prev + nuclei[j].predecessor;
        if (pred->sflag2 == 0)
        {
          memcpy (nuclei[j].identity, pred->identity, sizeof (char) * 50);
        }
        else
        {
          NewBornID (parameters, logfile, pred, nuclei + pred->successor1,
                     nuclei + pred->successor2);
        }
      }

      if (nuclei[j].sflag2 != 0)
      {
        SisterID (movie, parameters, nuclei_next + nuclei[j].successor1,
                  nuclei_next + nuclei[j].successor2, nuc_ct);
      }
    }
  }

  *lineage_ct_p = lin_ct;

  return 1;
}


static int
CountCells (const NUCLEUS_t * const nuclei, const int nuc_ct)
{
  int i;
  int cell_ct = 0;

  for (i = 0; i < nuc_ct; i++)
  {
    if (nuclei[i].status != 0 && strcmp (nuclei[i].identity, "polar") != 0)
      cell_ct++;
  }

  return cell_ct;
}



static void
PolarBodies (const MOVIE_t * const movie, const TOOL_t * const tools,
             const PARAMETER_t * const parameters, FILE * logfile, int tx,
             int *polar_ct_p)
{
  NUCLEUS_t *nuclei, *nuclei_next;
  int i, nuc_ct, polar_ct = 0;
  int t, tp_number = movie->tp_number - 1;
  int suc;

  nuclei = tools->nuclei_record[tx];
  nuc_ct = tools->nuc_ct_rec[tx];

  for (i = 0; i < nuc_ct; i++)
  {
    if (nuclei[i].status == 0)
      continue;
    if (strcmp (nuclei[i].identity, "polar") == 0)
      continue;
    if (nuclei[i].size < parameters->polar_size)
    {
      (void) snprintf (nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "polar");
      polar_ct++;
    }
  }
  *polar_ct_p = *polar_ct_p + polar_ct;

  if (polar_ct == 0)
    return;

  for (t = tx; t < tp_number; t++)
  {
    nuclei = tools->nuclei_record[t];
    nuc_ct = tools->nuc_ct_rec[t];
    nuclei_next = tools->nuclei_record[t + 1];
    for (i = 0; i < nuc_ct; i++)
    {
      if (strcmp (nuclei[i].identity, "polar") != 0)
        continue;
      if (nuclei[i].sflag1 == 0)
        polar_ct--;
      if (polar_ct == 0)
        break;
      if (nuclei[i].sflag2 != 0)
      {
        (void) fprintf (logfile,
                        "Polar body divided: %d:%d -> %d:%d and %d:%d\n",
                        t + 1, i + 1, t + 2, nuclei[i].successor1 + 1,
                        t + 2, nuclei[i].successor2 + 1);
      }
      else
      {
        suc = nuclei[i].successor1;
        (void) snprintf (nuclei_next[suc].identity, NUCLEUS_IDENTITY_SIZE,
                         "polar");
      }
    }
    if (polar_ct == 0)
      break;
  }
}



static int
InitialID (const MOVIE_t * const movie, const TOOL_t * const tools,
           PARAMETER_t * const parameters, FILE * logfile,
           int *const start_p, int *const lineage_ct_p)
{
  NUCLEUS_t *nuclei;
  int nuc_ct;
  int cell_ct;
  int polar_ct = 0;
  int t, i, j;
  int first_four = -1, last_four = -1, four_cells;
  int lin_ct = *lineage_ct_p;

  int tp_number = movie->tp_number;

  nuclei = tools->nuclei_record[0];
  nuc_ct = tools->nuc_ct_rec[0];
  cell_ct = CountCells (nuclei, nuc_ct);

  if (cell_ct <= 6)
  {
    PolarBodies (movie, tools, parameters, logfile, 0, &polar_ct);
    cell_ct = CountCells (nuclei, nuc_ct);
  }

  if (cell_ct > 4)
  {
    for (i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].status == 0)
        continue;
      if (strcmp (nuclei[i].identity, "polar") == 0)
        continue;

      lin_ct++;
      (void) snprintf (nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
                       lin_ct);
    }
    parameters->axis = 0;
    *start_p = 0;
    (void) fprintf (logfile,
                    "Starting with more than 4 cells.  No canonical ID assigned.\n\n");
    *lineage_ct_p = lin_ct;
    return 1;
  }

  parameters->axis = 1;
  for (t = 0; t < tp_number; t++)
  {
    nuclei = tools->nuclei_record[t];
    nuc_ct = tools->nuc_ct_rec[t];
    cell_ct = CountCells (nuclei, nuc_ct);
    if (cell_ct > 4)
      break;
    if (cell_ct == 4)
    {
      if (first_four == -1)
        first_four = t;
      last_four = t;
    }
  }

  if (first_four == -1)
  {
    nuclei = tools->nuclei_record[0];
    nuc_ct = tools->nuc_ct_rec[0];
    for (i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].status == 0)
        continue;
      if (strcmp (nuclei[i].identity, "polar") == 0)
        continue;
      lin_ct++;
      (void) snprintf (nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
                       lin_ct);
    }
    parameters->axis = 0;
    *start_p = 0;
    *lineage_ct_p = lin_ct;
    (void) fprintf (logfile, "Movie too short to see four cells; t = %d\n",
                    parameters->t);
    return 1;
  }

  four_cells = (first_four + last_four) / 2;
  *start_p = four_cells;

  if (FourCellID (movie, tools, parameters, logfile, four_cells, lineage_ct_p)
      == 0
      || BackAssignment (movie, tools, parameters, logfile, four_cells,
                         lineage_ct_p) == 0)
  {
    for (j = 0; j <= four_cells; j++)
    {
      nuclei = tools->nuclei_record[j];
      nuc_ct = tools->nuc_ct_rec[j];
      for (i = 0; i < nuc_ct; i++)
      {
        if (nuclei[i].status != 0)
        {
          nuclei[i].identity[0] = '\0';
        }
      }
    }
    nuclei = tools->nuclei_record[0];
    nuc_ct = tools->nuc_ct_rec[0];
    for (i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].status == 0)
        continue;
      if (strcmp (nuclei[i].identity, "polar") == 0)
        continue;
      lin_ct++;
      (void) snprintf (nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
                       lin_ct);
    }
    parameters->axis = 0;
    *start_p = 0;
    *lineage_ct_p = lin_ct;

    return 1;
  }

  return 0;
}



void
IdentityAssignment (const MOVIE_t * const movie, const TOOL_t * const tools,
                    PARAMETER_t * const parameters, FILE * logfile,
                    int *const lineage_ct_p)
{
  int t;
  int start;
  int lin_ct;
  const int tp_number = movie->tp_number;
  int rotate_axis = 1;

  /* initiation */
  if (InitialID (movie, tools, parameters, logfile, &start, lineage_ct_p)
      != 0)
  {
    (void) fprintf (logfile, "InitialID failed");
  }

  lin_ct = *lineage_ct_p;
  for (t = start; t < tp_number; t++)
  {
    NUCLEUS_t *const nuclei = tools->nuclei_record[t];
    NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
    const int nuc_ct = tools->nuc_ct_rec[t];
    int i;

    if (rotate_axis != 0 && nuc_ct > 40)
    {
      RotateAxis (parameters);
      rotate_axis = 0;
    }

    for (i = 0; i < nuc_ct; i++)
    {
      if (nuclei[i].status == 0)
        continue;

      if (nuclei[i].identity[0] == '\0')
      {
        if (nuclei[i].pflag == 0)
        {
          lin_ct++;
          (void) snprintf (nuclei[i].identity, NUCLEUS_IDENTITY_SIZE, "Nuc%d",
                           lin_ct);
        }
        else
        {
          NUCLEUS_t *const pred = nuclei_prev + nuclei[i].predecessor;
          if (nuclei[i].id_tag == '\0')
          {
            memcpy (nuclei[i].identity, pred->identity, sizeof (char) * 50);
          }
          else
            NewBornID (parameters, logfile, pred, nuclei + pred->successor1,
                       nuclei + pred->successor2);
        }
      }

      if (nuclei[i].sflag2 != 0)
      {
        NUCLEUS_t *const nuclei_next = tools->nuclei_record[t + 1];
        SisterID (movie, parameters, nuclei_next + nuclei[i].successor1,
                  nuclei_next + nuclei[i].successor2, nuc_ct);
      }
    }

    /* keep track of rotation */
  }
  *lineage_ct_p = lin_ct;
}
