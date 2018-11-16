#include "struct_image_info.h"

/* TODO: Make use of image_dims_t */

uint8_t *
image_bytes_get(const image_t * const obj) {
  return obj->bytes;
}

uint32_t
image_width_get(const image_t * const obj)
{
  return obj->width;
}

uint32_t
image_height_get(const image_t * const obj)
{
  return obj->height;
}

uint32_t
image_size_get(const image_t * const obj)
{
  return obj->width * obj->height;
}
