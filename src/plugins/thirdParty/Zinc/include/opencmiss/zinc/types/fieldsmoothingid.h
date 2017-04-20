/**
 * @file fieldsmoothingid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDSMOOTHINGID_H__
#define CMZN_FIELDSMOOTHINGID_H__

/**
 * @brief Parameters for smoothing a field.
 * @see cmzn_field_smooth
 *
 * Specification of algorithm and parameters for smoothing a field.
 */
struct cmzn_fieldsmoothing;
typedef struct cmzn_fieldsmoothing *cmzn_fieldsmoothing_id;

/**
 * Algorithm for smoothing a field with derivative degrees of freedom.
 */
enum cmzn_fieldsmoothing_algorithm
{
	CMZN_FIELDSMOOTHING_ALGORITHM_INVALID = 0,
	/*!< Invalid algorithm */
	CMZN_FIELDSMOOTHING_ALGORITHM_AVERAGE_DELTA_DERIVATIVES_UNSCALED = 1
	/*!< Sets unit scale factors with first derivatives set to average of
	  nodal value deltas along neighbouring lines. Default algorithm. */
};

#endif
