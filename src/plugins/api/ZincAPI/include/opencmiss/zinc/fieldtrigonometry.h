/**
 * @file fieldtrigonometry.h
 *
 * The public interface to fields that perform trigonometric operators.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDTRIGONOMETRY_H__
#define CMZN_FIELDTRIGONOMETRY_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field where the components are the sine value (using radians) of the
 * components of the source_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field Input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_sin(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the components are the cosine value (using radians) of the
 * components of the source_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field Input field (components in radians)
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_cos(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the components are the trigonometric tangent value
 * (using radians) of the components of the source_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field Input field (components in radians)
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_tan(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the components are the arcsine value (using radians) of the
 * components of the source_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field Input field (components in radians)
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_asin(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the components are the arccosine value (using radians) of the
 * components of the source_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field Input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_acos(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the components are the arctangent value (using radians) of the
 * components of the source_field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field Input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_atan(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the components are calculated using the atan2 c function,
 * so that the angle returned (in radians) is the angle between a positive x axis in
 * a plane and the vector (x,y) where x is the source_field_one component and y is
 * the source_field_two component.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_atan2(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

#ifdef __cplusplus
}
#endif

#endif
