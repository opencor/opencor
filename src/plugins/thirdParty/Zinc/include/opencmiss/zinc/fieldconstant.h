/**
 * @file fieldconstant.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDCONSTANT_H__
#define CMZN_FIELDCONSTANT_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field with the components specified in the array values.
 * Internally this a composite field.
 *
 * @param field_module  Region field module which will own new field.
 * @param number_of_values  The number of values in the array.
 * @param values The array of constant values
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_constant(cmzn_fieldmodule_id field_module,
	int number_of_values, const double *values);

/**
 * Creates a string constant field with the supplied
 * string value in <string_constant>.
 *
 * @param field_module  Region field module which will own new field.
 * @param string_constant The constant char string.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_string_constant(cmzn_fieldmodule_id field_module,
	const char *string_constant);

#ifdef __cplusplus
}
#endif

#endif // CMZN_FIELDCONSTANT_H
