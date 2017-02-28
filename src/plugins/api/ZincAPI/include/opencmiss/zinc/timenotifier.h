/**
 * @file timenotifier.h
 *
 * The public interface to time notifier object which maintains conditions for
 * notification of time changes from a timekeeper.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TIMENOTIFIER_H__
#define CMZN_TIMENOTIFIER_H__

#include "types/timekeeperid.h"
#include "types/timenotifierid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The type used for time notifier callback. It is a pointer to a function which
 * takes the same arguments.
 *
 * @param cmzn_timenotifier_id  Handle to time notifier.
 * @param current_time  Time in the time notifier when the callback is being
 *    triggered by the time notifier.
 * @param User_data  any data user want to pass into the callback function.
 * @return  return one if such the callback function
 *    has been called successfully otherwise 0.
 */
typedef void (*cmzn_timenotifier_callback)(cmzn_timenotifierevent_id timenotifierevent,
	void *user_data);

/**
 * Get new handle to time notifier, incrementing reference count.
 *
 * @param timenotifier  Handle to time notifier.
 * @return  New handle to time notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timenotifier_id cmzn_timenotifier_access(
	cmzn_timenotifier_id timenotifier);

/**
 * Destroys handle to the time notifier (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param timenotifier_address  The address to the handle to time notifier
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timenotifier_destroy(cmzn_timenotifier_id *timenotifier_address);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_timenotifier_set_callback
 *
 * @see cmzn_timenotifier_set_callback
 * @param timenotifier  Handle to the time notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_timenotifier_get_callback_user_data(cmzn_timenotifier_id timenotifier);

/**
 * Set the callback function and user_data for the time notifier.
 * This function also activates callbacks.
 *
 * @see cmzn_timenotifier_callback_function
 * @param timenotifier  Handle to the time notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User is responsible for
 *   the life time of such object.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_timenotifier_set_callback(cmzn_timenotifier_id timenotifier,
	cmzn_timenotifier_callback function, void *user_data_in);

/**
 * Stop and clear selection callback. This will stop the callback and also
 * remove the callback function from the selection notifier.
 *
 * @param selectionnotifier  Handle to the selection notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_timenotifier_clear_callback(cmzn_timenotifier_id timenotifier);

/**
 * Gets the current time from the time notifier.
 *
 * @param timenotifier  Handle to time notifier.
 * @return  Current time or 0 if invalid argument.
 */
ZINC_API double cmzn_timenotifier_get_time(cmzn_timenotifier_id timenotifier);

/**
 * Gets the next callback time required by this notifier. This function does
 * not take the maximum and minimum of the timekeeper into consideration.
 *
 * @see cmzn_timekeeper_get_next_callback_time
 *
 * @param timenotifier  The timenotifier to get next callback time for.
 *
 * @param direction  Enumeration indicating rather next forward/reverse time will be calculated.
 * @return  next callback time on success.
 */
ZINC_API double cmzn_timenotifier_get_next_callback_time(
	cmzn_timenotifier_id timenotifier,
	enum cmzn_timekeeper_play_direction play_direction);

/**
 * If the timenotifier is of regular type, then this function returns the derived
 * regular time notifier handle.
 *
 * @param timenotifier  The base timenotifier to be cast.
 * @return  Handle to derived time notifier regular, or NULL/invalid handle if
 * wrong type or failed.
 */
ZINC_API cmzn_timenotifier_regular_id cmzn_timenotifier_cast_regular(
	cmzn_timenotifier_id timenotifier);

/**
 * Cast regular timenotifier back to its base timenotifier and return the timenotifier.
 * IMPORTANT NOTE: Returned timenotifier does not have incremented reference count and
 * must not be destroyed. Use cmzn_timenotifier_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the argument.
 *
 * @param timenotifier_regular  Handle to the timenotifier regular to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_timenotifier_id cmzn_timenotifier_regular_base_cast(
	cmzn_timenotifier_regular_id timenotifier_regular)
{
	return (cmzn_timenotifier_id)(timenotifier_regular);
}

/**
 * Destroys handle to the regular timenotifier (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param regular_timenotifier_address  Address of handle to the regular timenotifier.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timenotifier_regular_destroy(
	cmzn_timenotifier_regular_id *timenotifier_regular_address);

/**
 * This controls the rate which the time dependent object is called back.
 * The default value is 10 which means time notifier will receive 10 callbacks
 * per unit of time in the time keeper.
 * i.e. If  the update frequency of
 * time notifier is set to be 10, the actual interval between each callbacks is:
 * 1/(update frequency) which is 0.1s.
 * Note that the time notifier does not promise to receive callback exactly
 * 0.1s after the previous callback.
 *
 * @param timenotifier_regular  Handle to regular time notifier.
 * @param frequency  The number of times which time notifier will receive
 * callback per unit of time in the time keeper. Must be positive.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timenotifier_regular_set_frequency(
	cmzn_timenotifier_regular_id timenotifier_regular, double frequency);

/**
 * Sets the offset of times when a regular time notifier notifies clients.
 * Notification occurs at the given time_offset and every mulitple of 1/freqency
 * away from the offset.
 *
 * @param timenotifier_regular  Handle to regular time notifier.
 * @param time_offset  This set the time that notifier will receive callback.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timenotifier_regular_set_offset(
	cmzn_timenotifier_regular_id timenotifier_regular, double time_offset);

/**
 * Returns a new handle to the time notifier event with reference count incremented.
 *
 * @param timenotifierevent  The time notifier event to obtain a new handle to.
 * @return  New handle to time notifier event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timenotifierevent_id cmzn_timenotifierevent_access(
	cmzn_timenotifierevent_id timenotifierevent);

/**
 * Destroys this handle to the time notifier event (and sets it to NULL).
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param timenotifierevent_address  Address of timenotifier event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_timenotifierevent_destroy(cmzn_timenotifierevent_id *timenotifierevent_address);

/**
 * Get the time when this timenotifier event is triggered.
 *
 * @param timenotifierevent  Handle to the timenotifier event.
 * @return  The time when this timenotifier event is triggered
 */
ZINC_API double cmzn_timenotifierevent_get_time(cmzn_timenotifierevent_id timenotifierevent);

#ifdef __cplusplus
}
#endif

#endif
