#ifndef SN__RUN_STARRYNITE_CONTINUATION_T_H
#define SN__RUN_STARRYNITE_CONTINUATION_T_H 1

#include <stdbool.h>
#include <stdint.h>
#include "tool_t.h"
#include "movie_t.h"
#include "parameter_t.h"

typedef int
(*run_starrynite_continuation_t)(const char * const input_prefix,
    bool skip_svm_classifier, movie_t *movie, parameter_t *pParameters,
    tool_t *Tools, int t, int cell_ct,
    int new_lineage_ct, uint32_t image_width, uint32_t image_height,
    uint32_t image_size);

#endif /* ! SN__RUN_STARRYNITE_CONTINUATION_T_H */
