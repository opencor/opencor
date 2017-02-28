/**
 * @file fieldsmoothing.h
 *
 * Public interface to field smoothing object used to specify algorithm and
 * parameters for smoothing a field.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDSMOOTHING_H__
#define CMZN_FIELDSMOOTHING_H__

#include "types/fieldsmoothingid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a new field smoothing object using the default algorithm.
 * @see cmzn_fieldsmoothing_algorithm
 * @see cmzn_fieldsmoothing_set_algorithm
 * @see cmzn_field_smooth
 *
 * @param fieldmodule  Region field module owning the fields to be smoothed.
 * @return  Handle to new field smoothing, or NULL/invalid handle on failure.
*/
ZINC_API cmzn_fieldsmoothing_id cmzn_fieldmodule_create_fieldsmoothing(
	cmzn_fieldmodule_id fieldmodule);

/**
 * Returns a new handle to the field smoothing with reference count incremented.
 *
 * @param smoothing  The field smoothing to obtain a new handle to.
 * @return  New handle to field smoothing, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldsmoothing_id cmzn_fieldsmoothing_access(
	cmzn_fieldsmoothing_id smoothing);

/**
 * Destroys handle to the field smoothing, and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param smoothing_address  Address of handle to field smoothing to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldsmoothing_destroy(cmzn_fieldsmoothing_id *smoothing_address);

/**
 * Set the smoothing algorithm to be used for smoothing a field.
 *
 * @param fieldmodule  Region field module owning the fields to be smoothed.
 * @param algorithm  The smoothing algorithm to use.
 * @return  Handle to new field smoothing, or NULL/invalid handle on failure.
*/
ZINC_API int cmzn_fieldsmoothing_set_algorithm(
	cmzn_fieldsmoothing_id smoothing, enum cmzn_fieldsmoothing_algorithm algorithm);

/**
 * Sets the time at which to smooth the field, if time varying.
 *
 * @param smoothing  The field smoothing specification to modify.
 * @param time  The time value to be set.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldsmoothing_set_time(cmzn_fieldsmoothing_id smoothing,
	double time);

#ifdef __cplusplus
}
#endif

#endif
