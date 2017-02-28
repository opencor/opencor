/**
 * @file fieldlogicaloperators.h
 *
 * The public interface to fields that perform logical operations.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDLOGICALOPERATORS_H__
#define CMZN_FIELDLOGICALOPERATORS_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field whose component values are 1 if that component of
 * source_field_one AND source_field_two is non-zero, 0 otherwise.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First input field
 * @param source_field_two  Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_and(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one, cmzn_field_id source_field_two);

/**
 * Creates a field whose component values are 1 if that component of
 * source_field_one EQUALS that component of source_field_two, 0 otherwise.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First input field
 * @param source_field_two  Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_equal_to(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one, cmzn_field_id source_field_two);

/**
 * Creates a field whose component values are 1 if that component of
 * source_field_one is greater than the component value in source_field_two.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_greater_than(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one, cmzn_field_id source_field_two);

/**
 * Creates a field whose component values are 1 if that component of
 * source_field_one is less than the component value in source_field_two.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one First input field
 * @param source_field_two Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_less_than(
	cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one, cmzn_field_id source_field_two);

/**
 * Creates a field returning 1 (true) at locations where the source field is
 * defined and 0 (false) elsewhere.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to check whether defined at location.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_is_defined(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field);

/**
 * Creates a field whose component values are 1 if that component of
 * source_field_one OR source_field_two is non-zero, 0 otherwise.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First input field
 * @param source_field_two  Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_or(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one, cmzn_field_id source_field_two);

/**
 * Creates a field whose component values are 1 if that component of the
 * source_field is zero, 0 otherwise; effectively a component-wise logical not
 * operator. Returned field has same number of components as source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  The source field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_not(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field);

/**
 * Creates a field whose component values are 1 if that component of
 * source_field_one OR source_field_two is non-zero (but not both), 0 otherwise.
 * Automatic scalar broadcast will apply, see field.h.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  First input field
 * @param source_field_two  Second input field
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_xor(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one, cmzn_field_id source_field_two);

#ifdef __cplusplus
}
#endif

#endif
