/**
 * @file fieldimageprocessingid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDIMAGEPROCESSINGID_H__
#define CMZN_FIELDIMAGEPROCESSINGID_H__

/**
 * @brief Image processing derived field type performing the ITK binary threshold filter.
 *
 * Image processing derived field type performing the ITK binary threshold filter.
 */
struct cmzn_field_imagefilter_binary_threshold;
typedef struct cmzn_field_imagefilter_binary_threshold * cmzn_field_imagefilter_binary_threshold_id;

/**
 * @brief Image processing derived field type performing the ITK discrete gaussian filter.
 *
 * Image processing derived field type performing the ITK discrete gaussian filter.
 */
struct cmzn_field_imagefilter_discrete_gaussian;
typedef struct cmzn_field_imagefilter_discrete_gaussian * cmzn_field_imagefilter_discrete_gaussian_id;

/**
 * @brief Image processing derived field type performing the ITK threshold filter.
 *
 * Image processing derived field type performing the ITK threshold filter.
 */
struct cmzn_field_imagefilter_threshold;
typedef struct cmzn_field_imagefilter_threshold * cmzn_field_imagefilter_threshold_id;

/**
 * @brief Image processing derived field type performing the ITK histogram field.
 *
 * Image processing derived field type performing the ITK histogram field.
 */
struct cmzn_field_imagefilter_histogram;
typedef struct cmzn_field_imagefilter_histogram * cmzn_field_imagefilter_histogram_id;

/**
 * The condition to be true for pixel values to be assigned to the outside value
 * for the threshold image filter field.
 */
enum cmzn_field_imagefilter_threshold_condition
{
	CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_INVALID = 0,
		/*!< Unspecified imagefilter threshold condition */
	CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_ABOVE = 1,
		/*!< Assign where pixel values are greater than or equal to the upper threshold */
	CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_BELOW = 2,
		/*!< Assign where pixel values are less than or equal to lower threshold
		     This is the default condition for the threshold image filter. */
	CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_OUTSIDE = 3
		/*!< Assign where pixel values are outside the lower to upper threshold range */
};

#endif
