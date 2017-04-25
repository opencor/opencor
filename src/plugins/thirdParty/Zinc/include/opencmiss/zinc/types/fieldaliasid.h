/**
 * @file fieldaliasid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDALIASID_H__
#define CMZN_FIELDALIASID_H__

/**
 * @brief A specialised field type creating a local alias
 *
 * A specialised field type creating a local alias for a field from another
 * region. Currently only non-spatially-varying fields can be successfully used
 * from an alias field.
 */
struct cmzn_field_alias;
typedef struct cmzn_field_alias *cmzn_field_alias_id;

#endif /* CMZN_FIELD_ALIAS_ID_H */
