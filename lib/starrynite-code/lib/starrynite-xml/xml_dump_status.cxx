/*
 * sn_movie_show_xml.cxx
 *
 *  Created on: Apr 16, 2009
 *      Author: blick
 */

#include <iostream>
#include <memory>
#include <algorithm>
#include <numeric>
#include <list>

#include "frame.hxx"
#include "movie.hxx"
#include "status.hxx"

extern "C"
{
#include "parameter_t.h"
#include "movie_t.h"
#include "xml_dump_status.h"
}

namespace starrynite
{
  movie
  from_movie_t(const movie_t * const arg)
  {
    starrynite::frame movie_frame_rval(arg->frame.width, arg->frame.height,
        arg->frame.image_size, arg->frame.num_planes);

    starrynite::movie movie_rval(movie_frame_rval, arg->max_distance,
        arg->xy_res, arg->z_res, arg->plane_start, arg->plane_end,
        arg->t_interval, arg->time_start, arg->time_end, arg->num_timepts);

    return movie_rval;
  }

  parameter_movie
  parameter_movie_from_parameter_t(const parameter_t *arg)
  {
    parameter_movie rval(
        parameter_movie::time_start_type(arg->movie.time_start),
        parameter_movie::time_end_type(arg->movie.time_end),
        parameter_movie::num_timepts_type(arg->movie.num_timepts),
        parameter_movie::plane_start_type(arg->movie.plane_start),
        parameter_movie::plane_end_type(arg->movie.plane_end),
        parameter_movie::xy_res_type(arg->movie.xy_res),
        parameter_movie::z_res_type(arg->movie.z_res),
        parameter_movie::t_interval_type(arg->movie.t_interval));

    return rval;
  }

  parameter_sizes
  parameter_sizes_from_parameter_t(const parameter_t *arg) {
    parameter_sizes rval = parameter_sizes();

    rval.size(xsd::cxx::tree::sequence<int>(arg->sizes, arg->sizes + arg->movie.num_timepts));

    return rval;
  }

  parameter
  from_parameter_t(const parameter_t *arg)
  {
    parameter rval(parameter_movie_from_parameter_t(arg),
        parameter_sizes_from_parameter_t(arg),
        parameter::graphic_output_type(arg->graphic_output),
        parameter::neighborhood_size_type(arg->neighborhood_size),
        parameter::nuc_size_type(arg->nuc_size), parameter::polar_size_type(
            arg->polar_size), parameter::max_nuc_size_type(arg->max_nuc_size),
        parameter::min_nuc_size_type(arg->min_nuc_size),
        parameter::nuc_size1_type(arg->nuc_size1), parameter::nuc_size2_type(
            arg->nuc_size2), parameter::nuc_size3_type(arg->nuc_size3),
        parameter::nuc_size4_type(arg->nuc_size4),
        parameter::nuc_size_factor1_type(arg->nuc_size_factor1),
        parameter::nuc_size_factor2_type(arg->nuc_size_factor2),
        parameter::nuc_size_factor3_type(arg->nuc_size_factor3),
        parameter::nuc_size_factor4_type(arg->nuc_size_factor4),
        parameter::z_res_fudge_type(arg->z_res_fudge),
        parameter::z_factor_type(arg->z_factor), parameter::z_size1_type(
            arg->z_size1), parameter::cube_size_type(arg->cube_size),
        parameter::cen_ct_limit_type(arg->cen_ct_limit), parameter::t_type(
            arg->t),
        parameter::noise_time_switch1_type(arg->noise_time_switch1),
        parameter::noise_time_switch2_type(arg->noise_time_switch2),
        parameter::noise_fraction_type(arg->noise_fraction),
        parameter::max_weight_cutoff_type(arg->max_weight_cutoff),
        parameter::nuc_density_cutoff_type(arg->nuc_density_cutoff),
        parameter::noise_factor_type(arg->noise_factor),
        parameter::nuc_density_cutoff1_type(arg->nuc_density_cutoff1),
        parameter::noise_factor1_type(arg->noise_factor1),
        parameter::nuc_density_cutoff2_type(arg->nuc_density_cutoff2),
        parameter::noise_factor2_type(arg->noise_factor2),
        parameter::nuc_density_cutoff3_type(arg->nuc_density_cutoff3),
        parameter::noise_factor3_type(arg->noise_factor3),
        parameter::axis_type(arg->axis), parameter::ap_type(arg->ap),
        parameter::lr_type(arg->lr), parameter::dv_type(arg->dv),
        parameter::minimal_cell_cycle_type(arg->minimal_cell_cycle),
        parameter::ambiguity_cutoff_type(arg->ambiguity_cutoff),
        parameter::shrink_elastisity_type(arg->shrink_elastisity),
        parameter::expand_elastisity_type(arg->expand_elastisity));

    return rval;
  }
}

int
xml_dump_status(const char * const input_prefix, int skip_svm_classifier,
    movie_t *movie, parameter_t *pParameters, int t, int cell_ct,
    int new_lineage_ct, uint32_t image_width, uint32_t image_height,
    uint32_t image_size)
{
  try
    {
      starrynite::movie m = starrynite::from_movie_t(movie);
      starrynite::parameter p = starrynite::from_parameter_t(pParameters);
      starrynite::status s(p, m);
      xml_schema::namespace_infomap map;
      map[""].name = "http://starrynite.sf.net/ns/1";
      starrynite::status_(std::cout, s, map);
    }
  catch (const xml_schema::exception& e)
    {
      std::cerr << e << std::endl;
      return 1;
    }
  return 0;
}
