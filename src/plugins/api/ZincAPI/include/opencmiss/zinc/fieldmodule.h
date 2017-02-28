/**
 * @file fieldmodule.h
 *
 * Public interface to the field module including its generic functions.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDMODULE_H__
#define CMZN_FIELDMODULE_H__

#include "types/elementid.h"
#include "types/fieldcacheid.h"
#include "types/fieldid.h"
#include "types/fieldmoduleid.h"
#include "types/nodeid.h"
#include "types/regionid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
Global functions
----------------
*/

/**
 * Returns a new handle to the field module with reference count incremented.
 *
 * @param fieldmodule  Handle to field module.
 * @return  New handle to field module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldmodule_id cmzn_fieldmodule_access(cmzn_fieldmodule_id fieldmodule);

/**
 * Destroys handle to the field module and sets pointer/handle to NULL.
 * Internally this decrements the reference count.
 *
 * @param fieldmodule_address  Address of field module handle.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmodule_destroy(cmzn_fieldmodule_id *fieldmodule_address);

/**
 * Begin caching or increment cache level for this field module. Call this
 * function before making multiple changes to fields, nodes, elements etc. from
 * this field module to minimise number of change messages sent to clients.
 * Must call matching fieldmodule end change method after making changes.
 * Note that field module changes are always cached when the region changes are
 * being cached. Can be nested.
 * @see cmzn_fieldmodule_end_change
 * @see cmzn_region_begin_change
 *
 * @param fieldmodule  The field module to begin change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmodule_begin_change(cmzn_fieldmodule_id fieldmodule);

/**
 * Decrement cache level or end caching of changes for this field module.
 * Call matching fieldmodule begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_fieldmodule_begin_change
 *
 * @param fieldmodule  The field module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmodule_end_change(cmzn_fieldmodule_id fieldmodule);

/**
 * Returns the field of the specified name from the field module.
 *
 * @param fieldmodule  Region field module in which to find the field.
 * @param field_name  The name of the field to find.
 * @return  Handle to field, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_find_field_by_name(
	cmzn_fieldmodule_id fieldmodule, const char *field_name);

/**
 * Creates a field cache for storing a known location and field values and
 * derivatives at that location. Required to evaluate and assign field values.
 *
 * @param fieldmodule  The field module to create a field cache for.
 * @return  Handle to new field cache, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldcache_id cmzn_fieldmodule_create_fieldcache(
	cmzn_fieldmodule_id fieldmodule);

/**
 * Create a field iterator object for iterating through the fields in the field
 * module, in alphabetical order of name. The iterator initially points at the
 * position before the first field, so the first call to the field iterator
 * next() method returns the first field and advances the iterator.
 * Iterator becomes invalid if fields are added, removed or renamed while in use.
 * @see cmzn_fielditerator_next
 *
 * @param fieldmodule  Handle to the field module whose fields are to be
 * iterated over.
 * @return  Handle to field iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fielditerator_id cmzn_fieldmodule_create_fielditerator(
	cmzn_fieldmodule_id fieldmodule);

/**
 * Create a notifier for getting callbacks for changes to the fields and related
 * objects in the field module.
 *
 * @param fieldmodule  Handle to the field module to get notifications for.
 * @return  Handle to new field module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldmodulenotifier_id cmzn_fieldmodule_create_fieldmodulenotifier(
	cmzn_fieldmodule_id fieldmodule);

/**
 * Defines, for all elements of all meshes in field module, face elements of
 * dimension one lower in the associated face mesh, and all their faces
 * recursively down to 1 dimensional lines.
 *
 * @param fieldmodule  Handle to the field module owning the meshes to define
 * faces for.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmodule_define_all_faces(cmzn_fieldmodule_id fieldmodule);

/**
 * Gets the region this field module can create fields for.
 *
 * @param fieldmodule  The field module to query.
 * @return  Handle to owning region, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_fieldmodule_get_region(cmzn_fieldmodule_id fieldmodule);

/**
 * Check if two field module handles refer to the same region.
 *
 * @param fieldmodule1  The first field module to match.
 * @param fieldmodule2  The second field module to match.
 * @return  Boolean true if the two field modules match, false if not.
 */
ZINC_API bool cmzn_fieldmodule_match(cmzn_fieldmodule_id fieldmodule1,
	cmzn_fieldmodule_id fieldmodule2);

/**
 * Returns a new handle to the field module notifier with reference count
 * incremented.
 *
 * @param notifier  The field module notifier to obtain a new handle to.
 * @return  New handle to field module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldmodulenotifier_id cmzn_fieldmodulenotifier_access(
	cmzn_fieldmodulenotifier_id notifier);

/**
 * Destroys handle to the field module notifier and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param notifier_address  Address of field module notifier handle to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_fieldmodulenotifier_destroy(cmzn_fieldmodulenotifier_id *notifier_address);

/**
 * Stop and clear field module callback. This will stop the callback and also
 * remove the callback function from the fieldmodule notifier.
 *
 * @param notifier  Handle to the fieldmodule notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmodulenotifier_clear_callback(cmzn_fieldmodulenotifier_id notifier);

/**
 * Assign the callback function and user data for the fieldmodule notifier.
 * This function also starts the callback.
 *
 * @see cmzn_fieldmodulenotifier_callback_function
 * @param notifier  Handle to the fieldmodule notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User must ensure this
 * object's lifetime exceeds the duration for which callbacks are active.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmodulenotifier_set_callback(cmzn_fieldmodulenotifier_id notifier,
	cmzn_fieldmodulenotifier_callback_function function, void *user_data_in);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_fieldmodulenotifier_set_callback
 *
 * @see cmzn_fieldmodulenotifier_set_callback
 * @param notifier  Handle to the field module notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_fieldmodulenotifier_get_callback_user_data(
 cmzn_fieldmodulenotifier_id notifier);

/**
 * Returns a new handle to the fieldmodule event with reference count incremented.
 *
 * @param event  The field module event to obtain a new handle to.
 * @return  New handle to field module event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldmoduleevent_id cmzn_fieldmoduleevent_access(
	cmzn_fieldmoduleevent_id event);

/**
 * Destroys this handle to the fieldmodule event and sets it to NULL.
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param event_address  Address of field module event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fieldmoduleevent_destroy(cmzn_fieldmoduleevent_id *event_address);

/**
 * Get logical OR of flags indicating how fields in the field module have changed.
 * @see cmzn_field_change_flag
 *
 * @param event  Handle to the field module event.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_field_change_flag values.
 */
ZINC_API cmzn_field_change_flags cmzn_fieldmoduleevent_get_summary_field_change_flags(
	cmzn_fieldmoduleevent_id event);

/**
 * Get logical OR of flags indicating how the field has changed.
 * @see cmzn_field_change_flag
 *
 * @param event  Handle to the field module event to query.
 * @param field  The field to query about.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_field_change_flag values. Returns
 * CMZN_FIELD_CHANGE_FLAG_NONE in case of invalid arguments.
 */
ZINC_API cmzn_field_change_flags cmzn_fieldmoduleevent_get_field_change_flags(
	cmzn_fieldmoduleevent_id event, cmzn_field_id field);

/**
 * Get object describing changes to elements in a mesh including elements
 * involved in partial change to fields.
 * Note that changes to nodes are propagated to elements referencing them, and
 * changes to parent elements are propagated to their faces. The logic for this
 * is conservative e.g. it assumes all fields use all nodes, so you may
 * sometimes get false notification of changes.
 *
 * @param event  Handle to the field module event to query.
 * @param mesh  The mesh to get change information for.
 * @return  Handle to mesh changes, or NULL/invalid handle on failure.

 */
ZINC_API cmzn_meshchanges_id cmzn_fieldmoduleevent_get_meshchanges(
	cmzn_fieldmoduleevent_id event, cmzn_mesh_id mesh);

/**
 * Get object describing changes to nodes in a nodeset including nodes involved
 * in partial change to field.
 *
 * @param event  Handle to the field module event to query.
 * @param nodeset  The nodeset to get change information for.
 * @return  Handle to nodeset changes, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_nodesetchanges_id cmzn_fieldmoduleevent_get_nodesetchanges(
	cmzn_fieldmoduleevent_id event, cmzn_nodeset_id nodeset);

#ifdef __cplusplus
}
#endif

#endif
