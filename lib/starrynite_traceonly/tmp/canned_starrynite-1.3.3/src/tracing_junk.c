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
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "tracing.h"
#include "nuclei.h"
#include "types.h"
#include "utility.h"
#include "parameters-tools.h"
#include "error-codes.h"

/* BEGIN FUNCTION PROTOTYPES */

/* These functions are used to map potential matches: */

static void MeasureNuclearDistances (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static int compare_distance (
  const void *a,
  const void *b
);

static void SortPartners (
  const TOOL_t *,
  int t
);

static void AddLink (
  FILE *,
  PARTNER_t * partners,
  int index,
  int nuclei_ct
);

static void RemoveLink (
  FILE *,
  PARTNER_t * partners,
  int index,
  int nuclei_ct
);

static void LinkNearestPredecessor (
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static void TentativeBranch (
  const TOOL_t *,
  int t
);

static int Encroached (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t,
  int pred,
  int index
);

static void AddAmbiguousPredecessors (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  const int t,
  const int index
);

static void AddAmbiguousSuccessors (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  const int t,
  const int index
);

static int EvaluateDivision (
  const TOOL_t *,
  FILE *,
  int t,
  int m_index,
  int d1_index,
  int d2_index
);

static void EvaluateLinks (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static void IntroduceAmbiguity (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static int compare_morphology (
  const void *a,
  const void *b
);

static void UpdateCounts (
  const PARAMETER_t *,
  const TOOL_t *,
  int t,
  int index
);

static void ReduceAmbiguity (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static void ResetBranch (
  const TOOL_t *,
  int t
);

/* These functions are used to record unambiguous matches: */

static void CountUnambiguousMatches (
  const PARAMETER_t *,
  const TOOL_t *,
  int t
);

static void ReportIrregularity (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static void SkipComplexity (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static void ExtendBranch (
  const TOOL_t *,
  int t,
  int p_index,
  int s_index
);

static void Bifurcation_1 (
  const TOOL_t *,
  int t,
  int p_index,
  int s_index
);

static void RecordDivision (
  const TOOL_t *,
  int t,
  int m_index,
  int d_index2
);

static void Bifurcation_2 (
  const MOVIE_t *,
  const TOOL_t *,
  FILE *,
  int t,
  int p_index,
  int s_index
);

static void UnambiguousMatch (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

/* These functions are used to resolve ambiguous matches: */

static void ForwardBranchInfo (
  const TOOL_t *,
  int t
);

static void BackwardBranchInfo (
  const TOOL_t *,
  int t
);

static void UpdateBranchInfo (
  const TOOL_t *,
  int t
);

static int ScoringExtension (
  const NUCLEUS_t * const pred,
  const NUCLEUS_t * const suc
);

static int ScoringDivision (
  FILE *,
  const NUCLEUS_t * const mother,
  const NUCLEUS_t * const dau1,
  const NUCLEUS_t * const dau2
);

static int ScoringDeath (
  const NUCLEUS_t * const nuc
);

static void CalculateMorphologyScore (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t,
  int p_index,
  int s_index,
  PARTNER_t * par
);

static void MorphologyScores (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static int ScoringExtensionTiming (
  const int t,
  const NUCLEUS_t * const pred,
  const NUCLEUS_t * const suc
);

static int ScoringDivisionTiming (
  const PARAMETER_t *,
  const int t,
  const NUCLEUS_t * const mother,
  const NUCLEUS_t * const dau1,
  const NUCLEUS_t * const dau2
);

static void CalculateTimingScore (
  const PARAMETER_t *,
  const TOOL_t *,
  int t,
  int p_index,
  int s_index,
  PARTNER_t * par
);

static void TimingScores (
  const PARAMETER_t *,
  const TOOL_t *,
  int t
);

static void ResolveAmbiguity (
  const MOVIE_t *,
  const PARAMETER_t *,
  const TOOL_t *,
  FILE *,
  int t
);

/* These functions are used to resolve false positive/negative nuclei: */

static void AddGhostNucleus (
  const TOOL_t *,
  int t,
  NUCLEUS_t * nuc,
  int pred
);

static void RemoveCell (
  const MOVIE_t *,
  const TOOL_t *,
  FILE *,
  int t,
  int index,
  int *err_ct_p
);

static int MissingCells (
  const MOVIE_t *,
  const TOOL_t *,
  FILE *,
  int t
);

static void ResetNuclei (
  const TOOL_t *,
  int t,
  const MOVIE_t * const movie
);

/* END FUNCTION PROTOTYPES */

static void
MeasureNuclearDistances (const MOVIE_t * const movie,
                         const PARAMETER_t * const parameters,
                         const TOOL_t * const tools,
                         FILE * const logfile, const int t)
{
  /* TODO: modify s.t. there is a single return call */

  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  if (!nuc_ct || !nuc_ct_prev)
    return;

  /* ZB 200908, the partner arrays are now pre-allocated and not freed 
  for (i = 0; i < nuc_ct_prev; i++)
  {
    nuclei_prev[i].s_partners =
      (PARTNER_t *) malloc_exit (sizeof (PARTNER_t) * nuc_ct);
  }

  for (i = 0; i < nuc_ct; i++)
  {
    nuclei[i].p_partners =
      (PARTNER_t *) malloc_exit (sizeof (PARTNER_t) * nuc_ct_prev);
  }
  */

  for (i = 0; i < nuc_ct_prev; i++)
  {
    int j;
    PARTNER_t *const partners1 = nuclei_prev[i].s_partners;
    for (j = 0; j < nuc_ct; j++)
    {
      PARTNER_t *const partners2 = nuclei[j].p_partners;
      float distance;
      if (!nuclei_prev[i].status || !nuclei[j].status)
      {
        distance = movie->max_distance;
      }
      else
      {
        distance =
          NucDistance (parameters, nuclei_prev + i, nuclei + j,
                       &movie->frame);
      }
      partners1[j].index = j;
      partners1[j].distance = distance;
      partners1[j].availability = 0;
      partners2[i].index = i;
      partners2[i].distance = distance;
      partners2[i].availability = 0;
    }
  }
}

static int
compare_distance (const void *a, const void *b)
{
  const PARTNER_t *const par1 = a;
  const PARTNER_t *const par2 = b;

  int val;

  if (par1->distance < par2->distance)
  {
    val = -1;
  }
  else if (par1->distance > par2->distance)
  {
    val = 1;
  }
  else
  {
    val = par1->index - par2->index;
  }

  return val;
}



static void
SortPartners (const TOOL_t * const tools, const int t)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (!nuclei_prev[i].s_partners)
      continue;
    qsort (nuclei_prev[i].s_partners, nuc_ct, sizeof (PARTNER_t),
           compare_distance);
  }

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    if (!nuclei[i].p_partners)
      continue;
    qsort (nuclei[i].p_partners, nuc_ct_prev, sizeof (PARTNER_t),
           compare_distance);
  }
}



static void
AddLink (FILE * const logfile, PARTNER_t * const partners, const int index,
         const int nuclei_ct)
{
  int i;

  /* TODO: replace call to exit() with a return value != 0 */

  /* post-condition: .availability = 1 for the first element
   * in partners whose .index = index */
  for (i = 0; i < nuclei_ct; i++)
  {
    if (partners[i].index != index)
      continue;
    if (partners[i].availability != 0)
    {
      fprintf (logfile, "Wrong label for links.\n");
      exit (ERROR_ADDLINK);
    }
    partners[i].availability = 1;
    break;
  }

}


static void
RemoveLink (FILE * const logfile, PARTNER_t * const partners, const int index,
            const int nuclei_ct)
{
  int i;

  /* TODO: replace call to exit() with a return value != 0 */

  /* post-condition: .availability = 0 for the first element
   * in partners whose .index = index */
  for (i = 0; i < nuclei_ct; i++)
  {
    if (partners[i].index != index)
      continue;
    if (partners[i].availability == 0)
    {
      fprintf (logfile, "Wrong label for links.\n");
      exit (ERROR_REMOVELINK);
    }
    partners[i].availability = 0;
    break;
  }
}



static void
LinkNearestPredecessor (const PARAMETER_t * const parameters,
                        const TOOL_t * const tools,
                        FILE * const logfile, const int t)
{
  int i;

  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct; i++)
  {
    if (nuclei[i].status == 0)
      continue;

    PARTNER_t *const partners = nuclei[i].p_partners;
    if (partners == NULL)
      continue;
    if (partners->distance > cutoff)
      continue;
    partners->availability = 1;

    const int pred = partners->index;
    nuclei[i].p_ct = 1;
    nuclei[i].predecessor = pred;
    nuclei[i].pdist = partners->distance;
    nuclei_prev[pred].s_ct++;
    AddLink (logfile, nuclei_prev[pred].s_partners, i, nuc_ct);
    if (nuclei_prev[pred].s_ct > 2)
      continue;
    if (nuclei_prev[pred].successor1 == -1)
    {
      nuclei_prev[pred].successor1 = i;
      nuclei_prev[pred].sdist1 = partners->distance;
    }
    else
    {
      if (nuclei_prev[pred].sdist1 <= partners->distance)
      {
        nuclei_prev[pred].successor2 = i;
        nuclei_prev[pred].sdist2 = partners->distance;
      }
      else
      {
        nuclei_prev[pred].successor2 = nuclei_prev[pred].successor1;
        nuclei_prev[pred].sdist2 = nuclei_prev[pred].sdist1;
        nuclei_prev[pred].successor1 = i;
        nuclei_prev[pred].sdist1 = partners->distance;
      }
    }
  }
}


static void
TentativeBranch (const TOOL_t * const tools, const int t)
{
  int i;

  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    if (!nuclei[i].p_ct)
      continue;

    const int pred = nuclei[i].predecessor;

    if (nuclei_prev[pred].s_ct != 1)
      continue;
    nuclei_prev[pred].end_time = nuclei[i].end_time;
    nuclei[i].birthday = nuclei_prev[pred].birthday;
    if (nuclei[i].max_size < nuclei_prev[pred].max_size)
    {
      nuclei[i].max_size = nuclei_prev[pred].max_size;
      nuclei[i].max_size_time = nuclei_prev[pred].max_size_time;
    }
    if (nuclei[i].max_weight < nuclei_prev[pred].max_weight)
    {
      nuclei[i].max_weight = nuclei_prev[pred].max_weight;
      nuclei[i].max_weight_time = nuclei_prev[pred].max_weight_time;
    }
  }
}


static int
Encroached (const MOVIE_t * const movie, const PARAMETER_t * const parameters,
            const TOOL_t * const tools, FILE * const logfile, const int t,
            const int pred, const int index)
{
  /* TODO: Modify s.t. there is a single return call */
  int i;

  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;
  PARTNER_t *const partners = nuclei_prev[pred].s_partners;

  const float dist1 =
    NucDistance (parameters, nuclei_prev + pred, nuclei + index,
                 &movie->frame);

  for (i = 0; i < nuc_ct; i++)
  {
    if (partners[i].index == index)
    {
      if (i > 2)
      {
        fprintf (logfile, "%d th suc to %d:%d (%.2f), ", i + 1,
                 movie->time_start - 1 + t, pred + 1, dist1);
        return 1;
      }
      break;
    }
  }

  for (i = 0; i < nuc_ct; i++)
  {
    if (partners[i].distance > cutoff)
      break;

    const int suc = partners[i].index;
    if (suc == index)
      break;

    const float dist2 =
      NucDistance (parameters, nuclei_prev + pred, nuclei + suc,
                   &movie->frame);
    const float dist3 =
      NucDistance (parameters, nuclei + index, nuclei + suc, &movie->frame);
    /*if (dist1 + dist2 > 1.3 * dist3) { */
    if (dist3 < dist1 && dist2 + dist3 < 2.0 * dist1)
    {
      fprintf (logfile, " %d:%d (%.2f) encroached by %d:%d (%.2f), ",
               movie->time_start - 1 + t, pred + 1, dist1,
               movie->time_start - 1 + t + 1, suc + 1, dist2);
      return 1;
    }
  }

  return 0;
}



static void
AddAmbiguousPredecessors (const MOVIE_t * const movie,
                          const PARAMETER_t * const parameters,
                          const TOOL_t * const tools, FILE * const logfile,
                          const int t, const int index)
{

  /* TODO: modify s.t. there is a single return call */
  int k;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  float cutoff, cutoff1;
  PARTNER_t *partners;
  int pred;
  int new_ct = 0;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  pred = nuclei[index].predecessor;
  fprintf (logfile, "Adding pred for %d:%d (%.2f to %d:%d), ",
           movie->time_start - 1 + t + 1, index + 1, nuclei[index].pdist,
           movie->time_start - 1 + t, pred + 1);
  cutoff1 = 0.0;
  partners = nuclei[index].p_partners;
  for (k = 0; k < nuc_ct_prev; k++)
  {
    if (partners[k].distance > cutoff)
      break;
    if (partners[k].index != nuclei[index].predecessor)
    {
      cutoff1 = parameters->ambiguity_cutoff * partners[k].distance;
      break;
    }
  }
  if (!cutoff1)
  {
    fprintf (logfile, "nothing close enough\n");
    return;
  }
  for (k = 0; k < nuc_ct_prev; k++)
  {
    if (partners[k].distance > cutoff)
      break;
    if (!partners[k].availability)
    {
      if (partners[k].distance > cutoff1)
        break;
      new_ct++;
      if (new_ct == 3)
        break;
      pred = partners[k].index;
      if (Encroached (movie, parameters, tools, logfile, t, pred, index))
        continue;
      fprintf (logfile, "adding %d:%d (%.2f), ", movie->time_start - 1 + t,
               pred + 1, partners[k].distance);
      nuclei[index].p_ct++;
      partners[k].availability = 1;
      nuclei_prev[pred].s_ct++;
      AddLink (logfile, nuclei_prev[pred].s_partners, index, nuc_ct);
    }
  }
  fprintf (logfile, "\n");
}



static void
AddAmbiguousSuccessors (const MOVIE_t * const movie,
                        const PARAMETER_t * const parameters,
                        const TOOL_t * const tools, FILE * const logfile,
                        const int t, const int index)
{
  /* TODO: modify s.t. there is a single return call */
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];
  PARTNER_t *const partners = nuclei_prev[index].s_partners;

  const float cutoff = 2.0 * parameters->nuc_size;
  float cutoff1 = -1.;

  fprintf (logfile, "Adding suc for %d:%d", movie->time_start - 1 + t,
           index + 1);
  for (i = 0; i < nuc_ct; i++)
  {
    if (partners[i].distance > cutoff)
      break;
    if (partners[i].index != nuclei_prev[index].successor1
        && partners[i].index != nuclei_prev[index].successor2)
    {
      cutoff1 = parameters->ambiguity_cutoff * partners[i].distance;
      assert (cutoff1 > 0.);
      break;
    }
  }

  if (cutoff1 < 0.)
  {
    fprintf (logfile, " nothing close enough\n");
    return;
  }

  for (i = 0; i < nuc_ct; i++)
  {
    if (partners[i].distance > cutoff)
      break;
    if (partners[i].availability == 0)
    {
      const int suc = partners[i].index;
      if (partners[i].distance > cutoff1)
        break;
      partners[i].availability = 1;
      fprintf (logfile, ", adding %d:%d", movie->time_start - 1 + t + 1,
               suc + 1);
      nuclei_prev[index].s_ct++;
      nuclei[suc].p_ct++;
      AddLink (logfile, nuclei[suc].p_partners, index, nuc_ct_prev);
    }
  }
  fprintf (logfile, "\n");
}


static int
EvaluateDivision (const TOOL_t * const tools, FILE * const logfile, int t,
                  int m_index, int d1_index, int d2_index)
{
  /* TODO: modify s.t. there is a single return call */
  NUCLEUS_t *nuclei_prev, *nuclei;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *mother, *dau1, *dau2;

  int score = 0;
  int ct = 0, ct2 = 0, ct3 = 0;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  mother = nuclei_prev + m_index;
  dau1 = nuclei + d1_index;
  dau2 = nuclei + d2_index;

  if (mother->condensed == 3)
  {
    score += 2;
    fprintf (logfile, " 1");
  }
  else
    fprintf (logfile, " 0");

  if (dau1->condensed == 3 || mother->max_size - dau1->size >= 4)
  {
    ct++;
    score += 2;
    fprintf (logfile, " 1");
  }
  else
  {
    fprintf (logfile, " 0");
  }
  if (1.0 * (mother->max_weight - dau2->weight) / mother->max_weight > 0.3)
  {
    ct++;
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    fprintf (logfile, " 0");
  }

  if (dau2->condensed == 3 || mother->max_size - dau2->size >= 4)
  {
    ct2++;
    score += 2;
    fprintf (logfile, " 1");
  }
  else
  {
    fprintf (logfile, " 0");
  }

  {
    float x;

    x = 1.0 * abs (dau1->weight - dau2->weight) / dau2->weight;

    if (x < 0.1)
    {
      ct2++;
      score += 1;
      fprintf (logfile, " 1");
    }
    else
    {
      fprintf (logfile, " 0");
    }

  }
  if (abs (dau1->size - dau2->size) <= 2)
  {
    ct3++;
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    fprintf (logfile, " 0");
  }
  if (1.0 * (mother->max_weight - dau1->weight) / mother->max_weight > 0.3)
  {
    ct3++;
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    fprintf (logfile, " 0");
  }

  if (!ct || !ct2)
    return 0;
  if (nuc_ct > 100)
  {
    if (!ct3)
      return 0;
  }
  else
  {
    score += 2;
  }

  return score;
}


static void
EvaluateLinks (const MOVIE_t * const movie,
               const PARAMETER_t * const parameters,
               const TOOL_t * const tools, FILE * const logfile, const int t)
{
  int i, k;
  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  const NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];
  int dau1, dau2;
  int suc;
  int score;
  PARTNER_t *partners;

  /* TODO: replace call to exit() with a return value != 0 */

  const float cutoff = 2.0 * parameters->nuc_size;

  /* evaluate divisions */
  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (nuclei_prev[i].s_ct < 2)
      continue;
    if (nuclei_prev[i].s_ct > 2)
    {
      fprintf (logfile, "Breaking multiple hard suc's for %d:%d",
               movie->time_start - 1 + t, i + 1);
      partners = nuclei_prev[i].s_partners;
      for (k = 0; k < nuc_ct; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (partners[k].availability)
        {
          suc = partners[k].index;
          nuclei[suc].p_ambiguity = 1;
          fprintf (logfile, ", %d:%d", movie->time_start - 1 + t + 1,
                   suc + 1);
        }
      }
      fprintf (logfile, "\n");
    }
    else
    {
      dau1 = nuclei_prev[i].successor1;
      dau2 = nuclei_prev[i].successor2;
      if (nuclei[dau1].p_ct > 1 || nuclei[dau2].p_ct > 1)
      {
        fprintf (logfile,
                 "Daughter cell already ambiguous for %d:%d.  Exiting.\n",
                 movie->time_start - 1 + t, i + 1);
        exit (ERROR_EVALUATELINKS);
      }
      fprintf (logfile,
               "Evaluating division: %2d:%-3d -> %2d:%-3d, %2d:%-3d,",
               movie->time_start - 1 + t, i + 1,
               movie->time_start - 1 + t + 1, dau1 + 1,
               movie->time_start - 1 + t + 1, dau2 + 1);

#if 0
      {
        fprintf (logfile, " %2d %2d",
                 t - 1 - (nuclei_prev + i)->max_weight_time,
                 t - 1 - (nuclei_prev + i)->max_size_time);
      }
#endif /* 0 */

      score = EvaluateDivision (tools, logfile, t, i, dau1, dau2);
      fprintf (logfile, "   %2d", score);
      if (score < 7)
      {
        fprintf (logfile, "    open");
        nuclei[dau1].p_ambiguity = 1;
        nuclei[dau2].p_ambiguity = 1;
      }
      fprintf (logfile, " \n");
    }
  }

#if 0
  /* evaluate missing successors, first or second */
  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (nuclei_prev[i].s_ct >= 2)
      continue;
    if (!nuclei_prev[i].s_ct)
    {
      /* apparent death */
      fprintf (logfile, "opening ambiguity for cell death: %d:%d\n",
               movie->time_start - 1 + t, i + 1);
      nuclei_prev[i].s_ambiguity = 1;
    }
    else
    {
      /* apparent non-dividing cell */
      partners = nuclei_prev[i].s_partners;
      for (k = 0; k < nuc_ct; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (partners[k].availability)
        {
          suc = partners[k].index;
          /* apparent division without second successor */
          if ((nuclei_prev[i].weight -
               nuclei[suc].weight) / nuclei_prev[i].weight > 0.3)
          {
            if (t - nuclei_prev[i].birthday > 3
                && nuclei_prev[i].end_time - t > 3)
            {
              fprintf (logfile,
                       "opening ambiguity for second daughter: %d:%d -> %d:%d\n",
                       movie->time_start - 1 + t, i + 1,
                       movie->time_start - 1 + t + 1, suc + 1);
              nuclei_prev[i].s_ambiguity = 1;
            }
          }
          break;
        }
      }
    }
  }
#endif
}


static void
IntroduceAmbiguity (const MOVIE_t * const movie,
                    const PARAMETER_t * const parameters,
                    const TOOL_t * const tools, FILE * const logfile,
                    const int t)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  const NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  const NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  for (i = 0; i < nuc_ct; i++)
  {
    if (!nuclei[i].status)
      continue;
    if (!nuclei[i].p_ambiguity)
      continue;
    if (nuclei[i].ghost)
      continue;
    if (nuclei[i].p_ct == 1)
      AddAmbiguousPredecessors (movie, parameters, tools, logfile, t, i);
    else
    {
      fprintf (logfile,
               "Daughter cell already ambiguous: %d:%d -> %d:%d.  Exiting.\n",
               movie->time_start - 1 + t, nuclei[i].predecessor + 1,
               movie->time_start - 1 + t + 1, i + 1);
      exit (ERROR_INTRODUCEAMBIGUITY);
    }
  }

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (nuclei_prev[i].s_ambiguity)
    {
      AddAmbiguousSuccessors (movie, parameters, tools, logfile, t, i);
    }
  }
}


static int
compare_morphology (const void *const a, const void *const b)
{
  /* TODO: modify s.t. there is a single return call */
  const PARTNER_t *const par1 = a;
  const PARTNER_t *const par2 = b;

  if (par1->availability)
  {
    if (!par2->availability)
      return -1;
    else
      return par2->score2m - par1->score2m;
  }
  else
  {
    if (par2->availability)
      return 1;
    else
      return -1;
  }
}


static void
UpdateCounts (const PARAMETER_t * const parameters,
              const TOOL_t * const tools, const int t, const int index)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;
  const PARTNER_t *const partners = nuclei[index].p_partners;

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (partners[i].distance > cutoff)
      break;
    if (partners[i].availability)
    {
      const int pred = partners[i].index;
      nuclei[index].predecessor = pred;
      nuclei[index].pdist = partners[i].distance;
      nuclei_prev[pred].s_ct2++;
      break;
    }
  }
}


static void
ReduceAmbiguity (const MOVIE_t * const movie,
                 const PARAMETER_t * const parameters,
                 const TOOL_t * const tools, FILE * const logfile,
                 const int t)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (nuclei_prev[i].status == 0)
      continue;
    if (nuclei_prev[i].s_ct2 == 2)
    {
      if (nuclei_prev[i].s_ct > 2)
      {
        int k;
        PARTNER_t *const partners = nuclei_prev[i].s_partners;
        fprintf (logfile, "Removing successors for %d:%d :",
                 movie->time_start - 1 + t, i + 1);
        for (k = 0; k < nuc_ct; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
          {
            int suc = partners[k].index;
            if (nuclei[suc].p_ct > 1)
            {
              fprintf (logfile, " %d:%d,",
                       movie->time_start - 1 + t + 1, suc + 1);
              nuclei_prev[i].s_ct--;
              partners[k].availability = 0;
              nuclei[suc].p_ct--;
              RemoveLink (logfile, nuclei[suc].p_partners, i, nuc_ct_prev);
              if (nuclei[suc].p_ct == 1)
                UpdateCounts (parameters, tools, t, suc);
            }
          }
        }
        fprintf (logfile, "\n");
      }
    }
    else if (nuclei_prev[i].s_ct2 == 1)
    {
      if (nuclei_prev[i].s_ct > 2)
      {
        int sis = -1;
        int k;
        PARTNER_t *const partners = nuclei_prev[i].s_partners;
        fprintf (logfile, "Removing successors for %d:%d :\n",
                 movie->time_start - 1 + t, i + 1);
        for (k = 0; k < nuc_ct; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
          {
            int suc = partners[k].index;
            if (nuclei[suc].p_ct != 1)
              continue;
            sis = suc;
            partners[k].score2m = -10000;
            break;
          }
        }
        assert (sis != -1);
        fprintf (logfile, "    hard suc %d:%d\n",
                 movie->time_start - 1 + t + 1, sis + 1);
        for (k = 0; k < nuc_ct; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
          {
            int suc = partners[k].index;
            if (nuclei[suc].p_ct == 1)
              continue;
            fprintf (logfile, "    scoring soft suc %d:%d, ",
                     movie->time_start - 1 + t + 1, suc + 1);
            partners[k].score2m =
              ScoringDivision (logfile, nuclei_prev + i, nuclei + sis,
                               nuclei + suc);
            fprintf (logfile, ", with score %d and distance %.2f\n",
                     partners[k].score2m, partners[k].distance);
          }
        }
        qsort (nuclei_prev[i].s_partners, nuc_ct,
               sizeof (PARTNER_t), compare_morphology);
        for (k = 0; k < nuc_ct; k++)
        {
          int suc = partners[k].index;
          if (!partners[k].availability)
            break;
          if (nuclei[suc].p_ct == 1)
            break;
          if (k < 1)
            continue;
          fprintf (logfile, "    %d:%d removed",
                   movie->time_start - 1 + t + 1, suc + 1);
          nuclei_prev[i].s_ct--;
          partners[k].availability = 0;
          nuclei[suc].p_ct--;
          RemoveLink (logfile, nuclei[suc].p_partners, i, nuc_ct_prev);
          if (nuclei[suc].p_ct == 1)
            UpdateCounts (parameters, tools, t, suc);
        }
        fprintf (logfile, "\n");
        qsort (nuclei_prev[i].s_partners, nuc_ct,
               sizeof (PARTNER_t), compare_distance);
      }
    }
    else if (nuclei_prev[i].s_ct2 == 0)
    {
      if (nuclei_prev[i].s_ct > 2)
      {
        PARTNER_t *const partners = nuclei_prev[i].s_partners;
        int s_ct = 0;
        int k;
        fprintf (logfile, "Removing successors for %d:%d :\n",
                 movie->time_start - 1 + t, i + 1);
        for (k = 0; k < nuc_ct; k++)
        {
          int suc = partners[k].index;
          if (partners[k].distance > cutoff)
            break;
          if (!partners[k].availability)
            continue;
          s_ct++;
          if (s_ct < 3)
            continue;
          fprintf (logfile, "    %d:%d removed",
                   movie->time_start - 1 + t + 1, suc + 1);
          nuclei_prev[i].s_ct--;
          partners[k].availability = 0;
          nuclei[suc].p_ct--;
          RemoveLink (logfile, nuclei[suc].p_partners, i, nuc_ct_prev);
          if (nuclei[suc].p_ct == 1)
            UpdateCounts (parameters, tools, t, suc);
        }
        fprintf (logfile, "\n");
      }
    }
  }
}

static void
ResetBranch (const TOOL_t * const tools, int t)
{
  NUCLEUS_t *nuclei, *nuc;
  int nuc_ct;
  int j;

  nuc_ct = tools->nuc_ct_rec[t];
  nuclei = tools->nuclei_record[t];

  for (j = 0; j < nuc_ct; j++)
  {
    nuc = nuclei + j;
    if (!nuc->status)
      continue;

    nuc->birthday = t;
    nuc->end_time = t;

    nuc->max_size = nuc->size;
    nuc->max_size_time = t;
    nuc->max_weight = nuc->weight;
    nuc->max_weight_time = t;

    nuc->p_ct2 = 0;
    nuc->s_ct2 = 0;
    nuc->predecessor = -1;
    nuc->successor1 = -1;
    nuc->successor2 = -1;
    nuc->pdist = -1.0;
    nuc->sdist1 = -1.0;
    nuc->sdist2 = -1.0;
    nuc->pflag = 0;
    nuc->sflag1 = 0;
    nuc->sflag2 = 0;
  }
}



/**************************************
 **                                  **
 **    RECORD UNAMBIGUOUS MATCHES    **
 **                                  **
 **************************************/


static void
CountUnambiguousMatches (const PARAMETER_t * const parameters,
                         const TOOL_t * const tools, int t)
{
  int i, j, k;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  int suc, pred;
  PARTNER_t *partners;
  float cutoff;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (!nuclei_prev[i].s_ct)
      continue;
    partners = nuclei_prev[i].s_partners;
    for (k = 0; k < nuc_ct; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (partners[k].availability)
      {
        suc = partners[k].index;
        if (nuclei[suc].p_ct == 1)
          nuclei_prev[i].s_ct2 = nuclei_prev[i].s_ct2 + 1;
      }
      if (nuclei_prev[i].s_ct2 == nuclei_prev[i].s_ct)
        break;
    }
  }

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (!nuclei[j].p_ct)
      continue;
    partners = nuclei[j].p_partners;
    for (k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (partners[k].availability)
      {
        pred = partners[k].index;
        if (nuclei_prev[pred].s_ct == 1)
          nuclei[j].p_ct2 = nuclei[j].p_ct2 + 1;
      }
      if (nuclei[j].p_ct2 == nuclei[j].p_ct)
        break;
    }
  }
}


static void
ReportIrregularity (const MOVIE_t * const movie,
                    const PARAMETER_t * const parameters,
                    const TOOL_t * const tools, FILE * const logfile, int t)
{
  /* 1->0, 0->1 or 1 -> >2 matches */
  int i, j, k;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  int suc;
  PARTNER_t *partners;
  float cutoff;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (nuclei_prev[i].s_ct == 0)
    {
      if (nuclei_prev[i].ghost < 2)
        fprintf (logfile, "Putative cell death (no successor), %d:%d\n",
                 movie->time_start - 1 + t, i + 1);
      continue;
    }
    if (nuclei_prev[i].s_ct > 2)
    {
      fprintf (logfile,
               "More than 2 successors (%d of %d hard), %d:%d -> ",
               nuclei_prev[i].s_ct2, nuclei_prev[i].s_ct,
               movie->time_start - 1 + t, i + 1);
      partners = nuclei_prev[i].s_partners;
      for (k = 0; k < nuc_ct; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (partners[k].availability)
        {
          suc = partners[k].index;
          fprintf (logfile, "%d:%d with ",
                   movie->time_start - 1 + t + 1, suc + 1);
          if (nuclei[suc].p_ct == 1)
            fprintf (logfile, "hard ");
          else
            fprintf (logfile, "soft ");
          fprintf (logfile, "distance %.2f; ", partners[k].distance);
        }
      }
      fprintf (logfile, "\n");
      continue;
    }
  }

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (!nuclei[j].p_ct)
    {
      fprintf (logfile,
               "Emerging cell (no candidate predecessor), %d:%d\n",
               movie->time_start - 1 + t + 1, j + 1);
      continue;
    }
#if 0
    if (nuclei[j].p_ct > 1)
    {
      fprintf (logfile, "Multiple predecessors, %d:%d <- ",
               movie->time_start - 1 + t + 1, j + 1);
      partners = nuclei[j].p_partners;
      for (k = 0; k < nuc_ct_prev; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (partners[k].availability)
        {
          fprintf (logfile, "%d:%d with distance %.2f; ",
                   movie->time_start - 1 + t,
                   partners[k].index + 1, partners[k].distance);
        }
      }
      fprintf (logfile, "\n");
      continue;
    }
#endif
  }
}


static void
SkipComplexity (const MOVIE_t * const movie,
                const PARAMETER_t * const parameters,
                const TOOL_t * const tools, FILE * const logfile, int t)
{
  /* mark and skip 1 -> >2 matches */
  int i, j, k;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  int suc, pred;
  PARTNER_t *partners;
  float cutoff;
  int newct;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (!nuclei_prev[i].sskip)
    {
      if (nuclei_prev[i].s_ct2 > 2
          || nuclei_prev[i].s_ct - nuclei_prev[i].s_ct2 > 2)
      {
        if (nuclei_prev[i].s_ct2 > 2)
          fprintf (logfile, "S_Skipping %d:%d, >2 hard successors\n",
                   movie->time_start - 1 + t, i + 1);
        else
          fprintf (logfile, "S_Skipping %d:%d, >2 soft successors\n",
                   movie->time_start - 1 + t, i + 1);
        nuclei_prev[i].sskip = 1;
      }
    }
    if (nuclei_prev[i].sskip)
    {
      fprintf (logfile, "P_Skipping based on %d:%d:",
               movie->time_start - 1 + t, i + 1);
      partners = nuclei_prev[i].s_partners;
      for (k = 0; k < nuc_ct; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (partners[k].availability)
        {
          suc = partners[k].index;
          nuclei[suc].pskip = 1;
          fprintf (logfile, " %d:%d,", movie->time_start - 1 + t + 1,
                   suc + 1);
        }
      }
      fprintf (logfile, "\n");
    }
  }

  newct = 1;
  while (newct)
  {
    newct = 0;
    for (j = 0; j < nuc_ct; j++)
    {
      if (!nuclei[j].status)
        continue;
      if (nuclei[j].pskip)
      {
        partners = nuclei[j].p_partners;
        for (k = 0; k < nuc_ct_prev; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
          {
            pred = partners[k].index;
            if (!nuclei_prev[pred].sskip)
            {
              newct++;
              nuclei_prev[pred].sskip = 1;
              fprintf (logfile, "S_skipping %d:%d due to %d:%d\n",
                       movie->time_start - 1 + t, pred + 1,
                       movie->time_start - 1 + t + 1, j + 1);
            }
          }
        }
      }
    }

    for (i = 0; i < nuc_ct_prev; i++)
    {
      if (!nuclei_prev[i].status)
        continue;
      if (nuclei_prev[i].sskip)
      {
        partners = nuclei_prev[i].s_partners;
        for (k = 0; k < nuc_ct; k++)
        {
          if (partners[k].distance > cutoff)
            break;
          if (partners[k].availability)
          {
            suc = partners[k].index;
            if (!nuclei[suc].pskip)
            {
              newct++;
              nuclei[suc].pskip = 1;
              fprintf (logfile, "P_skipping %d:%d due to %d:%d\n",
                       movie->time_start - 1 + t + 1, suc + 1,
                       movie->time_start - 1 + t, i + 1);
            }
          }
        }
      }
    }
  }
}


static void
ExtendBranch (const TOOL_t * const tools, int t, int p_index, int s_index)
{
  NUCLEUS_t *pred = tools->nuclei_record[t - 1] + p_index;
  NUCLEUS_t *suc = tools->nuclei_record[t] + s_index;

  pred->successor1 = s_index;
  pred->sdist1 = suc->pdist;
  pred->sflag1 = 1;
  if (!pred->condensed && suc->condensed)
  {
    pred->condensed = suc->condensed - 1;
  }

  suc->pflag = 1;
  suc->birthday = pred->birthday;
  suc->generation = pred->generation;
  if (!suc->condensed && pred->condensed)
  {
    suc->condensed = pred->condensed - 1;
  }

  if (suc->max_weight < pred->max_weight)
  {
    suc->max_weight = pred->max_weight;
    suc->max_weight_time = pred->max_weight_time;
  }
  if (suc->max_size < pred->max_size)
  {
    suc->max_size = pred->max_size;
    suc->max_size_time = pred->max_size_time;
  }
}


static void
Bifurcation_1 (const TOOL_t * const tools, int t, int p_index, int s_index)
{
  NUCLEUS_t *pred = tools->nuclei_record[t - 1] + p_index;
  NUCLEUS_t *suc = tools->nuclei_record[t] + s_index;

  pred->successor1 = s_index;
  pred->sdist1 = suc->pdist;
  pred->sflag1 = 1;
  if (!pred->condensed && suc->condensed)
  {
    pred->condensed = suc->condensed - 1;
  }

  suc->pflag = 1;
  if (!suc->condensed && pred->condensed)
  {
    suc->condensed = pred->condensed - 1;
  }
}


static void
RecordDivision (const TOOL_t * const tools, int t, int m_index, int d_index2)
{
  NUCLEUS_t *mother = tools->nuclei_record[t - 1] + m_index;
  NUCLEUS_t *daughter1 = tools->nuclei_record[t] + mother->successor1;
  NUCLEUS_t *daughter2 = tools->nuclei_record[t] + d_index2;

  mother->successor2 = d_index2;
  mother->sdist2 = daughter2->pdist;
  mother->sflag2 = 1;
  mother->end_time = t - 1;

  daughter2->pflag = 1;

  daughter2->generation = mother->generation + 1;
  daughter2->birthday = t;
  daughter1->generation = mother->generation + 1;
  daughter1->birthday = t;
}


static void
Bifurcation_2 (const MOVIE_t * const movie, const TOOL_t * const tools,
               FILE * const logfile, int t, int p_index, int s_index)
{
  NUCLEUS_t *pred = tools->nuclei_record[t - 1] + p_index;
  /*  NUCLEUS_t *suc= *(tools->nuclei_record+t) + s_index; */

  if (!pred->sflag1)
    Bifurcation_1 (tools, t, p_index, s_index);
  else
  {
    if (!pred->sflag2)
      RecordDivision (tools, t, p_index, s_index);
    else
    {
      pred->sskip = 1;
      fprintf (logfile, "Seeding skip: >2 successors assigned to %d:%d\n",
               movie->time_start - 1 + t, p_index + 1);
    }
  }
}


static void
UnambiguousMatch (const MOVIE_t * const movie,
                  const PARAMETER_t * const parameters,
                  const TOOL_t * const tools, FILE * const logfile,
                  const int t)
{
  /* mutual matches */
  /* TODO: replace call to exit() with a return value != 0 */
  int j;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  const int nuc_ct = tools->nuc_ct_rec[t];
  const NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  const float cutoff = 2.0 * parameters->nuc_size;

  for (j = 0; j < nuc_ct; j++)
  {
    const PARTNER_t *const partners = nuclei[j].p_partners;
    int k;
    int pred = -1;

    /* find 1<-1 mapping */
    if (nuclei[j].status == 0)
      continue;

    for (k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (partners[k].availability)
      {
        pred = partners[k].index;
        nuclei[j].predecessor = partners[k].index;
        nuclei[j].pdist = partners[k].distance;
        break;
      }
    }

    if (nuclei[j].pskip)
    {
      continue;
    }
    else if (nuclei[j].p_ct != 1)
    {
      continue;
    }
    /* error check */
    else if (pred == -1)
    {
      fprintf (logfile, "predecessor not found, %d:%d\n",
               movie->time_start - 1 + t + 1, j + 1);
      exit (ERROR_UNAMBIGUOUSMATCH1);
    }
    else if (!nuclei_prev[pred].status || !nuclei_prev[pred].s_ct)
    {
      fprintf (logfile, "predecessor dead, %d:%d <- %d:%d\n",
               movie->time_start - 1 + t + 1, j + 1,
               movie->time_start - 1 + t, pred + 1);
      exit (ERROR_UNAMBIGUOUSMATCH2);
    }
    else if (nuclei_prev[pred].sskip)
    {
      fprintf (logfile, "predecessor s_skipped, %d:%d <- %d:%d\n",
               movie->time_start - 1 + t + 1, j + 1,
               movie->time_start - 1 + t, pred + 1);
      exit (ERROR_UNAMBIGUOUSMATCH3);
    }
    /* recording unambiguous predecessors */
    else if (nuclei_prev[pred].s_ct == 1)
    {
      if (nuclei_prev[pred].s_ct2 == 1)
        ExtendBranch (tools, t, pred, j);       /* 1<->1 mapping */
    }
    else
    {
      if (nuclei_prev[pred].s_ct2 == 1)
        Bifurcation_1 (tools, t, pred, j);
      else
        Bifurcation_2 (movie, tools, logfile, t, pred, j);      /* if s_ct2 > 2, skip */
    }
  }
}



/*************************************
 **                                 **
 **    RESOLVE AMBIGUOUS MATCHES    **
 **                                 **
 *************************************/


static void
ForwardBranchInfo (const TOOL_t * const tools, int t)
{
  /* branch info need further update upon decisions on ambiguous cases */
  int j;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  int pred;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (nuclei[j].pflag)
    {
      pred = nuclei[j].predecessor;
      if (nuclei_prev[pred].sflag1 && nuclei_prev[pred].s_ct == 1)
      {
        nuclei[j].start_state = nuclei_prev[pred].start_state;
        nuclei[j].start_scorem = nuclei_prev[pred].start_scorem;
        nuclei[j].birthday = nuclei_prev[pred].birthday;
      }
      else if (nuclei_prev[pred].sflag2)
      {
        nuclei[j].start_state = 1;      /* division */
      }
      else
      {
        nuclei[j].start_state = 2;      /* potential division */
        nuclei[j].start_scorem = nuclei_prev[pred].end_scorem;
      }
    }
    else
    {
      if (!nuclei[j].p_ct)
        nuclei[j].start_state = 0;      /* emerging */
      if (nuclei[j].p_ct == 1)
        nuclei[j].start_state = 0;      /* emerging */
      else
      {
        nuclei[j].start_state = 2;      /* potential division */
      }
    }
  }
}


static void
BackwardBranchInfo (const TOOL_t * const tools, int t)
{
  /* branch info need further update upon decisions on ambiguous cases */
  int i;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  int suc;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (nuclei_prev[i].sflag1 && nuclei_prev[i].s_ct == 1)
    {
      suc = nuclei_prev[i].successor1;
      nuclei_prev[i].end_time = nuclei[suc].end_time;
      nuclei_prev[i].end_state = nuclei[suc].end_state;
      nuclei_prev[i].end_scorem = nuclei[suc].end_scorem;
    }
    else
    {
      if (!nuclei_prev[i].s_ct)
        nuclei_prev[i].end_state = 1;   /* potential death/fn */
      else if (nuclei_prev[i].s_ct == 1)
        nuclei_prev[i].end_state = 2;   /* potential death/fn */
      else
      {
        if (nuclei_prev[i].s_ct2 >= 2)
          nuclei_prev[i].end_state = 3; /* division */
        else
          nuclei_prev[i].end_state = 4; /* potential division */
      }
    }
  }
}


static void
UpdateBranchInfo (const TOOL_t * const tools, int t)
{
  int j;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  int pred;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (nuclei[j].pflag)
    {
      pred = nuclei[j].predecessor;
      if (nuclei_prev[pred].sskip)
        nuclei[j].birthday = t;
      else if (nuclei_prev[pred].sflag2)
      {
        /* sister will take care of herself */
        nuclei[j].birthday = t;
        nuclei[j].generation = nuclei_prev[pred].generation + 1;
      }
      else
      {
        nuclei[j].birthday = nuclei_prev[pred].birthday;
        nuclei[j].generation = nuclei_prev[pred].generation;
      }
    }
    else
      nuclei[j].birthday = t;
  }
}


static int
ScoringExtension (const NUCLEUS_t * const pred, const NUCLEUS_t * const suc)
{
  int score = 0;

  if (abs (pred->size - suc->size) <= 2)
    score += 1;
  else
    score -= 1;

  if (1.0 * abs (pred->weight - suc->weight) / suc->weight < 0.3)
    score += 3;
  else
    score -= 1;

  if (!pred->condensed && !suc->condensed)
    score += 3;

  return score;
}


static int
ScoringDivision (FILE * const logfile, const NUCLEUS_t * const mother,
                 const NUCLEUS_t * const dau1, const NUCLEUS_t * const dau2)
{
  int score = 0;

  if (mother->condensed == 3)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
    fprintf (logfile, " 0");

  if (dau1->condensed == 3 || mother->max_size - dau1->size >= 4)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    score -= 1;
    fprintf (logfile, " 0");
  }
  if (dau2->condensed == 3 || mother->max_size - dau2->size >= 4)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    score -= 1;
    fprintf (logfile, " 0");
  }

  if (1.0 * (mother->max_weight - dau1->weight) / mother->max_weight > 0.3)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    score -= 1;
    fprintf (logfile, " 0");
  }
  if (1.0 * (mother->max_weight - dau2->weight) / mother->max_weight > 0.3)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    score -= 1;
    fprintf (logfile, " 0");
  }

  if (abs (dau1->size - dau2->size) <= 2)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    score -= 1;
    fprintf (logfile, " 0");
  }
  if (1.0 * abs (dau1->weight - dau2->weight) / dau2->weight < 0.1)
  {
    score += 1;
    fprintf (logfile, " 1");
  }
  else
  {
    score -= 1;
    fprintf (logfile, " 0");
  }

  return score;
}


static int
ScoringDeath (const NUCLEUS_t * const nuc)
{
  int score = 0;

  score -= 3;

  return score;
}


static void
CalculateMorphologyScore (const MOVIE_t * const movie,
                          const PARAMETER_t * const parameters,
                          const TOOL_t * const tools, FILE * const logfile,
                          const int t, const int p_index, const int s_index,
                          PARTNER_t * const par)
{
  /* TODO: replace call to exit() with a return value != 0 */
  int k;

  int my_extension, sis_extension;

  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const pred = tools->nuclei_record[t - 1] + p_index;
  NUCLEUS_t *const suc = tools->nuclei_record[t] + s_index;

  const float cutoff = 2.0 * parameters->nuc_size;

  assert (par != NULL);

  if (pred->s_ct2 > 2)
  {
    fprintf (logfile, "S_Skip missed: %d:%d\n", movie->time_start - 1 + t,
             p_index + 1);
    exit (ERROR_CALCULATEMORPHOLOGYSCORE1);
  }
  else if (pred->s_ct2 == 2)
  {
    par->score1m = 100;
    par->score2m = -100;
  }
  else if (pred->s_ct2 == 1)
  {
    par->score1m =
      ScoringExtension (pred, tools->nuclei_record[t] + pred->successor1);
    par->score2m =
      ScoringDivision (logfile, pred,
                       tools->nuclei_record[t] + pred->successor1, suc);
    if (par->score2m > pred->end_scorem)
      pred->end_scorem = par->score2m;
    if (par->score2m > suc->start_scorem)
      suc->start_scorem = par->score2m;
  }
  else
  {
    /* pred->s_ct2 == 0 */
    if (pred->s_ct == 1)
    {
      par->score1m = ScoringDeath (pred);
      par->score2m = ScoringExtension (pred, suc);
    }
    else if (pred->s_ct == 2)
    {
      const PARTNER_t *const partners = pred->s_partners;
      NUCLEUS_t *sis = NULL;
      for (k = 0; k < nuc_ct; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (!partners[k].availability)
          continue;
        if (partners[k].index != s_index)
        {
          sis = tools->nuclei_record[t] + partners[k].index;
          break;
        }
      }
      assert (sis != NULL);
      my_extension = ScoringExtension (pred, suc);
      sis_extension = ScoringExtension (pred, sis);
      par->score1m = sis_extension;
      if (my_extension > sis_extension)
        par->score1m += 3;
      /*score = ScoringDeath(pred);
         if (score > par->score1m) par->score1m = score; */
      par->score2m = ScoringDivision (logfile, pred, suc, sis);
      if (par->score2m > pred->end_scorem)
        pred->end_scorem = par->score2m;
      if (par->score2m > suc->start_scorem)
        suc->start_scorem = par->score2m;
      /*score = ScoringExtension(pred, suc);
         if (score > par->score2m) par->score2m = score; */
    }
    else
    {
      /* s_ct > 2 */
      fprintf (logfile, "S_Skip missed: %d:%d\n",
               movie->time_start - 1 + t, p_index + 1);
      exit (ERROR_CALCULATEMORPHOLOGYSCORE2);
    }
  }
}


static void
MorphologyScores (const MOVIE_t * const movie,
                  const PARAMETER_t * const parameters,
                  const TOOL_t * const tools, FILE * const logfile, int t)
{
  int j, k;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  PARTNER_t *partners;
  float cutoff;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (nuclei[j].p_ct <= 1)
      continue;
    if (nuclei[j].pskip)
      continue;
    partners = nuclei[j].p_partners;
    for (k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (!partners[k].availability)
        continue;
      CalculateMorphologyScore (movie, parameters, tools, logfile, t,
                                partners[k].index, j, partners + k);
    }
  }
}


static int
ScoringExtensionTiming (const int t, const NUCLEUS_t * const pred,
                        const NUCLEUS_t * const suc)
{
  int score = 0;

  if (pred->condensed || suc->condensed)
  {
    if (pred->birthday && t - pred->birthday > 2)
    {
      if (suc->end_time - t > 2 || suc->end_state < 3)
        score -= 3;
    }
  }
  return score;
}


static int
ScoringDivisionTiming (const PARAMETER_t * const parameters, const int t,
                       const NUCLEUS_t * const mother,
                       const NUCLEUS_t * const dau1,
                       const NUCLEUS_t * const dau2)
{
  int score = 0;

  if (dau1->end_time - t < parameters->minimal_cell_cycle)
  {
    if (dau1->end_state == 4)
    {
      if (mother->end_scorem <= dau1->end_scorem)
        score -= 1;
      else
        score += 1;
    }
    else if (dau1->end_state == 3)
      score -= 20;
    else
      score += 1;
  }
  else
    score += 1;

  if (dau2->end_time - t < parameters->minimal_cell_cycle)
  {
    if (dau2->end_state == 4)
    {
      if (mother->end_scorem <= dau2->end_scorem)
        score -= 1;
      else
        score += 1;
    }
    else if (dau2->end_state == 3)
      score -= 20;
    else
      score += 1;
  }
  else
    score += 1;

  if (t - mother->birthday < parameters->minimal_cell_cycle)
  {
    if (mother->start_state == 1)
      score -= 20;
    else if (mother->start_state == 2)
    {
      if (mother->end_scorem <= mother->start_scorem)
        score -= 1;
      else
        score += 1;
    }
  }

  return score;
}


static void
CalculateTimingScore (const PARAMETER_t * const parameters,
                      const TOOL_t * const tools, const int t,
                      const int p_index, const int s_index,
                      PARTNER_t * const par)
{
  int k;

  const int nuc_ct = tools->nuc_ct_rec[t];
  const NUCLEUS_t *const pred = tools->nuclei_record[t - 1] + p_index;
  const NUCLEUS_t *const suc = tools->nuclei_record[t] + s_index;

  const float cutoff = 2.0 * parameters->nuc_size;

  assert (par != NULL);

  if (pred->s_ct2 >= 2)
  {
    par->score1 = par->score1m;
    par->score2 = par->score2m;
  }
  else if (pred->s_ct2 == 1)
  {
    par->score1 =
      par->score1m + ScoringExtensionTiming (t, pred,
                                             tools->nuclei_record[t] +
                                             pred->successor1);
    par->score2 =
      par->score2m + ScoringDivisionTiming (parameters, t, pred,
                                            tools->nuclei_record[t] +
                                            pred->successor1, suc);
  }
  else if (pred->s_ct == 1)
  {
    par->score1 = par->score1m;
    par->score2 = par->score2m;
  }
  else
  {
    if (pred->s_ct == 2)
    {
      const PARTNER_t *const partners = pred->s_partners;
      NUCLEUS_t *sis = NULL;
      int score;
      for (k = 0; k < nuc_ct; k++)
      {
        if (partners[k].distance > cutoff)
          break;
        if (!partners[k].availability)
          continue;
        if (partners[k].index != s_index)
        {
          sis = tools->nuclei_record[t] + partners[k].index;
          break;
        }
      }
      assert (sis != NULL);
      par->score1 = par->score1m + ScoringExtensionTiming (t, pred, sis);
      score = ScoringDeath (pred);

      if (score > par->score1)
        par->score1 = score;

      par->score2 =
        par->score2m + ScoringDivisionTiming (parameters, t, pred, suc, sis);

      score =
        ScoringExtension (pred, suc) + ScoringExtensionTiming (t, pred, suc);
      if (score > par->score2)
        par->score2 = score;
    }
  }
}


static void
TimingScores (const PARAMETER_t * const parameters,
              const TOOL_t * const tools, int t)
{
  int j, k;
  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  PARTNER_t *partners;
  float cutoff;


  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (nuclei[j].p_ct <= 1)
      continue;
    if (nuclei[j].pskip)
      continue;
    partners = nuclei[j].p_partners;
    for (k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (!partners[k].availability)
        continue;
      CalculateTimingScore (parameters, tools, t, partners[k].index, j,
                            partners + k);
    }
  }
}


static void
ResolveAmbiguity (const MOVIE_t * const movie,
                  const PARAMETER_t * const parameters,
                  const TOOL_t * const tools, FILE * const logfile, int t)
{
  int j, k;

  int nuc_ct_prev, nuc_ct;
  NUCLEUS_t *nuclei_prev, *nuclei;
  PARTNER_t *partners;
  float cutoff;
  int pred, match;
  int max_score, total, score;

  nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  nuc_ct = tools->nuc_ct_rec[t];
  nuclei_prev = tools->nuclei_record[t - 1];
  nuclei = tools->nuclei_record[t];

  cutoff = 2.0 * parameters->nuc_size;

  for (j = 0; j < nuc_ct; j++)
  {
    if (!nuclei[j].status)
      continue;
    if (nuclei[j].p_ct <= 1)
      continue;
    if (nuclei[j].pskip)
      continue;
    pred = -1;
    partners = nuclei[j].p_partners;
    total = 0;
    for (k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (!partners[k].availability)
        continue;
      total += partners[k].score1;
    }
    max_score = -10000;
    match = -1;
    for (k = 0; k < nuc_ct_prev; k++)
    {
      if (partners[k].distance > cutoff)
        break;
      if (!partners[k].availability)
        continue;
      score = partners[k].score2 + (total - partners[k].score1);
      if (score > max_score)
      {
        match = k;
        max_score = score;
      }
    }
    /* recording resolved predecessors */
    pred = partners[match].index;
    nuclei[j].predecessor = pred;
    nuclei[j].pdist = partners[match].distance;
    if (nuclei_prev[pred].s_ct == 1)
    {
      ExtendBranch (tools, t, pred, j);
    }
    else
    {
      Bifurcation_2 (movie, tools, logfile, t, pred, j);
    }
  }
}



/****************************************
 **                                    **
 **    RESOLVE FALSE POS/NEG NUCLEI    **
 **                                    **
 ****************************************/


static void
AddGhostNucleus (const TOOL_t * const tools, int t, NUCLEUS_t * nuc, int pred)
{
  NUCLEUS_t *nuclei;
  int nuc_ct;

  nuc_ct = tools->nuc_ct_rec[t];
  nuclei = tools->nuclei_record[t];

  tools->nuc_ct_rec[t] = nuc_ct + 1;
  tools->nuclei_record[t] =
    (NUCLEUS_t *) malloc_exit (sizeof (NUCLEUS_t) * (nuc_ct + 1));
  memcpy (tools->nuclei_record[t], nuclei, sizeof (NUCLEUS_t) * nuc_ct);
  free (nuclei);
  nuclei = tools->nuclei_record[t];
  nuclei[nuc_ct] = *nuc;
  nuclei[nuc_ct].ghost = 1;
  nuclei[nuc_ct].status = 1;
  /* ZB 200908, the partner arrays are now pre-allocated instead of being allocated and freed at each round of tracing 
  nuclei[nuc_ct].p_partners = NULL;
  nuclei[nuc_ct].s_partners = NULL;
  */
  /* ZB 200908 */
  nuclei[nuc_ct].p_partners = (PARTNER_t *) malloc_exit (sizeof (PARTNER_t) * 1000);
  nuclei[nuc_ct].s_partners = (PARTNER_t *) malloc_exit (sizeof (PARTNER_t) * 1000);
  /* end of modification */
  nuc->ghost_proof = 1;
}


static void
RemoveCell (const MOVIE_t * const movie, const TOOL_t * const tools,
            FILE * const logfile, int t, int index, int *err_ct_p)
{
  // const int nuc_ct = tools->nuc_ct_rec[t];
  /* TODO: replace call to exit() with a return value != 0 */
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  if (nuclei[index].ghost != 0)
  {
    if (t == 0)
    {
      fprintf (logfile, "ghost cell at first time point: "
               "%d:%d\nExiting.", movie->time_start - 1 + t + 1, index + 1);
      exit (ERROR_REMOVE_CELL1);
    }

    const int pred = nuclei[index].predecessor;
    NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];

    if (pred == -1)
    {
      fprintf (logfile, "ghost cell has no predecessor: "
               "%d:%d\nExiting.", movie->time_start - 1 + t + 1, index + 1);
      exit (ERROR_REMOVE_CELL2);
    }

    if (nuclei_prev[pred].ghost)
    {
      fprintf (logfile,
               "ghost cell has ghost predecessor: "
               "%d:%d <- %d:%d\nExiting\n",
               movie->time_start - 1 + t + 1, index + 1,
               movie->time_start - 1 + t, pred + 1);
      exit (ERROR_REMOVE_CELL3);
    }

    nuclei[index].status = 0;
    nuclei[index].ghost = 3;
    fprintf (logfile, "ghost %d:%d removed\n", movie->time_start - 1 + t + 1,
             index + 1);

    if (nuclei_prev[pred].sflag2 == 0)
    {
      RemoveCell (movie, tools, logfile, t - 1, pred, err_ct_p);
    }
    else
    {
      fprintf (logfile,
               "Cell division nullified, %d:%d -> %d:%d, %d:%d\n",
               movie->time_start - 1 + t, pred + 1,
               movie->time_start - 1 + t + 1,
               nuclei_prev[pred].successor1 + 1,
               movie->time_start - 1 + t + 1,
               nuclei_prev[pred].successor2 + 1);
      if (index == nuclei_prev[pred].successor1)
      {
        nuclei_prev[pred].successor1 = nuclei_prev[pred].successor2;
      }
      nuclei_prev[pred].successor2 = -1;
      nuclei_prev[pred].sflag2 = 0;
      *err_ct_p = *err_ct_p + 1;
      nuclei_prev[pred].ghost_proof = 1;
      fprintf (logfile, "%d:%d becomes ghost proof\n",
               movie->time_start - 1 + t, pred + 1);
    }
  }                             /* nuclei[index].ghost != 0 */
  else
  {
    /* nuclei[index].ghost == 0 */
    const int pred = nuclei[index].predecessor;

    if (t == 0 || pred == -1 || nuclei[index].pskip)
    {
      nuclei[index].status = 0;
      *err_ct_p = *err_ct_p + 1;
      if (t)
        fprintf (logfile, "Emerging cell removed, %d:%d\n",
                 movie->time_start - 1 + t + 1, index + 1);
      fprintf (logfile, "False positive removed, %d:%d\n",
               movie->time_start - 1 + t + 1, index + 1);
    }
    else
    {
      NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];
      if (nuclei_prev[pred].sflag2)
      {
        nuclei[index].status = 0;
        *err_ct_p = *err_ct_p + 1;
        fprintf (logfile, "False positive removed, %d:%d\n",
                 movie->time_start - 1 + t + 1, index + 1);
        fprintf (logfile,
                 "Cell division nullified, %d:%d -> %d:%d, %d:%d\n",
                 movie->time_start - 1 + t, pred + 1,
                 movie->time_start - 1 + t + 1,
                 nuclei_prev[pred].successor1 + 1,
                 movie->time_start - 1 + t + 1,
                 nuclei_prev[pred].successor2 + 1);
        if (index == nuclei_prev[pred].successor1)
        {
          nuclei_prev[pred].successor1 = nuclei_prev[pred].successor2;
        }
        nuclei_prev[pred].successor2 = -1;
        nuclei_prev[pred].sflag2 = 0;
      }
      else if (nuclei_prev[pred].ghost)
      {
        nuclei[index].status = 0;
        *err_ct_p = *err_ct_p + 1;
        fprintf (logfile, "False positive removed, %d:%d\n",
                 movie->time_start - 1 + t + 1, index + 1);
        RemoveCell (movie, tools, logfile, t - 1, pred, err_ct_p);
      }
      else
      {
        nuclei[index].ghost_proof = 1;
        fprintf (logfile, "%d:%d becomes ghost proof\n",
                 movie->time_start - 1 + t + 1, index + 1);
        fprintf (logfile, "Cell death pronounced, %d:%d\n",
                 movie->time_start - 1 + t + 1, index + 1);
      }
    }
  }
}


static int
MissingCells (const MOVIE_t * const movie, const TOOL_t * const tools,
              FILE * const logfile, const int t)
{
  int i;

  const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];
  NUCLEUS_t *const nuclei_prev = tools->nuclei_record[t - 1];

  int err_ct = 0;

  for (i = 0; i < nuc_ct_prev; i++)
  {
    if (!nuclei_prev[i].status)
      continue;
    if (nuclei_prev[i].sskip)
      continue;
    if (nuclei_prev[i].sflag1)
    {
      if (nuclei_prev[i].ghost == 2)
      {
        int ppred = nuclei_prev[i].predecessor;
        fprintf (logfile, "Cell resusicated, %d:%d to %d:%d to %d:%d\n",
                 movie->time_start - 1 + t - 1, ppred + 1,
                 movie->time_start - 1 + t, i + 1,
                 movie->time_start - 1 + t + 1,
                 nuclei_prev[i].successor1 + 1);
        nuclei_prev[i].ghost = 3;
      }
    }
    else
    {
      /* missing successor */
      if (!nuclei_prev[i].ghost)
      {
        if (!nuclei_prev[i].ghost_proof)
        {
          fprintf (logfile,
                   "Adding ghost cell %d:%d to succeed %d:%d\n",
                   movie->time_start - 1 + t + 1,
                   tools->nuc_ct_rec[t] + 1, movie->time_start - 1 + t,
                   i + 1);
          AddGhostNucleus (tools, t, nuclei_prev + i, i);
          err_ct++;
        }
      }
      else if (nuclei_prev[i].ghost == 2)
      {
        RemoveCell (movie, tools, logfile, t - 1, i, &err_ct);
      }
    }
  }

  return err_ct;
}


static void
ResetNuclei (const TOOL_t * const tools, const int t,const MOVIE_t * const movie)
{
  int i;

  const int nuc_ct = tools->nuc_ct_rec[t];
  NUCLEUS_t *const nuclei = tools->nuclei_record[t];

  for (i = 0; i < nuc_ct; i++)
  {
    NUCLEUS_t *const nuc = nuclei + i;
    /* ZB 200908
    if (nuc->p_partners)
    {
      free (nuc->p_partners);
      nuc->p_partners = NULL;
    }
    if (nuc->s_partners)
    {
      free (nuc->s_partners);
      nuc->s_partners = NULL;
    }
    */
    /* ZB 200908, re-initiating the pre-allocated arrays for partners */
    assert (nuc->p_partners);
    assert (nuc->s_partners);

    int j;
    PARTNER_t *const partners1 = nuc->p_partners;
    PARTNER_t *const partners2 = nuc->s_partners;
    for (j = 0; j < 1000; j++)
    {
      partners1[j].distance = movie->max_distance;
    }
    for (j = 0; j < 1000; j++)
    {
      partners2[j].distance = movie->max_distance;
    }
    /* not re-initiating other values of partners, 'cuz the arrays are always sorted by distance before being used, and having the max_distance would put the unused elements to the end of the array, so whatever residue values they carry from previous round of tracing won't interfere */
    /* end of modification */

    if (nuc->status == 0)
      continue;

    if (nuc->ghost == 1)
      nuc->ghost = 2;

    nuc->birthday = t;
    nuc->generation = 0;
    nuc->end_time = t;
    nuc->start_state = 0;
    nuc->end_state = 0;
    nuc->start_scorem = -10000;
    nuc->end_scorem = -10000;
    nuc->max_size = nuc->size;
    nuc->max_size_time = t;
    nuc->max_weight = nuc->weight;
    nuc->max_weight_time = t;

    nuc->p_ambiguity = 0;
    nuc->s_ambiguity = 0;
    nuc->pskip = 0;
    nuc->sskip = 0;
    nuc->p_ct = 0;
    nuc->s_ct = 0;
    nuc->p_ct2 = 0;
    nuc->s_ct2 = 0;
    nuc->predecessor = -1;
    nuc->successor1 = -1;
    nuc->successor2 = -1;
    nuc->pdist = -1.0;
    nuc->sdist1 = -1.0;
    nuc->sdist2 = -1.0;
    nuc->pflag = 0;
    nuc->sflag1 = 0;
    nuc->sflag2 = 0;
  }
}



/*******************************
 **                           **
 **    TOP LEVEL FUNCTIONS    **
 **                           **
 *******************************/


void
Trace (const MOVIE_t * const movie, PARAMETER_t * const parameters,
       const TOOL_t * const tools, FILE * const logfile, const int round)
{
  int t, tp_number;

  tp_number = movie->tp_number;
  fprintf (logfile, "\ntracing, round %d:\n\n", round);

  /* ZB 200908, pre-allocate the p_partners and s_partners arrays, instead of dynamic allocation and freeing */
  if (round == 1)  /* pre-allocation only needed once */
  { 
    for (t = 0; t < tp_number; t++)
    {
      int i;
      const int nuc_ct = tools->nuc_ct_rec[t];
      NUCLEUS_t *const nuclei = tools->nuclei_record[t];

      if (nuc_ct >= 1000) exit(314);

      for (i = 0; i < nuc_ct; i++)
      {
	NUCLEUS_t *const nuc = nuclei + i;
	nuc->p_partners = (PARTNER_t *) malloc_exit (sizeof (PARTNER_t) * 1000);
	nuc->s_partners = (PARTNER_t *) malloc_exit (sizeof (PARTNER_t) * 1000);
	
	int j;
	PARTNER_t *const partners1 = nuc->p_partners;
	PARTNER_t *const partners2 = nuc->s_partners;
	for (j = 0; j < 1000; j++)
	{
	  partners1[j].distance = movie->max_distance;
	}
	for (j = 0; j < 1000; j++)
	{
	  partners2[j].distance = movie->max_distance;
	}
      }
    }
  }
  /* end of modification */
  fprintf (logfile, "\nallocated neighbor arrays \n\n");
   fflush (logfile);

  /* find nearest matches */
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    MeasureNuclearDistances (movie, parameters, tools, logfile, t);
    SortPartners (tools, t);
    LinkNearestPredecessor (parameters, tools, logfile, t);
  }

  /* add further potential matches, creating ambiguity */
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    TentativeBranch (tools, t);
  }
  for (t = tp_number - 1; t > 0; t--)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    TentativeBranch (tools, t);
  }
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    EvaluateLinks (movie, parameters, tools, logfile, t);
    IntroduceAmbiguity (movie, parameters, tools, logfile, t);
    CountUnambiguousMatches (parameters, tools, t);
    ReduceAmbiguity (movie, parameters, tools, logfile, t);
  }
  for (t = 0; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    ResetBranch (tools, t);
  }

  /* define trusted branches in the lineage tree */
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    CountUnambiguousMatches (parameters, tools, t);
    ReportIrregularity (movie, parameters, tools, logfile, t);
    SkipComplexity (movie, parameters, tools, logfile, t);
    UnambiguousMatch (movie, parameters, tools, logfile, t);
  }

  /* resolve ambiguity */
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    MorphologyScores (movie, parameters, tools, logfile, t);
  }
  fprintf (logfile, "\n");
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    ForwardBranchInfo (tools, t);
  }                             /* getting extra info from upstream of a branch */
  for (t = tp_number - 1; t; t--)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    BackwardBranchInfo (tools, t);
  }                             /* getting extra info from downstream of a branch */
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    TimingScores (parameters, tools, t);
  }
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    ResolveAmbiguity (movie, parameters, tools, logfile, t);
    SkipComplexity (movie, parameters, tools, logfile, t);
  }
  for (t = 1; t < tp_number; t++)
  {
    UpdateTimeInfo (parameters, movie, tools, t);
    UpdateBranchInfo (tools, t);
  }

  /* correct false negative and positive nuclei */

  {
    int err_ct = 0;
    for (t = 1; t < tp_number; t++)
    {
      err_ct += MissingCells (movie, tools, logfile, t);
    }
    if (err_ct > 0)
    {
      for (t = 0; t < tp_number; t++)
        ResetNuclei (tools, t,movie);
      Trace (movie, parameters, tools, logfile, round + 1);
    }
  }
}


void
RemoveGhostNuclei (const MOVIE_t * const movie,
                   const PARAMETER_t * const parameters,
                   const TOOL_t * const tools, FILE * const logfile)
{
  int t, tp_number;
  NUCLEUS_t *nuclei, *nuc;
  int nuc_ct;
  int j;

  tp_number = movie->tp_number - 1;
  for (t = 0; t < tp_number; t++)
  {
    nuc_ct = tools->nuc_ct_rec[t];
    nuclei = tools->nuclei_record[t];
    for (j = 0; j < nuc_ct; j++)
    {
      nuc = nuclei + j;
      if (!nuc->status)
        continue;
      if (!nuc->ghost)
        continue;
      if (!nuc->sflag1)
      {
        nuc->status = 0;
        fprintf (logfile, "hanging ghost, %d:%d\n",
                 movie->time_start - 1 + t, j + 1);
      }
    }
  }
}
