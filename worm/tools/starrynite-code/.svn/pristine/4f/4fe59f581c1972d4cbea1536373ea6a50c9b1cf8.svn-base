/*
 * run_starrynite_dump_helper.c
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

#include <stdbool.h>

#include "run_starrynite_dump_helper.h"
#include "run_starrynite_dump.h"

int
run_starrynite_dump_helper(const char * const input_prefix,
    const char * const param_file_name, bool skip_svm_classifier)
{
  int skip_intval = skip_svm_classifier ? 1 : 0;
  run_starrynite_dump(input_prefix, param_file_name, skip_intval);
}
