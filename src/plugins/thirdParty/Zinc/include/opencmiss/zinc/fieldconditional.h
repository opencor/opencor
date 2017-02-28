/**
 * @file fieldconditional.h
 *
 * Implements zinc fields which conditionally calculate their inputs.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDCONDITIONAL_H__
#define CMZN_FIELDCONDITIONAL_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a conditional field with the same number of components as each of the
 * source_fields. For each component, if the value of source_field_one is TRUE
 * (non-zero) then the result will be the value of source_field_two, otherwise the
 * component result will be taken from source_field_three.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field_one  Conditional field.
 * @param source_field_two  TRUE = non-zero conditional component results.
 * @param source_field_three  FALSE = zero conditional component results.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_if(cmzn_fieldmodule_id field_module,
	cmzn_field_id source_field_one,
	cmzn_field_id source_field_two,
	cmzn_field_id source_field_three);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CMZN_FIELDCONDITIONAL_H */
