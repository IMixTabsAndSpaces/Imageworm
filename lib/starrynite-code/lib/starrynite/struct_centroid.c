#include "struct_centroid.h"

bool
centroid_is_valid_get(const centroid_t * const centroid)
{
  return centroid->is_valid;
}

int
centroid_x_get(const centroid_t * const centroid)
{
  return centroid->x;
}

int
centroid_y_get(const centroid_t * const centroid)
{
  return centroid->y;
}

int
centroid_z_get(const centroid_t * const centroid)
{
  return centroid->z;
}

int
centroid_weight_get(const centroid_t * const centroid)
{
  return centroid->weight;
}

void
centroid_is_valid_set(centroid_t * const centroid, const bool is_valid)
{

  centroid->is_valid = is_valid;
}

void
centroid_x_set(centroid_t * const centroid, const int x)
{
  centroid->x = x;
}

void
centroid_y_set(centroid_t * const centroid, const int y)
{
  centroid->y = y;
}

void
centroid_z_set(centroid_t * const centroid, const int z)
{
  centroid->z = z;
}

void
centroid_weight_set(centroid_t * const centroid, const int weight)
{
  centroid->weight = weight;
}
