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

#include "BackwardBranchInfo.h"
#include "CountUnambiguousMatches.h"
#include "EvaluateLinks.h"
#include "ForwardBranchInfo.h"
#include "IntroduceAmbiguity.h"
#include "link_nearest_predecessors.h"
#include "logfile.h"
#include "nucleus_list_init_predecessor_distances.h"
#include "MissingCells.h"
#include "MorphologyScores.h"
#include "UpdateTimeInfo.h"
#include "ReduceAmbiguity.h"
#include "ReportIrregularity.h"
#include "ResetBranch.h"
#include "ResetNuclei.h"
#include "ResolveAmbiguity.h"
#include "SkipComplexity.h"
#include "SortPartners.h"
#include "nucleus_list_try_joining_predecessors.h"
#include "TimingScores.h"
#include "Trace.h"
#include "nucleus_t.h"
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "UnambiguousMatch.h"
#include "UpdateBranchInfo.h"
#include "struct_metric.h"
#include "init_metric.h"

/*******************************
 **                           **
 **    TOP LEVEL FUNCTIONS    **
 **                           **
 *******************************/

void
Trace(const movie_t * const movie, parameter_t * const parameters,
    const tool_t * const tools, const int round)
{
  /* from movie, Trace uses max_distance, z_res/xy_res,
   * &frame, num_timepts, and time_start
   */

  const float z_xy_res_ratio = movie->z_res / movie->xy_res;

  fprintf(sn_logfile, "\ntracing, round %d:\n\n", round);

  /* find nearest matches */
  for (int t = 1; t < movie->num_timepts; t++)
    {
      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];
      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];

      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      const float cutoff = 2.0 * parameters->nuc_size;

      metric_t metric;
      init_metric(&metric, movie->frame.width, parameters->z_factor);

      nucleus_list_init_predecessor_distances(nuclei, nuc_ct, nuclei_prev,
          nuc_ct_prev, &metric, movie->max_distance);

      SortPartners(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev);

      link_nearest_predecessors(nuclei, nuc_ct, nuclei_prev, cutoff);
    }

  /* add further potential matches, creating ambiguity */
  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];
      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];

      nucleus_list_try_joining_predecessors(nuclei, nuc_ct, nuclei_prev);
    }
  for (int t = movie->num_timepts - 1; t > 0; t--)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];
      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];

      nucleus_list_try_joining_predecessors(nuclei, nuc_ct, nuclei_prev);
    }
  for (int t = 1; t < movie->num_timepts; t++)
    {
      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];

      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];

      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      const float z_factor = parameters->z_factor;
      const float cutoff = 2.0 * parameters->nuc_size;
      const float ambiguity_cutoff = parameters->ambiguity_cutoff;

      EvaluateLinks(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev, cutoff,
          movie->time_start, t);

      IntroduceAmbiguity(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev,
          movie->time_start, t, movie->frame.width, z_factor, cutoff,
          ambiguity_cutoff);

      CountUnambiguousMatches(parameters, tools, t);

      ReduceAmbiguity(movie->time_start, parameters, tools, t);
    }
  for (int t = 0; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);
      ResetBranch(tools, t);
    }

  /* define trusted branches in the lineage tree */
  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);
      CountUnambiguousMatches(parameters, tools, t);
      ReportIrregularity(movie->time_start, parameters, tools, t);
      SkipComplexity(movie->time_start, parameters, tools, t);
      UnambiguousMatch(movie->time_start, parameters, tools, t);
    }

  /* resolve ambiguity */
  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];

      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];

      const float cutoff = 2.0 * parameters->nuc_size;

      MorphologyScores(movie->time_start, nuclei, nuc_ct, nuclei_prev,
          nuc_ct_prev, t, cutoff);
    }
  fprintf(sn_logfile, "\n");

  /* get extra info from upstream of a branch */
  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];

      const nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];

      ForwardBranchInfo(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev);
    }

  /* get extra info from downstream of a branch */
  for (int t = movie->num_timepts - 1; t; t--)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      const nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];

      nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];

      BackwardBranchInfo(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev);
    }

  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);
      TimingScores(parameters, tools, t);
    }

  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);
      ResolveAmbiguity(movie->time_start, parameters, tools, t);
      SkipComplexity(movie->time_start, parameters, tools, t);
    }

  for (int t = 1; t < movie->num_timepts; t++)
    {
      UpdateTimeInfo(parameters, z_xy_res_ratio, tools->nuc_ct_rec, t);

      nucleus_t * const nuclei = tools->nuclei_record[t];
      const int nuc_ct = tools->nuc_ct_rec[t];

      const nucleus_t * const nuclei_prev = tools->nuclei_record[t - 1];
      const int nuc_ct_prev = tools->nuc_ct_rec[t - 1];

      UpdateBranchInfo(nuclei, nuc_ct, nuclei_prev, nuc_ct_prev, t);
    }

  /* correct false negative and positive nuclei */
    {
      int err_ct = 0;
      for (int t = 1; t < movie->num_timepts; t++)
        {
          err_ct += MissingCells(movie->time_start, tools, t);
        }
      if (err_ct > 0)
        {
          for (int t = 0; t < movie->num_timepts; t++)
            {
              nucleus_t * const nuclei = tools->nuclei_record[t];
              const int nuc_ct = tools->nuc_ct_rec[t];

              ResetNuclei(nuclei, nuc_ct, t);
            }
          Trace(movie, parameters, tools, round + 1);
        }
    }
}
