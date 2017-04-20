/**
 * @file tessellation.h
 *
 * Public interface to tessellation objects. These describe how elements or
 * other continuous field domains are subdivided into graphics.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_TESSELLATION_H__
#define CMZN_TESSELLATION_H__

#include "types/tessellationid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returns a new handle to the tessellation module with reference count
 * incremented.
 *
 * @param tessellationmodule  Handle to tessellation module.
 * @return  New handle to tessellation module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellationmodule_id cmzn_tessellationmodule_access(
    cmzn_tessellationmodule_id tessellationmodule);

/**
 * Destroys this handle to the tessellation module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param tessellationmodule_address  Address of handle to tessellation module
 *   to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellationmodule_destroy(
    cmzn_tessellationmodule_id *tessellationmodule_address);

/**
 * Create and return a new tessellation.
 *
 * @param tessellationmodule  The handle to the tessellation module the
 * tessellation will belong to.
 * @return  Handle to new tessellation, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellation_id cmzn_tessellationmodule_create_tessellation(
    cmzn_tessellationmodule_id tessellationmodule);

/**
 * Create a tessellation iterator object for iterating through the
 * tessellations in the tessellation module, in alphabetical order of name. The
 * iterator initially points at the position before the first tessellation, so
 * the first call to the iterator next() method returns the first tessellation
 * and advances the iterator. The iterator becomes invalid if tessellations are
 * added, removed or renamed while in use.
 * @see cmzn_tessellationiterator_next
 *
 * @param tessellationmodule  Handle to the tessellation module whose
 * tessellations are to be iterated over.
 * @return  Handle to tessellation iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellationiterator_id cmzn_tessellationmodule_create_tessellationiterator(
    cmzn_tessellationmodule_id tessellationmodule);

/**
 * Begin caching or increment cache level for this tessellation module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end_change after completing changes.
 * Can be nested.
 * @see cmzn_tessellationmodule_end_change
 *
 * @param tessellationmodule  The tessellation module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellationmodule_begin_change(cmzn_tessellationmodule_id tessellationmodule);

/**
 * Decrement cache level or end caching of changes for the tessellation module.
 * Call tessellation module begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_tessellationmodule_begin_change
 *
 * @param tessellationmodule  The tessellation module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_tessellationmodule_end_change(cmzn_tessellationmodule_id tessellationmodule);

/**
 * Find the tessellation with the specified name, if any.
 *
 * @param tessellationmodule  Tessellation module to search.
 * @param name  The name of the tessellation.
 * @return  Handle to tessellation, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_tessellation_id cmzn_tessellationmodule_find_tessellation_by_name(
    cmzn_tessellationmodule_id tessellationmodule, const char *name);

/**
 * Get the default tessellation to be used by new lines, surfaces and
 * isosurfaces graphics. If there is none, one is automatically created with
 * minimum divisions 1, refinement factors 6, and circle divisions 12,
 * and given the name "default".
 *
 * @param tessellationmodule  Tessellation module to query.
 * @return  Handle to default tessellation, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_tessellation_id cmzn_tessellationmodule_get_default_tessellation(
    cmzn_tessellationmodule_id tessellationmodule);

/**
 * Set the default tessellation to be used by new lines, surfaces and
 * isosurfaces graphics.
 *
 * @param tessellationmodule  Tessellation module to modify.
 * @param tessellation  The tessellation to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellationmodule_set_default_tessellation(
    cmzn_tessellationmodule_id tessellationmodule,
    cmzn_tessellation_id tessellation);

/**
 * Get the default tessellation to be used by new points and streamlines
 * graphics. If there is none, one is automatically created with
 * minimum divisions 1, refinement factors 1, and circle divisions 12,
 * and given the name "default_points".
 *
 * @param tessellationmodule  Tessellation module to query.
 * @return  Handle to default points tessellation, or NULL/invalid handle if
 * none or failed.
 */
ZINC_API cmzn_tessellation_id cmzn_tessellationmodule_get_default_points_tessellation(
    cmzn_tessellationmodule_id tessellationmodule);

/**
 * Set the default tessellation to be used by new points and streamlines
 * graphics.
 *
 * @param tessellationmodule  Tessellation module to modify.
 * @param tessellation  The tessellation to set as default for points.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellationmodule_set_default_points_tessellation(
    cmzn_tessellationmodule_id tessellationmodule,
    cmzn_tessellation_id tessellation);

/**
 * Read the json description to the tessellation module. This will change
 * the tessellations in the tessellation module.
 *
 * @param tessellationmodule  Handle to the tessellation module.
 * @description  The string containing json description
 * @return  CMZN_OK on success, otherwise ERROR status.
 */
ZINC_API int cmzn_tessellationmodule_read_description(
	cmzn_tessellationmodule_id tessellationmodule, const char *description);

/**
 * Write the json file describing the tessellations in the tessellation module,
 * which can be used to store the current tessellation settings.
 *
 * @param tessellationmodule  Handle to the tessellation module.
 * @return  c string containing the json description of tessellation module, otherwise 0;
 */
ZINC_API char *cmzn_tessellationmodule_write_description(
	cmzn_tessellationmodule_id tessellationmodule);

/**
 * Returns a new handle to the tessellation with reference count incremented.
 *
 * @param tessellation  Handle to tessellation.
 * @return  New handle to tessellation, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellation_id cmzn_tessellation_access(cmzn_tessellation_id tessellation);

/**
 * Destroys handle to the tessellation (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param tessellation_address  The address to the handle of the tessellation
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_tessellation_destroy(cmzn_tessellation_id *tessellation_address);

/**
 * Gets the number of line segments used to approximate circles in graphics
 * produced with this tessellation. This applies to lines with a circle profile,
 * and to sphere, cylinder and other glyphs with circular features.
 *
 * @param tessellation  The tessellation to query.
 * @return  The number of circle divisions, or 0 on error.
 */
ZINC_API int cmzn_tessellation_get_circle_divisions(
    cmzn_tessellation_id tessellation);

/**
 * Sets the number of line segments used to approximate circles in graphics
 * produced with this tessellation. This applies to lines with a circle profile,
 * and to sphere, cylinder and other glyphs with circular features.
 *
 * @param tessellation  The tessellation to modify.
 * @param circleDivisions  The number of line segments used to approximate a
 * a circle, at least 3, but larger even numbers are recommended.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellation_set_circle_divisions(
    cmzn_tessellation_id tessellation, int circleDivisions);

/**
 * Get managed status of tessellation in its owning tessellation module.
 * @see cmzn_tessellation_set_managed
 *
 * @param tessellation  The tessellation to query.
 * @return  1 (true) if tessellation is managed, otherwise 0 (false).
 */
ZINC_API bool cmzn_tessellation_is_managed(cmzn_tessellation_id tessellation);

/**
 * Set managed status of tessellation in its owning tessellation module.
 * If set (managed) the tessellation will remain indefinitely in the tessellation
 * module even if no external references are held.
 * If not set (unmanaged) the tessellation will be automatically removed from the
 * module when no longer referenced externally, effectively marking it as
 * pending destruction.
 * All new objects are unmanaged unless stated otherwise.
 *
 * @param tessellation  The tessellation to modify.
 * @param value  The new value for the managed flag: true or false.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellation_set_managed(cmzn_tessellation_id tessellation,
    bool value);

/**
 * Return an allocated string containing tessellation name.
 *
 * @param tessellation  handle to the zinc tessellation.
 * @return  allocated string containing tessellation name, or NULL on failure.
 * Up to caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_tessellation_get_name(cmzn_tessellation_id tessellation);

/**
 * Set/change name for tessellation. Must be unique in the tessellation module.
 *
 * @param tessellation  The handle to zinc tessellation.
 * @param name  name to be set to the tessellation
 * @return  status CMZN_OK if successfully set/change name for tessellation,
 * any other value on failure.
 */
ZINC_API int cmzn_tessellation_set_name(cmzn_tessellation_id tessellation, const char *name);

/**
 * Gets the minimum number of line segments used to approximate curves in each
 * element dimension for coarse tessellation.
 *
 * @see cmzn_tessellation_set_minimum_divisions
 * @param tessellation  The tessellation to query.
 * @param valuesCount  The size of the minimum_divisions array to fill. Values
 * for dimensions beyond the size set use the last divisions value.
 * @param valuesOut  Array to receive numbers of divisions.
 * @return  The actual number of minimum divisions values that have been
 * explicitly set using cmzn_tessellation_set_minimum_divisions. This can be
 * more than the number requested, so a second call may be needed with a
 * larger array. Returns 0 on error.
 */
ZINC_API int cmzn_tessellation_get_minimum_divisions(
    cmzn_tessellation_id tessellation, int valuesCount, int *valuesOut);

/**
 * Sets the minimum number of line segments used to approximate curves in each
 * element dimension of tessellation. Intended to be used where coarse
 * tessellation is acceptable, e.g. where fields vary linearly over elements.
 * The default minimum_divisions value for new tessellations is 1, size 1.
 * Note: The value set for the last dimension applies to all higher dimensions.
 *
 * @param tessellation  The tessellation to modify.
 * @param valuesCount  The size of the valuesIn array, >= 1.
 * @param valuesIn  Array of number of divisions (>=1) for each dimension, with
 * the last number in array applying to all higher dimensions.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellation_set_minimum_divisions(
    cmzn_tessellation_id tessellation, int valuesCount, const int *valuesIn);

/**
 * Gets the refinements to be used in product with the minimum divisions
 * to approximate curves in each element dimension for fine tessellation.
 *
 * @see cmzn_tessellation_set_refinement_factors
 * @param tessellation  The tessellation to query.
 * @param valuesCount  The size of the refinement_factors array to fill. Values
 * for dimensions beyond the size set use the last refinement value.
 * @param valuesOut  Array to receive refinement factors.
 * @return  The actual number of refinement factor values that have been
 * explicitly set using cmzn_tessellation_set_refinement_factors. This can be
 * more than the number requested, so a second call may be needed with a
 * larger array. Returns 0 on error.
 */
ZINC_API int cmzn_tessellation_get_refinement_factors(
    cmzn_tessellation_id tessellation, int valuesCount, int *valuesOut);

/**
 * Sets the refinements to be used in product with the minimum divisions
 * to approximate curves in each element dimension for fine tessellation.
 * The refinement factors are applied whenever the basis functions of the
 * graphics coordinate field (replaced by tessellation field if specified) are
 * non-linear anywhere. If there is no tessellation field or if it matches the
 * coordinate field, a non-linear coordinate system also triggers refinement.
 * The default refinement_factors value for new tessellations is 1, size 1.
 * Note: The value set for the last dimension applies to all higher dimensions.
 *
 * @see cmzn_tessellation_set_minimum_divisions
 * @param tessellation  The tessellation to modify.
 * @param valuesCount  The size of the refinement_factors array, >= 1.
 * @param valuesIn  Array of number of fine subdivisions (>=1) per
 * minimum_division for each dimension, with the last number in array
 * applying to all higher dimensions.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellation_set_refinement_factors(
    cmzn_tessellation_id tessellation, int valuesCount, const int *valuesIn);

/**
 * Returns a new handle to the iterator with reference count incremented.
 *
 * @param iterator  The tessellation iterator to obtain a new handle to.
 * @return  New handle to tessellation iterator, or NULL/invalid handle on
 * failure.
 */
ZINC_API cmzn_tessellationiterator_id cmzn_tessellationiterator_access(
    cmzn_tessellationiterator_id iterator);

/**
 * Destroys this handle to the tessellation iterator and sets it to NULL.
 *
 * @param iterator_address  Address of handle to tessellation_iterator to
 * destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellationiterator_destroy(cmzn_tessellationiterator_id *iterator_address);

/**
 * Returns a handle to the next tessellation in the container being iterated
 * over then advances the iterator position. The caller is required to destroy
 * the returned tessellation handle.
 *
 * @param iterator  Tessellation iterator to query and advance.
 * @return  Handle to the next tessellation, or NULL/invalid handle if none or
 * failed.
 */
ZINC_API cmzn_tessellation_id cmzn_tessellationiterator_next(cmzn_tessellationiterator_id iterator);

/**
 * Create a notifier for getting callbacks for changes to the tessellations in the
 * tessellation module.
 *
 * @param tessellationmodule  Handle to the tessellation module to get notifications for.
 * @return  Handle to new tessellation module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellationmodulenotifier_id cmzn_tessellationmodule_create_tessellationmodulenotifier(
	cmzn_tessellationmodule_id tessellationmodule);

/**
 * Returns a new handle to the tessellation module notifier with reference count
 * incremented.
 *
 * @param notifier  The tessellation module notifier to obtain a new handle to.
 * @return  New handle to tessellation module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellationmodulenotifier_id cmzn_tessellationmodulenotifier_access(
	cmzn_tessellationmodulenotifier_id notifier);

/**
 * Destroys handle to the tessellation module notifier and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param notifier_address  Address of tessellation module notifier handle to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_tessellationmodulenotifier_destroy(
	cmzn_tessellationmodulenotifier_id *notifier_address);

/**
 * Stop and clear tessellation module callback. This will stop the callback and also
 * remove the callback function from the tessellationmodule notifier.
 *
 * @param notifier  Handle to the tessellationmodule notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_tessellationmodulenotifier_clear_callback(
	cmzn_tessellationmodulenotifier_id notifier);

/**
 * Assign the callback function and user data for the tessellationmodule notifier.
 * This function also starts the callback.
 *
 * @see cmzn_tessellationmodulenotifier_callback_function
 * @param notifier  Handle to the tessellationmodule notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User must ensure this
 * object's lifetime exceeds the duration for which callbacks are active.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_tessellationmodulenotifier_set_callback(
	cmzn_tessellationmodulenotifier_id notifier,
	cmzn_tessellationmodulenotifier_callback_function function, void *user_data_in);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_tessellationmodulenotifier_set_callback
 *
 * @see cmzn_tessellationmodulenotifier_set_callback
 * @param notifier  Handle to the tessellation module notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_tessellationmodulenotifier_get_callback_user_data(
 cmzn_tessellationmodulenotifier_id notifier);

/**
 * Returns a new handle to the tessellationmodule event with reference count incremented.
 *
 * @param event  The tessellation module event to obtain a new handle to.
 * @return  New handle to tessellation module event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellationmoduleevent_id cmzn_tessellationmoduleevent_access(
	cmzn_tessellationmoduleevent_id event);

/**
 * Destroys this handle to the tessellationmodule event and sets it to NULL.
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param event_address  Address of tessellation module event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_tessellationmoduleevent_destroy(cmzn_tessellationmoduleevent_id *event_address);

/**
 * Get logical OR of flags indicating how tessellations in the tessellation module have changed.
 * @see cmzn_tessellation_change_flag
 *
 * @param event  Handle to the tessellation module event.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_tessellation_change_flag values.
 */
ZINC_API cmzn_tessellation_change_flags cmzn_tessellationmoduleevent_get_summary_tessellation_change_flags(
	cmzn_tessellationmoduleevent_id event);

/**
 * Get logical OR of flags indicating how the tessellation has changed.
 * @see cmzn_tessellation_change_flag
 *
 * @param event  Handle to the tessellation module event to query.
 * @param tessellation  The tessellation to query about.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_tessellation_change_flag values. Returns
 * CMZN_SPECTRUM_CHANGE_FLAG_NONE in case of invalid arguments.
 */
ZINC_API cmzn_tessellation_change_flags cmzn_tessellationmoduleevent_get_tessellation_change_flags(
	cmzn_tessellationmoduleevent_id event, cmzn_tessellation_id tessellation);

#ifdef __cplusplus
}
#endif

#endif
