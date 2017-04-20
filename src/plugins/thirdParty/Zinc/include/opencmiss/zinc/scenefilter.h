/**
 * @file scenefilter.h
 *
 * Public interface to scene filter objects for controlling which parts of a
 * scene are visible or processed.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENEFILTER_H__
#define CMZN_SCENEFILTER_H__

#include "types/fieldid.h"
#include "types/graphicsid.h"
#include "types/regionid.h"
#include "types/scenefilterid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the scene filter module with reference count
 * incremented.
 *
 * @param filtermodule  The scene filter module to obtain a new handle to.
 * @return  New handle to scene filter module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefiltermodule_id cmzn_scenefiltermodule_access(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Destroys handle to the scene filter module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param filtermodule_address  Address of handle to scene filter module
 * to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_scenefiltermodule_destroy(
	cmzn_scenefiltermodule_id *filtermodule_address);

/**
 * Creates and returns a scene filter which matches any graphics with visibility
 * flag set AND its owning region and all ancestor region scenes' visibility
 * flags set i.e. scene visibility flags work hierarchically.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_visibility_flags(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Creates and returns a scene filter which matches any graphics with given field domain type.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @param domain_type  The field domain type graphics attribute to be matched by
 * this filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_field_domain_type(
	cmzn_scenefiltermodule_id filtermodule,
	enum cmzn_field_domain_type domain_type);

/**
 * Creates a scene filter which matches any graphics with the supplied name.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @param match_name  The name of a graphics must be matched by this filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_graphics_name(
	cmzn_scenefiltermodule_id filtermodule, const char *match_name);

/**
 * Creates a scene filter which matches any graphics with matching type.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @param graphics_type  The type of a graphics must be matched by this filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_graphics_type(
	cmzn_scenefiltermodule_id filtermodule, enum cmzn_graphics_type graphics_type);

/**
 * Creates a scene filter which matches any graphics in region or any
 * of its sub-regions.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @param match_region  The region to be matched by this filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_region(
	cmzn_scenefiltermodule_id filtermodule,cmzn_region_id match_region);

/**
 * Creates and returns a scene filter which returns the logical AND of a
 * collective of operand filters, i.e. true if all operand filters are true.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_operator_and(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Creates and returns a scene filter which returns the logical OR of a
 * collective of operand filters, i.e. true if any operand filter is true.
 *
 * @param filtermodule  Handle to the scene filter module to own the new filter.
 * @return  Handle to new scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_create_scenefilter_operator_or(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Begin caching or increment cache level for this scene filter module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end_change after completing changes.
 * Can be nested.
 * @see cmzn_scenefiltermodule_end_change
 *
 * @param filtermodule  The scene filter module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_scenefiltermodule_begin_change(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Decrement cache level or end caching of changes for the scene filter module.
 * Call scene filter module begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_scenefiltermodule_begin_change
 *
 * @param filtermodule  The scene filter module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefiltermodule_end_change(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Find the scene filter with the specified name, if any.
 *
 * @param filtermodule  scene filter module to search.
 * @param name  The name of the scene filter.
 * @return  Handle to scene filter, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_find_scenefilter_by_name(
	cmzn_scenefiltermodule_id filtermodule, const char *name);

/**
 * Get the default scene filter, if any.
 *
 * @param filtermodule  scene filter module to query.
 * @return  Handle to default scene filter, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefiltermodule_get_default_scenefilter(
	cmzn_scenefiltermodule_id filtermodule);

/**
 * Set the default scene filter.
 *
 * @param filtermodule  scene filter module to modify
 * @param filter  The scene filter to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_scenefiltermodule_set_default_scenefilter(
	cmzn_scenefiltermodule_id filtermodule, cmzn_scenefilter_id filter);

/**
 * Returns a new handle to the filter with reference count incremented.
 *
 * @param filter  The filter to obtain a new handle to.
 * @return  New handle to scene filter, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefilter_access(cmzn_scenefilter_id filter);

/**
 * Destroys this handle to the filter (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param filter_address  The address to the handle of the filter
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_destroy(cmzn_scenefilter_id *filter_address);

/**
 * Get managed status of scene filter in its owning scene filter module.
 * @see cmzn_scenefilter_set_managed
 *
 * @param filter  The scene filter to query.
 * @return  true if scene filter is managed, otherwise false.
 */
ZINC_API bool cmzn_scenefilter_is_managed(cmzn_scenefilter_id filter);

/**
 * Set managed status of scene filter in its owning scene filter module.
 * If set (managed) the scene filter will remain indefinitely in the scene filter module even
 * if no external references are held.
 * If not set (unmanaged) the scene filter will be automatically removed from the
 * module when no longer referenced externally, effectively marking it as
 * pending destruction.
 * All new objects are unmanaged unless stated otherwise.
 *
 * @param filter  The scene filter to modify.
 * @param value  The new value for the managed flag: true or false.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_scenefilter_set_managed(cmzn_scenefilter_id filter,
	bool value);

/**
 * Query whether graphics is shown (or processed) with a scene filter.
 *
 * @param filter  The filter to perform the check.
 * @param graphics  The graphics to query.
 * @return  Boolean true if this filter shows (includes) this graphics,
 * otherwise false (hides).
 */
ZINC_API bool cmzn_scenefilter_evaluate_graphics(cmzn_scenefilter_id filter,
	cmzn_graphics_id graphics);

/**
 * Return an allocated string containing scene filter name.
 *
 * @param filter  handle to the scene filter.
 * @return  allocated string containing scene filter name, or NULL on failure.
 * Up to caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_scenefilter_get_name(cmzn_scenefilter_id filter);

/**
 * Set name of the scene filter. Must be unique in the scene filter module.
 *
 * @param filter  The filter to modify.
 * @param name  name to be set to the scene filter
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_set_name(cmzn_scenefilter_id filter,
	const char *name);

/**
 * Get state of inverse flag which if set reverses filter match condition.
 *
 * @param filter  The scene filter to query.
 * @return  true if scene filter matches inverse, otherwise false.
 */
ZINC_API bool cmzn_scenefilter_is_inverse(cmzn_scenefilter_id filter);

/**
 * Set state of inverse flag which if set reverses filter match condition.
 * Default is false i.e. normal condition.
 *
 * @param filter  The scene filter to modify.
 * @param value  Boolean true to set inverse, false for non-inverse.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_scenefilter_set_inverse(cmzn_scenefilter_id filter,
	bool value);

/**
 * If the filter is of operator and or or type, then this function returns the
 * operator derived scenefilter handle.
 *
 * @param filter  The generic filter to be cast.
 * @return  Handle to derived scene filter operator, or NULL/invalid handle if
 * wrong type or failed.
 */
ZINC_API cmzn_scenefilter_operator_id cmzn_scenefilter_cast_operator(
	cmzn_scenefilter_id filter);

/**
 * Cast operator filter back to its base filter and return the filter.
 * IMPORTANT NOTE: Returned filter does not have incremented reference count and
 * must not be destroyed. Use cmzn_scenefilter_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the argument.
 * Use this function to call base-class API, e.g.:
 * cmzn_scenefilter_set_name(cmzn_scenefilter_operator_base_cast(filter), "bob");
 *
 * @param operator_filter  Handle to the filter operator to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_scenefilter_id cmzn_scenefilter_operator_base_cast(
	cmzn_scenefilter_operator_id operator_filter)
{
	return (cmzn_scenefilter_id)(operator_filter);
}

/**
 * Destroys handle to the operator filter (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param operator_filter_address  Address of handle to the operator filter.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_operator_destroy(
	cmzn_scenefilter_operator_id *operator_filter_address);

/**
 * Adds operand to the end of the list of operands for the operator filter.
 * If operand is already in the list, it is moved to the end.
 * Fails if operand depends on the operator filter.
 *
 * @param operator_filter  The operator filter to be modified.
 * @param operand  The filter to be added
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_operator_append_operand(
	cmzn_scenefilter_operator_id operator_filter,
	cmzn_scenefilter_id operand);

/**
 * Get the first operand filter in the operator filter's list of operands.
 * @param operator_filter  The operator filter to be iterated over.
 * @return  Handle to first operand scene filter, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefilter_operator_get_first_operand(
	cmzn_scenefilter_operator_id operator_filter);

/**
 * Get the next filter after <ref_operand> in the operator filter's list of
 * operands.
 *
 * @param operator_filter  The operator filter to be iterated over.
 * @param ref_operand  The filter to be referenced
 * @return  Handle to next operand scene filter, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenefilter_operator_get_next_operand(
	cmzn_scenefilter_operator_id operator_filter,
	cmzn_scenefilter_id ref_operand);

/**
 * Check whether operand is active in the operator filter.
 *
 * @param operator_filter  The operator filter providing a list of filters.
 * @param operand  The filter to be checked.
 * @return  Boolean true if operand is valid and active, otherwise false.
 */
ZINC_API bool cmzn_scenefilter_operator_is_operand_active(
	cmzn_scenefilter_operator_id operator_filter,
	cmzn_scenefilter_id operand);

/**
 * Set an operand in the operator filter to be active or inactive.
 *
 * @param operator_filter  The operator filter providing a list of filters.
 * @param operand  The filter to be set.
 * @param is_active  Value to set: true to mark as active, false for inactive.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_operator_set_operand_active(
	cmzn_scenefilter_operator_id operator_filter,
	cmzn_scenefilter_id operand, bool is_active);

/**
 * Insert a filter before ref_operand in the list of operands for the operator
 * filter. If the operand is already in the list of operands it is moved to the
 * new location. Fails if operand depends on the operator filter.
 *
 * @param operator_filter  The operator filter to be modified.
 * @param operand  The operand filter to be inserted.
 * @param ref_operand  The reference filter to insert before.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_operator_insert_operand_before(
	cmzn_scenefilter_operator_id operator_filter,
	cmzn_scenefilter_id operand, cmzn_scenefilter_id ref_operand);

/**
 * Remove a filter from the list of operands in an operator filter.
 *
 * @param operator_filter  The operator filter to be modified.
 * @param operand  The filter to be removed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenefilter_operator_remove_operand(
	cmzn_scenefilter_operator_id operator_filter,
	cmzn_scenefilter_id operand);

#ifdef __cplusplus
}
#endif

#endif
