/**
 * @file fieldnodesetoperators.h
 *
 * Implements field operators that sum or process fields over a nodeset.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDNODESETOPERATORS_H__
#define CMZN_FIELDNODESETOPERATORS_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"
#include "types/nodeid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field which computes the sum of each source field component over
 * all nodes in the nodeset for which it is defined. Returned field has same
 * number of components as the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to sum.
 * @param nodeset  The set of nodes to sum field over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_nodeset_sum(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	cmzn_nodeset_id nodeset);

/**
 * Creates a field which computes the mean of each source field component over
 * all nodes in the nodeset for which it is defined. Returned field has same
 * number of components as the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to obtain mean component values for.
 * @param nodeset  The set of nodes to obtain mean over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_nodeset_mean(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	cmzn_nodeset_id nodeset);

/**
 * Creates a field which computes the sum of the squares of each source field
 * component over all nodes in the nodeset for which it is defined. Returned
 * field has same number of components as the source field.
 * This field type supports least-squares optimisation by giving individual
 * terms being squared and summed.
 * @see cmzn_optimisation_add_objective_field
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to sum squared component values of.
 * @param nodeset  The set of nodes to sum field over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_nodeset_sum_squares(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	cmzn_nodeset_id nodeset);

/**
 * Creates a field which computes the mean of the squares of each source field
 * component over all nodes in the nodeset for which it is defined. Returned
 * field has same number of components as the source field.
 * This field type supports least-squares optimisation by giving individual
 * terms being squared and summed, each divided by the square root of the number
 * of terms.
 * @see cmzn_optimisation_add_objective_field
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to obtain mean squared component values for.
 * @param nodeset  The set of nodes to obtain mean over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_nodeset_mean_squares(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	cmzn_nodeset_id nodeset);

/**
 * Creates a field which computes the minimum of each source field
 * component over all nodes in the nodeset for which it is defined. Returned
 * field has same number of components as the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to obtain minimum values for.
 * @param nodeset  The set of nodes to obtain minimum over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_nodeset_minimum(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	cmzn_nodeset_id nodeset);

/**
 * Creates a field which computes the maximum of each source field
 * component over all nodes in the nodeset for which it is defined. Returned
 * field has same number of components as the source field.
 *
 * @param field_module  Region field module which will own new field.
 * @param source_field  Field to obtain maximum values for.
 * @param nodeset  The set of nodes to obtain maximum over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_nodeset_maximum(
	cmzn_fieldmodule_id field_module, cmzn_field_id source_field,
	cmzn_nodeset_id nodeset);

#ifdef __cplusplus
}
#endif

#endif
