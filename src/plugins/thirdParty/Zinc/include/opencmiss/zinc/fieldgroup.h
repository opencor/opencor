/**
 * @file fieldgroup.h
 *
 * Implements a zinc field which maintains a group or selection of objects
 * from the region including the region itself, other fields representing domain
 * object groups (e.g. node, element), and related groups from child regions.
 * The field evaluates to 1 (true) at domain locations in the group, and 0
 * elsewhere.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDGROUP_H__
#define CMZN_FIELDGROUP_H__

#include "types/elementid.h"
#include "types/fieldid.h"
#include "types/fieldgroupid.h"
#include "types/fieldmoduleid.h"
#include "types/nodeid.h"
#include "types/regionid.h"
#include "types/fieldsubobjectgroupid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a group field which can contain an arbitrary set of subregions or
 * region subobjects, and works as a boolean-valued field returning 1 on domains
 * in the group, 0 otherwise.
 *
 * @param field_module  Region field module which will own new field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_group(cmzn_fieldmodule_id field_module);

/**
 * If the field is of group type, then this function returns the derived group
 * field handle.
 *
 * @param field  The generic field to be cast.
 * @return  Handle to derived group field, or NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_group_id cmzn_field_cast_group(cmzn_field_id field);

/**
 * Cast group field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the group argument.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_group_base_cast(group_field), "bob");
 *
 * @param group  Handle to the group field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_group_base_cast(cmzn_field_group_id group)
{
	return (cmzn_field_id)(group);
}

/**
 * Destroys handle to the group field (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param group_address  Address of handle to the group field.
 * @return  Status CMZN_OK if successfully destroyed the group handle,
 * 		any other value on failure.
 */
ZINC_API int cmzn_field_group_destroy(cmzn_field_group_id *group_address);

/**
 * Query if this group and all its subregion and sub-object groups are empty.
 *
 * @param group  Handle to group field to query.
 * @return  Boolean true if group and its subgroups are empty, otherwise false.
 */
ZINC_API bool cmzn_field_group_is_empty(cmzn_field_group_id group);

/**
 * Query if this group contains no objects from the local region.
 *
 * @param group  Handle to group field to query.
 * @return  Boolean true if group is empty locally, otherwise false.
 */
ZINC_API bool cmzn_field_group_is_empty_local(cmzn_field_group_id group);

/**
 * Remove all objects from this group, clear all its subgroups, and remove &
 * destroy them if possible.
 *
 * @param group  Handle to group field to modify.
 * @return  Status CMZN_OK if group and its child groups cleared successfully,
 * 		any other value on failure.
 */
ZINC_API int cmzn_field_group_clear(cmzn_field_group_id group);

/**
 * Remove all local objects from group, but leave subregion subgroups intact.
 *
 * @param group  Handle to group field to modify.
 * @return  Status CMZN_OK if group is successfully cleared locally,
 * 		any other value on failure.
 */
ZINC_API int cmzn_field_group_clear_local(cmzn_field_group_id group);

/**
 * Remove and destroy all empty subregion and subobject groups of this group.
 * Empty subgroups in use by other clients may remain after call.
 *
 * @param group  Handle to group field to modify.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_group_remove_empty_subgroups(cmzn_field_group_id group);

/**
 * Add the local/owning region of this group field to the group, i.e. all local
 * objects/domains. This function is not hierarchical: subregions are not added.
 * Note that subobject groups such as node and element groups attached to this
 * group field are left intact by this function.
 *
 * @param group  Handle to group field to modify.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_group_add_local_region(cmzn_field_group_id group);

/**
 * Query if group contains its local/owning region, i.e. all local objects/
 * domains.
 * This function is not hierarchical: subregions are not checked.
 *
 * @param group  Handle to group field to query.
 * @return  Boolean true if all of local region is in group, otherwise false.
 */
ZINC_API bool cmzn_field_group_contains_local_region(cmzn_field_group_id group);

/**
 * Remove the local/owning region of this group field from the group, i.e. all local
 * objects/domains. This function is not hierarchical: subregions are not removed.
 * Note that subobject groups such as node and element groups attached to this
 * group field are left intact by this function.
 *
 * @param group  Handle to group field to modify.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_group_remove_local_region(cmzn_field_group_id group);

/**
 * Add the specified region to the group i.e. all its objects/domains.
 * The specified region must be in the tree of this group's local/owning region
 * and not already in the group.
 * This function is not hierarchical: subregions are not added.
 *
 * @param group  Handle to group field to modify.
 * @param region  Handle to region to be added.
 * @return  Status CMZN_OK if successfully add region into group, any other
 * 		value on failure.
 */
ZINC_API int cmzn_field_group_add_region(cmzn_field_group_id group, cmzn_region_id region);

/**
 * Query if specified region is in the group i.e. all its objects/domains.
 * The specified region must be in the tree of this group's local/owning region.
 * This function is not hierarchical: subregions are not checked.
 *
 * @param group  Handle to group field to query.
 * @param region  Handle to region to check.
 * @return  Boolean true if group contains region, otherwise false.
 */
ZINC_API bool cmzn_field_group_contains_region(cmzn_field_group_id group, cmzn_region_id region);

/**
 * Remove specified region from group if currently in it.
 * The specified region must be in the tree of this group's local/owning region.
 * This function is not hierarchical: subregions are not removed.
 *
 * @param group  Handle to group field to modify.
 * @param region  Handle to region to be removed.
 * @return  Status CMZN_OK if region successfully removed from group, any other
 * 		value on failure.
 */
ZINC_API int cmzn_field_group_remove_region(cmzn_field_group_id group, cmzn_region_id region);

/**
 * Get mode controlling how subelements (faces, lines, nodes) are handled
 * when parent elements are added or removed from subobject groups.
 *
 * @param group  Handle to group field to modify.
 * @return  The subelement handling mode, or SUBELEMENT_HANDLING_MODE_INVALID
 * on error.
 */
ZINC_API enum cmzn_field_group_subelement_handling_mode
	cmzn_field_group_get_subelement_handling_mode(cmzn_field_group_id group);

/**
 * Set mode controlling how subelements (faces, lines, nodes) are handled
 * when parent elements are added or removed from subobject groups.
 * Note this mode only affects what happens for subsequently added or removed
 * elements - it does not change the contents of the group.
 *
 * @param group  Handle to group field to modify.
 * @param mode  The new subelement handling mode.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_group_set_subelement_handling_mode(cmzn_field_group_id group,
	enum cmzn_field_group_subelement_handling_mode mode);

/**
 * Create a group field for the specified subregion, include it in the specified
 * group and return a handle to the newly created sub-group field.
 * The specified region must be in the tree of this group's local/owning region
 * and not already in the group.
 *
 * @param group  Handle to group field to modify.
 * @param subregion  Handle to region to create a subgroup for.
 * @return  Handle to new, empty subregion group field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_group_id cmzn_field_group_create_subregion_field_group(
	cmzn_field_group_id group, cmzn_region_id subregion);

/**
 * Get the group field for subregion in the specified group if it exists.
 * The specified region must be in the tree of this group's local/owning region.
 *
 * @param group  Handle to group field to query.
 * @param subregion  Handle to region to get the subgroup for.
 * @return  Handle to subregion group field, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_field_group_id cmzn_field_group_get_subregion_field_group(cmzn_field_group_id group,
	cmzn_region_id subregion);

/**
 * Create and return a node group field compatible with the supplied
 * nodeset, i.e. able to contain nodes from its master nodeset.
 * The node group field is registered as a sub-object group for this group.
 * The nodeset can be from the group's own region or any subregion of it.
 * Subregion groups for any intermediate and the final nodeset-owning regions
 * are automatically created to contain the node group, if needed.
 * Fails if a compatible node group field already exists.
 *
 * @param group  Handle to group field to modify.
 * @param nodeset  Handle to a nodeset the node group is to be compatible with.
 * If not already a master nodeset, the master is obtained from it.
 * @return  Handle to new, empty node group field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_node_group_id cmzn_field_group_create_field_node_group(
	cmzn_field_group_id group, cmzn_nodeset_id nodeset);

/**
 * Find and return handle to the sub-object node group compatible with the
 * specified nodeset, if one exists for the group. The nodeset can be from
 * the group's own region or any subregion of it.
 *
 * @param group  Handle to group field to query.
 * @param nodeset  Handle to a nodeset the node group is to be compatible with.
 * If not already a master nodeset, the master is obtained from it.
 * @return  Handle to node group field, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_field_node_group_id cmzn_field_group_get_field_node_group(
	cmzn_field_group_id group, cmzn_nodeset_id nodeset);

/**
 * Create and return an element group field compatible with the
 * supplied mesh, i.e. able to contain elements from its master mesh. The
 * element group field is registered as a sub-object group for this group.
 * The mesh can be from the group's own region or any subregion of it.
 * Subregion groups for any intermediate and the final mesh-owning regions are
 * automatically created to contain the element group, if needed.
 * Fails if a compatible element group field already exists.
 *
 * @param group  Handle to group field to modify.
 * @param mesh  Handle to a mesh the element group is to be compatible with.
 * If not already a master mesh, the master is obtained from it.
 * @return  Handle to new, empty element group field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_element_group_id cmzn_field_group_create_field_element_group(
	cmzn_field_group_id group, cmzn_mesh_id mesh);

/**
 * Find and return handle to the sub-object element group compatible with the
 * specified mesh, if one exists for the group.
 * The mesh can be from the group's own region or any subregion of it.
 *
 * @param group  Handle to group field to query.
 * @param mesh  Handle to a mesh the element group is to be compatible with.
 * If not already a master mesh, the master is obtained from it.
 * @return  Handle to element group field, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_field_element_group_id cmzn_field_group_get_field_element_group(
	cmzn_field_group_id group, cmzn_mesh_id mesh);

/**
 * Get a subgroup of the given group for the specified domain.
 *
 * @param group the group field
 * @param domain the domain field
 * @return  Handle to field representing subobject group, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_field_id cmzn_field_group_get_subobject_group_field_for_domain_field(
	cmzn_field_group_id group, cmzn_field_id domain);

/**
 * Return the first non-empty subregion group in the group tree including itself.
 *
 * @param group  the group field
 * @returns  Handle to first non-empty subregion group field, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_field_group_id cmzn_field_group_get_first_non_empty_subregion_field_group(
	cmzn_field_group_id group);

#ifdef __cplusplus
}
#endif

#endif
