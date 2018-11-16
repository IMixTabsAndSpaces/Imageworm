#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "tiffio.h"
#include <string.h>
#include "tool_t.h"
#include "movie_t.h"
#include "types.h"
#include "struct_nucleus.h"
#include "FreeImageStack.h"
#include "GetImageStacks.h"
#include "xalloc.h"
#include "struct_image_info.h"
//#include "tiff-io.h"
//#include "error-codes.h"

#include "svm_classifier.h"

double
compute_the_distance_between_two_points(double x1, double y1, double z1,
    double x2, double y2, double z2);
int
compute_age_of_parent(const tool_t * const tools, int time_index,
    int current_index, int previous_index);
int
compute_age_of_child(const tool_t * const tools, int time_index,
    int n_time_points, int current_index, int previous_index, int next_index_1,
    int next_index_2);
void
swap_ints(int * x1, int * x2);
void
swap_doubles(double * x1, double * x2);
void
swap_strings(char * x1, char * x2);
void
find_minimum_of_doubles(double * myarray, int length, double * minimum_value,
    int * minimum_index);
void
find_nearest_neighbors_of_parent(const tool_t * const tools, int t,
    int n_nuclei_starrynite, int index, double x, double y, double z,
    int * nns_parent, double * distances, int n_nns_to_be_found,
    int frame_width);

int
compute_normalized_nucleus_supports(const movie_t * const movie,
    tool_t * const tools, int t, const char * const input_prefix,
    uint32_t *image_width, uint32_t *image_height, double x, double y,
    double z, double diameter, double * normalized_pixel_supports,
    double * average_normalized_pixel_support,
    double * std_normalized_pixel_support);
uint8_t
min_uint8_t_array(uint8_t const * array1, int length);
uint8_t
max_uint8_t_array(uint8_t const * array1, int length);
int
min_int_array(int * array1, int length);
int
max_int_array(int * array1, int length);
double
min_double_array(double * array1, int length);
double
max_double_array(double * array1, int length);
double
sum_double(double * array1, int length);
void
scale_features(size_t number_of_features,
    double feature_vector[number_of_features],
    double feature_ranges[number_of_features][2]);
int
svm_classifier_mov_vs_dividing(double * feature_vector, int number_of_features,
    const char * svm_learned_parameters_filename, double * decision_output);
double
rbf_kernel(double * feature_vector_1, double * feature_vector_2,
    int number_of_features, double gamma);
void
read_feature_ranges(size_t number_of_features,
    const char * feature_ranges_filename,
    double feature_ranges[number_of_features][2]);

double** allocate_2d_double(int n_rows, int n_cols);
void deallocate_2d_double(double ** myarray, int n_rows);

void initialize_2d_double(double ** myarray, int n_rows, int n_cols, double value);

/*
 SPECIAL NOTE FOR THE CURRENT VERSION OF THE SVM CLASSIFIER:

 1. Special tratment of the cases when the number of nuclei at time t is less than 4 (to have at least 3 neighbors of parent).
 2. Special tratment of t=1, t=number of time points-1, and t=number of time points.
 3. Operates on division calls of StarryNite.
 4. Special tratment of the cases where the next index 1 of a child is -1

 */

#define NUMBER_OF_FEATURES 60

int
svm_classifier(const movie_t * const movie, tool_t * const tools,
    const char * const input_prefix, uint32_t *image_width,
    uint32_t *image_height)
{

  //FILE * ofp_test = fopen("svm_output_081305.txt", "w");
  //FILE * ofp_features = fopen("svm_features.txt", "w");

  //printf("%s\n", DATADIR);

  uint32_t image_size;

  int j, k;
  const int n_nns_parent = 3;
  int nns_parent[n_nns_parent];
  const char feature_ranges_filename[] = DATADIR "/svm_feature_ranges.txt";
  const char svm_learned_parameters_filename[] =
      DATADIR "/svm_model_parameters.txt";

  double feature_ranges[NUMBER_OF_FEATURES][2] =
    {
      { 0. } };
  double feature_vector[NUMBER_OF_FEATURES];

  double normalized_pixel_supports_parent[50];
  double normalized_pixel_supports_child_1[50];
  double normalized_pixel_supports_child_2[50];
  double distances_from_parent_to_nns[n_nns_parent];

  double average_normalized_pixel_support_parent,
      average_normalized_pixel_support_child_1,
      average_normalized_pixel_support_child_2;
  double std_normalized_pixel_support_parent,
      std_normalized_pixel_support_child_1,
      std_normalized_pixel_support_child_2;
  int number_of_planes_parent, number_of_planes_child_1,
      number_of_planes_child_2;
  int previous_index_parent, next_index_1_parent, next_index_2_parent;
  int next_index_1_child_1, next_index_2_child_1, next_index_1_child_2,
      next_index_2_child_2;
  int next_index_1_child_1_t_plus_2, next_index_2_child_1_t_plus_2,
      next_index_1_child_2_t_plus_2, next_index_2_child_2_t_plus_2;
  int index_child_1, index_child_2;
  int parent_index;
  int child_1_previous_index, child_2_previous_index;

  double decision_function_out;

  int time_start, time_end;
  int plane_start, plane_end;
  int time_index, t;
  int n_nuclei_edited;
  int n_nuclei_starrynite_t_minus_1, n_nuclei_starrynite_t_plus_1,
      n_nuclei_starrynite_t_plus_2;
  int dividing_parent_flag;
  //int n_nns_parent_small_flag, close_to_start_time_flag, close_to_end_time_flag,
  //	next_index_1_child_1_minus_1_flag, next_index_1_child_2_minus_1_flag;

  int absolute_time, age_of_parent, age_of_child_1, age_of_child_2;
  double x_parent, y_parent, z_parent, x_child_1, y_child_1, z_child_1,
      x_child_2, y_child_2, z_child_2, x_child_1_t_plus_2, y_child_1_t_plus_2,
      z_child_1_t_plus_2, x_child_2_t_plus_2, y_child_2_t_plus_2,
      z_child_2_t_plus_2, x_nns_parent_0, y_nns_parent_0, z_nns_parent_0,
      x_nns_parent_1, y_nns_parent_1, z_nns_parent_1, x_nns_parent_2,
      y_nns_parent_2, z_nns_parent_2;

  double diameter_parent, diameter_child_1, diameter_child_2,
      distance_parent_child_1, distance_parent_child_2,
      distance_of_movement_child_1, distance_of_movement_child_2,
      distance_between_children_t_plus_1, distance_between_children_t_plus_2,
      distance_nn_1_parent_child_1, distance_nn_2_parent_child_1,
      distance_nn_3_parent_child_1, distance_nn_1_parent_child_2,
      distance_nn_2_parent_child_2, distance_nn_3_parent_child_2,
      normalized_pixel_intensity_parent, normalized_pixel_intensity_child_1,
      normalized_pixel_intensity_child_2,
      average_normalized_pixel_intensity_parent,
      average_normalized_pixel_intensity_child_1,
      average_normalized_pixel_intensity_child_2;

  // Define the SVM's learned parameters filename
  // Read the number of time points and the number of planes in each image stack
  const int n_time_points = movie->num_timepts;
  const int n_planes = movie->frame.num_planes;
  const int frame_width = movie->frame.width;

  //////////////////////////////////////////////////////////////////////////////////////
  // Initialize feature ranges (mins and maxes) for the SVM (These will be used
  // to scale the feature values to the range [-1,1]

  read_feature_ranges(NUMBER_OF_FEATURES, feature_ranges_filename,
      feature_ranges);

  int jj;

  ///////////////////////////////////////////////////////////////////////////////////
  // Main for loop
  for (t = 0; t < n_time_points; t++)
  //for (t = 14; t < 15; t++)
    {
      const int time_index = t + 1; // 1 <= time_index <= n_time_points

	//printf("%s %d\n", "time index = ", time_index);

      // Read the annotation at time = t
      const int nuc_ct = tools->nuc_ct_rec[t];
	
      for (j = 0; j < nuc_ct; j++)
        {

          tools->nuclei_record[t][j].is_dividing_call_correct = 0; //zero means SVM ignores that nucleus at this time
	  tools->nuclei_record[t][j].svm_score_mov_vs_div = 0.0; //zero is the default SVM decision function output

	  //printf("%d %d\n", tools->nuclei_record[t][j].successor1, tools->nuclei_record[t][j].successor2);
	  
          if ((tools->nuclei_record[t][j].successor1 != -1)
              && (tools->nuclei_record[t][j].successor2 != -1)) //Means that StarryNite calls this nucleus as a dividing parent at time=t
            {

		//printf("%s %d\n", "nucleus index = ", j);

              //grandparent (at t-1): tools->nuclei_record[t-1][previous_index_parent]
              //parent (at t): tools->nuclei_record[t][j]
              //child 1 (at t+1): tools->nuclei_record[t+1][next_index_1_parent]
              //child 2 (at t+1): tools->nuclei_record[t+1][next_index_2_parent]
              //child 1 (at t+2): tools->nuclei_record[t+2][next_index_1_child_1]
              //child 2 (at t+2): tools->nuclei_record[t+2][next_index_1_child_2]
   
              previous_index_parent = tools->nuclei_record[t][j].predecessor;
              next_index_1_parent = tools->nuclei_record[t][j].successor1;
              next_index_2_parent = tools->nuclei_record[t][j].successor2;
              index_child_1 = next_index_1_parent;
              index_child_2 = next_index_2_parent;

		if (t < n_time_points - 1)
		{
	              next_index_1_child_1
        	          = tools->nuclei_record[t + 1][index_child_1].successor1;
	              next_index_2_child_1
        	          = tools->nuclei_record[t + 1][index_child_1].successor2;
	              next_index_1_child_2
        	          = tools->nuclei_record[t + 1][index_child_2].successor1;
	              next_index_2_child_2
        	          = tools->nuclei_record[t + 1][index_child_2].successor2;
		}
		
		else
		{
			next_index_1_child_1 = -1;
			next_index_1_child_2 = -1;
			next_index_2_child_1 = -1;
			next_index_2_child_2 = -1;
		}

              //Compute the features of the SVM

              //Compute the distances between parent and children and define child 1 as the nearest nucleus
              x_parent = (double) tools->nuclei_record[t][j].x;
              y_parent = (double) tools->nuclei_record[t][j].y
                  / (double) movie->frame.width;
              z_parent = (double) tools->nuclei_record[t][j].z + 1.0;

	      if (t < n_time_points - 1)
	      {
	          x_child_1 = (double) tools->nuclei_record[t + 1][index_child_1].x;
	          y_child_1 = (double) tools->nuclei_record[t + 1][index_child_1].y
        	      / (double) movie->frame.width;
	          z_child_1 = (double) tools->nuclei_record[t + 1][index_child_1].z
        	      + 1.0;
	          x_child_2 = (double) tools->nuclei_record[t + 1][index_child_2].x;
        	  y_child_2 = (double) tools->nuclei_record[t + 1][index_child_2].y
                      / (double) movie->frame.width;
	          z_child_2 = (double) tools->nuclei_record[t + 1][index_child_2].z
        	      + 1.0;
	      }

	      else
              {
                  x_child_1 = -1;
                  y_child_1 = -1;
                  z_child_1 = -1;
		  x_child_2 = -1;
		  y_child_2 = -1;
		  z_child_2 = -1;
              }

	      if ((t < n_time_points - 2) && (next_index_1_child_1 != -1))
	      {
	          x_child_1_t_plus_2
        	      = (double) tools->nuclei_record[t + 2][next_index_1_child_1].x;
	          y_child_1_t_plus_2
        	      = (double) tools->nuclei_record[t + 2][next_index_1_child_1].y
                      / (double) movie->frame.width;
	          z_child_1_t_plus_2
        	      = (double) tools->nuclei_record[t + 2][next_index_1_child_1].z
                      + 1.0;
	      }

	      else
	      {
		  x_child_1_t_plus_2 = -1;
		  y_child_1_t_plus_2 = -1;
		  z_child_1_t_plus_2 = -1;
              }

              if ((t < n_time_points - 2) && (next_index_1_child_2 != -1))
              {
	          x_child_2_t_plus_2
        	      = (double) tools->nuclei_record[t + 2][next_index_1_child_2].x;
	          y_child_2_t_plus_2
        	      = (double) tools->nuclei_record[t + 2][next_index_1_child_2].y
                      / (double) movie->frame.width;
	          z_child_2_t_plus_2
        	      = (double) tools->nuclei_record[t + 2][next_index_1_child_2].z
                      + 1.0;
	      }

	      else
              {
                  x_child_2_t_plus_2 = -1;
                  y_child_2_t_plus_2 = -1;
                  z_child_2_t_plus_2 = -1;
              }

	      absolute_time = time_index;
	      diameter_parent = (double) tools->nuclei_record[t][j].size;
	      parent_index = j;
              child_1_previous_index = parent_index;
              child_2_previous_index = parent_index;

              age_of_parent = compute_age_of_parent(tools, t, parent_index,
              	previous_index_parent);
		
		for (jj=0; jj < 50; jj++)
			normalized_pixel_supports_parent[jj] = 0.0;

		
   	      number_of_planes_parent = compute_normalized_nucleus_supports(
                  movie, tools, t, input_prefix, image_width, image_height,
                  x_parent, y_parent, z_parent, diameter_parent,
                  normalized_pixel_supports_parent,
                  &average_normalized_pixel_support_parent,
                  &std_normalized_pixel_support_parent);

	      if (t < n_time_points - 1)
              {

	          distance_parent_child_1
        	      = compute_the_distance_between_two_points(x_parent, y_parent,
                       z_parent, x_child_1, y_child_1, z_child_1); //d1
	          distance_parent_child_2
        	      = compute_the_distance_between_two_points(x_parent, y_parent,
                       z_parent, x_child_2, y_child_2, z_child_2); //d2

                  /* TODO: add a datatype to capture what is being swapped here */
                  if (distance_parent_child_1 > distance_parent_child_2)
                  {
                      //In the current version child 1 is the one that is closer to the parent
                      swap_doubles(&distance_parent_child_1,
                       &distance_parent_child_2);
                      swap_doubles(&x_child_1, &x_child_2);
                      swap_doubles(&y_child_1, &y_child_2);
                      swap_doubles(&z_child_1, &z_child_2);
		      swap_ints(&index_child_1, &index_child_2);	
		      swap_doubles(&x_child_1_t_plus_2, &x_child_2_t_plus_2);
                      swap_doubles(&y_child_1_t_plus_2, &y_child_2_t_plus_2);
                      swap_doubles(&z_child_1_t_plus_2, &z_child_2_t_plus_2);
                      swap_ints(&next_index_1_child_1, &next_index_1_child_2);
                      swap_ints(&next_index_2_child_1, &next_index_2_child_2);
                  }

		  diameter_child_1
        	      = (double) tools->nuclei_record[t + 1][index_child_1].size;

	          diameter_child_2
                      = (double) tools->nuclei_record[t + 1][index_child_2].size;

		  distance_between_children_t_plus_1
                  = compute_the_distance_between_two_points(x_child_1,
                      y_child_1, z_child_1, x_child_2, y_child_2, z_child_2);

		  find_nearest_neighbors_of_parent(tools, t, nuc_ct, parent_index,
                  	x_parent, y_parent, z_parent, nns_parent,
                  	distances_from_parent_to_nns, n_nns_parent, frame_width);
		
                   if ((nns_parent[0] > 0) && (nns_parent[0] < nuc_ct))
		   {
                   	x_nns_parent_0
                      		= (double) tools->nuclei_record[t][nns_parent[0]].x;

                  	y_nns_parent_0 
                      		= (double) tools->nuclei_record[t][nns_parent[0]].y
                          		/ (double) movie->frame.width;
                  	z_nns_parent_0
                      		= (double) tools->nuclei_record[t][nns_parent[0]].z + 1.0;

			distance_nn_1_parent_child_1
        	        	= compute_the_distance_between_two_points(x_nns_parent_0,
	                      	y_nns_parent_0, z_nns_parent_0, x_child_1, y_child_1,
                      		z_child_1);

			distance_nn_1_parent_child_2
                  		= compute_the_distance_between_two_points(x_nns_parent_0,
                      		y_nns_parent_0, z_nns_parent_0, x_child_2, y_child_2,
                      		z_child_2);
                   }

              	   else
                   {
			//assign the pre-computed median values here
                   	distance_nn_1_parent_child_1 = 4.3424532237;
			distance_nn_1_parent_child_2 = 5.3930047283;
			distances_from_parent_to_nns[0] = 4.1857376889;
		   }

              	   if ((nns_parent[1] > 0) && (nns_parent[1] < nuc_ct))
                   {
                   	x_nns_parent_1
                      		= (double) tools->nuclei_record[t][nns_parent[1]].x;
                  	y_nns_parent_1 
                      		= (double) tools->nuclei_record[t][nns_parent[1]].y
                          		/ (double) movie->frame.width;
                  	z_nns_parent_1
                      		= (double) tools->nuclei_record[t][nns_parent[1]].z + 1.0;

			distance_nn_2_parent_child_1
                  		= compute_the_distance_between_two_points(x_nns_parent_1,
                      		y_nns_parent_1, z_nns_parent_1, x_child_1, y_child_1,
                      		z_child_1);

			distance_nn_2_parent_child_2
                  		= compute_the_distance_between_two_points(x_nns_parent_1,
                      		y_nns_parent_1, z_nns_parent_1, x_child_2, y_child_2,
                      		z_child_2);
                   }

              	   else
                   {
			//assign the pre-computed median values here
			distance_nn_2_parent_child_1 = 4.8816185840;
                        distance_nn_2_parent_child_2 = 5.7065313457;
			distances_from_parent_to_nns[1] = 4.6926857981;
                   }

                   if ((nns_parent[2] > 0) && (nns_parent[2] < nuc_ct))
		   {
                   	x_nns_parent_2
                     		= (double) tools->nuclei_record[t][nns_parent[2]].x;

                  	y_nns_parent_2 
                      		= (double) tools->nuclei_record[t][nns_parent[2]].y
                          		/ (double) movie->frame.width;
                  	z_nns_parent_2
                      		= (double) tools->nuclei_record[t][nns_parent[2]].z + 1.0;

			distance_nn_3_parent_child_1
                  		= compute_the_distance_between_two_points(x_nns_parent_2,
                      		y_nns_parent_2, z_nns_parent_2, x_child_1, y_child_1,
                      		z_child_1);
              
			distance_nn_3_parent_child_2
                  		= compute_the_distance_between_two_points(x_nns_parent_2,
                      		y_nns_parent_2, z_nns_parent_2, x_child_2, y_child_2,
                      		z_child_2);
                   }

              	   else
                   {
			//assign the pre-computed median values here
			distance_nn_3_parent_child_1 = 5.2680641606;
                        distance_nn_3_parent_child_2 = 6.0057056205;
			distances_from_parent_to_nns[2] = 5.0505940245;
                   }

		for (jj=0; jj < 50; jj++)
                        normalized_pixel_supports_child_1[jj] = 0.0;

		for (jj=0; jj < 50; jj++)
                        normalized_pixel_supports_child_2[jj] = 0.0;

			if ((x_child_1 < 0) || (y_child_1 < 0) || (z_child_1 < 0))
				printf("Error! one of the coordinates is less than zero");

			if ((x_child_1 > (double)*image_width-1) || (y_child_1 > (double)*image_height-1))
                                printf("Error! one of the coordinates is out of boundary");

		   number_of_planes_child_1 = compute_normalized_nucleus_supports(
                   	movie, tools, t, input_prefix, image_width, image_height,
                  	x_child_1, y_child_1, z_child_1, diameter_child_1,
                  	normalized_pixel_supports_child_1,
                  	&average_normalized_pixel_support_child_1,
                  	&std_normalized_pixel_support_child_1);
              
			if ((x_child_2 < 0) || (y_child_2 < 0) || (z_child_2 < 0))
                                printf("Error! one of the coordinates is less than zero");

                        if ((x_child_2 > (double)*image_width-1) || (y_child_2 > (double)*image_height-1))
                                printf("Error! one of the coordinates is out of boundary");

		   number_of_planes_child_2 = compute_normalized_nucleus_supports(
                  	movie, tools, t, input_prefix, image_width, image_height,
                  	x_child_2, y_child_2, z_child_2, diameter_child_2,
                  	normalized_pixel_supports_child_2,
                  	&average_normalized_pixel_support_child_2,
                  	&std_normalized_pixel_support_child_2);

		   if (t < n_time_points - 2)
		   {
		      	age_of_child_1 = compute_age_of_child(tools, t + 1,
                  		n_time_points, index_child_1, child_1_previous_index,
                  		next_index_1_child_1, next_index_2_child_1);

	              	age_of_child_2 = compute_age_of_child(tools, t + 1,
        	        	n_time_points, index_child_2, child_2_previous_index,
                		next_index_1_child_2, next_index_2_child_2);

            	      	distance_of_movement_child_1
                      		= compute_the_distance_between_two_points(x_child_1,
                      			y_child_1, z_child_1, x_child_1_t_plus_2,
                      			y_child_1_t_plus_2, z_child_1_t_plus_2);
              
		      	distance_of_movement_child_2
                  		= compute_the_distance_between_two_points(x_child_2,
                      			y_child_2, z_child_2, x_child_2_t_plus_2,
                      			y_child_2_t_plus_2, z_child_2_t_plus_2);

             	      	distance_between_children_t_plus_2
                   		= compute_the_distance_between_two_points(x_child_1_t_plus_2,
                    			y_child_1_t_plus_2, z_child_1_t_plus_2,
                      			x_child_2_t_plus_2, y_child_2_t_plus_2,
                      			z_child_2_t_plus_2);
		  }

		  else
		  {
			//assign the pre-computed median values here
	              	age_of_child_1 = 20.0;
                      	age_of_child_2 = 20.0;
		      	distance_of_movement_child_1 = 0.7806407625;
		      	distance_of_movement_child_2 = 0.8249242389;    
		      	distance_between_children_t_plus_2 = 4.5255386420;
		  }    	  

	      }

	      else
	      {
		  //assign the pre-computed median values here
		  distance_parent_child_1 = 1.4856648343; 
		  distance_parent_child_2 = 2.6777602581;
		  diameter_child_1 = 35.0;
		  diameter_child_2 = 34.0;
		  age_of_child_1 = 20.0;
		  age_of_child_2 = 20.0;
                  distance_of_movement_child_1 = 0.7806407625;
                  distance_of_movement_child_2 = 0.8249242389;       
		  distance_between_children_t_plus_1 = 3.9189922174;
		  distance_between_children_t_plus_2 = 4.5255386420;
		  distance_nn_1_parent_child_1 = 4.3424532237;
                  distance_nn_1_parent_child_2 = 5.3930047283;
	   	  distance_nn_2_parent_child_1 = 4.8816185840;
                  distance_nn_2_parent_child_2 = 5.7065313457;
 		  distance_nn_3_parent_child_1 = 5.2680641606;
                  distance_nn_3_parent_child_2 = 6.0057056205;
		  number_of_planes_child_1 = -1;
		  number_of_planes_child_2 = -1;
		  average_normalized_pixel_support_child_1 = 0.2912557528;
		  average_normalized_pixel_support_child_2 = 0.2831343740;
		  std_normalized_pixel_support_child_1 = 0.0648047141;
		  std_normalized_pixel_support_child_2 = 0.0596830911;
		  distances_from_parent_to_nns[0] = 4.1857376889;
		  distances_from_parent_to_nns[1] = 4.6926857981;
		  distances_from_parent_to_nns[2] = 5.0505940245;
	      }

              //CONSTRUCT THE FEATURE VECTOR FOR SVM
              feature_vector[0] = (double) absolute_time;
              feature_vector[1] = (double) age_of_parent;
              feature_vector[2] = (double) age_of_child_1;
              feature_vector[3] = (double) age_of_child_2;
              //feature_vector[4] = diameter_parent;
              feature_vector[4] = diameter_child_1;
              feature_vector[5] = diameter_child_2;
              feature_vector[6] = distance_parent_child_1;
              feature_vector[7] = distance_parent_child_2;
              feature_vector[8] = distance_of_movement_child_1;
              feature_vector[9] = distance_of_movement_child_2;
              feature_vector[10] = distance_between_children_t_plus_1;
              feature_vector[11] = distance_between_children_t_plus_2;
              feature_vector[12] = distance_nn_1_parent_child_1;
              feature_vector[13] = distance_nn_2_parent_child_1;
              feature_vector[14] = distance_nn_3_parent_child_1;
              feature_vector[15] = distance_nn_1_parent_child_2;
              feature_vector[16] = distance_nn_2_parent_child_2;
              feature_vector[17] = distance_nn_3_parent_child_2;

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 >= 0)
                {

                  feature_vector[18]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1];
                }
              else
                {
                  feature_vector[18] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 >= 0)
                {

                  feature_vector[19]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1];
                }
              else
                {
                  feature_vector[19] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 >= 0)
                {

                  feature_vector[20]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1];
                }
              else
                {
                  feature_vector[20] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 - 1
                  >= 0)
                {
                  feature_vector[21]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 - 1];
                }
              else
                {
                  feature_vector[21] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 1
                  >= 0)
                {

                  feature_vector[22]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 1];
                }
              else
                {
                  feature_vector[22] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 - 1
                  >= 0)
                {
                  feature_vector[23]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 - 1];
                }
              else
                {
                  feature_vector[23] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 + 1
                  <= number_of_planes_parent - 1)
                {
                  feature_vector[24]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 + 1];
                }
              else
                {
                  feature_vector[24] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 + 1
                  <= number_of_planes_child_1 - 1)
                {
                  feature_vector[25]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 + 1];
                }
              else
                {
                  feature_vector[25] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 + 1
                  <= number_of_planes_child_2 - 1)
                {
                  feature_vector[26]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 + 1];
                }
              else
                {
                  feature_vector[26] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 - 2
                  >= 0)
                {
                  feature_vector[27]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 - 2];
                }
              else
                {
                  feature_vector[27] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 2
                  >= 0)
                {

                  feature_vector[28]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 2];
                }
              else
                {
                  feature_vector[28] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 - 2
                  >= 0)
                {

                  feature_vector[29]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 - 2];
                }
              else
                {
                  feature_vector[29] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 + 2
                  <= number_of_planes_parent - 1)
                {
                  feature_vector[30]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 + 2];
                }
              else
                {
                  feature_vector[30] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 + 2
                  <= number_of_planes_child_1 - 1)
                {
                  feature_vector[31]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 + 2];
                }
              else
                {
                  feature_vector[31] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 + 2
                  <= number_of_planes_child_2 - 1)
                {
                  feature_vector[32]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 + 2];
                }
              else
                {
                  feature_vector[32] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 3
                  >= 0)
                {
                  feature_vector[33]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 3];
                }
              else
                {
                  feature_vector[33] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 3
                  >= 0)
                {
                  feature_vector[34]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 3];
                }
              else
                {
                  feature_vector[34] = 0.0;
                }

              /* feature 35 */

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 - 3
                  >= 0)
                {
                  feature_vector[35]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 - 3];
                }
              else
                {
                  feature_vector[35] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 + 3
                  <= number_of_planes_parent - 1)
                {

                  feature_vector[36]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 + 3];
                }
              else
                {
                  feature_vector[36] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 + 3
                  <= number_of_planes_child_1 - 1)
                {

                  feature_vector[37]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 + 3];
                }
              else
                {
                  feature_vector[37] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 + 3
                  <= number_of_planes_child_2 - 1)
                {

                  feature_vector[38]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 + 3];
                }
              else
                {
                  feature_vector[38] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 4
                  >= 0)
                {

                  feature_vector[39]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 4];
                }
              else
                {
                  feature_vector[39] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 4
                  >= 0)
                {

                  feature_vector[40]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 4];
                }
              else
                {
                  feature_vector[40] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 - 4
                  >= 0)
                {

                  feature_vector[41]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 - 4];
                }
              else
                {
                  feature_vector[41] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 + 4
                  <= number_of_planes_parent - 1)
                {

                  feature_vector[42]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 + 4];
                }
              else
                {
                  feature_vector[42] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 + 4
                  <= number_of_planes_child_1 - 1)
                {

                  feature_vector[43]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 + 4];
                }
              else
                {
                  feature_vector[43] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 + 4
                  <= number_of_planes_child_2 - 1)
                {

                  feature_vector[44]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 + 4];
                }
              else
                {
                  feature_vector[44] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 5
                  >= 0)
                {

                  feature_vector[45]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 5];
                }
              else
                {
                  feature_vector[45] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 - 5
                  >= 0)
                {

                  feature_vector[46]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 - 5];
                }
              else
                {
                  feature_vector[46] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 - 5
                  >= 0)
                {
                  feature_vector[47]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 - 5];
                }
              else
                {
                  feature_vector[47] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_parent / 2.0) - 1 + 5
                  <= number_of_planes_parent - 1)
                {
                  feature_vector[48]
                      = normalized_pixel_supports_parent[(int) ceil(
                          (double) number_of_planes_parent / 2.0) - 1 + 5];
                }
              else
                {
                  feature_vector[48] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_1 / 2.0) - 1 + 5
                  <= number_of_planes_child_1 - 1)
                {
                  feature_vector[49]
                      = normalized_pixel_supports_child_1[(int) ceil(
                          (double) number_of_planes_child_1 / 2.0) - 1 + 5];
                }
              else
                {
                  feature_vector[49] = 0.0;
                }

              if ((int) ceil((double) number_of_planes_child_2 / 2.0) - 1 + 5
                  <= number_of_planes_child_2 - 1)
                {
                  feature_vector[50]
                      = normalized_pixel_supports_child_2[(int) ceil(
                          (double) number_of_planes_child_2 / 2.0) - 1 + 5];
                }
              else
                {
                  feature_vector[50] = 0.0;
                }

              feature_vector[51] = average_normalized_pixel_support_parent;
              feature_vector[52] = average_normalized_pixel_support_child_1;
              feature_vector[53] = average_normalized_pixel_support_child_2;
              feature_vector[54] = std_normalized_pixel_support_parent;
              feature_vector[55] = std_normalized_pixel_support_child_1;
              feature_vector[56] = std_normalized_pixel_support_child_2;
              feature_vector[57] = distances_from_parent_to_nns[0];
              feature_vector[58] = distances_from_parent_to_nns[1];
              feature_vector[59] = distances_from_parent_to_nns[2];

              //for (k = 0; k < NUMBER_OF_FEATURES; k++)
              //	printf("%s %d %s %f\n", "feature vector[", k, "] = ", feature_vector[k]);

              //fprintf(ofp_features, "%d ", 1);
              //for (k = 0; k < number_of_features; k++)
              //        fprintf(ofp_features, "%d%s%f ", (k+1), ":", feature_vector[k]);
              //fprintf(ofp_features, "\n");

              scale_features(NUMBER_OF_FEATURES, feature_vector, feature_ranges);
              dividing_parent_flag = svm_classifier_mov_vs_dividing(
                  feature_vector, NUMBER_OF_FEATURES,
                  svm_learned_parameters_filename, &decision_function_out);
              //fprintf(ofp_test, "%s %d %f\n", "svm output = ", dividing_parent_flag, decision_function_out);
	      //printf("%s %d %f\n", "svm output = ", dividing_parent_flag, decision_function_out);

              tools->nuclei_record[t][j].is_dividing_call_correct
                  = dividing_parent_flag;

 	      tools->nuclei_record[t][j].svm_score_mov_vs_div = (float)decision_function_out;

            }
        }

    }

  ///////////////////////////////////////////////////////////////////////////////////

  //fclose(ofp_test);
  //fclose(ofp_features);

  return 1;
}

double** allocate_2d_double(int n_rows, int n_cols)
{

        int i;
        double** myarray = (double**)malloc(n_rows * sizeof(double*));

        for (i = 0; i < n_rows; i++)

                myarray[i] = (double*)malloc(n_cols * sizeof(double));

        return myarray;
}

void deallocate_2d_double(double ** myarray, int n_rows)
{
        int i;

        for (i=0; i < n_rows; i++)
        {

                free(myarray[i]);
        }

        free(myarray);

}

void initialize_2d_double(double ** myarray, int n_rows, int n_cols, double value)
{
	int i, j;

	for (i = 0; i < n_rows; i++)
	{

		for (j = 0; j < n_cols; j++)
		{
			myarray[i][j] = value;
		}
	}
}

void
find_minimum_of_doubles(double * myarray, int length, double * minimum_value,
    int * minimum_index)
{

  int i;

  *minimum_value = myarray[0];
  *minimum_index = 0;

  for (i = 1; i < length; i++)
    {

      if (myarray[i] < *minimum_value)
        {
          *minimum_value = myarray[i];
          *minimum_index = i;
        }
    }
}

double
compute_the_distance_between_two_points(double x1, double y1, double z1,
    double x2, double y2, double z2)
{

  double distance;
  double x_diff;
  double y_diff;
  double z_diff;

  x_diff = 0.09 * (x1 - x2); //scale with 0.09 for unit conversion (convert to microns) since x and y are in pixels
  y_diff = 0.09 * (y1 - y2);
  z_diff = z1 - z2;

  distance = pow(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff, 0.5);
  return distance;
}

void
swap_ints(int * x1, int * x2)
{
  int temp;

  temp = *x1;
  *x1 = *x2;
  *x2 = temp;
}

void
swap_doubles(double * x1, double * x2)
{
  double temp;

  temp = *x1;
  *x1 = *x2;
  *x2 = temp;
}

void
swap_strings(char * x1, char * x2)
{
  int length1, length2, max_length;

  length1 = strlen(x1);
  length2 = strlen(x2);

  if (length1 > length2)

    max_length = length1;

  else
    max_length = length2;

  char * temp = (char *) malloc(max_length * sizeof(char));

  strcpy(temp, x1);
  strcpy(x1, x2);
  strcpy(x2, temp);

  free(temp);
}

uint8_t
min_uint8_t_array(uint8_t const * array1, int length)
{

  int i;
  uint8_t min_value;

  min_value = array1[0];

  for (i = 1; i < length; i++)
    {
      if (array1[i] < min_value)
        {
          min_value = array1[i];
        }
    }

  return min_value;
}

uint8_t
max_uint8_t_array(uint8_t const * array1, int length)
{

  int i;
  uint8_t max_value;

  max_value = array1[0];

  for (i = 1; i < length; i++)
    {
      if (array1[i] > max_value)
        {
          max_value = array1[i];
        }
    }

  return max_value;
}

int
min_int_array(int * array1, int length)
{

  int i;
  int min_value;

  min_value = array1[0];

  for (i = 1; i < length; i++)
    {
      if (array1[i] < min_value)
        {
          min_value = array1[i];
        }
    }

  return min_value;
}

int
max_int_array(int * array1, int length)
{

  int i;
  int max_value;

  max_value = array1[0];

  for (i = 1; i < length; i++)
    {
      if (array1[i] > max_value)
        {
          max_value = array1[i];
        }
    }

  return max_value;
}

double
min_double_array(double * array1, int length)
{

  int i;
  double min_value;

  min_value = array1[0];

  for (i = 1; i < length; i++)
    {
      if (array1[i] < min_value)
        {
          min_value = array1[i];
        }
    }

  return min_value;
}

double
max_double_array(double * array1, int length)
{

  int i;
  double max_value;

  max_value = array1[0];

  for (i = 1; i < length; i++)
    {
      if (array1[i] > max_value)
        {
          max_value = array1[i];
        }
    }

  return max_value;
}

double
sum_double(double * array1, int length)
{
  int i;
  double sum_value;

  sum_value = 0.0;

  for (i = 0; i < length; i++)

    sum_value += array1[i];

  return sum_value;

}

int
compute_age_of_parent(const tool_t * const tools, int t, int current_index,
    int previous_index)
{
  double age_of_parent;
  int time_index_past;
  int done;
  int n_nuclei_starrynite_past;
  int parent_past_index, parent_past_previous_index, parent_past_next_index_1,
      parent_past_next_index_2, parent_past_status;
  int number_of_passes;

  time_index_past = t - 1;
  done = 0;
  number_of_passes = 0;

  if (time_index_past >= 0)
    {
      while (!done)
        {

          if ((previous_index < 0) || (time_index_past < 0))
            {
              done = 1;
              age_of_parent = t - time_index_past;
              continue;
            }

          parent_past_previous_index
              = tools->nuclei_record[time_index_past][previous_index].predecessor;
          parent_past_next_index_1
              = tools->nuclei_record[time_index_past][previous_index].successor1;
          parent_past_next_index_2
              = tools->nuclei_record[time_index_past][previous_index].successor2;
          parent_past_status
              = (int) tools->nuclei_record[time_index_past][previous_index].is_valid;

          if ((parent_past_next_index_1 != current_index)
              && (parent_past_next_index_2 != current_index))
            {
              //printf("%s %d\n", "Warning: The previous index and next index annotations are not consistent at time = ", time_index_past);
              done = 1;
              age_of_parent = t - time_index_past;
              continue;
            }

          if ((parent_past_previous_index != -1) && (parent_past_next_index_1
              != -1) && (parent_past_next_index_2 == -1) && (parent_past_status
              == 1) && (parent_past_next_index_1 == current_index))
            {
              //The past nucleus is moving. Go to the previous time index
              time_index_past -= 1;
              current_index = previous_index;
              previous_index = parent_past_previous_index;
            }

          else
            {
              //The past nucleus is either not moving or there is no such nucleus
              done = 1;
              age_of_parent = t - time_index_past;
              continue;
            }

          if (time_index_past == 0)
            {

              done = 1;
              age_of_parent = t;
              continue;
            }

          number_of_passes += 1;

          if (number_of_passes > t)
            {
              done = 1;
              age_of_parent = t;
              continue;
            }

        }
    }

  else
    {
      age_of_parent = 1;
    }

  return age_of_parent;
}

int
compute_age_of_child(const tool_t * const tools, int t, int n_time_points,
    int current_index, int previous_index, int next_index_1, int next_index_2)
{

  int time_index_fut;
  int age_of_child;
  int done;
  double age_of_parent;
  int n_nuclei_starrynite_fut;
  int length;
  int index_next_t, previous_index_next_t, next_index_1_next_t,
      next_index_2_next_t, status_next_t;
  int number_of_passes;

  time_index_fut = t + 1;

  if (((next_index_1 != -1) && (next_index_2 != -1)) || ((next_index_1 == -1)
      && (next_index_2 == -1)))
    {

      age_of_child = time_index_fut - t;
    }

  else
    {
      number_of_passes = 0;

      if ((time_index_fut <= n_time_points) && ((next_index_1 != -1)
          && (next_index_2 == -1)))
        {

          done = 0;
          while (!done)
            {

              index_next_t = next_index_1;

              if ((next_index_1 < 0) || (time_index_fut > n_time_points - 1))
                {
                  age_of_child = time_index_fut - t;
                  done = 1;
                  continue;
                }

              previous_index_next_t
                  = tools->nuclei_record[time_index_fut][next_index_1].predecessor;
              next_index_1_next_t
                  = tools->nuclei_record[time_index_fut][next_index_1].successor1;
              next_index_2_next_t
                  = tools->nuclei_record[time_index_fut][next_index_1].successor2;
              status_next_t
                  = tools->nuclei_record[time_index_fut][next_index_1].is_valid;

              if (previous_index_next_t != current_index)
                {
                  //printf("%s %d\n", "Warning: The previous index and next index annotations are not consistent at time = ", time_index_fut);
                  done = 1;
                  age_of_child = time_index_fut - t;
                  continue;
                }

              if ((next_index_1_next_t != -1) && (next_index_2_next_t == -1)
                  && (status_next_t == 1))
                {

                  time_index_fut += 1;
                  previous_index = current_index;
                  current_index = next_index_1;
                  next_index_1 = next_index_1_next_t;
                  next_index_2 = next_index_2_next_t;
                }

              else
                {
                  done = 1;
                  age_of_child = time_index_fut - t;
                  continue;
                }

              if (time_index_fut == n_time_points)
                {
                  done = 1;
                  age_of_child = time_index_fut - t;
                  continue;
                }

              number_of_passes += 1;

              if (number_of_passes > n_time_points - t)
                {
                  done = 1;
                  age_of_parent = t;
                  continue;
                }

            }
        }
    }

  return age_of_child;
}

void
find_nearest_neighbors_of_parent(const tool_t * const tools, int t,
    int n_nuclei_starrynite, int index, double x, double y, double z,
    int * nns_parent, double * distances, int n_nns_to_be_found,
    int frame_width)
{

  int done, i;
  int n_nns_found;
  double * distances_from_parent = (double *) malloc(n_nuclei_starrynite
      * sizeof(double));
  int nucleus_index;
  double minimum_distance;
  int minimum_index;
  double x_neighbor, y_neighbor, z_neighbor;
  done = 0;
  n_nns_found = 0;

  for (nucleus_index = 0; nucleus_index < n_nuclei_starrynite; nucleus_index++)
    {
      if (nucleus_index == index)
        continue;

      x_neighbor = (double) tools->nuclei_record[t][nucleus_index].x;
      y_neighbor = (double) tools->nuclei_record[t][nucleus_index].y
          / (double) frame_width;
      z_neighbor = (double) tools->nuclei_record[t][nucleus_index].z + 1.0;

      distances_from_parent[nucleus_index]
          = compute_the_distance_between_two_points(x, y, z, x_neighbor,
              y_neighbor, z_neighbor);
    }

  if (index < 0)
    {
      for (i = 0; i < n_nns_to_be_found; i++)
      {
        distances[i] = 90.0;
	nns_parent[i] = -1;
      }

      free(distances_from_parent);
      return;
    }

  distances_from_parent[index] = 90;

  for (i = 0; i < n_nns_to_be_found; i++)

    nns_parent[i] = -1;

  for (i = 0; i < n_nns_to_be_found; i++)
    {

      if (i > n_nuclei_starrynite - 2)

        distances[i] = 90.0;

      else
        {
          find_minimum_of_doubles(distances_from_parent, n_nuclei_starrynite,
              &minimum_distance, &minimum_index);
          distances[i] = minimum_distance;
          nns_parent[i] = minimum_index;

          if (minimum_index >= 0)
            distances_from_parent[minimum_index] = 90.0;

          //else
          //printf("minimum index less than zero\n");
        }
    }

  free(distances_from_parent);
}

int
compute_normalized_nucleus_supports(const movie_t * const movie,
    tool_t * const tools, int t, const char * const input_prefix,
    uint32_t *myimage_width, uint32_t *myimage_height, double x, double y,
    double z, double diameter, double * normalized_pixel_supports,
    double * average_normalized_pixel_support,
    double * std_normalized_pixel_support)
{
  uint32_t sub_image_width, sub_image_height;
  uint8_t max_pixel_intensity, min_pixel_intensity;
  double threshold_intensity;
  int n_pixels_within_nucleus_support;
  int number_of_normalized_supports;

  int j, k, m, n;
  double delta_z;
  double left_end_of_nucleus, right_end_of_nucleus, left_plane_index,
      right_plane_index;
  double sum_of_pixel_values, total_number_of_pixels;
  double left_edge, right_edge, bottom_edge, upper_edge;
  double number_of_pixels_in_the_square;
  double xc, yc;
  double temp_diff;
  int xx, yy;
  //uint8_t * sub_image;

  delta_z = (diameter / 2.0) * 0.09;
  left_end_of_nucleus = z - delta_z;
  right_end_of_nucleus = z + delta_z;
  left_plane_index = ceil(left_end_of_nucleus);
  right_plane_index = floor(right_end_of_nucleus);

  sum_of_pixel_values = 0.0;
  total_number_of_pixels = 0.0;

  //Free Image stack because it's already been allocated by StarryNite (at the end svm will leave one image stack allocated but this will be freed by Starrynite at the end (CleanUpTools)
  FreeImageStack(tools->image_stack, movie->frame.num_planes);
  tools->image_stack = NULL;

  tools->image_stack = GetImageStacks(movie->frame.num_planes,
      movie->time_start, movie->plane_start, t, input_prefix, false,
      myimage_width, myimage_height);

  if (left_plane_index < 0)
    left_plane_index = 0;

  if (right_plane_index > movie->frame.num_planes - 1)
    right_plane_index = (double) movie->frame.num_planes - 1;

  for (j = (int) left_plane_index; j <= (int) right_plane_index; j++)
    {
      left_edge = ceil(x - diameter / 2.0);
      right_edge = floor(x + diameter / 2.0);
      bottom_edge = ceil(y - diameter / 2.0);
      upper_edge = floor(y + diameter / 2.0);

      if (left_edge < 1)
        left_edge = 1.0;

      if (bottom_edge < 1)
        bottom_edge = 1.0;

      if (right_edge > *myimage_width - 1)
        right_edge = *myimage_width;

      if (upper_edge > *myimage_height - 1)
        upper_edge = *myimage_height;

      number_of_pixels_in_the_square = (right_edge - left_edge + 1)
          * (upper_edge - bottom_edge + 1);
      sub_image_width = (uint32_t) (right_edge - left_edge + 1.0);

      if (((int) (bottom_edge - 1) * (int) *myimage_width + (int) (left_edge
          - 1) < 0) || ((int) (bottom_edge - 1) * (int) *myimage_width
          + (int) (left_edge - 1) > (*myimage_width) * (*myimage_height) - 1))
        {
          printf("Hey indices out of range 2a");
          printf("%s %d %d\n", "m,n = ", m, n);
        }

      max_pixel_intensity = 0;
      min_pixel_intensity = tools->image_stack[j][(int) (bottom_edge - 1)
          * (int) *myimage_width + (int) (left_edge - 1)];

      for (m = bottom_edge; m <= upper_edge; m++)
        {
          for (n = left_edge; n <= right_edge; n++)
            {

              if (((m - 1) * (int) *myimage_width + (n - 1) < 0) || ((m - 1)
                  * (int) *myimage_width + (n - 1) > (*myimage_width)
                  * (*myimage_height) - 1))
                {
                  printf("Hey indices out of range 2");
                  printf("%s %d %d\n", "m,n = ", m, n);
                }

              if (tools->image_stack[j][(m - 1) * (int) *myimage_width
                  + (n - 1)] < min_pixel_intensity)
                min_pixel_intensity = tools->image_stack[j][(m - 1)
                    * (int) *myimage_width + (n - 1)];

              if (tools->image_stack[j][(m - 1) * (int) *myimage_width
                  + (n - 1)] > max_pixel_intensity)
                max_pixel_intensity = tools->image_stack[j][(m - 1)
                    * (int) *myimage_width + (n - 1)];

            }

        }

      threshold_intensity = min_pixel_intensity + (max_pixel_intensity
          - min_pixel_intensity) / 2.0;

      n_pixels_within_nucleus_support = 0;

      for (m = bottom_edge; m <= upper_edge; m++)
        {
          for (n = left_edge; n <= right_edge; n++)
            {
              //if ((double)sub_image[(m-(int)bottom_edge)*(int)sub_image_width + (n-(int)left_edge)] > threshold_intensity)
              if ((double) tools->image_stack[j][(m - 1) * (int) *myimage_width
                  + (n - 1)] > threshold_intensity)

                n_pixels_within_nucleus_support += 1;
            }
        }

      if (number_of_pixels_in_the_square <= 0)
	
		printf("Warning: Number of pixels in the square around nucleus is less than or equal to zero\n");
	

	if (number_of_pixels_in_the_square <= 0)
	
		normalized_pixel_supports[j - (int) left_plane_index] = 0.0;
	else
	      normalized_pixel_supports[j - (int) left_plane_index]
        	  = (double) n_pixels_within_nucleus_support
              / (double) number_of_pixels_in_the_square;

	if (normalized_pixel_supports[j - (int) left_plane_index] < 0)
	{
		printf("Warning: Normalized pixel support is less than zero\n");
		printf("%s %d\n", "plane index = ", j);
	}

	if (normalized_pixel_supports[j - (int) left_plane_index] > 1)
        {
                printf("Warning: Normalized pixel support is greater than one\n");
                printf("%s %d\n", "plane index = ", j);
        }

	
      //printf("%s %f\n", "normalized pixel support = ", normalized_pixel_supports[j-(int)left_plane_index]);

    }

  *average_normalized_pixel_support = sum_double(normalized_pixel_supports,
      (int) (right_plane_index - left_plane_index + 1)) / (right_plane_index
      - left_plane_index + 1);

  *std_normalized_pixel_support = 0.0;
  number_of_normalized_supports = (int) (right_plane_index - left_plane_index
      + 1);

  for (j = 0; j < number_of_normalized_supports; j++)
    {
      temp_diff = (normalized_pixel_supports[j]
          - *average_normalized_pixel_support);
      *std_normalized_pixel_support += temp_diff * temp_diff;
    }

  *std_normalized_pixel_support /= (double) (number_of_normalized_supports - 1);

  return number_of_normalized_supports;
}

void
scale_features(const size_t number_of_features,
    double feature_vector[number_of_features],
    double feature_ranges[number_of_features][2])
{
  // This function uses previously stored ranges (minimum and maximum
  // values) for each feature does scaling to [-1,1] range. There might
  // be some values that are < -1 or > 1 if the features are out of the
  // range of values stored from the training data.

  for (int i = 0; i < number_of_features; i++)
    {
      const double denominator = feature_ranges[i][1] - feature_ranges[i][0];

      if (denominator != 0.0) {
        feature_vector[i] = 2 * (feature_vector[i] - feature_ranges[i][0])
            / denominator - 1;
      }
    }
}

double
rbf_kernel(double * feature_vector_1, double * feature_vector_2,
    int number_of_features, double gamma)
{
  double diff_norm_square = 0.0;

  for (int i = 0; i < number_of_features; i++)
    {
      const double temp_diff = feature_vector_1[i] - feature_vector_2[i];
      diff_norm_square += temp_diff * temp_diff;
    }

  //printf("%s %g ", "sum = ", diff_norm_square);

  return exp(-1.0 * gamma * diff_norm_square);
}

int
svm_classifier_mov_vs_dividing(double * feature_vector, int number_of_features,
    const char * svm_learned_parameters_filename,
    double * decision_function_output)
{

  //THIS FUNCTION IMPLEMENTS THE DECISION FUNCTION FOR AN RBF KERNEL
  //THE PARAMETERS SHOULD BE LEARNED BY LIBSVM AND THE .MODEL FILE SHOULD BE SUPPORTED AS THE INPUT FILE

  //printf("%s\n", svm_learned_parameters_filename);
  FILE * ifp_svm_model_parameters = fopen(svm_learned_parameters_filename, "r");
  int line_counter;
  char * a_line = (char *) malloc(5000 * sizeof(char));
  char * temp_str;
  double * sv_coeff;

  int i, j, ii, jj, p;
  double gamma;
  double rho;
  int total_sv;
  int sv_index;
  int nr_class;
  double decision_function_value;
  int classifier_output;
  double rbf_kernel_output;
  double * kvalues;

  nr_class = 2;
  double * nSV = (double *) malloc(nr_class * sizeof(double));
  int * start = (int*) malloc(nr_class * sizeof(int));

  double sum;
  int si, sj, ci, cj;
  int k;
  double * dec_values = (double *) malloc(nr_class * (nr_class - 1) / 2
      * sizeof(double));
  int * label = (int *) malloc(nr_class * sizeof(int));

  double ** support_vectors;

  if (ifp_svm_model_parameters == NULL)
    {
      printf(
          "Warning: The SVM model file that contains the learned parameters could not be found. Skipping the SVM classifier");
      return 0;
    }

  //READ MODEL PARAMETERS
  line_counter = 0;

  while (fgets(a_line, 5000, ifp_svm_model_parameters) != NULL)
    {

      line_counter += 1;

      if (line_counter == 3)
        {
          //Read gamma
          temp_str = strtok(a_line, " ");
          temp_str = strtok(NULL, " ");
          gamma = atof(temp_str);
        }

      if (line_counter == 5)
        {
          //Read total number of Support Vectors
          temp_str = strtok(a_line, " ");
          temp_str = strtok(NULL, " ");
          total_sv = atoi(temp_str);

        }

      if (line_counter == 6)
        {
          //Read rho = -b
          temp_str = strtok(a_line, " ");
          temp_str = strtok(NULL, " ");
          rho = atof(temp_str);

        }

      if (line_counter == 7)
        {
          //Read possible labels
          temp_str = strtok(a_line, " ");
          temp_str = strtok(NULL, " ");
          label[0] = atoi(temp_str);
          temp_str = strtok(NULL, " ");
          label[1] = atoi(temp_str);
        }

      if (line_counter == 8)
        {
          //Read number of support vectors
          temp_str = strtok(a_line, " ");
          temp_str = strtok(NULL, " ");
          nSV[0] = atoi(temp_str);
          temp_str = strtok(NULL, " ");
          nSV[1] = atoi(temp_str);
        }

      if (line_counter == 9)

        break;

    }

  sv_coeff = (double *) malloc(total_sv * sizeof(double));
  kvalues = (double *) malloc(total_sv * sizeof(double));
  //double (*support_vectors)[number_of_features] = calloc(total_sv
   // * number_of_features, sizeof(double));

  support_vectors = allocate_2d_double(total_sv, number_of_features);
  initialize_2d_double(support_vectors, total_sv, number_of_features, 0.0);

  sv_index = 0;
  //Read Support Vectors
  while (fgets(a_line, 5000, ifp_svm_model_parameters) != NULL)
    {

      temp_str = strtok(a_line, " ");
      sv_coeff[sv_index] = atof(temp_str);

      temp_str = strtok(NULL, ":");
      while ((temp_str != NULL) && (temp_str[0] != '\n'))
        {
          i = atoi(temp_str);
          temp_str = strtok(NULL, " :");
          support_vectors[sv_index][i - 1] = atof(temp_str);
          temp_str = strtok(NULL, " :");
        }

      sv_index += 1;
    }

  for (i = 0; i < total_sv; i++)
  {
    kvalues[i] = rbf_kernel(feature_vector, support_vectors[i],
        number_of_features, gamma);

    //printf("%s%f ", "k values[i] = ", (float)kvalues[i]);

    //if (i == 4)
	//{
	//	for (j = 0; j < number_of_features; j++)
	//	{
	//		printf("%s%d %d", "i, j = ", i, j);
	//		printf("%s%f ", "feature vector[j] = ", (float)feature_vector[j]);			
	//		printf("%s%f ", "support vectors[i][j] = ", (float)support_vectors[i][j]);
	//		printf("%s%d ", "number of features = ", (int)number_of_features);
	//		printf("%s%f ", "gamma = ", (float)gamma);
	//	}
	//}
  }

  //printf("\n");

  start[0] = 0;
  for (i = 1; i < nr_class; i++)
    start[i] = start[i - 1] + nSV[i - 1];

  p = 0;
  for (ii = 0; ii < nr_class; ii++)
    {
      for (jj = ii + 1; jj < nr_class; jj++)
        {
          sum = 0.0;
          si = start[ii];
          sj = start[jj];
          ci = nSV[ii];
          cj = nSV[jj];

	  //printf("%s%d ", "ci = ", ci);
          for (k = 0; k < ci; k++)
	  {
            sum += sv_coeff[si + jj + k - 1] * kvalues[si + k];
	    //printf("%s%d ", "si + jj + k - 1 = ", si + jj + k - 1);
	    //printf("%s%f ", "sum = ", sum);
	  }

	  //printf("\n");
	  //printf("%s%d ", "cj = ", cj);
          for (k = 0; k < cj; k++)
	  {
            sum += sv_coeff[sj + ii + k] * kvalues[sj + k];
	    //printf("%s%d ", "si + ii + k  = ", sj + ii + k);
	    //printf("%s%f ", "sum = ", sum);
	  }

	  //printf("\n");

          sum -= rho;
          dec_values[p] = sum;
          p += 1;
        }
    }

  decision_function_value = dec_values[0] * label[0];

  //decision_function_value = 0.0;
  //for (i = 0; i < total_sv; i++)
  //{
  //	//printf("%s %f\n", "gamma = ", gamma);
  //	//for (j = 0; j < number_of_features; j++)
  //	//	printf("%f ", support_vectors[i][j]);

  //	rbf_kernel_output = rbf_kernel(feature_vector, support_vectors[i], number_of_features, gamma);

  //	if (i==0)
  //	{
  //               //printf("%s %f\n", "sv_coeff[i] = ", sv_coeff[i]);
  //		printf("%s %f\n", "rbf kernel output = ", rbf_kernel_output);
  //	}

  //	decision_function_value += sv_coeff[i]*rbf_kernel_output;
  //}
  //decision_function_value -= rho;

  if (decision_function_value >= 0)

    classifier_output = 1;

  else
    classifier_output = -1;

  //printf("%s %f\n", "decision function value = ", decision_function_value);

  fclose(ifp_svm_model_parameters);
  free(a_line);
  free(sv_coeff);
  free(kvalues);
  free(nSV);
  free(start);
  free(dec_values);
  free(label);

  deallocate_2d_double(support_vectors, total_sv);

  *decision_function_output = decision_function_value;
  return classifier_output;
}

void
read_feature_ranges(const size_t number_of_features,
    const char * feature_ranges_filename,
    double feature_ranges[number_of_features][2])
{
  FILE * ifp = fopen(feature_ranges_filename, "r");

  assert (ifp != NULL);
  if (ifp == NULL)
    {
      printf("%s", "Error! The feature range file could not be opened");
    }

  char a_line[5000];
  char * temp_str;

  //Skip the first two lines
  temp_str = fgets(a_line, 5000, ifp);
  assert(temp_str != NULL);

  temp_str = fgets(a_line, 5000, ifp);
  assert(temp_str != NULL);

  int feature_index = 0;

  while (fgets(a_line, 5000, ifp) != NULL)
    {
      temp_str = strtok(a_line, " ");
      feature_index = atoi(temp_str);

      assert(feature_index <= number_of_features);
      if (feature_index > number_of_features)
        {
          printf("Error! The number of features SVM is expecting is less "
            "than the number of features in the feature range file");
        }

      temp_str = strtok(NULL, " ");
      feature_ranges[feature_index - 1][0] = atof(temp_str);

      temp_str = strtok(NULL, " ");
      feature_ranges[feature_index - 1][1] = atof(temp_str);
    }

  assert(feature_index == number_of_features);

  if (feature_index != number_of_features)
    {
      printf("Error! The number of features SVM is expecting is not "
        "equal to the number of features in the feature range file");
    }

  fclose(ifp);

}
