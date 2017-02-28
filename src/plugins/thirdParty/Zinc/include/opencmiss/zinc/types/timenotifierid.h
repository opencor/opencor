/**
 * @file timenotifierid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_TIMENOTIFIERID_H__
#define CMZN_TIMENOTIFIERID_H__

/**
 * @brief Description of timing requirements and for setting up time callbacks.
 *
 * A timenotifier informs zinc of the timing requirements of a client and can
 * be set to receive callbacks when the time changes.
 * @see cmzn_timekeeper_create_timenotifier_regular function.
 */
struct cmzn_timenotifier;
typedef struct cmzn_timenotifier *cmzn_timenotifier_id;

/**
 * @brief Information about changes to the timekeeper/time.
 *
 * Information about changes to the timekeeper/time, sent with each callback
 * from the time notifier.
 */
struct cmzn_timenotifierevent;
typedef struct cmzn_timenotifierevent * cmzn_timenotifierevent_id;

/**
 * @brief A derived timenotifier type which requests notifications at regular intervals.
 *
 * A derived timenotifier type which requests notifications at regular intervals.
 */
struct cmzn_timenotifier_regular;
typedef struct cmzn_timenotifier_regular *cmzn_timenotifier_regular_id;

#endif
