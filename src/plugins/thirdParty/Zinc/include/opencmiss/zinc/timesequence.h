/**
 * @file timesequence.h
 *
 * The public interface to time sequence which represents a sequence of times
 * usually used to index node parameters.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TIMESEQUENCE_H__
#define CMZN_TIMESEQUENCE_H__

#include "types/fieldmoduleid.h"
#include "types/timesequenceid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Finds or creates a time sequence in the field module which matches the
 * sequence of times provided.
 *
 * @param field_module  The field module to search or create in.
 * @param number_of_times  The size of the times array.
 * @param times  Array of times. Note later times must not be less than earlier
 * times.
 * @return  Handle to time sequence matching times array, or NULL/invalid handle
 * on failure.
 */
ZINC_API cmzn_timesequence_id cmzn_fieldmodule_get_matching_timesequence(
	cmzn_fieldmodule_id field_module, int number_of_times, const double *times);

/**
 * Returns a new handle to the time sequence with reference count
 * incremented.
 *
 * @param timesequence  Handle to time sequence.
 * @return  New time sequence reference with incremented reference count.
 * @return  New handle to time sequence matching the times array, or
 * NULL/invalid handle on failure.
 */
ZINC_API cmzn_timesequence_id cmzn_timesequence_access(
	cmzn_timesequence_id timesequence);

/**
 * Destroys handle to the time sequence and sets pointer/handle to NULL.
 * Internally this decrements the reference count.
 *
 * @param timesequence_address  Address of time sequence handle.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_timesequence_destroy(cmzn_timesequence_id *timesequence_address);

/**
 * Gets the number of times in the time sequence.
 *
 * @param timesequence  The time sequence to query.
 * @return  The number of times.
 */
ZINC_API int cmzn_timesequence_get_number_of_times(
	cmzn_timesequence_id timesequence);

/**
 * Gets the time at the given time_index in the time sequence.
 *
 * @param timesequence  The time sequence to query.
 * @param time_index  The index of the time to get, starting at 1.
 * @return  The time or 0 if error.
 */
ZINC_API double cmzn_timesequence_get_time(cmzn_timesequence_id timesequence,
	int time_index);

/**
 * Sets the time for the given time_index in the time sequence.
 * This can only be done while the time sequence is not in use by other objects.
 * If the sequence does not have as many times as the <time_index> then it will
 * be expanded and the unspecified times also set to <time>.
 *
 * @param timesequence  The time sequence to modify.
 * @param time_index  The index of the time to set, starting at 1.
 * @param time  The time to set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timesequence_set_time(cmzn_timesequence_id timesequence,
	int time_index, double time);

#ifdef __cplusplus
}
#endif

#endif
