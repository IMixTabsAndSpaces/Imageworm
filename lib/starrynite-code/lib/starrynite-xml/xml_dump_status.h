/*
 * movie_show_xml.h
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

#ifndef MOVIE_SHOW_XML_H_
#define MOVIE_SHOW_XML_H_

#include "movie_t.h"

int
xml_dump_status (const char * const input_prefix,
    int skip_svm_classifier, movie_t *movie, parameter_t *pParameters,
    int t, int cell_ct,
    int new_lineage_ct, uint32_t image_width, uint32_t image_height,
    uint32_t image_size);

#endif /* MOVIE_SHOW_XML_H_ */
