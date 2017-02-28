/**
 * @file fieldvectoroperators.h
 *
 * The public interface to fields that perform vector operations.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDVECTOROPERATORS_H__
#define CMZN_FIELDVECTOROPERATORS_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a vector field which is the cross product of the source_fields.
 * The number of source fields determines the number of components i.e.
 * dimension of the source_fields and the result. Allowable numbers:
 * 1 : single 2-D vector source field. Returns orthogonal vector in 2-D plane
 *     given by 90 degree rotation anticlockwise.
 * 2 : two 3-D vector source fields. Returns familiar 3-D cross product vector
 *     orthogonal to both source vectors in a right hand sense, whose magnitude
 *     is the area of the parallelogram formed by the source vectors on 2 sides.
 * 3 : cross product of three 4-D vector source fields.
 *
 * @param field_module  Region field module which will own the new field.
 * @param number_of_source_fields  The size of the source_fields array, one less
 * than the dimension i.e. number of components of each source_field and the
 * result. Only 1, 2 and 3 fields i.e. 2-D, 3-D and 4-D are supported.
 * @param source_fields  Array of fields with number of components equal to the
 * dimension i.e. one more than the number_of_source_fields.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_cross_product(
	cmzn_fieldmodule_id field_module, int number_of_source_fields,
	cmzn_field_id *source_fields);

/**
 * Convenience function creating a field giving the 3-D cross product of two
 * 3-component vector source fields. Resulting field has 3 components.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First source field. Must have 3 components.
 * @param source_field_two  Second source field.  Must have 3 components.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_cross_product_3d(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a scalar field whose value is the dot product of the two supplied
 * source fields, which must have equal numbers of components.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First source field.
 * @param source_field_two  Second source field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_dot_product(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a scalar field returning the magnitude of the vector source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Source field to normalise.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_magnitude(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * Creates a field returning the values of source vector field normalised to
 * unit length.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Source field to normalise.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_normalise(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * Creates a field which has one component equal to the sum of all components of
 * the source field. Also called the L1, taxicab or manhattan norm.
 * For weighted sum of components use a dot_product with a constant weights field.
 * @see cmzn_fieldmodule_create_field_dot_product
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  The field whose components are to be summed.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_sum_components(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

#ifdef __cplusplus
}
#endif

#endif
