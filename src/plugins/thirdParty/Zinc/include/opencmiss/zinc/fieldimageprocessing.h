/**
 * @file fieldimageprocessing.h
 *
 * Implements zinc fields which deal with image processing
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDIMAGEPROCESSING_H__
#define CMZN_FIELDIMAGEPROCESSING_H__

#include "types/fieldid.h"
#include "types/fieldimageprocessingid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field performing ITK binary dilate image filter on scalar source
 * field image. Sets number of components to same number as <source_field>.
 * The <radius> and <dilate_value> specify the radius of pixels to use
 * for dilation and what pixel value to use for dilation
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_binary_dilate(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	int radius, double dilate_value);

/**
 * Creates a field performing ITK binary erode image filter on scalar source
 * field image. Sets number of components to same number as <source_field>.
 * The <radius> and <erode_value> specify the radius of pixels to use
 * for dilation and what pixel value to use for dilation
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_binary_erode(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	int radius, double erode_value);

/**
 * Creates a field which applies an ITK binary threshold image filter on source.
 * The newly created field consists of binary values (either 0 or 1) which are
 * determined by applying the threshold range to the source field.
 * Input values with an intensity range between lower_threshold and the
 * upper_threshold are set to 1, the rest are set to 0.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  The field to be filtered
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_binary_threshold(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * If field can be cast to an imagefilter binary threshold do so and return a
 * handle to the derived field.
 *
 * @param field  Id of the field to cast
 * @return  Handle to derived image filter binary threshold field, or
 * NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_imagefilter_binary_threshold_id
	cmzn_field_cast_imagefilter_binary_threshold(cmzn_field_id field);

/**
 * Get the lower threshold value for this image filter.
 *
 * @param imagefilter_binary_threshold  handle of the binary threshold image filter.
 * @return  the lower threshold set for this filter.
 */
ZINC_API double cmzn_field_imagefilter_binary_threshold_get_lower_threshold(
	cmzn_field_imagefilter_binary_threshold_id imagefilter_binary_threshold);

/**
 *  Set the lower threshold value for this image filter.
 *
 * @param imagefilter_binary_threshold  handle of the binary threshold image filter.
 * @param lower_threshold  Threshold value below which all pixels are set to 0
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_binary_threshold_set_lower_threshold(
	cmzn_field_imagefilter_binary_threshold_id imagefilter_binary_threshold,
	double lower_threshold);

/**
 *  Get the upper threshold value for this image filter.
 *
 * @param imagefilter_binary_threshold  handle of the binary threshold image filter.
 * @return  the upper threshold set for this filter.
 */
ZINC_API double cmzn_field_imagefilter_binary_threshold_get_upper_threshold(
	cmzn_field_imagefilter_binary_threshold_id imagefilter_binary_threshold);

/**
 *  Set the upper threshold value for this image filter.
 *
 * @param imagefilter_binary_threshold  handle of the binary threshold image filter.
 * @param upper_threshold  Threshold value above which all pixels are set to 0
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_binary_threshold_set_upper_threshold(
	cmzn_field_imagefilter_binary_threshold_id imagefilter_binary_threshold,
	double upper_threshold);

/**
 * Cast find_mesh_location field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param imagefilter_binary_threshold  Handle to the imagefilter_binary_threshold field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_imagefilter_binary_threshold_base_cast(
	cmzn_field_imagefilter_binary_threshold_id imagefilter_binary_threshold)
{
	return (cmzn_field_id)(imagefilter_binary_threshold);
}

/**
 * Destroys handle to the imagefilter_binary_threshold field and sets it
 * to NULL. Internally this decrements the reference count.
 *
 * @param imagefilter_binary_threshold_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_binary_threshold_destroy(
	cmzn_field_imagefilter_binary_threshold_id *imagefilter_binary_threshold_address);

/**
 * Creates a field returning result of ITK canny edge detection filter on the
 * source field image. Sets number of components to same number as source field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_canny_edge_detection(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	double variance, double maximumError,
  double upperThreshold, double lowerThreshold);

/**
 * Creates a field performing ITK connected threshold image filter on scalar
 * source field image. Sets number of components to same number as source field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_connected_threshold(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	double lower_threshold, double upper_threshold, double replace_value,
	int num_seed_points, int dimension, const double *seed_points);

/**
 * Creates a field performing ITK curvature anisotropic diffusion image filter
 * on scalar source field image.
 * Sets number of components to same number as <source_field>.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  The field to be filtered
 * @param timeStep  The time step
 * @param conductance  The conductance
 * @param numIterations  The number of iterations to be performed
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_curvature_anisotropic_diffusion(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	double timeStep, double conductance, int numIterations);

/**
 * Creates a field applying the ITK discrete gaussian image filter to the source
 * field. This means that each pixel value in the new field
 * is based on a weighted average of the pixel and the surrounding pixel values
 * from the source field. Pixels further away are given a lower weighting.
 * Increasing the variance increases the width of the gaussian distribution
 * used and hence the number of pixels used to calculate the weighted average.
 * This smooths the image more.  A limit is set on the max_kernel_width used
 * to approximate the guassian to ensure the calculation completes.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  The field to be filtered
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_discrete_gaussian(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * If field can be cast to an imagefilter discrete gaussian do so and return
 * a handle to the derived field.
 *
 * @param field Id of the field to cast
 * @return  Handle to derived image filter discrete gaussian field, or
 * NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_imagefilter_discrete_gaussian_id
	cmzn_field_cast_imagefilter_discrete_gaussian(cmzn_field_id field);

/**
 * Get the variance value for this image filter.
 *
 * @param imagefilter_threshold  handle of the threshold image filter.
 * @return  the current variance value set on this filter.
 */
ZINC_API double cmzn_field_imagefilter_discrete_gaussian_get_variance(
	cmzn_field_imagefilter_discrete_gaussian_id imagefilter_discrete_gaussian);

/**
 * Set the variance for this image filter.
 *
 * Increasing the variance increases the width of the gaussian distribution
 * used and hence the number of pixels used to calculate the weighted average.
 * This smooths the image more.
 *
 * @param imagefilter_discrete_gaussian  handle of the discrete gaussian image filter.
 * @param variance  The variance of the gaussian distribution used in the filter
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_discrete_gaussian_set_variance(
	cmzn_field_imagefilter_discrete_gaussian_id imagefilter_discrete_gaussian,
	double variance);

/**
 * Get the max kernel width for this image filter.
 *
 * @param imagefilter_threshold  handle of the threshold image filter.
 * @return  the current variance value set on this filter.
 */
ZINC_API int cmzn_field_imagefilter_discrete_gaussian_get_max_kernel_width(
	cmzn_field_imagefilter_discrete_gaussian_id imagefilter_discrete_gaussian);

/**
 * Set the max kernel width for this image filter.
 *
 * The max kernel width is a limit is set on the filter
 * to approximate the guassian to ensure the calculation completes.
 *
 * @param imagefilter_discrete_gaussian  handle of the discrete gaussian image filter.
 * @param max_kernel_width  The limit on the maximum kernel width that may be used
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_discrete_gaussian_set_max_kernel_width(
	cmzn_field_imagefilter_discrete_gaussian_id imagefilter_discrete_gaussian,
	int max_kernel_width);

/**
 * Cast imagefilter_discrete_gaussian field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param imagefilter_discrete_gaussian  Handle to the imagefilter_discrete_gaussian field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_imagefilter_discrete_gaussian_base_cast(
	cmzn_field_imagefilter_discrete_gaussian_id imagefilter_discrete_gaussian)
{
	return (cmzn_field_id)(imagefilter_discrete_gaussian);
}

/**
 * Destroys handle to the imagefilter_discrete_gaussian field and sets it
 * to NULL. Internally this decrements the reference count.
 *
 * @param imagefilter_discrete_gaussian_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_discrete_gaussian_destroy(
		cmzn_field_imagefilter_discrete_gaussian_id *imagefilter_discrete_gaussian_address);

/**
 * Creates a field performing ITK gradient magnitude recursive gaussian image
 * filter on scalar source field image.
 * Sets number of components to same number as <source_field>.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_gradient_magnitude_recursive_gaussian(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	double sigma);

/**
 * Creates a field performing ITK mean image filter on source_field image.
 * Sets number of components to same number as <source_field>.
 *
 * @param field_module  The field module for the region to own the new field.
 * @param source_field  The source field to be processed.
 * @param radius_sizes_count  The size of the radius sizes array.
 * @param radius_sizes  Array of radius sizes to use in each image axis.
 * If there are fewer sizes than image dimension the last value is used on
 * subsequent image axes.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_mean(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	int radius_sizes_count, const int *radius_sizes);

/**
 * Creates a field performing ITK rescale intensity image filter on scalar
 * source field image. Sets number of components to same number as source field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_rescale_intensity(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	double outputMin, double outputMax);

/**
 * Creates a field performing ITK sigmoid image filter on scalar source field
 * image. Sets number of components to same number as <source_field>.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_sigmoid(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	double min, double max,	double alpha, double beta);

/**
 * Creates a field applying the ITK threshold image filter to the source field.
 * The newly created field replaces certain values with a specified outside
 * value, based on which threshold mode and the threshold values.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field The field to be filtered
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_threshold(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * If field can be cast to an imagefilter threshold do so and return a
 * handle to the derived field.
 *
 * @param field Id of the field to cast
 * @return  Handle to derived image filter threshold field, or
 * NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_imagefilter_threshold_id cmzn_field_cast_imagefilter_threshold(cmzn_field_id field);

/**
 * Get the threshold condition for this image filter.
 *
 * @param imagefilter_threshold  handle of the threshold image filter.
 * @return  The current threshold condition, otherwise CONDITION_INVALID.
 */
ZINC_API enum cmzn_field_imagefilter_threshold_condition
	cmzn_field_imagefilter_threshold_get_condition(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold);

/**
 * Set the threshold condition for this image filter.
 * @see cmzn_field_imagefilter_threshold_condition
 *
 * @param imagefilter_threshold  handle of the threshold image filter.
 * @param condition  The threshold condition to be true for pixel values to be
 * assigned to the outside value: either BELOW, ABOVE or OUTSIDE.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_threshold_set_condition(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold,
	enum cmzn_field_imagefilter_threshold_condition condition);

/**
 *  Get the outside value for this image filter.
 *
 * @param imagefilter_threshold  The threshold image filter to query.
 * @return  The current outside value mode set on this filter.
 */
ZINC_API double cmzn_field_imagefilter_threshold_get_outside_value(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold);

/**
 * Set the outside value for this image filter.
 *
 * @param imagefilter_threshold  The threshold image filter to modify.
 * @param outside_value  The value to replace all thresholded values with.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_threshold_set_outside_value(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold, double outside_value);

/**
 * Get the lower threshold pixel value.
 *
 * @param imagefilter_threshold  The threshold image filter to query.
 * @return  The current lower threshold value, or 0.0 on error.
 */
ZINC_API double cmzn_field_imagefilter_threshold_get_lower_threshold(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold);

/**
 * Set the lower threshold value for this image filter. Depending on the
 * condition this sets the lowest pixel value below which the outside value
 * is set.
 *
 * @param imagefilter_threshold  The threshold image filter to modify.
 * @param lower_value  The lower pixel value to be set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_threshold_set_lower_threshold(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold, double lower_value);

/**
 * Get the upper threshold pixel value.
 *
 * @param imagefilter_threshold  The threshold image filter to query.
 * @return  The current upper threshold value, or 0.0 on error.
 */
ZINC_API double cmzn_field_imagefilter_threshold_get_upper_threshold(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold);

/**
 * Set the upper threshold value for this image filter. Depending on the
 * condition this sets the highest pixel value above which the outside value
 * is set.
 *
 * @param imagefilter_threshold  The threshold image filter to modify.
 * @param upper_value  The upper pixel value to be set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_threshold_set_upper_threshold(
	cmzn_field_imagefilter_threshold_id imagefilter_threshold, double upper_value);

/**
 * Cast imagefilter_threshold field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param imagefilter_discrete_gaussian  Handle to the imagefilter_discrete_gaussian field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_imagefilter_threshold_base_cast(
		cmzn_field_imagefilter_threshold_id imagefilter_threshold)
{
	return (cmzn_field_id)(imagefilter_threshold);
}

/**
 * Destroys handle to the imagefilter_threshold field and sets it
 * to NULL. Internally this decrements the reference count.
 *
 * @param imagefilter_threshold_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_threshold_destroy(
		cmzn_field_imagefilter_threshold_id *imagefilter_threshold_address);

/**
 * Creates a field performing ITK histogram image filter on source field image.
 * If neither histogramMinimum or histogramMaximum are specified then the minimums and
 * maximums are calculated based on the minimum and maximum values in the input image.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field The field to generate the histogram for.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_imagefilter_histogram(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * If field can be cast to an imagefilter histogram do so and return a
 * handle to the derived field.
 *
 * @param field Id of the field to cast
 * @return  Handle to derived image filter histogram field, or
 * NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_imagefilter_histogram_id cmzn_field_cast_imagefilter_histogram(cmzn_field_id field);

/**
 * Gets the minimum values to be computed for generating the histogram.
 *
 * @see cmzn_field_imagefilter_histogram_set_compute_minimum_values
 * @param imagefilter_histogram  The image filter to query.
 * @param valuesCount  The size of the minimum_values array to fill. Values
 * for component beyond the size set use the last value.
 * @param valuesOut  Array to receive minimum_values.
 * @return  The actual number of minimum_values, this should be the same
 * 	as the number of source components Returns 0 on error.
 */
ZINC_API  int cmzn_field_imagefilter_histogram_get_compute_minimum_values(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	int valuesCount, double *valuesOut);

/**
 * Sets the minimum values to be computed for generating the histogram.
 * The default minimum values for new histogram filter is 0.0 for each source
 * component.
 * Note: The value set for the last source component applies to higher order
 *  components.
 *
 * @param imagefilter_histogram  The image filter to modify.
 * @param valuesCount  The size of the valuesIn array, >= 1.
 * @param valuesIn  Array of minimum values (>=1) for each source component,
 * with the last number in array applying to all higher order components.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_histogram_set_compute_minimum_values(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	int valuesCount, const double *valuesIn);

/**
 * Gets the maximum values to be computed for generating the histogram.
 *
 * @see cmzn_field_imagefilter_histogram_set_compute_maximum_values
 * @param imagefilter_histogram  The image filter to query.
 * @param valuesCount  The size of the maximum_values array to fill. Values
 * for component beyond the size set use the last value.
 * @param valuesOut  Array to receive maximum_values.
 * @return  The actual number of maximum_values, this should be the same
 * 	as the number of source components Returns 0 on error.
 */
ZINC_API  int cmzn_field_imagefilter_histogram_get_compute_maximum_values(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	int valuesCount, double *valuesOut);

/**
 * Sets the maximum values to be computed for generating the histogram.
 * The default maximum values for new histogram filter is 1.0 for each source
 * component.
 * Note: The value set for the last source component applies to higher order
 *  components.
 *
 * @param imagefilter_histogram  The image filter to modify.
 * @param valuesCount  The size of the valuesIn array, >= 1.
 * @param valuesIn  Array of maximum values (>=1) for each source component,
 * with the last number in array applying to all higher order components.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_histogram_set_compute_maximum_values(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	int valuesCount, const double *valuesIn);

/**
 * Gets the number of bins used to generate histogram. Higher number of bins
 * increase the density and gives greater precision to the density estimation.
 *
 * @see cmzn_field_imagefilter_histogram_set_number_of_bins
 * @param imagefilter_histogram  The image filter to query.
 * @param valuesCount  The size of the number_of_bins array to fill. Values
 * for component beyond the size set use the last value.
 * @param valuesOut  Array to receive numbers of bins.
 * @return  The actual number of number of bin values, this should be the same
 * 	as the number of source components Returns 0 on error.
 */
ZINC_API int cmzn_field_imagefilter_histogram_get_number_of_bins(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	int valuesCount, int *valuesOut);

/**
 * Sets the number of bins used to generate histogram. Higher number of bins
 * increase the density and gives greater precision to the density estimation.
 * This value can be set for each source field component.
 * The default number of bins for new histogram filter is 64 for each source
 * component.
 * Note: The value set for the last source component applies to higher order
 *  components.
 *
 * @param imagefilter_histogram  The image filter to modify.
 * @param valuesCount  The size of the valuesIn array, >= 1.
 * @param valuesIn  Array of number of bins (>=1) for each source component, with
 * 	the last number in array applying to all higher order
 *  	components.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_histogram_set_number_of_bins(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	int valuesCount, const int *valuesIn);

/**
 * Get the marginal scale value for this image filter. The Marginal
 * scale is used to define a fraction of the bin width to be used as a
 * tolerance around the upper bound of the bin.
 *
 * @see cmzn_field_imagefilter_histogram_set_marginal_scale
 * @param imagefilter_histogram  The histogram image filter to query.
 * @return  The current marginal value, or 0.0 on error.
 */
ZINC_API double cmzn_field_imagefilter_histogram_get_marginal_scale(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram);

/**
 * Set the marginal scale value for this image filter. The Marginal
 * scale is used to define a fraction of the bin width to be used as a
 * tolerance around the upper bound of the bin.
 * The default marginal_scale for new histogram filter is 10.
 *
 * @param imagefilter_histogram  The histogram image filter to modify.
 * @param marginal_scale  The marginal_scale value to be set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_imagefilter_histogram_set_marginal_scale(
	cmzn_field_imagefilter_histogram_id imagefilter_histogram,
	double marginal_scale);

/**
 * Cast imagefilter_histogram field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param imagefilter_discrete_gaussian  Handle to the imagefilter_discrete_gaussian field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_imagefilter_histogram_base_cast(
		cmzn_field_imagefilter_histogram_id imagefilter_histogram)
{
	return (cmzn_field_id)(imagefilter_histogram);
}

/**
 * Destroys handle to the imagefilter_histogram field and sets it
 * to NULL. Internally this decrements the reference count.
 *
 * @param imagefilter_histogram_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_imagefilter_histogram_destroy(
		cmzn_field_imagefilter_histogram_id *imagefilter_histogram_address);

#ifdef __cplusplus
}
#endif

#endif
