/*
 * run_starrynite_helpers.cxx
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

#include <stdbool.h>

#include "run_starrynite.h"
#include "run_starrynite_continuation_t.h"

int
run_starrynite_before_tracing_helper(const char * const input_prefix,
    const char * const param_file_name, int skip_svm_classifier,
    run_starrynite_continuation_t continuation)
{
  bool skip_bool = (skip_svm_classifier != 0);

  run_starrynite_before_tracing(input_prefix, param_file_name, skip_bool,
      continuation);
}
