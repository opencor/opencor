/**
 * @file contextid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_CONTEXTID_H__
#define CMZN_CONTEXTID_H__

/**
 * @brief The context is the primary object created for each instance of Zinc.
 *
 * The context is the primary object created for each instance of Zinc. All
 * other objects are created or obtained directly or indirectly from it.
 * If multiple contexts are used, ensure that only objects stemming from the
 * same context are used together.
 */
struct cmzn_context;
typedef struct cmzn_context * cmzn_context_id;

#endif
