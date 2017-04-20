/**
 * @file timekeeper.h
 *
 * The public interface of timekeeper which maintains a current time and manages
 * several timenotifiers.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TIMEKEEPER_H__
#define CMZN_TIMEKEEPER_H__

#include "types/timenotifierid.h"
#include "types/timekeeperid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the timekeeper module with reference count
 * incremented.
 *
 * @param timekeepermodule  Handle to timekeeper module.
 * @return  New handle to timekeeper module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timekeepermodule_id cmzn_timekeepermodule_access(
	cmzn_timekeepermodule_id timekeepermodule);

/**
* Destroys handle to the timekeeper module (and sets it to NULL).
* Internally this decrements the reference count.
*
* @param timekeepermodule_address  Address of handle to timekeeper module
*   to destroy.
* @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
*/
ZINC_API int cmzn_timekeepermodule_destroy(
	cmzn_timekeepermodule_id *timekeepermodule_address);

/**
 * Get the default timekeeper.
 *
 * @param timekeepermodule  Handle to timekeeper module to query.
 * @return  Handle to default timekeeper, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timekeeper_id cmzn_timekeepermodule_get_default_timekeeper(
	cmzn_timekeepermodule_id timekeepermodule);

/**
 * Read the json description to the timekeeper module. This will change
 * the timekeeper in the timekeeper module.
 *
 * @param timekeepermodule  Handle to the timekeeper module.
 * @description  The string containing json description
 * @return  CMZN_OK on success, otherwise ERROR status.
 */
ZINC_API int cmzn_timekeepermodule_read_description(
	cmzn_timekeepermodule_id timekeepermodule, const char *description);

/**
 * Write the json file describing the timekeepers in the timekeeper module,
 * which can be used to store the current timekeeper settings.
 *
 * @param timekeepermodule  Handle to the timekeeper module.
 * @return  c string containing the json description of timekeeper module,
 * 	otherwise 0;
 */
ZINC_API char *cmzn_timekeepermodule_write_description(
	cmzn_timekeepermodule_id timekeepermodule);

/**
 * Get new handle to timekeeper, incrementing reference count.
 *
 * @param timekeeper  Handle to timekeeper.
 * @return  New handle to timekeeper, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timekeeper_id cmzn_timekeeper_access(cmzn_timekeeper_id timekeeper);

/**
 * Destroys handle to the time keeper (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param timekeeper_address  The address to the handle to time keeper
 * @return  Status CMZN_OK if successfully destroy the time keeper,
 * any other value on failure.
 */
ZINC_API int cmzn_timekeeper_destroy(cmzn_timekeeper_id *timekeeper_address);

/**
 * Create and returns a time notifier with regular update time in time keeper.
 * The returned time notifier will automatically be added to the time keeper.
 *
 * @param update_frequency  The number of times which time notifier will receive
 *    callback per unit of time in the time keeper.
 * @param time_offset  This value will set the exact time the notification
 *    happenes and allow setting the callback time other than t=0.
 *    Time notifier will receive/send out notification when
 *    time_offset + original callback time is reached.
 * @return  Handle to new timekeeper, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timenotifier_id cmzn_timekeeper_create_timenotifier_regular(
	cmzn_timekeeper_id timekeeper, double update_frequency, double time_offset);

/**
 * Gets the maximum time in the timekeeper.
 *
 * @param timekeeper  The timekeeper to query.
 * @return  Current time or 0 if invalid argument.
 */
ZINC_API double cmzn_timekeeper_get_maximum_time(cmzn_timekeeper_id timekeeper);

/**
 * Sets the maximum time in the timekeeper.
 *
 * @param timenotifier  The timekeeper to modify.
 * @param maximum_time  The new maximum time.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timekeeper_set_maximum_time(cmzn_timekeeper_id timekeeper, double maximum_time);

/**
 * Gets the minimum time in the timekeeper.
 *
 * @param timekeeper  The timekeeper to query.
 * @return  Current time or 0 if invalid argument.
 */
ZINC_API double cmzn_timekeeper_get_minimum_time(cmzn_timekeeper_id timekeeper);

/**
 * Sets the minimum time in the timekeeper.
 *
 * @param timenotifier  The timekeeper to modify.
 * @param minimum_time  The new minimum time.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timekeeper_set_minimum_time(cmzn_timekeeper_id timekeeper, double minimum_time);

/**
 * Gets the current time from the timekeeper.
 *
 * @param timekeeper  The timekeeper to query.
 * @return  Current time or 0 if invalid argument.
 */
ZINC_API double cmzn_timekeeper_get_time(cmzn_timekeeper_id timekeeper);

/**
 * Sets the current time in the timekeeper. Timenotifiers are informed of the
 * time change.
 *
 * @param timekeeper  The timekeeper to modify.
 * @param time  The new current time.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_timekeeper_set_time(cmzn_timekeeper_id timekeeper, double time);

/**
 * Gets the next callback time required by any of the time notifiers in the
 * timekeeper. This function takes the minimum and maximum time into consideration.
 * If the direction of playback is forward and next callback time exceeds the maximum time,
 * the next callback time will be timekeeper_minimum + (callback_time - timekeeper_maximum).
 * If the direction of playback is reverse and next callback time is smaller than the
 * minimum time, the next callback time will be timekeeper_maximum +
 * (timekeeper_minimum - callback_time).
 *
 * @see cmzn_timenotifier_get_next_callback_time
 *
 * @param timekeeper  The timekeeper to get next callback time.
 *
 * @param direction  Enumeration indicating rather next forward/reverse time will be calculated.
 * @return  next callback time on success.
 */
ZINC_API double cmzn_timekeeper_get_next_callback_time(cmzn_timekeeper_id timekeeper,
	enum cmzn_timekeeper_play_direction direction);

#ifdef __cplusplus
}
#endif

#endif
