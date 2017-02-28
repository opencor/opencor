/**
 * @file selectionid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SELECTIONID_H__
#define CMZN_SELECTIONID_H__

/**
 * @brief Manages individual user notification of changes to the selection group.
 *
 * Manages individual user notification of changes to the selection group in a
 * scene.
 */
struct cmzn_selectionnotifier;
typedef struct cmzn_selectionnotifier * cmzn_selectionnotifier_id;

/**
 * @brief Information about changes to the selection group in the scene.
 *
 * Information about changes to the selection group in the scene, sent with
 * each callback from the selection notifier.
 */
struct cmzn_selectionevent;
typedef struct cmzn_selectionevent * cmzn_selectionevent_id;

typedef void (*cmzn_selectionnotifier_callback_function)(
	cmzn_selectionevent_id selectionevent, void *client_data);

/**
 * Bit flags summarising changes to the selection.
 */
enum cmzn_selectionevent_change_flag
{
	CMZN_SELECTIONEVENT_CHANGE_FLAG_NONE = 0,      /*!< no change */
	CMZN_SELECTIONEVENT_CHANGE_FLAG_ADD = 1,       /*!< one or more objects added */
	CMZN_SELECTIONEVENT_CHANGE_FLAG_REMOVE = 2,    /*!< one or more objects removed */
	CMZN_SELECTIONEVENT_CHANGE_FLAG_FINAL = 32768  /*!< final notification: owning object destroyed */
};

/**
 * Type for passing logical OR of #cmzn_selectionevent_change_flag
 */
typedef int cmzn_selectionevent_change_flags;

#endif /* CMZN_SELECTION_ID_H */
