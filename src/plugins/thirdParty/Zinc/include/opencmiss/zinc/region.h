/**
 * @file region.h
 *
 * The public interface to region objects used to managed models and submodels
 * in a tree-like structure. Each region owns its own fields and subregions,
 * and may have a scene attached to it which holds its graphics.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_REGION_H__
#define CMZN_REGION_H__

#include "types/elementid.h"
#include "types/fieldid.h"
#include "types/fieldmoduleid.h"
#include "types/nodeid.h"
#include "types/sceneid.h"
#include "types/streamid.h"
#include "types/regionid.h"

#include "opencmiss/zinc/zincsharedobject.h"

/*
Global functions
----------------
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the region with reference count incremented.
 *
 * @param region  Handle to a region.
 * @return  New handle to region, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_region_access(cmzn_region_id region);

/**
 * Destroys this handle to the region, and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param region_address  The address to the handle of the region
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_destroy(cmzn_region_id *region_address);

/**
 * Begin caching or increment cache level for this region only. Call this
 * function before making multiple changes to the region or its fields and
 * objects via its field_module to minimise number of change messages sent to
 * clients. Must call region end change method after making changes.
 * Important: Do not pair with region end hierarchical change method!
 * Note: region change caching encompasses field_module change caching so there
 * is no need to call fieldmodule begin/end change methods as well.
 * Can be nested.
 * @see cmzn_region_end_change
 *
 * @param region  The region to begin change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_begin_change(cmzn_region_id region);

/**
 * Decrement cache level or end caching of changes for this region only.
 * Call region begin change method before making multiple field or region changes
 * and call this afterwards. When change level is restored to zero in region,
 * cached change messages are sent out to clients.
 * Important: Do not pair with region begin hierarchical change method!
 * @see cmzn_region_begin_change
 *
 * @param region  The region to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_end_change(cmzn_region_id region);

/**
 * Begin caching or increment cache level for all regions in a tree, used to
 * efficiently and safely make hierarchical field changes or modify the tree.
 * Must call region end hierarchical_change method after modifications made.
 * Can be nested.
 * Important: Do not pair with non-hierarchical region end change method!
 * @see cmzn_region_end_hierarchical_change
 *
 * @param region  The root of the region tree to begin change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_begin_hierarchical_change(cmzn_region_id region);

/**
 * Decrement cache level or end caching of changes for all regions in a tree.
 * Call region begin hierarchical change method before making hierarchical field
 * changes or modifying the region tree, and call this afterwards. When change
 * level is restored to zero in any region, cached change messages are sent out.
 * Important: Do not pair with non-hierarchical region begin change method!
 * @see cmzn_region_begin_hierarchical_change
 *
 * @param region  The root of the region tree to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_end_hierarchical_change(cmzn_region_id region);

/**
 * Returns the name of the region.
 *
 * @param region  The region whose name is requested.
 * @return  On success: allocated string containing region name. Up to caller to
 * free using cmzn_deallocate().
 */
ZINC_API char *cmzn_region_get_name(cmzn_region_id region);

/**
 * Sets the name of the region. Any name is valid as long as it is unique in the
 * parent region, however use of forward slash characters '/' in names is
 * discouraged since it prevent regions being found by paths.
 *
 * @param region  The region to be named.
 * @param name  The new name for the region.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_region_set_name(cmzn_region_id region, const char *name);

/**
 * Returns a handle to the parent region of this region.
 *
 * @param region  The child region.
 * @return  Handle to parent region, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_region_id cmzn_region_get_parent(cmzn_region_id region);

/**
 * Returns a handle to the first child region of this region.
 *
 * @param region  The region whose first child is requested.
 * @return  Handle to first child region, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_region_id cmzn_region_get_first_child(cmzn_region_id region);

/**
 * Returns a reference to this region's next sibling region.
 *
 * @param region  The region whose next sibling is requested.
 * @return  Handle to next sibling region, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_region_id cmzn_region_get_next_sibling(cmzn_region_id region);

/**
 * Returns a reference to this region's previous sibling region.
 *
 * @param region  The region whose previous sibling is requested.
 * @return  Handle to previous sibling region, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_region_id cmzn_region_get_previous_sibling(cmzn_region_id region);

/**
 * Replaces the region reference with a reference to its next sibling.
 * Convenient for iterating through a child list, equivalent to:
 * {
 *   cmzn_region_id temp = cmzn_region_get_next_sibling(*region_address);
 *   cmzn_region_destroy(region_address);
 *   *region_address = temp;
 * }
 *
 * @param region_address  The address of the region reference to replace.
 */
ZINC_API void cmzn_region_reaccess_next_sibling(cmzn_region_id *region_address);

/**
 * Adds new_child to the end of the list of child regions of this region.
 * If the new_child is already in the region tree, it is first removed.
 * Fails if new_child contains this region.
 * Fails if new_child is unnamed or the name is already used by another child of
 * this region.
 *
 * @param region  The intended parent region of new_child.
 * @param new_child  The child to add.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_append_child(cmzn_region_id region, cmzn_region_id new_child);

/**
 * Inserts new_child before the existing ref_child in the list of child regions
 * of this region. If ref_child is NULL new_child is added at the end of the
 * list. If the new_child is already in the region tree, it is first removed.
 * Fails if new_child contains this region.
 * Fails if new_child is unnamed or the name is already used by another child of
 * this region.
 *
 * @param region  The intended parent region of new_child.
 * @param new_child  The child to append.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_region_insert_child_before(cmzn_region_id region,
	cmzn_region_id new_child, cmzn_region_id ref_child);

/**
 * Removes old_child from the list of child regions of this region.
 * Fails if old_child is not a child of this region.
 *
 * @param region  The current parent region of old_child.
 * @param old_child  The child to remove.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_region_remove_child(cmzn_region_id region,
	cmzn_region_id old_child);

/**
 * Returns true if region is or contains the subregion.
 *
 * @param region  The region being tested as container.
 * @param subregion  The region being tested for containment.
 * @return  Boolean true if this region is or contains subregion, otherwise false.
 */
ZINC_API bool cmzn_region_contains_subregion(cmzn_region_id region,
	cmzn_region_id subregion);

/**
 * Finds child region with supplied name, if any.
 *
 * @param region  Handle to region to search.
 * @param name  The name of the child.
 * @return  Handle to child region, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_region_id cmzn_region_find_child_by_name(
	cmzn_region_id region, const char *name);

/**
 * Returns a handle to the subregion at the path relative to this region.
 * The format of the path string is CHILD_NAME/CHILD_NAME/...
 * i.e. forward slash characters '/' are used as parent/child name separators.
 * Single leading and trailing separator characters are ignored.
 * Hence, both name="" and name="/" find the region itself.
 *
 * @param region  Handle to the top region to search.
 * @param path  The directory-style path to the subregion.
 * @return  Handle to subregion, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_region_id cmzn_region_find_subregion_at_path(cmzn_region_id region,
	const char *path);

/**
 * Get field module which manages this region's fields, which must be passed
 * to field factory create methods.
 *
 * @param region  Handle to region.
 * @return  Handle to field module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldmodule_id cmzn_region_get_fieldmodule(cmzn_region_id region);

/**
 * Creates and returns a reference to a region compatible with base_region,
 * i.e. able to exist in the same region tree.
 *
 * @see cmzn_context_create_region
 * @param base_region  An existing region.
 * @return  Handle to new region, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_region_create_region(cmzn_region_id base_region);

/**
 * Create a child region with provided name in parent region.
 * Fails if a child of that name exists already.
 *
 * @see cmzn_region_set_name
 * @param parent_region  Handle to parent region for the new region.
 * @param name  The name for the new region.
 * @return  Handle to new child region, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_region_create_child(cmzn_region_id parent_region,
	const char *name);

/**
 * Create a region at the specified relative path, creating any intermediary
 * regions if required.
 * Fails if a subregion exists at that path already.
 *
 * @param top_region  The region the path is relative to.
 * @param path  Region path, a series of valid region names separated by a
 * forward slash "/". Leading and trailing separator slashes are optional.
 * @return  Handle to new subregion, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_region_create_subregion(cmzn_region_id top_region,
	const char *path);

/**
 * Reads region data using stream resource objects provided in the
 * stream information object.
 * @see cmzn_streaminformation_id
 *
 * @param region  The region to read the resources in streaminformation into.
 * @param streaminformation  Handle to the stream information region containing
 * 		information about resources to read from.
 * @return  Status CMZN_OK if data successfully read and merged into specified
 * region, any other value on failure.
 */
ZINC_API int cmzn_region_read(cmzn_region_id region,
	cmzn_streaminformation_region_id streaminformation_region);

/**
 * Convenient function to read a file with the provided name into a region
 * directly.
 *
 * @param region  The region to be read into
 * @param file_name  name of the file to read from.
 * @return  Status CMZN_OK if data successfully read and merged into specified
 * region, any other value on failure.
 */
ZINC_API int cmzn_region_read_file(cmzn_region_id region, const char *file_name);

/**
 * Writes region data to stream resource objects described in the
 * stream information object.
 *
 * @param region  The region to be written out.
 * @param streaminformation  Handle to the stream information region containing
 * 		information about resources to write to.
 * @return  Status CMZN_OK if data is successfully written out, any other value
 * on failure.
 */
ZINC_API int cmzn_region_write(cmzn_region_id region,
	cmzn_streaminformation_region_id streaminformation_region);

/**
 * Convenient function to write the region into a file with the provided name.
 *
 * @param region  The region to be written out.
 * @param file_name  name of the file to write to..
 *
 * @return  Status CMZN_OK if data is successfully written out, any other value
 * otherwise.
 */
ZINC_API int cmzn_region_write_file(cmzn_region_id region, const char *file_name);

/**
 * Return handle to the scene for this region, which contains
 * graphics for visualising fields in the region.
 *
 * @param cmiss_region  The region of query.
 * @return  Handle to scene, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scene_id cmzn_region_get_scene(cmzn_region_id region);

#ifdef __cplusplus
}
#endif

#endif
