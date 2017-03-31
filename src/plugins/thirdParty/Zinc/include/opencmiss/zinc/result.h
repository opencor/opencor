/**
 * @file result.h
 *
 * Zinc C API function result/error codes.
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_RESULT_H__
#define CMZN_RESULT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Result / error codes returned by API functions.
 *
 * WARNING: Planned future binary compatibility break.
 * At a future date we will change the value of RESULT_OK to 0, and renumber
 * RESULT_ERROR_GENERAL and other values. This will bring the Zinc API in to
 * line with common C API conventions. To maintain your source compatibility
 * through this break please ensure all code checking results codes returned by
 * functions compare against enumerated symbol names, not their current values.
 */
enum cmzn_result
{
	CMZN_RESULT_ERROR_IN_USE = -8,
		/*!< Operation failed or partially completed due to object(s) being in use.
		 * e.g. returned when attempting to destroy node in use by elements */
	CMZN_RESULT_ERROR_ARGUMENT_CONTEXT = -7,
		/*!< An argument is from a different Zinc context */
	CMZN_RESULT_ERROR_NOT_IMPLEMENTED = -6,
		/*!< Cannot complete operation due to unimplemented feature */
	CMZN_RESULT_ERROR_INCOMPATIBLE_DATA = -5,
		/*!< Incompatible data or object definitions prevented operation */
	CMZN_RESULT_ERROR_ALREADY_EXISTS = -4,
		/*!< Object already exists or is present in owner/set */
	CMZN_RESULT_ERROR_NOT_FOUND = -3,
		/*!< Object or resource not found */
	CMZN_RESULT_ERROR_MEMORY = -2,
		/*!< Failed to allocate memory */
	CMZN_RESULT_ERROR_ARGUMENT = -1,
		/*!< Invalid argument(s) passed to API function */
	CMZN_RESULT_ERROR_GENERAL = 0,
		/*!< Unspecified error occurred. Can include invalid argument(s) for old APIs */
	CMZN_RESULT_OK = 1
		/*!< Successful result */
};

#ifdef __cplusplus
}
#endif

#endif
