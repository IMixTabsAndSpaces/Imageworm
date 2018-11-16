#ifndef SN__STRUCT_CENTROID_H
#define SN__STRUCT_CENTROID_H 1

#include <stdbool.h>

#include "centroid_t.h"

/** The 2D-scan uses a watered-down version of centroid. where only x and
 * weight are used.  Be careful to traverse max_stat as the boundary
 * elements were not initialized (padded) */
struct centroid
{
  bool is_valid;
  int x;
  /* y is recorded as the address of the first pixel in the row */
  int y;
  int z;
  int weight;
};

#endif /* ! SN__STRUCT_CENTROID_H */
