/**
 * @file fieldcacheid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDCACHEID_H__
#define CMZN_FIELDCACHEID_H__

/**
 * @brief Cache for setting domain locations at which fields are evaluated or
 * assigned.
 *
 * Cache for setting domain locations at which fields are to be evaluated or
 * assigned, and for storing intermediate values and data structures so that
 * multiple evaluations are more efficient.
 */
struct cmzn_fieldcache;
typedef struct cmzn_fieldcache *cmzn_fieldcache_id;

#endif
