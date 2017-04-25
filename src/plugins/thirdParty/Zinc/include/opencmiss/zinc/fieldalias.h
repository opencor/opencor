/**
 * @file fieldalias.h
 *
 * Implements a zinc field which is an alias for another field, commonly from a
 * different region to make it available locally.
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDALIAS_H__
#define CMZN_FIELDALIAS_H__

#include "types/fieldid.h"
#include "types/fieldaliasid.h"
#include "types/fieldmoduleid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a zinc field which is an alias for another field, possibly from
 * another region.
 * This is the main mechanism for reusing field definitions from other regions.
 *
 * @param field_module  Region field module which will own new field.
 * @param original_field  The field which the new field will be an alias for. Can
 * be from a different region.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_alias(cmzn_fieldmodule_id field_module,
	cmzn_field_id original_field);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CMZN_FIELDALIAS_H */
