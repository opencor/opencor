/**
 * @file fieldgroupid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDGROUPID_H__
#define CMZN_FIELDGROUPID_H__

/**
 * @brief A generic group field used for grouping local subobjects.
 *
 * A generic group field type able to record which local subobjects are in the
 * group (recorded by attached node group and element group fields), or whether
 * the entire local region is in the group. This field returns true/1 at domain
 * locations in the group, false/0 otherwise.
 * The group field also maintains links to child groups in child regions, for
 * building subsets of entire region trees.
 */
struct cmzn_field_group;
typedef struct cmzn_field_group *cmzn_field_group_id;

/**
 * Enumeration controlling how subelements (faces, lines, nodes) are handled
 * when parent elements are added or removed from subobject groups.
 */
enum cmzn_field_group_subelement_handling_mode
{
	CMZN_FIELD_GROUP_SUBELEMENT_HANDLING_MODE_INVALID = 0,
	/*!< Invalid mode */
	CMZN_FIELD_GROUP_SUBELEMENT_HANDLING_MODE_NONE = 1,
	/*!< Do not add/remove faces, lines or nodes with parent elements.
	 * This is the default mode for new groups. */
	CMZN_FIELD_GROUP_SUBELEMENT_HANDLING_MODE_FULL = 2
	/*!< Ensure group contains all faces, lines and nodes for any higher dimension
	 * element added to the group; on removal of an element remove any of its
	 * faces, lines and nodes not part of any neighbour element.
	 * More expensive to maintain. */
};

#endif
