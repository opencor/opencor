/**
 * @file element.h
 *
 * The public interface to Zinc finite elements and meshes.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_ELEMENT_H__
#define CMZN_ELEMENT_H__

#include "types/differentialoperatorid.h"
#include "types/elementid.h"
#include "types/fieldid.h"
#include "types/fieldmoduleid.h"
#include "types/nodeid.h"

#include "opencmiss/zinc/zincsharedobject.h"

/*
Global types
------------
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
Global functions
----------------
*/

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_element_shape_type cmzn_element_shape_type_enum_from_string(
	const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param type  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_element_shape_type_enum_to_string(enum cmzn_element_shape_type type);

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_elementbasis_function_type cmzn_elementbasis_function_type_enum_from_string(
	const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param type  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_elementbasis_function_type_enum_to_string(
	enum cmzn_elementbasis_function_type type);

/**
 * Creates an element_basis object for describing element basis functions.
 *
 * @param fieldmodule  Handle to a field module. Note the returned basis can be
 * used to define fields in any field module of the region tree.
 * @param dimension  The dimension of element chart the basis is for.
 * @param function_type  The basis function type to use in each dimension
 * i.e. basis function is initially homogeneous.
 * @return  Handle to new element basis, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_elementbasis_id cmzn_fieldmodule_create_elementbasis(
	cmzn_fieldmodule_id fieldmodule, int dimension,
	enum cmzn_elementbasis_function_type function_type);

/**
 * Get a handle to the default mesh of a given dimension. Cmgui is currently
 * limited to 1 mesh of each dimension from 1 to 3. These meshes have default
 * names of "mesh_Nd", where "N" is the dimension.
 *
 * @param fieldmodule  The field module the mesh belongs to.
 * @param dimension  The dimension of the mesh from 1 to 3.
 * @return  Handle to the mesh, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_mesh_id cmzn_fieldmodule_find_mesh_by_dimension(
	cmzn_fieldmodule_id fieldmodule, int dimension);

/**
 * Get a handle to a finite element mesh from its name. A mesh is the container
 * of elements of a fixed dimension. Valid names may be any element_group field,
 * or any of the following special names:
 * "mesh3d" = 3-D elements.
 * "mesh2d" = 2-D elements including faces of 3-D elements.
 * "mesh1d" = 1-D elements including faces (lines) of 2-D elements.
 * Note that the default names for element group fields created from a group
 * is GROUP_NAME.MESH_NAME, with mesh names as above.
 *
 * @param fieldmodule  The field module the mesh belongs to.
 * @param name  The name of the finite element mesh.
 * @return  Handle to the mesh, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_mesh_id cmzn_fieldmodule_find_mesh_by_name(
	cmzn_fieldmodule_id fieldmodule, const char *mesh_name);

/**
 * Returns a new handle to the mesh with reference count incremented.
 *
 * @param mesh  The mesh to obtain a new handle to.
 * @return  New handle to the mesh, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_mesh_id cmzn_mesh_access(cmzn_mesh_id mesh);

/**
 * Destroys this handle to the finite element mesh and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param mesh_address  Address of handle to the mesh to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_mesh_destroy(cmzn_mesh_id *mesh_address);

/**
 * Returns whether the element is from the mesh.
 *
 * @param mesh  The mesh to query.
 * @param element  The element to query about.
 * @return  Boolean true if element is in the mesh, otherwise false.
 */
ZINC_API bool cmzn_mesh_contains_element(cmzn_mesh_id mesh,
	cmzn_element_id element);

/**
 * Create a blank template from which new elements can be created in this mesh.
 * Also used for defining new fields over elements.
 *
 * @param mesh  Handle to the mesh the template works with.
 * @return  Handle to new element template, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_elementtemplate_id cmzn_mesh_create_elementtemplate(
	cmzn_mesh_id mesh);

/**
 * Create a new element in this mesh with shape and fields described by the
 * element_template. Returns handle to new element.
 * @see cmzn_mesh_define_element
 *
 * @param mesh  Handle to the mesh to create the new element in.
 * @param identifier  Non-negative integer identifier of new element, or -1 to
 * automatically generate, starting from 1. Fails if supplied identifier already
 * used by an existing element.
 * @param element_template  Template for element shape and fields.
 * @return  Handle to new element, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_element_id cmzn_mesh_create_element(cmzn_mesh_id mesh,
	int identifier, cmzn_elementtemplate_id element_template);

/**
 * Create an element iterator object for iterating through the elements in the
 * mesh which are ordered from lowest to highest identifier. The iterator
 * initially points at the position before the first element, so the first call
 * to the element iterator next method returns the first element and advances the
 * iterator.
 * Iterator becomes invalid if mesh is modified or any of its elements are
 * given new identifiers.
 * @see cmzn_elementiterator_next
 *
 * @param mesh  Handle to the mesh whose elements are to be iterated over.
 * @return  Handle to new element iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_elementiterator_id cmzn_mesh_create_elementiterator(
	cmzn_mesh_id mesh);

/**
 * Create a new element in this mesh with shape and fields described by the
 * element_template.
 * @see cmzn_mesh_create_element
 *
 * @param mesh  Handle to the mesh to create the new element in.
 * @param identifier  Non-negative integer identifier of new element, or -1 to
 * automatically generate, starting from 1. Fails if supplied identifier already
 * used by an existing element.
 * @param element_template  Template for element shape and fields.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_mesh_define_element(cmzn_mesh_id mesh, int identifier,
	cmzn_elementtemplate_id element_template);

/**
 * Destroy all elements in mesh, also removing them from any related groups.
 * All handles to the destroyed element become invalid.
 *
 * @param mesh  Handle to mesh to destroy elements from.
 * @return  Status CMZN_OK if all elements destroyed, any other value if failed.
 */
ZINC_API int cmzn_mesh_destroy_all_elements(cmzn_mesh_id mesh);

/**
 * Destroy the element if it is in the mesh. Removes element from any related
 * groups it is in. All handles to the destroyed element become invalid.
 *
 * @param mesh  Handle to the mesh whose element is to be destroyed.
 * @param element  The element to destroy.
 * @return  Status CMZN_OK if element is successfully destroyed, any other
 * value if failed.
 */
ZINC_API int cmzn_mesh_destroy_element(cmzn_mesh_id mesh, cmzn_element_id element);

/**
 * Destroy all elements in the mesh for which the conditional field is true i.e.
 * non-zero valued in element. These elements are removed from any related
 * groups they are in. All handles to removed elements become invalid.
 * Results are undefined if conditional field is not constant over element.
 * Note that group and element_group fields are valid conditional fields.
 *
 * @param mesh  Handle to the mesh to destroy elements from.
 * @param conditional_field  Field which if non-zero in the element indicates it
 * is to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_mesh_destroy_elements_conditional(cmzn_mesh_id mesh,
	cmzn_field_id conditional_field);

/**
 * Return a handle to the element in the mesh with this identifier.
 *
 * @param mesh  Handle to the mesh to find the element in.
 * @param identifier  Non-negative integer identifier of element.
 * @return  Handle to element, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_element_id cmzn_mesh_find_element_by_identifier(cmzn_mesh_id mesh,
	int identifier);

/**
 * Returns the differential operator giving a field derivative of the given
 * order with respect to the mesh's chart. The term identifies which of the
 * possible differential operator terms are available for the order and
 * dimension of the mesh.
 *
 * @param mesh  Handle to the mesh to get differential operator from.
 * @param order  The order of the derivative. Currently must be 1.
 * @param term  Which of the (dimensions)^order differential operators is
 * required, starting at 1. For order 1, corresponds to a chart axis.
 * @return  Handle to differential operator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_differentialoperator_id cmzn_mesh_get_chart_differentialoperator(
	cmzn_mesh_id mesh, int order, int term);

/**
 * Returns the number of dimensions of the mesh.
 *
 * @param mesh  Handle to the mesh to query.
 * @return  dimension of mesh.
 */
ZINC_API int cmzn_mesh_get_dimension(cmzn_mesh_id mesh);

/**
 * Returns handle to field module for region this mesh belongs to.
 *
 * @param mesh  The mesh from which to obtain the field module.
 * @return  Handle to field module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fieldmodule_id cmzn_mesh_get_fieldmodule(cmzn_mesh_id mesh);

/**
 * Get the master mesh which owns the elements for this mesh. Can be the
 * same as the supplied mesh if it is a master.
 *
 * @param mesh  The mesh to query.
 * @return  Handle to the master mesh, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_mesh_id cmzn_mesh_get_master_mesh(cmzn_mesh_id mesh);

/**
 * Return the name of the mesh.
 *
 * @see cmzn_deallocate
 * @param mesh  The mesh whose name is requested.
 * @return  On success: allocated string containing mesh name. Up to caller to
 * free using cmzn_deallocate().
 */
ZINC_API char *cmzn_mesh_get_name(cmzn_mesh_id mesh);

/**
 * Return the number of elements in the mesh.
 *
 * @param mesh  Handle to the mesh to query.
 * @return  Number of elements in mesh.
 */
ZINC_API int cmzn_mesh_get_size(cmzn_mesh_id mesh);

/**
 * Check if two mesh handles refer to the same object.
 *
 * @param mesh1  The first mesh to match.
 * @param mesh2  The second mesh to match.
 * @return  Boolean true if the two meshes match, false if not.
 */
ZINC_API bool cmzn_mesh_match(cmzn_mesh_id mesh1, cmzn_mesh_id mesh2);

/**
 * If the mesh is a mesh group i.e. subset of elements from a master mesh,
 * get the mesh group specific interface for add/remove functions.
 *
 * @param field  The mesh to be cast.
 * @return  Handle to derived mesh group, or NULL/invalid handle if wrong type or failed.
 */
ZINC_API cmzn_mesh_group_id cmzn_mesh_cast_group(cmzn_mesh_id mesh);

/**
 * Destroys this handle to the mesh group and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param mesh_group_address  Address of mesh group handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_mesh_group_destroy(cmzn_mesh_group_id *mesh_group_address);

/**
 * Cast mesh group back to its base mesh class.
 * IMPORTANT NOTE: Returned mesh does not have incremented reference count and
 * must not be destroyed. Use cmzn_mesh_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the mesh_group.
 * Use this function to call base-class API, e.g.:
 * int size = cmzn_mesh_get_size(cmzn_mesh_group_base_cast(mesh_group);
 *
 * @param mesh_group  Handle to the mesh group to cast.
 * @return  Non-accessed handle to the mesh or NULL if failed.
 */
ZINC_C_INLINE cmzn_mesh_id cmzn_mesh_group_base_cast(
	cmzn_mesh_group_id mesh_group)
{
	return (cmzn_mesh_id)(mesh_group);
}

/**
 * Add specified element to mesh group.
 *
 * @param mesh_group  Handle to mesh group to modify.
 * @param element  Handle to element to add. Must be from the group's master mesh.
 * @return  Status CMZN_OK on success, CMZN_ERROR_ALREADY_EXISTS if element
 * was already in the group but otherwise successful, or any other value on
 * more serious failure.
 */
ZINC_API int cmzn_mesh_group_add_element(cmzn_mesh_group_id mesh_group,
	cmzn_element_id element);

/**
 * Ensure this mesh group contains all elements from the master mesh for which
 * the conditional field is true i.e. non-zero valued in the element.
 * Results are undefined if conditional field is not constant over element.
 * Note that group and element_group fields are valid conditional fields.
 *
 * @param mesh_group  Handle to the mesh group to add elements to.
 * @param conditional_field  Field which if non-zero in an element indicates it
 * is to be added.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_mesh_group_add_elements_conditional(cmzn_mesh_group_id mesh_group,
	cmzn_field_id conditional_field);

/**
 * Remove all elements from mesh group.
 *
 * @param mesh_group  Handle to mesh group to modify.
 * @return  Status CMZN_OK if all elements removed, any other value if failed.
 */
ZINC_API int cmzn_mesh_group_remove_all_elements(cmzn_mesh_group_id mesh_group);

/**
 * Remove specified element from mesh group.
 *
 * @param mesh_group  Handle to mesh group to modify.
 * @param element  Handle to element to remove.
 * @return  Status CMZN_OK on success, CMZN_ERROR_NOT_FOUND if element was not
 * in the group but otherwise successful, or any other value on more serious
 * failure.
 */
ZINC_API int cmzn_mesh_group_remove_element(cmzn_mesh_group_id mesh_group,
	cmzn_element_id element);

/**
 * Remove all elements from the mesh group for which the conditional field is
 * true i.e. non-zero valued in the element.
 * Results are undefined if conditional field is not constant over element.
 * Note that group and element_group fields are valid conditional fields.
 *
 * @param mesh_group  Handle to the mesh group to remove elements from.
 * @param conditional_field  Field which if non-zero in the element indicates it
 * is to be removed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_mesh_group_remove_elements_conditional(cmzn_mesh_group_id mesh_group,
	cmzn_field_id conditional_field);

/**
 * Returns a new handle to the element basis with reference count incremented.
 *
 * @param mesh  The element basis to obtain a new handle to.
 * @return  New handle to element basis, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_elementbasis_id cmzn_elementbasis_access(
	cmzn_elementbasis_id element_basis);

/**
 * Destroys this handle to the element_basis and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param element_basis_address  Address of handle to element_basis to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementbasis_destroy(cmzn_elementbasis_id *element_basis_address);

/**
 * Gets the number of dimensions of the elements this basis works with.
 *
 * @param element_basis  Element basis to query.
 * @return  The number of dimensions.
 */
ZINC_API int cmzn_elementbasis_get_dimension(cmzn_elementbasis_id element_basis);

/**
 * Gets the basis function type for a component of the basis.
 *
 * @param element_basis  Element basis to query.
 * @param chart_component  The chart component to get the function for from 1 to
 * dimension.
 * @return  The basis function type.
 */
ZINC_API enum cmzn_elementbasis_function_type cmzn_elementbasis_get_function_type(
	cmzn_elementbasis_id element_basis, int chart_component);

/**
 * Sets a simple basis function type for a component of the basis.
 *
 * @param element_basis  Element basis to modify.
 * @param chart_component  The chart component to set the function for from 1 to
 * dimension.
 * @param basis_type  The basis type to use on the chosen chart component.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementbasis_set_function_type(cmzn_elementbasis_id element_basis,
	int chart_component, enum cmzn_elementbasis_function_type function_type);

/**
 * If the basis is valid, gets the number of nodes the basis expects.
 *
 * @param element_basis  Element basis to query.
 * @return  number of nodes expected, or 0 if basis is incomplete or invalid.
 */
ZINC_API int cmzn_elementbasis_get_number_of_nodes(
	cmzn_elementbasis_id element_basis);

/**
 * If the basis is valid, gets the number of functions it has, which equals the
 * number of parameters required.
 * Note that with Hermite bases the parameters are grouped by nodes, i.e. all
 * parameters for the first node are first, followed by those for the second
 * node, and so on. Within the parameters for each node, parameters are ordered
 * by the value/derivative type: the value is first, then DS1 derivatives cycle
 * fastest followed by DS2 then DS3.
 *
 * @param element_basis  Element basis to query.
 * @return  Number of parameters expected, or 0 if basis is incomplete or
 * invalid.
 */
ZINC_API int cmzn_elementbasis_get_number_of_functions(
	cmzn_elementbasis_id element_basis);

/**
 * Gets the number of functions / parameters for a given basis node.
 * @see cmzn_elementbasis_get_number_of_nodes
 *
 * @param element_basis  Element basis to query.
 * @param basis_node_index  The basis node index starting at 1, up to number
 * used by basis.
 * @return  The number of functions/parameters, or 0 if invalid arguments.
 */
ZINC_API int cmzn_elementbasis_get_number_of_functions_per_node(
	cmzn_elementbasis_id element_basis, int basis_node_index);

/**
 * Returns a new handle to the element iterator with reference count incremented.
 *
 * @param mesh  The element iterator to obtain a new handle to.
 * @return  New handle to element iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_elementiterator_id cmzn_elementiterator_access(
	cmzn_elementiterator_id element_iterator);

/**
 * Destroys this handle to the element_iterator and sets it to NULL.
 *
 * @param element_iterator_address  Address of handle to element_iterator to
 * destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementiterator_destroy(
	cmzn_elementiterator_id *element_iterator_address);

/**
 * Returns a handle to the next element in the container being iterated over
 * then advances the iterator position. The caller is required to destroy the
 * returned element handle.
 *
 * @param element_iterator  Element iterator to query and advance.
 * @return  Handle to next element, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_element_id cmzn_elementiterator_next(
	cmzn_elementiterator_id element_iterator);

/**
 * Returns a new handle to the element template with reference count incremented.
 *
 * @param mesh  The element template to obtain a new handle to.
 * @return  New handle to element template, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_elementtemplate_id cmzn_elementtemplate_access(
	cmzn_elementtemplate_id element_template);

/**
 * Destroys this handle to the element_template and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param element_template_address  Address of handle to element_template
 * to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_destroy(
	cmzn_elementtemplate_id *element_template_address);

/**
 * Gets the current element shape type set in the element_template.
 *
 * @param element_template  Element template to query.
 * @return  The element shape type enumeration set in the element template.
 */
ZINC_API enum cmzn_element_shape_type cmzn_elementtemplate_get_element_shape_type(
	cmzn_elementtemplate_id element_template);

/**
 * Sets the element shape to a standard element shape type. The shape must have
 * the same dimension as the mesh from which the element template was created.
 * Special value CMZN_ELEMENT_SHAPE_TYPE_INVALID indicates an unspecified shape
 * of known; when this is set in the template it does not override the shape
 * of any elements it is merged into. Beware that face mappings are lost if
 * shape changes are merged into global elements.
 * Shape must be set before the template can set local nodes, create new elements
 * and merge into existing elements.
 *
 * @param element_template  Element template to modify.
 * @param shape_type  Enumeration of standard element shapes.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_set_element_shape_type(
	cmzn_elementtemplate_id element_template, enum cmzn_element_shape_type shape_type);

/**
 * Gets the number of local nodes this element_template can address.
 *
 * @param element_template  Element template to query.
 * @return  The number of local nodes, or 0 on error.
 */
ZINC_API int cmzn_elementtemplate_get_number_of_nodes(
	cmzn_elementtemplate_id element_template);

/**
 * Sets the number of local nodes this element_template can address. This must
 * be done before defining fields that index them.
 * This number cannot be reduced.
 *
 * @param element_template  Element template to modify.
 * @param number_of_nodes  The number of nodes.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_elementtemplate_set_number_of_nodes(
	cmzn_elementtemplate_id element_template, int number_of_nodes);

/**
 * Defines per element constant field or field component in the
 * element_template.
 *
 * @param element_template  Element template to modify.
 * @param field  The field to define. Must be finite_element type.
 * @param component_number  The component to define from 1 to the number of
 * field components, or -1 to define all components.
 *
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_define_field_element_constant(
	cmzn_elementtemplate_id element_template,
	cmzn_field_id field, int component_number);

/**
 * Defines a nodally interpolated element field or field component in the
 * element template. For Lagrange, simplex and constant basis function types
 * there are as many nodes as basis functions and the single parameter per
 * node has VALUE label. For Hermite bases with multiple functions per node,
 * default parameters are expected in numerical order of the node value label
 * e.g. for bicubic Hermite the first four functions are for the first node and
 * expected in order: VALUE D_DS1 D_DS2 D2_DS1DS2.
 * By default, versions are initialised to 1, and no scaling is used.
 * In all cases local nodes cycle fastest in lowest element xi direction.
 * Shape must be set before calling this function.
 *
 * @param elementtemplate  Element template to modify.
 * @param field  The field to define. Must be finite_element type.
 * @param component_number  The component to define from 1 to the number of
 * field components, or -1 to define all components with identical basis and
 * nodal mappings.
 * @param basis  The element basis to use for all field components.
 * @param basis_number_of_nodes  The number of nodes indexed by the basis,
 * equals the size of the local_node_indexes array.
 * @param local_node_indexes  Array containing the local node indexes of the
 * nodes from which element field parameters are mapped, which range from 1 to
 * the number of nodes set for the element_template. Local nodes are ordered
 * by lowest xi coordinate varying fastest, e.g. for biquadratic Lagrange:
 * xi = (0,0), (0.5,0), (1,0), (0,0.5), (0.5,0.5) ...
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_define_field_simple_nodal(
	cmzn_elementtemplate_id elementtemplate,
	cmzn_field_id field,  int component_number,
	cmzn_elementbasis_id basis, int basis_number_of_nodes,
	const int *local_node_indexes);

/**
 * For a field defined by node interpolation, sets the node value label (value
 * or derivative) mapped to one function parameter in the element basis.
 * Note that any values set by this function are reset if the field is
 * re-defined at the node.
 * @see cmzn_elementtemplate_define_field_simple_nodal
 *
 * @param elementtemplate  Element template to modify.
 * @param field  The field to modify mapping for.
 * @param component_number  The component to modify mapping for, from 1 to the
 * number of field components, or -1 to set all components identically.
 * @param basis_node_index  The basis node index starting at 1, up to number
 * used by basis.
 * @see cmzn_elementbasis_get_number_of_nodes
 * @param node_function_index  The function index starting at 1, up to the
 * number per node in the element basis.
 * @see cmzn_elementbasis_get_number_of_functions_per_node
 * @param node_value_label  The node value label to set.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_set_map_node_value_label(
	cmzn_elementtemplate_id elementtemplate, cmzn_field_id field,
	int component_number, int basis_node_index, int node_function_index,
	enum cmzn_node_value_label node_value_label);

/**
 * For a field defined by node interpolation, sets the version number of the
 * value mapped to one function parameter in the element basis.
 * Note that any values set by this function are reset if the field is
 * re-defined at the node.
 * @see cmzn_elementtemplate_define_field_simple_nodal
 *
 * @param elementtemplate  Element template to modify.
 * @param field  The field to modify mapping for.
 * @param component_number  The component to modify mapping for, from 1 to the
 * number of field components, or -1 to set all components identically.
 * @param basis_node_index  The basis node index starting at 1, up to number
 * used by basis.
 * @see cmzn_elementbasis_get_number_of_nodes
 * @param node_function_index  The function index starting at 1, up to the
 * number per node in the element basis.
 * @see cmzn_elementbasis_get_number_of_functions_per_node
 * @param node_version  The version number to set, starting at 1.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_set_map_node_version(
	cmzn_elementtemplate_id elementtemplate, cmzn_field_id field,
	int component_number, int basis_node_index, int node_function_index,
	int version_number);

/**
 * Gets the global node at a given local node index in the element_template.
 * May only be called after the definition of element fields are complete and
 * valid.
 *
 * @param element_template  Element template to query.
 * @param local_node_index  The index from 1 to number of nodes in template.
 * @return  Handle to global node, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_node_id cmzn_elementtemplate_get_node(
	cmzn_elementtemplate_id element_template, int local_node_index);

/**
 * Sets the global node at a given local node index in the element_template.
 * May only be called after the definition of element fields are complete and
 * valid. Nodes are reset if new element fields are defined on the template.
 *
 * @param element_template  Element template to modify.
 * @param local_node_index  The index from 1 to number of nodes in template.
 * @param node  The global node to set at that index.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_elementtemplate_set_node(cmzn_elementtemplate_id element_template,
	int local_node_index, cmzn_node_id node);

/**
 * Returns a new handle to the element with reference count incremented.
 *
 * @param element  Handle to an element.
 * @return  New handle to the element, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_element_id cmzn_element_access(cmzn_element_id element);

/**
 * Destroys this handle to the element and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param element_address  Address of handle to the element to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_element_destroy(cmzn_element_id *element_address);

/**
 * Returns the number of dimensions of the element's chart.
 *
 * @param element  The element to query.
 * @return  The dimension.
 */
ZINC_API int cmzn_element_get_dimension(cmzn_element_id element);

/**
 * Returns the non-negative integer uniquely identifying the element in its
 * mesh.
 *
 * @param element  The element to query.
 * @return  The non-negative integer identifier of the element, or a negative
 * value if element is invalid.
 */
ZINC_API int cmzn_element_get_identifier(cmzn_element_id element);

/**
 * Set an integer uniquely identifying the element in its mesh.
 *
 * @param element  The element to query.
 * @param identifier  unique identifier to be set for the element
 *
 * @return  CMZN_OK if set the identifier successfully,
 * 	any other value on fail.
 */
ZINC_API int cmzn_element_set_identifier(cmzn_element_id element, int identifier);

/**
 * Get the mesh which owns this element.
 *
 * @param element  The element to query.
 * @return  Handle to the owning mesh, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_mesh_id cmzn_element_get_mesh(cmzn_element_id element);

/**
 * Gets the shape type of the element. Note that legacy meshes may return an
 * unknown shape type for certain custom element shapes e.g. polygon shapes.
 * It is intended that future revisions of the API will offer more detailed
 * shape query and modification functions.
 *
 * @param element  Element to query.
 * @return  The element's shape type.
 */
ZINC_API enum cmzn_element_shape_type cmzn_element_get_shape_type(
	cmzn_element_id element);

/**
 * Modifies the element to use the fields as defined in the element_template.
 * Note that mappings may be optimised or modified in the merge process, often
 * to minimise the number of local nodes in the merged element.
 *
 * @param element  The element to modify.
 * @param element_template  Template containing element field definitions.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_element_merge(cmzn_element_id element,
	cmzn_elementtemplate_id element_template);

/**
 * Returns a new handle to the mesh changes with reference count incremented.
 *
 * @param meshchanges  The mesh changes to obtain a new handle to.
 * @return  New handle to mesh changes, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_meshchanges_id cmzn_meshchanges_access(
	cmzn_meshchanges_id meshchanges);

/**
 * Destroys this handle to the meshchanges and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param meshchanges_address  Address of handle to the mesh changes to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_meshchanges_destroy(cmzn_meshchanges_id *meshchanges_address);

/**
 * Returns if and how the element has changed in the mesh changes. Note that
 * individual changes are not stored for each element, hence this function
 * returns the summary of all element changes if the element has changed, and
 * CHANGE_FLAG_NONE if not changed.
 *
 * @param meshchanges  The mesh changes to query.
 * @param element  The element to query about.
 * @return  Logical OR of change bit flags. See #cmzn_element_change_flag.
 */
ZINC_API cmzn_element_change_flags cmzn_meshchanges_get_element_change_flags(
	cmzn_meshchanges_id meshchanges, cmzn_element_id element);

/**
 * Returns the number of elements recorded as changed in the mesh changes, or
 * -1 if all considered changed, i.e. too many changes occurred to record.
 *
 * @param meshchanges  The mesh changes to query.
 * @return  The number of changes to elements, or -1 if all changed.
 */
ZINC_API int cmzn_meshchanges_get_number_of_changes(
	cmzn_meshchanges_id meshchanges);

/**
 * Returns logical OR of change flags for all elements in the mesh changes.
 *
 * @param meshchanges  The mesh changes to query.
 * @return  Logical OR of change bit flags. See #cmzn_element_change_flag.
 */
ZINC_API cmzn_element_change_flags cmzn_meshchanges_get_summary_element_change_flags(
	cmzn_meshchanges_id meshchanges);

#ifdef __cplusplus
}
#endif

#endif /* CMZN_ELEMENT_H__ */
