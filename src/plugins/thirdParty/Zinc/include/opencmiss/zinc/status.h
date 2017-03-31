/**
 * @file status.h
 *
 * C function status/error codes.
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_STATUS_H__
#define CMZN_STATUS_H__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generic status codes returned by API functions to indicate success or error.
 * DEPRECATED: Please use 'result' codes defined in result header instead.
 * @see cmzn_result
 *
 * WARNING: Planned future binary compatibility break.
 * At a future date we will change the value of CMZN_OK to 0, and
 * CMZN_ERROR_GENERAL to some other value, to bring the Zinc API in to line with
 * common C API conventions. To maintain your source compatibility through this
 * break please ensure all code checking integer status codes returned by
 * functions compare against enumerated symbol names, not their current values.
 */
enum cmzn_status
{
	CMZN_ERROR_IN_USE = -8,
		/*!< Operation failed or partially completed due to object(s) being in use.
		 * e.g. returned when attempting to destroy node in use by elements */
	CMZN_ERROR_ARGUMENT_CONTEXT = -7,
		/*!< An argument is from a different Zinc context */
	CMZN_ERROR_NOT_IMPLEMENTED = -6,
		/*!< Cannot complete operation due to unimplemented feature */
	CMZN_ERROR_INCOMPATIBLE_DATA = -5,
		/*!< Incompatible data or object definitions prevented operation */
	CMZN_ERROR_ALREADY_EXISTS = -4,
		/*!< Object already exists or is present in owner/set */
	CMZN_ERROR_NOT_FOUND = -3,
		/*!< Object or resource not found */
	CMZN_ERROR_MEMORY = -2,
		/*!< Failed to allocate memory. */
	CMZN_ERROR_ARGUMENT = -1,
		/*!< Invalid argument(s) passed to API function. */
	CMZN_ERROR_GENERAL = 0,
		/*!< Unspecified error occurred. Can include invalid argument(s) for old APIs. */
	CMZN_OK = 1
		/*!< Successful return */
};

#ifdef __cplusplus
}
#endif

#endif
