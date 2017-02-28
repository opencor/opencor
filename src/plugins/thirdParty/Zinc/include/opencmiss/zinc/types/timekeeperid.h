/**
 * @file timekeeperid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_TIMEKEEPERID_H__
#define CMZN_TIMEKEEPERID_H__

/**
 * Specifies the direction of play when querying the next callback time
 * preferred for internal zinc objects.
 * @see cmzn_timekeeper_get_next_callback_time
 */
enum cmzn_timekeeper_play_direction
{
	CMZN_TIMEKEEPER_PLAY_DIRECTION_INVALID = 0,
	/*!< Unspecified play direction */
	CMZN_TIMEKEEPER_PLAY_DIRECTION_FORWARD = 1,
	/*!< Specify the time keeper to progress at increasing time value */
	CMZN_TIMEKEEPER_PLAY_DIRECTION_REVERSE = 2
	/*!< Specify the time keeper to progress at decreasing time value */
};

/**
 * @brief The timekeeper synchronises a current time across Zinc objects.
 *
 * The timekeeper maintains a current time that is communicated to objects
 * within Zinc. For example, time-varying graphics are shown at the current
 * timekeeper time; also, the time value field allows the current timekeeper
 * time to be used in field expressions.
 * Responsibility for timing animations is left to client code (as it is UI
 * or OS-dependent), however the timekeeper API assists clients to determine
 * the ideal callback time and to set the current time just before
 * rendering to synchronise animation.
 * Time notifiers allow clients to also use the timekeeper features including
 * callbacks.
 *
 * @see cmzn_timekeeper_get_next_callback_time
 * @see cmzn_timekeeper_set_time
 * @see cmzn_timenotifier_id
 */
struct cmzn_timekeeper;
typedef struct cmzn_timekeeper *cmzn_timekeeper_id;

/**
 * @brief Module for finding and managing timekeepers.
 *
 * Module for finding and managing timekeepers.
 */
struct cmzn_timekeepermodule;
typedef struct cmzn_timekeepermodule * cmzn_timekeepermodule_id;

#endif
