/**
 * @file fieldsubobjectgroup.h
 *
 * Implements region sub object groups, e.g. node group, element group.
 * These group fields evaluate to 1 (true) at domain locations in the group, and
 * 0 elsewhere.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDSUBOBJECTGROUP_H__
#define CMZN_FIELDSUBOBJECTGROUP_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"
#include "types/elementid.h"
#include "types/nodeid.h"
#include "types/fieldsubobjectgroupid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a node group field which defines a nodeset group i.e. a subset
 * of nodes from a master nodeset. As a field it evaluates to 1 for nodes in
 * the nodeset group and 0 elsewhere, i.e. it is the predicate for the sub-
 * domain, and this Boolean value can be combined in logical operations with
 * other fields.
 * @see cmzn_nodeset_group_id
 *
 * @param field_module  Region field module which will own new field.
 * @param mesh  Handle to a nodeset the node group is to be compatible with. If
 * it is not a master nodeset, the master is obtained from it.
 * Nodeset must be from the same region as field_module.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_node_group(
	cmzn_fieldmodule_id field_module, cmzn_nodeset_id nodeset);

/**
 * If field can be cast to a node group return a handle to the
 * derived field.
 *
 * @param field  handle to the field to cast
 * @return  Handle to derived node group field, or
 * NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_node_group_id cmzn_field_cast_node_group(cmzn_field_id field);

/**
 * Cast node group field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param group  Handle to the node group field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_node_group_base_cast(cmzn_field_node_group_id group)
{
	return (cmzn_field_id)(group);
}

/**
 * Destroy the reference to the node group.
 *
 * @param group_address  address to the handle to the node group field
 * @return  Status CMZN_OK if successfully destroy the node group,
 * any other value on failure.
 */
ZINC_API int cmzn_field_node_group_destroy(cmzn_field_node_group_id *node_group_address);

/**
 * Get a handle to the 'dual' nodeset group of this node group, which provides
 * most of the methods for working with it.
 *
 * @param node_group  Handle to node group field.
 * @return  Handle to nodeset group, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_nodeset_group_id cmzn_field_node_group_get_nodeset_group(
	cmzn_field_node_group_id node_group);

/**
 * Creates an element group field which defines a mesh group i.e. a
 * subset of elements from a master mesh. As a field it evaluates to 1 for
 * elements in the mesh group and 0 elsewhere, i.e. it is the predicate for the
 * sub-domain, and this Boolean value can be combined in logical operations with
 * other fields.
 * @see cmzn_mesh_group_id
 *
 * @param field_module  Region field module which will own new field.
 * @param mesh  Handle to a finite element mesh the element group is to be
 * compatible with. If it is not a master mesh, the master is obtained from it.
 * Mesh must be from the same region as field_module.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_element_group(
	cmzn_fieldmodule_id field_module, cmzn_mesh_id mesh);

/**
 * If field can be cast to an element group return a handle to the
 * derived field.
 *
 * @param field  handle to the field to cast
 * @return  Handle to derived element group field, or NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_element_group_id cmzn_field_cast_element_group(cmzn_field_id field);

/**
 * Cast element group field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param group  Handle to the element group field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_element_group_base_cast(cmzn_field_element_group_id group)
{
	return (cmzn_field_id)(group);
}

/**
 * Destroy the reference to the element group.
 *
 * @param element_group_address  address to the handle to the element group field
 * @return  Status CMZN_OK if successfully destroy the element group,
 * any other value on failure.
 */
ZINC_API int cmzn_field_element_group_destroy(cmzn_field_element_group_id *element_group_address);

/**
 * Get a handle to the 'dual' mesh group of this element group, which provides
 * most of the methods for working with it.
 *
 * @param element_group  Handle to element group field.
 * @return  Handle to mesh group, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_mesh_group_id cmzn_field_element_group_get_mesh_group(
	cmzn_field_element_group_id element_group);

#ifdef __cplusplus
}
#endif

#endif
