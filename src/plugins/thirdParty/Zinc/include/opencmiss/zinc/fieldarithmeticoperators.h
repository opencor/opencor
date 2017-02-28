/**
 * @file fieldarithmeticoperators.h
 *
 * The public interface to fields that perform arithmetic operators.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDARITHMETICOPERATORS_H__
#define CMZN_FIELDARITHMETICOPERATORS_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field where the which calculates the components of source_field_one
 * raised to the power of the components in source_field_two.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_power(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a field where the which multiplies the components of source_field_one and
 * source_field_two.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_multiply(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a field where the which divides the components of source_field_one
 * by source_field_two.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_divide(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a field where the which adds the components of source_field_one and
 * source_field_two.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_add(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a field which gives the result of subtracting source_field_two
 * from source_field_one.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First input field
 * @param source_field_two  Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_subtract(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two);

/**
 * Creates a field where the field components are the natural logarithm
 * of each component in the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field The input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_log(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the field components are the square root
 * of each component in the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field The input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_sqrt(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the field components are the natural exponent
 * of each component in the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field The input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_exp(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field where the field components are the absolute value of each
 * component in the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field The input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_abs(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

#ifdef __cplusplus
}
#endif

#endif /* CMZN_FIELDARITHMETICOPERATORS_H__ */
