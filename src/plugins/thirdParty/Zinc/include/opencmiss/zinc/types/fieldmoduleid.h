/**
 * @file fieldmoduleid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDMODULEID_H__
#define CMZN_FIELDMODULEID_H__

/**
 * @brief Container/manager of fields and domains within a region.
 *
 * Container/manager of fields and domains within a region.
 *
 * Note that fieldmodule create field methods documented as supporting
 * 'automatic scalar broadcast' for their source field arguments work as
 * follows: if given a mix of scalar and vector source fields, the scalar value
 * is applied to each component of the vector, e.g. the result of
 * ADD(CONSTANT([1 2 3 4], CONSTANT([10]) is [11 12 13 14]. Internally it works
 * by substituing the scalar operand with a concatenate field with the same
 * number of components as the vector operand, but filled with the scalar value.
 */
struct cmzn_fieldmodule;
typedef struct cmzn_fieldmodule *cmzn_fieldmodule_id;

/**
 * @brief Manages individual user notification of changes with a field module.
 *
 * Manages individual user notification of changes with a field module.
 */
struct cmzn_fieldmodulenotifier;
typedef struct cmzn_fieldmodulenotifier *cmzn_fieldmodulenotifier_id;

/**
 * @brief Information about changes to fields and other objects in the field module.
 *
 * Information about changes to fields and other objects in the field module,
 * sent with each callback from the fieldmodule notifier.
 */
struct cmzn_fieldmoduleevent;
typedef struct cmzn_fieldmoduleevent *cmzn_fieldmoduleevent_id;

typedef void (*cmzn_fieldmodulenotifier_callback_function)(
	cmzn_fieldmoduleevent_id event, void *client_data);

#endif /* CMZN_FIELD_MODULE_ID_H */
