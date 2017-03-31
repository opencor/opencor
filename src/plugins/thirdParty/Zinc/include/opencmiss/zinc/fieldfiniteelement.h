/**
 * @file fieldfiniteelement.h
 *
 * Implements fields interpolated over finite element meshes with
 * parameters indexed by nodes.
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDFINITEELEMENT_H__
#define CMZN_FIELDFINITEELEMENT_H__

#include "types/elementid.h"
#include "types/fieldid.h"
#include "types/fieldcacheid.h"
#include "types/fieldfiniteelementid.h"
#include "types/fieldmoduleid.h"
#include "types/nodeid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a real-valued finite_element field which can be interpolated over a
 * finite element mesh with parameters indexed by nodes.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param number_of_components  The number of components for the new field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_finite_element(
	cmzn_fieldmodule_id fieldmodule, int number_of_components);

/**
 * If the field is real-valued interpolated finite element then this function
 * returns the derived finite element field handle.
 *
 * @param field  The field to be cast.
 * @return  Handle to derived finite element field, or NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_finite_element_id cmzn_field_cast_finite_element(cmzn_field_id field);

/**
 * Cast finite element field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param finite_element_field  Handle to the finite element field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_finite_element_base_cast(
	cmzn_field_finite_element_id finite_element_field)
{
	return (cmzn_field_id)(finite_element_field);
}

/**
 * Destroys handle to the finite_element field (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param finite_element_field_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_finite_element_destroy(
	cmzn_field_finite_element_id *finite_element_field_address);

/**
 * Get parameters for finite element field at node.
 * Note that the node and other part locations such as time must be set in the
 * field cache passed to this function.
 * @see cmzn_fieldcache_set_node
 *
 * @param field  The finite element field to get parameters for.
 * @param cache  Store of location to get values and working cache. Must have
 * set the node in the cache, and time if field is time-varying.
 * @param component_number  The component to get parameters for, from 1 to the
 * number of field components, or -1 to get all components together. Must be
 * at least one component for version; missing components are set as zero.
 * @param node_value_label  The nodal value / derivative label to get
 * parameters for.
 * @param version_number  The nodal version number to get parameters for,
 * starting at 1.
 * @param values_count  Size of values array. Checked that it equals or
 * exceeds the number of components of field, or 1 if setting one component.
 * @param values_out  Array of real values to be set from the parameters.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_finite_element_get_node_parameters(
	cmzn_field_finite_element_id finite_element_field, cmzn_fieldcache_id cache,
	int component_number, enum cmzn_node_value_label node_value_label,
	int version_number, int values_count, double *values_out);

/**
 * Set parameters for finite element field at node.
 * Note that the node and other part locations such as time must be set in the
 * field cache passed to this function.
 * @see cmzn_fieldcache_set_node
 *
 * @param field  The finite element field to set parameters for.
 * @param cache  Store of location to assign at and working cache. Must have
 * set the node in the cache, and time if field is time-varying.
 * @param component_number  The component to set parameters for, from 1 to the
 * number of field components, or -1 to set all components together. Must be
 * at least one component for version; missing component values are ignored.
 * @param node_value_label  The nodal value / derivative label to set
 * parameters for.
 * @param version_number  The nodal version number to set parameters for,
 * starting at 1.
 * @param values_count  Size of values array. Checked that it equals or
 * exceeds the number of components of field, or 1 if setting one component.
 * @param values_in  Array of real values to be assigned to the parameters.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_finite_element_set_node_parameters(
	cmzn_field_finite_element_id finite_element_field, cmzn_fieldcache_id cache,
	int component_number, enum cmzn_node_value_label node_value_label,
	int version_number, int values_count, const double *values_in);

/**
 * Creates a field producing a value on 1-D line elements with as many
 * components as the source field, which gives the discontinuity of that field
 * between two adjacent surfaces by a chosen measure. An optional conditional
 * field restricts which adjacent surfaces qualify, with the first two used.
 * The field values are zero when the surfaces are continuous by the chosen
 * measure, and when there are fewer than two qualifying adjacent surfaces.
 * The default measure of discontinuity is C1.
 * In optimisation problems, adding an objective field consisting of the
 * integral of [squares of] this field over a 1-D mesh will favour high-
 * continuity solutions.
 * @see cmzn_field_edge_discontinuity_measure
 * @see cmzn_field_edge_discontinuity_set_measure
 * @see cmzn_field_edge_discontinuity_set_conditional_field
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param source_field  The source field to measure discontinuity of.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_edge_discontinuity(
	cmzn_fieldmodule_id fieldmodule, cmzn_field_id source_field);

/**
 * If the field is edge discontinuity type then returns the derived edge
 * discontinuity field handle.
 *
 * @param field  The field to be cast.
 * @return  Handle to derived edge discontinuity field, or NULL/invalid handle
 * if wrong type or failed.
 */
ZINC_API cmzn_field_edge_discontinuity_id cmzn_field_cast_edge_discontinuity(
	cmzn_field_id field);

/**
 * Cast edge discontinuity field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param edge_discontinuity_field  Handle to edge discontinuity field to cast.
 * @return  Non-accessed handle to the base field or NULL if invalid argument.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_edge_discontinuity_base_cast(
	cmzn_field_edge_discontinuity_id edge_discontinuity_field)
{
	return (cmzn_field_id)(edge_discontinuity_field);
}

/**
 * Destroys handle to the edge_discontinuity field (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param edge_discontinuity_field_address  Address of handle to the field to
 * destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_edge_discontinuity_destroy(
	cmzn_field_edge_discontinuity_id *edge_discontinuity_field_address);

/**
 * Get the conditional field controlling which surfaces are involved in the
 * measure of discontinuity.
 *
 * @param edge_discontinuity_field  Handle to the edge discontinuity field to
 * query.
 * @return  Handle to the conditional field, or NULL/invalid handle if none or
 * failed.
 */
ZINC_API cmzn_field_id cmzn_field_edge_discontinuity_get_conditional_field(
	cmzn_field_edge_discontinuity_id edge_discontinuity_field);

/**
 * Set the conditional field controlling which surfaces are involved in the
 * measure of discontinuity.
 *
 * @param edge_discontinuity_field  Handle to the edge discontinuity field to
 * modify.
 * @param conditional_field  Scalar field which is evaluated on surface
 * elements and must be true (non-zero) to include that element in the measure
 * of discontinuity. With no conditional_field and/or more adjacent qualifying
 * surfaces, the first two surfaces are used.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_edge_discontinuity_set_conditional_field(
	cmzn_field_edge_discontinuity_id edge_discontinuity_field,
	cmzn_field_id conditional_field);

/**
 * Get which measure of discontinuity is used: C1, G1, SURFACE_NORMAL.
 * @see cmzn_field_edge_discontinuity_measure
 *
 * @param edge_discontinuity_field  Handle to the edge discontinuity field to
 * query.
 * @return  Enumerated value specifying for measure of discontinuity, or
 * MEASURE_INVALID if invalid argument.
 */
ZINC_API cmzn_field_edge_discontinuity_measure
	cmzn_field_edge_discontinuity_get_measure(
	cmzn_field_edge_discontinuity_id edge_discontinuity_field);

/**
 * Set which measure of discontinuity is used: C1, G1, SURFACE_NORMAL.
 * @see cmzn_field_edge_discontinuity_measure
 *
 * @param edge_discontinuity_field  Handle to the edge discontinuity field to
 * modify.
 * @param measure  Enumerated value specifying which measure of discontinuity
 * is to be calculated.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_field_edge_discontinuity_set_measure(
	cmzn_field_edge_discontinuity_id edge_discontinuity_field,
	cmzn_field_edge_discontinuity_measure measure);

/**
 * Creates a field returning a value of a source field at an embedded location.
 * The new field has the same value type as the source field.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param source_field  Field to evaluate at the embedded location. Currently
 * restricted to having numerical values.
 * @param embedded_location_field  Field returning an embedded location, i.e.
 * find_mesh_location or stored mesh location fields.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_embedded(
	cmzn_fieldmodule_id fieldmodule, cmzn_field_id source_field,
	cmzn_field_id embedded_location_field);

/**
 * Creates a field returning the location in a mesh at which the calculated
 * source_field value equals the mesh_field value. Its values consist of an
 * element and coordinates in the element's local 'xi' coordinate chart.
 * Type-specific functions allow the search to find the nearest value and set a
 * conditional field.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param source_field  Source field whose value is to be searched for. Must have
 * the same number of numerical components as the mesh_field, and at least as
 * many as mesh dimension.
 * @param mesh_field  Field defined over the mesh which is to be matched with
 * source_field. Must have the same number of numerical components as the
 * source_field, and at least as many as mesh dimension.
 * @param mesh  The mesh to find locations in.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_find_mesh_location(
	cmzn_fieldmodule_id fieldmodule, cmzn_field_id source_field,
	cmzn_field_id mesh_field, cmzn_mesh_id mesh);

/**
 * If the field is of type find_mesh_location then this function returns the
 * derived find mesh location field handle.
 *
 * @param field  The field to be cast.
 * @return  Handle to derived find mesh location field, or NULL/invalid handle
 * if wrong type or failed.
 */
ZINC_API cmzn_field_find_mesh_location_id cmzn_field_cast_find_mesh_location(
	cmzn_field_id field);

/**
 * Cast find_mesh_location field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param find_mesh_location_field  Handle to the find_mesh_location field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_find_mesh_location_base_cast(
	cmzn_field_find_mesh_location_id find_mesh_location_field)
{
	return (cmzn_field_id)(find_mesh_location_field);
}

/**
 * Destroys handle to the find_mesh_location field and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param find_mesh_location_field_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_find_mesh_location_destroy(
	cmzn_field_find_mesh_location_id *find_mesh_location_field_address);

/**
 * Returns the mesh the find_mesh_location field is to find locations in.
 *
 * @param find_mesh_location_field  The field to query.
 * @return  Handle to the mesh, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_mesh_id cmzn_field_find_mesh_location_get_mesh(
	cmzn_field_find_mesh_location_id find_mesh_location_field);

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_field_find_mesh_location_search_mode
	cmzn_field_find_mesh_location_search_mode_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param format  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_field_find_mesh_location_search_mode_enum_to_string(
	enum cmzn_field_find_mesh_location_search_mode mode);

/**
 * Gets the search mode for the find_mesh_location field: whether finding
 * location with exact or nearest value.
 *
 * @param find_mesh_location_field  The field to query.
 * @return  The search mode.
 */
ZINC_API enum cmzn_field_find_mesh_location_search_mode
	cmzn_field_find_mesh_location_get_search_mode(
		cmzn_field_find_mesh_location_id find_mesh_location_field);

/**
 * Sets the search mode for the find_mesh_location field: whether finding
 * location with exact or nearest value.
 *
 * @param find_mesh_location_field  The field to modify.
 * @param search_mode  The search mode to set.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_find_mesh_location_set_search_mode(
	cmzn_field_find_mesh_location_id find_mesh_location_field,
	enum cmzn_field_find_mesh_location_search_mode search_mode);

/**
 * Creates a field which represents and returns node values/derivatives.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param field  The field for which the nodal values are stored, this
 * 	must be a finite element field.
 * @param node_value_label  The label of the node value/derivative to return.
 * @param version_number  The version number of the value or derivative to
 * return, starting from 1.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_node_value(
	cmzn_fieldmodule_id fieldmodule, cmzn_field_id field,
	enum cmzn_node_value_label node_value_label, int version_number);

/**
 * Creates a field which stores and returns mesh location values at nodes. Its
 * values consists of an element and coordinates in the element's local 'xi'
 * coordinate chart.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param mesh  The mesh for which locations are stored.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_stored_mesh_location(
	cmzn_fieldmodule_id fieldmodule, cmzn_mesh_id mesh);

/**
 * If the field is stored_mesh_location type, return type-specific handle to it.
 *
 * @param field  The field to be cast.
 * @return  Handle to derived stored mesh location field, or NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_stored_mesh_location_id cmzn_field_cast_stored_mesh_location(cmzn_field_id field);

/**
 * Cast stored_mesh_location field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param stored_mesh_location_field  Handle to the field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_stored_mesh_location_base_cast(
	cmzn_field_stored_mesh_location_id stored_mesh_location_field)
{
	return (cmzn_field_id)(stored_mesh_location_field);
}

/**
 * Destroys handle to the stored_mesh_location field (and sets it to
 * NULL). Internally this decrements the reference count.
 *
 * @param stored_mesh_location_field_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_stored_mesh_location_destroy(
	cmzn_field_stored_mesh_location_id *stored_mesh_location_field_address);

/**
 * Creates a field which stores and returns string values at nodes.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_stored_string(
	cmzn_fieldmodule_id fieldmodule);

/**
 * If the field is stored_string type, return type-specific handle to it.
 *
 * @param field  The field to be cast.
 * @return  Handle to derived stored string field, or NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_field_stored_string_id cmzn_field_cast_stored_string(cmzn_field_id field);

/**
 * Cast stored_string field back to its base field and return the field.
 * IMPORTANT NOTE: Returned field does not have incremented reference count and
 * must not be destroyed. Use cmzn_field_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the derived field.
 * Use this function to call base-class API, e.g.:
 * cmzn_field_set_name(cmzn_field_derived_base_cast(derived_field), "bob");
 *
 * @param stored_string_field  Handle to the field to cast.
 * @return  Non-accessed handle to the base field or NULL if failed.
 */
ZINC_C_INLINE cmzn_field_id cmzn_field_stored_string_base_cast(
	cmzn_field_stored_string_id stored_string_field)
{
	return (cmzn_field_id)(stored_string_field);
}

/**
 * Destroys handle to the stored_string field (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param stored_string_field_address  Address of handle to the field to
 * 		destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_field_stored_string_destroy(
	cmzn_field_stored_string_id *stored_string_field_address);

/**
 * Creates a field which returns 1 on 2-D faces and 1-D lines considered as
 * exterior to their top-level element, and 0 elsewhere.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_is_exterior(
	cmzn_fieldmodule_id fieldmodule);

/**
 * Creates a field which returns 1 on 2-D faces and 1-D lines considered
 * to lie on a specified face of their top-level element, and 0 elsewhere.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param face  The enumerated face type, defined with respect to the
 * top-level element.
 * @return  Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_is_on_face(
	cmzn_fieldmodule_id fieldmodule, cmzn_element_face_type face);

/**
 * Creates a field whose value equals source field calculated at the lookup node
 * instead of the domain location requested.
 *
 * @param fieldmodule  Region field module which will own new field.
 * @param source_field  Field to evaluate.
 * @param lookup_node  The node to evaluate the source field at.
 * @return Handle to new field, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_field_id cmzn_fieldmodule_create_field_node_lookup(
	cmzn_fieldmodule_id fieldmodule, cmzn_field_id source_field,
	cmzn_node_id lookup_node);

#ifdef __cplusplus
}
#endif

#endif
