/**
 * @file fieldcoordinatetransformation.h
 *
 * Field coordinate transformation operators.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN__FIELDCOORDINATETRANSFORMATION_H__
#define CMZN__FIELDCOORDINATETRANSFORMATION_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field which performs a coordinate transformation from the source
 * field values in their coordinate system type into the coordinate system type
 * of this field. Returned field has 3 components.
 * @see cmzn_field_set_coordinate_system_type
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Source field with values in its own coordinate system.
 * Must have 1 to 3 components.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_coordinate_transformation(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * Creates a field which performs a coordinate transformation of vectors from
 * their original coordinate system and coordinate positions, to the coordinate
 * system of this field. Sets the number of components in returned field to 3
 * times the number of vectors expected from the source vector_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param vector_field  Vector field to be transformed. Can be a single vector
 * (1,2 or 3 components), two vectors (4 or 6 components) or three vectors
 * (9 components).
 * @param coordinate_field  Field giving location where vector value is from.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_vector_coordinate_transformation(
	cmzn_fieldmodule_id field_module, cmzn_field_id vector_field,
	cmzn_field_id coordinate_field);

#ifdef __cplusplus
}
#endif

#endif /* ifndef CMZN__FIELDCOORDINATETRANSFORMATION_H */
