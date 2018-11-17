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

#ifndef SN__RUN_STARRYNITE_H
#define SN__RUN_STARRYNITE_H 1

#include <stdbool.h>
#include <stdint.h>
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"
#include "run_starrynite_continuation_t.h"

typedef int
(*run_starrynite_t)(const char * const input_prefix,
    const char * const param_file_name,
    bool skip_svm_classifier);

int
run_starrynite(const char * const input_prefix,
    const char * const param_file_name,
    bool skip_svm_classifier);

int
run_starrynite_before_tracing(const char * const input_prefix,
    const char * const param_file_name, bool skip_svm_classifier,
    run_starrynite_continuation_t continuation);

int
run_starrynite_from_tracing(const char * const input_prefix,
    bool skip_svm_classifier, movie_t *movie, parameter_t *pParameters,
    tool_t *tools, int t, int cell_ct,
    int new_lineage_ct, uint32_t image_width, uint32_t image_height,
    uint32_t image_size);

#endif /* ! SN__RUN_STARRYNITE_H */
