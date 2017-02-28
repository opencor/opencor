/**
 * @file nodeid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_NODESETID_H__
#define CMZN_NODESETID_H__

/**
 * @brief A set of nodes or points.
 *
 * A set of nodes or points, equivalent to a zero-dimensional mesh.
 */
struct cmzn_nodeset;
typedef struct cmzn_nodeset *cmzn_nodeset_id;

/**
 * @brief A subset of a master nodeset.
 *
 * A specialised nodeset consisting of a subset of nodes from a master nodeset.
 */
struct cmzn_nodeset_group;
typedef struct cmzn_nodeset_group *cmzn_nodeset_group_id;

/**
 * @brief A description of field parameters to define at a node.
 *
 * A description of field parameters to define at a node (incl. value/derivative
 * types, versions), used as a template for creating new nodes in a nodeset, or
 * merging into a node to define or undefine fields on it.
 */
struct cmzn_nodetemplate;
typedef struct cmzn_nodetemplate *cmzn_nodetemplate_id;

/**
 * @brief Point object used to represent finite element nodes.
 *
 * Point object used to represent finite element nodes, data points etc.
 * Important notes:
 * An unlimited number of fields may be defined at nodes to store parameters,
 * derivatives, host mesh locations, strings etc. Coordinates are just another
 * field: there can be zero or more coordinate fields defined on a node.
 * Also, since Zinc elements are not iso-parametric, it is not sufficient to
 * define field parameters at a node in the same manner as an existing field:
 * the new field must also be explicitly defined on the elements.
 */
struct cmzn_node;
typedef struct cmzn_node *cmzn_node_id;

/**
 * @brief An iterator for looping through all the nodes in a nodeset.
 *
 * An iterator for looping through all the nodes in a nodeset.
 */
struct cmzn_nodeiterator;
typedef struct cmzn_nodeiterator * cmzn_nodeiterator_id;

/**
 * Enumerated labels for field value/derivative parameters held at nodes.
 */
enum cmzn_node_value_label
{
	CMZN_NODE_VALUE_LABEL_INVALID = 0,     /*!< Unspecified node value label */
	CMZN_NODE_VALUE_LABEL_VALUE = 1,       /*!< literal field value */
	CMZN_NODE_VALUE_LABEL_D_DS1 = 2,       /*!< derivative w.r.t. arc length S1 */
	CMZN_NODE_VALUE_LABEL_D_DS2 = 3,       /*!< derivative w.r.t. arc length S2 */
	CMZN_NODE_VALUE_LABEL_D2_DS1DS2 = 4,   /*!< cross derivative w.r.t. arc lengths S1,S2 */
	CMZN_NODE_VALUE_LABEL_D_DS3 = 5,       /*!< derivative w.r.t. arc length S3 */
	CMZN_NODE_VALUE_LABEL_D2_DS1DS3 = 6,   /*!< cross derivative w.r.t. arc lengths S1,S3 */
	CMZN_NODE_VALUE_LABEL_D2_DS2DS3 = 7,   /*!< cross derivative w.r.t. arc lengths S2,S3 */
	CMZN_NODE_VALUE_LABEL_D3_DS1DS2DS3 = 8 /*!< triple cross derivative w.r.t. arc lengths S1,S2,S3 */
};

/**
 * @brief Object describing changes to a nodeset in a fieldmoduleevent.
 *
 * Object describing changes to a nodeset in a fieldmoduleevent
 */
struct cmzn_nodesetchanges;
typedef struct cmzn_nodesetchanges *cmzn_nodesetchanges_id;

/**
 * Bit flags summarising changes to a node or nodes in a nodeset.
 */
enum cmzn_node_change_flag
{
	CMZN_NODE_CHANGE_FLAG_NONE = 0,
		/*!< node(s) not changed */
	CMZN_NODE_CHANGE_FLAG_ADD = 1,
		/*!< node(s) added */
	CMZN_NODE_CHANGE_FLAG_REMOVE = 2,
		/*!< node(s) removed */
	CMZN_NODE_CHANGE_FLAG_IDENTIFIER = 4,
		/*!< node(s) identifier changed */
	CMZN_NODE_CHANGE_FLAG_DEFINITION = 8,
		/*!< node(s) definition other than identifier changed; currently none */
	CMZN_NODE_CHANGE_FLAG_FIELD = 16
		/*!< change to field values mapped to node(s) */
};

/**
 * Type for passing logical OR of #cmzn_node_change_flag
 */
typedef int cmzn_node_change_flags;

#endif
