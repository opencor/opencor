/**
 * @file differentialoperatorid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_DIFFERENTIALOPERATORID_H__
#define CMZN_DIFFERENTIALOPERATORID_H__

/**
 * @brief Describes the derivative of a field to evaluate.
 *
 * Describes the derivative of a field to evaluate. For example, del/del_xi1.
 * @see cmzn_mesh_get_chart_differentialoperator
 * @see cmzn_field_evaluate_derivative
 */
struct cmzn_differentialoperator;
typedef struct cmzn_differentialoperator *cmzn_differentialoperator_id;

#endif
