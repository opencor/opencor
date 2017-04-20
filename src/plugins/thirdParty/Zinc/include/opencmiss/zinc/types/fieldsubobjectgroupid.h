/**
 * @file fieldsubobjectgroupid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDSUBOBJECTGROUPID_H__
#define CMZN_FIELDSUBOBJECTGROUPID_H__

/**
 * @brief A field which defines a subset of nodes from a master nodeset.
 *
 * A field which defines a subset of nodes from a master nodeset, by returning
 * true/1 on nodes in the group, false/0 otherwise. From this field one must
 * obtain the nodeset group object to work with the resulting subset: to
 * add/remove nodes, create a node iterator etc.
 */
struct cmzn_field_node_group;
typedef struct cmzn_field_node_group *cmzn_field_node_group_id;

/**
 * @brief A field which defines a subset of elements from a master mesh.
 *
 * A field which defines a subset of elements from a master mesh, by returning
 * true/1 on elements in the group, false/0 otherwise. From this field one must
 * obtain the mesh group object to work with the resulting subset: to
 * add/remove elements, create an element iterator etc.
 */
struct cmzn_field_element_group;
typedef struct cmzn_field_element_group *cmzn_field_element_group_id;

#endif
