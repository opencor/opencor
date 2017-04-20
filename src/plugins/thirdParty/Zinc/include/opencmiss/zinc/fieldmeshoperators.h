/**
 * @file fieldmeshoperators.h
 *
 * Implements field operators that sum or process fields over a mesh.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDMESHOPERATORS_H__
#define CMZN_FIELDMESHOPERATORS_H__

#include "types/fieldid.h"
#include "types/fieldmoduleid.h"
#include "types/elementid.h"
#include "types/fieldmeshoperatorsid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a field which calculates the numerical integral over the mesh of
 * integrand.dV for volume in 3-D, dA for area in 2-D, dL for length in 1-D.
 * Returned field has same number of components as the integrand field,
 * with each component integrated separately.
 * By default, the integral is calculated by 1-point Gaussian quadrature,
 * sufficient only for linear interpolation. Separate methods are available to
 * set the numbers of quadrature points.
 * Note: uses absolute value of dV/dA/dL so works with right- or left- handed
 * element local coordinate systems. However, cannot handle elements that are
 * partly or completely inverted from expected handedness.
 * @see cmzn_field_mesh_integral_set_numbers_of_points
 *
 * @param field_module  Region field module which will own the new field.
 * @param integrand_field  Field to integrate. An integrand of constant value 1
 * gives the volume/area/length of the mesh.
 * @param coordinate_field  Field supplying coordinates; must have at least as
 * many components as the mesh, up to a maximum of 3. It is up to the user to
 * supply a field giving values in a Rectangular Cartesian coordinate system,
 * using a coordinate transformation field conversion if needed.
 * @param mesh  The mesh to integrate over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_mesh_integral(
	cmzn_fieldmodule_id field_module, cmzn_field_id integrand_field,
	cmzn_field_id coordinate_field, cmzn_mesh_id mesh);

/**
 * If the field is a mesh integral type field, return the derived field handle.
 *
 * @param field  The field to be cast.
 * @return  Handle to derived mesh integral field, or NULL/invalid handle if
 * wrong type or failed.
 */
ZINC_API cmzn_field_mesh_integral_id cmzn_field_cast_mesh_integral(cmzn_field_id field);

/**
 * Cast mesh integral field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param mesh_integral_field  Handle to the mesh integral field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_mesh_integral_base_cast(
	cmzn_field_mesh_integral_id mesh_integral_field)
{
	return (cmzn_field_id)(mesh_integral_field);
}

/**
 * Destroys handle to the mesh integral field (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param mesh_integral_field_address  Address of handle to the field to
 * destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_mesh_integral_destroy(
	cmzn_field_mesh_integral_id *mesh_integral_field_address);

/**
 * Get the numbers of quadrature points in each element axis.
 * @see cmzn_field_mesh_integral_set_numbers_of_points
 *
 * @param mesh_integral_field  Handle to the mesh integral field.
 * @param values_count  Size of the values array i.e. the requested count.
 * Pass 0 to request the number of values set.
 * @param values  Array to receive numbers of quadrature points.  Values for
 * dimensions beyond the size set use the last value set.
 * @return  The actual count of quadrature point numbers that have been
 * explicitly set using cmzn_field_mesh_integral_set_numbers_of_points. This
 * can be more than the number requested, so a second call may be needed with a
 * larger array. Returns 0 on error.
 */
ZINC_API int cmzn_field_mesh_integral_get_numbers_of_points(
	cmzn_field_mesh_integral_id mesh_integral_field,
	int values_count, int *values);

/**
 * Set the numbers of quadrature points in each element axis.
 * The numbers are applied on the top-level-element with the appropriate
 * values inherited on faces and lines; this is only important if different
 * numbers are used in each element axis.
 * Simplex elements use the maximum number set on any linked dimension.
 * For 1-D Gaussian quadrature, N points exactly integrates a polynomial of
 * degree 2N - 1, however 1 more point than the degree is often needed to avoid
 * spurious modes in many numerical solutions. Simplex elements use specialised
 * point arrangements that are sufficient for integrating a polynomial with the
 * same degree as the number of points.
 * The maximum number of Gauss points on each element axis is currently 4; if
 * a higher number is supplied, only 4 are used.
 * There is no upper limit on the numbers of points with midpoint quadrature.
 *
 * @param mesh_integral_field  Handle to the mesh integral field.
 * @param values_count  Size of the values array, at least 1.
 * @param values  Array containing numbers of quadrature points >=1, applied on
 * the corresponding element axis, with the last value in the arrray applying
 * on all subsequent element axes. For example [1, 2, 3] gives 1 point on xi1,
 * 2 points on xi2 and 3 points on xi3; [2] gives 2 points on all axes.
 * Use of different numbers is generally only applicable to meshes with
 * particular functions or geometries aligned with the same element local xi
 * direction in all elements; for example a tubular mesh with bicubic elements
 * around the exterior (xi1 and xi2) and linear through the thickness (x3)
 * would benefit from using [4, 4, 2] points for full integration.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_mesh_integral_set_numbers_of_points(
	cmzn_field_mesh_integral_id mesh_integral_field,
	int values_count, const int *values);

/**
 * Get element quadrature rule for numerical integration.
 *
 * @param mesh_integral_field  Handle to mesh integral field to query.
 * @return  The element quadrature rule, or
 * CMZN_ELEMENT_QUADRATURE_RULE_INVALID if bad argument.
 */
ZINC_API enum cmzn_element_quadrature_rule cmzn_field_mesh_integral_get_element_quadrature_rule(
	cmzn_field_mesh_integral_id mesh_integral_field);

/**
 * Set element quadrature rule for numerical integration. The current numbers
 * of quadrature points remains unchanged, and can be set by a separate API
 * method.
 * @see cmzn_element_quadrature_rule
 * @see cmzn_field_mesh_integral_set_numbers_of_points
 *
 * @param mesh_integral_field  Handle to mesh integral field to modify.
 * @param quadrature_rule  The quadrature rule to use.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_mesh_integral_set_element_quadrature_rule(
	cmzn_field_mesh_integral_id mesh_integral_field,
	enum cmzn_element_quadrature_rule quadrature_rule);

/**
 * Creates a specialisation of the mesh integral field that integrates the
 * squares of the components of the integrand field. Note that the
 * volume/area/length and weights are not squared in the integral.
 * This field type supports least-squares optimisation by giving individual
 * terms being squared and summed.
 * This field can be cast to a mesh integral field to set type-specific
 * attributes.
 * @see cmzn_fieldmodule_create_field_mesh_integral
 * @see cmzn_field_mesh_integral_id
 *
 * @param field_module  Region field module which will own the new field.
 * @param integrand_field  Field to integrate the square of.
 * @param coordinate_field  Field supplying coordinates; must have at least as
 * many components as the mesh, up to a maximum of 3. It is up to the user to
 * supply a field giving values in a Rectangular Cartesian coordinate system,
 * using a coordinate transformation field conversion if needed.
 * @param mesh  The mesh to integrate over.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_mesh_integral_squares(
	cmzn_fieldmodule_id field_module, cmzn_field_id integrand_field,
	cmzn_field_id coordinate_field, cmzn_mesh_id mesh);

#ifdef __cplusplus
}
#endif

#endif
