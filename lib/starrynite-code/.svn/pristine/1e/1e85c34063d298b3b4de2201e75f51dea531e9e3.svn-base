/*
 * run_starrynite_dump.cxx
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

#include "run_starrynite_helpers.h"
#include "run_starrynite_dump.h"
#include "run_starrynite_continuation_t.h"
#include "run_starrynite.h"
#include "xml_dump_status.h"
#include "tool_t.h"

static int
run_starrynite_dump_callback(const char * const input_prefix,
    bool skip_svm_classifier, movie_t *movie, parameter_t *pParameters,
    tool_t *tools, int t, int cell_ct,
    int new_lineage_ct, uint32_t image_width, uint32_t image_height,
    uint32_t image_size)
{
  const int skip_intval = skip_svm_classifier ? 1 : 0;

  xml_dump_status(input_prefix, skip_intval, movie,
      pParameters, t, cell_ct, new_lineage_ct,
      image_width, image_height, image_size);

  return run_starrynite_from_tracing(input_prefix, skip_svm_classifier, movie,
      pParameters, tools, t, cell_ct, new_lineage_ct,
      image_width, image_height, image_size);
}

int
run_starrynite_dump(const char * const input_prefix,
    const char * const param_file_name, bool skip_svm_classifier)
{
  run_starrynite_continuation_t continuation = run_starrynite_dump_callback;

  return run_starrynite_before_tracing(input_prefix, param_file_name,
      skip_svm_classifier, continuation);
}
