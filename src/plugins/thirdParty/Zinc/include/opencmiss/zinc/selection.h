/**
 * @file selection.h
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SELECTION_H__
#define CMZN_SELECTION_H__

#include "types/selectionid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the selection notifier with reference count incremented.
 *
 * @param selectionnotifier  The selection notifier to obtain a new handle to.
 * @return  New handle to new selection notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_selectionnotifier_id cmzn_selectionnotifier_access(
	cmzn_selectionnotifier_id selectionnotifier);

/**
 * Destroys this handle to the selection notifier (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param selectionnotifier  Handle to the selection notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_selectionnotifier_destroy(cmzn_selectionnotifier_id *selectionnotifier);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_selectionnotifier_set_callback
 *
 * @see cmzn_selectionnotifier_set_callback
 * @param selectionnotifier  Handle to the selection notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_selectionnotifier_get_callback_user_data(
	cmzn_selectionnotifier_id selectionnotifier);

/**
 * Stop and clear selection callback. This will stop the callback and also
 * remove the callback function from the selection notifier.
 *
 * @param selectionnotifier  Handle to the selection notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_selectionnotifier_clear_callback(cmzn_selectionnotifier_id selectionnotifier);

/**
 * Assign the callback function and user_data for the selection notifier.
 * This function also starts the callback.
 *
 * @see cmzn_selectionnotifier_callback_function
 * @param selectionnotifier  Handle to the selection notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to an user object. User is responsible for
 *   the life time of such object.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_selectionnotifier_set_callback(cmzn_selectionnotifier_id selectionnotifier,
		cmzn_selectionnotifier_callback_function function, void *user_data_in);

/**
 * Returns a new handle to the selection event with reference count incremented.
 *
 * @param selectionevent  The selection event to obtain a new handle to.
 * @return  New handle to new selection event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_selectionevent_id cmzn_selectionevent_access(
	cmzn_selectionevent_id selectionevent);

/**
 * Destroys handle to the selection event (and sets it to NULL).
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param selectionevent_address  Address of selection event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_selectionevent_destroy(cmzn_selectionevent_id *selectionevent_address);

/**
 * Get logical OR of flags indicating how the selection has changed, whether in
 * the local region or hierarchically.
 * @see cmzn_selectionevent_change_flag
 *
 * @param selectionevent  Handle to the selection event.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_selectionevent_change_flag values.
 */
ZINC_API cmzn_selectionevent_change_flags cmzn_selectionevent_get_change_flags(
	cmzn_selectionevent_id selectionevent);

#ifdef __cplusplus
}
#endif

#endif /* CMZN_SELECTION_H__ */
