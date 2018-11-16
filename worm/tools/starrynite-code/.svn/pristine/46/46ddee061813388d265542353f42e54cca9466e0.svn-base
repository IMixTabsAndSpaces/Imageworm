/*
 * struct_image_size.c
 *
 *  Created on: Apr 9, 2009
 *      Author: blick
 */

#include "struct_image_dims.h"

void
image_dims_init(image_dims_t * const obj, const uint32_t width,
    const uint32_t height)
{
  obj->width = width;
  obj->height = height;
}

uint32_t
image_dims_width_get(const image_dims_t * const obj)
{
  return obj->width;
}

uint32_t
image_dims_height_get(const image_dims_t * const obj)
{
  return obj->height;
}

uint32_t
image_dims_size_get(const image_dims_t * const obj)
{
  return obj->width * obj->height;
}

bool
image_dims_equals(const image_dims_t * const this,
    const image_dims_t * const that)
{
  return this->width == that->width && this->height == that->height;
}
