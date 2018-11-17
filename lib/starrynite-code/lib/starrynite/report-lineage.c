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

#include <stdio.h>
#include <assert.h>

#include "report-lineage.h"
#include "tool_t.h"
#include "movie_t.h"
#include "logfile.h"
#include "struct_nucleus.h"

void
ReportNuclei(const movie_t * const movie, const tool_t * const tools,
    const int t, bool skip_svm_classifier)
{
  const size_t filename_len = 50;
  char filename[filename_len];
  FILE *map_file;
  FILE *incorrect_division_calls_of_sn_file;
  FILE *correct_division_calls_of_sn_file;
  int i;

  int division = 0;
  int death = 0;
  int emerge = 0;
  int pskip = 0;
  int sskip = 0;
  int total = 0;

  const nucleus_t * const nuclei = tools->nuclei_record[t];
  const nucleus_t *nuclei_next = NULL;
  const int nuc_ct = tools->nuc_ct_rec[t];

  if (t < movie->num_timepts - 1)
    {
      nuclei_next = tools->nuclei_record[t + 1];
    }

  for (i = 0; i < nuc_ct; i++)
    {
      if (!nuclei[i].is_valid)
        {
          continue;
        }
      total++;

      if (!nuclei[i].has_predecessor)
        {
          if (nuclei[i].pskip)
            {
              pskip++;
            }
          else
            {
              emerge++;
            }
        }

      if (!nuclei[i].has_successor1)
        {
          if (nuclei[i].sskip)
            {
              sskip++;
            }
          else
            {
              death++;
            }
        }
      else if (nuclei_next && !nuclei_next[nuclei[i].successor1].is_valid)
        {
          death++;
        }

      if (nuclei[i].has_successor2)
        {
          division++;
        }
    }

  if (!t)
    {
      emerge = 0;
    }

  if (!nuclei_next)
    {
      death = 0;
    }

  fprintf(sn_logfile, "tp %03d:  %5d  %3d  %3d  %3d  %3d  %3d  %3d\n",
      movie->time_start + t, total, nuc_ct - total, division, death, emerge,
      pskip, sskip);

  for (i = 0; i < nuc_ct; i++)
    {
      if (!nuclei[i].is_valid)
        {
          continue;
        }
      if (t && !nuclei[i].has_predecessor)
        {
          if (nuclei[i].pskip)
            {
              fprintf(sn_logfile, "          p_skip: %s, %d\n",
                  nuclei[i].identity, i + 1);
            }
          else
            {
              fprintf(sn_logfile, "          emerge: %s, %d\n",
                  nuclei[i].identity, i + 1);
            }
        }
      if (nuclei_next)
        {
          if (!nuclei[i].has_successor1
              || !nuclei_next[nuclei[i].successor1].is_valid)
            {
              if (nuclei[i].sskip)
                {
                  fprintf(sn_logfile, "          s_skip: %s, %d\n",
                      nuclei[i].identity, i + 1);
                }
              else
                {
                  fprintf(sn_logfile, "          death: %s, %d\n",
                      nuclei[i].identity, i + 1);
                }
            }
        }
      if (nuclei[i].has_successor2)
        {
          fprintf(sn_logfile, "          division: %s %d -> %s %d ",
              nuclei[i].identity, i + 1,
              nuclei_next[nuclei[i].successor1].identity, nuclei[i].successor1
                  + 1);
          if (nuclei_next[nuclei[i].successor1].potential_predecessor_count > 1)
            {
              fprintf(sn_logfile, "(s), ");
            }
          else
            {
              fprintf(sn_logfile, "(h), ");
            }
          fprintf(sn_logfile, "%s %d",
              nuclei_next[nuclei[i].successor2].identity, nuclei[i].successor2
                  + 1);
          if (nuclei_next[nuclei[i].successor2].potential_predecessor_count > 1)
            {
              fprintf(sn_logfile, "(s), ");
            }
          else
            {
              fprintf(sn_logfile, "(h), ");
            }
          fprintf(sn_logfile, "\n");
        }
    }

  (void) snprintf(filename, filename_len, "t%03d-nuclei", movie->time_start + t);

  map_file = fopen(filename, "w");
  incorrect_division_calls_of_sn_file = fopen("svm_output_movements_called_as_divisions.txt", "a");
  correct_division_calls_of_sn_file = fopen("svm_output_correct_division_calls.txt", "a");

  for (i = 0; i < nuc_ct; i++)
    {
      (void) fprintf(map_file, "%4d,   ", i + 1);

      if (!nuclei[i].is_valid)
        {
          (void) fprintf(map_file, "0,  ");
        }
      else
        {
          (void) fprintf(map_file, "1,  ");
        }

      if (nuclei[i].has_predecessor)
        {
          (void) fprintf(map_file, "%4d,  ", nuclei[i].predecessor + 1);
        }
      else
        {
          (void) fprintf(map_file, "%4d,  ", -1);
        }

      if (!nuclei[i].has_successor1)
        {
          (void) fprintf(map_file, "%4d,  ", -1);
        }
      else
        {
          (void) fprintf(map_file, "%4d,  ", nuclei[i].successor1 + 1);
        }

      if (nuclei[i].has_successor2)
        {
          (void) fprintf(map_file, "%4d,  ", nuclei[i].successor2 + 1);
        }
      else
        {
          (void) fprintf(map_file, "%4d,  ", -1);
        }

      (void) fprintf(map_file, "%4d,  %4d,  %4.1f,  ", nuclei[i].x, nuclei[i].y
          / movie->frame.width, nuclei[i].z + 1);
      (void) fprintf(map_file, "%3d,  ", nuclei[i].size);

      if (nuclei[i].is_valid)
        {
          (void) fprintf(map_file, "%-12s,", nuclei[i].identity);
        }
      else
        {
          (void) fprintf(map_file, "nill        ,");
        }

      (void) fprintf(map_file, "  %8d,", nuclei[i].weight);

	//printf("%s%d\n", "skip_svm_classifier = ", skip_svm_classifier);

      if (!skip_svm_classifier)
        {

	  //printf("%s%d\n", "is_dividing_call_correct = ", nuclei[i].is_dividing_call_correct);

          if (nuclei[i].is_dividing_call_correct == 1)
            {
              //(void) fprintf(map_file, "  %s,", "correct division");
	      (void) fprintf(correct_division_calls_of_sn_file, "%s%d\t%s%d\t%s%.7f\n", "time = ",  (t+1), "nucleus index = ",  (i+1), "SVM score = ", nuclei[i].svm_score_mov_vs_div);	
	      //(void) fprintf(correct_division_calls_of_sn_file, "%s%d\t%s%d\t%s%d\t%s%.11f\n", "time = ",  (t+1), "nucleus index = ",  (i+1), "SVM class decision = ", nuclei[i].is_dividing_call_correct, "SVM score = ", nuclei[i].svm_score_mov_vs_div);
            }
          else if (nuclei[i].is_dividing_call_correct == -1)
            {
              //(void) fprintf(map_file, "  %s,", "incorrect division");
		(void) fprintf(incorrect_division_calls_of_sn_file, "%s%d\t%s%d\t%s%.7f\n", "time = ",  (t+1), "nucleus index = ",  (i+1), "SVM score = ", nuclei[i].svm_score_mov_vs_div);
		//(void) fprintf(incorrect_division_calls_of_sn_file, "%s%d\t%s%d\t%s%d\t%s%.11f\n", "time = ",  (t+1), "nucleus index = ",  (i+1), "SVM class decision = ", nuclei[i].is_dividing_call_correct, "SVM score = ", nuclei[i].svm_score_mov_vs_div);
            }
          else
            {
              /* assert(nuclei[i].is_dividing_call_correct == 0); */
               //(void) fprintf(map_file, "  %s,", "no call");
            }
        }

      (void) fprintf(map_file, "\n");
    }

  fclose(map_file);
  fclose(incorrect_division_calls_of_sn_file);
  fclose(correct_division_calls_of_sn_file);

}
