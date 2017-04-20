/**
 * @file fieldid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDID_H__
#define CMZN_FIELDID_H__

/**
 * @brief Base field type: an abstraction of a mathematical field.
 *
 * Base field type: an abstraction of a mathematical field, a function giving
 * values at locations in a domain (mesh, nodeset, over time).
 * Instances of derived field types give the actual expression in terms of
 * other fields or locations in the domain, all from the same region.
 */
struct cmzn_field;
typedef struct cmzn_field *cmzn_field_id;

/**
 * @brief An iterator for looping through all the fields in a fieldmodule.
 *
 * An iterator for looping through all the fields in a fieldmodule.
 */
struct cmzn_fielditerator;
typedef struct cmzn_fielditerator * cmzn_fielditerator_id;

/**
 * Bit flags summarising changes to a field or fields in a fieldmodule.
 */
enum cmzn_field_change_flag
{
	CMZN_FIELD_CHANGE_FLAG_NONE = 0,
		/*!< field(s) not changed */
	CMZN_FIELD_CHANGE_FLAG_ADD = 1,
		/*!< one or more fields added */
	CMZN_FIELD_CHANGE_FLAG_REMOVE = 2,
		/*!< one or more fields removed */
	CMZN_FIELD_CHANGE_FLAG_IDENTIFIER = 4,
		/*!< field identifier changed */
	CMZN_FIELD_CHANGE_FLAG_DEFINITION = 8,
		/*!< change to field attributes other than identifier.
		 * If change affects result, CMZN_FIELD_CHANGE_FLAG_FULL_RESULT
		 * will also be set; metadata changes do not flag result as changed. */
	CMZN_FIELD_CHANGE_FLAG_FULL_RESULT = 16,
		/*!< all resultant values of field changed, by its definition changing
		 * or by change to a field or other object it depends on. */
	CMZN_FIELD_CHANGE_FLAG_PARTIAL_RESULT = 32,
		/*!< change to field values on subset of domain: nodes, elements etc.
		 * If this flag is set but not CHANGE_FLAG_FULL_RESULT then nodeset and mesh
		 * changes describe where on the domain its values have changed. */
	CMZN_FIELD_CHANGE_FLAG_RESULT =
		CMZN_FIELD_CHANGE_FLAG_FULL_RESULT | CMZN_FIELD_CHANGE_FLAG_PARTIAL_RESULT,
		/*!< convenient value representing any change affecting result */
	CMZN_FIELD_CHANGE_FLAG_FINAL = 32768
		/*!< final notification: owning field module i.e. region has been destroyed */
};

/**
 * Type for passing logical OR of #cmzn_field_change_flag
 */
typedef int cmzn_field_change_flags;

/**
 * Field attribute describing the type of space that its values are to be
 * interpreted in. Although it is usually set for all fields (default is
 * rectangular cartesian, RC), the attribute is only relevant when field is
 * used to supply coordinates or vector values, e.g. to graphics, where it
 * prompts automatic conversion to the underlying RC coordinate system.
 */
enum cmzn_field_coordinate_system_type
{
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_INVALID = 0,
	/*!< Unspecified coordinate system type */
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_RECTANGULAR_CARTESIAN = 1,
	/*!< Default rectangular Cartesian coordinate system */
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_CYLINDRICAL_POLAR = 2,
	/*!< Cylindrical polar coordinate system with components r, theta, z.
	     Converts to rectangular Cartesian via:
	       x = r*cos(theta)
	       y = r*sin(theta)
	       z = z */
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_SPHERICAL_POLAR = 3,
	/*!< Spherical polar coordinate system with components r, theta, phi.
	     Converts to rectangular Cartesian via:
	       x = r*cos(phi)*cos(theta)
	       y = r*cos(phi)*sin(theta)
	       z = r*sin(phi) */
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_PROLATE_SPHEROIDAL = 4,
	/*!< Prolate spheroidal coordinate system with components lambda, mu, theta,
	     with a focus value giving scale. Converts to rectangular Cartesian via:
	       x = focus*cosh(lambda)*cos(mu)
	       y = focus*sinh(lambda)*sin(mu)*cos(theta)
	       z = focus*sinh(lambda)*sin(mu)*sin(theta)
	     @see cmzn_field_set_coordinate_system_focus */
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_OBLATE_SPHEROIDAL = 5,
	/*!< Oblate spheroidal coordinate system with components lambda, mu, theta,
	     with a focus value giving scale. Converts to rectangular Cartesian via:
	       x = focus*cosh(lambda)*cos(mu)*sin(theta)
	       y = focus*sinh(lambda)*sin(mu)
	       z = focus*cosh(lambda)*cos(mu)*cos(theta)
	     @see cmzn_field_set_coordinate_system_focus */
	CMZN_FIELD_COORDINATE_SYSTEM_TYPE_FIBRE = 6,
	/*!< A fibre/material orthonormal reference coordinate system defined within
	  elements by Euler angle rotations of a coordinate system initially oriented
	  with the derivatives of RC coordinates w.r.t. element 'xi' coordinates:
	  initial fibre vector = normalised dx/dxi1.
	  initial sheet vector = unit vector normal to fibre in xi1-xi2 plane,
	    calculated as: normal (x) fibre.
	  initial normal vector = unit vector normal to xi1-xi2 plane,
	    calculated as: dx/dxi1 (x) dx/dxi2, then normalised.
	  Up to three fibre angles (components of fibre field) are applied as
	  rotations in radians in the following sequence (later steps omitted for fewer
	  components):
	  1. fibre angle = anticlockwise (right-hand) rotation of initial fibre & sheet
	     vectors about initial normal vector.
	  2. imbrication angle = anticlockwise rotation of initial normal and rotated
	     fibre vectors about the rotated sheet vector.
	  3. sheet angle = anticlockwise rotation of the rotated sheet and rotated
	     normal vectors about the rotated fibre vector. */
};

/**
 * An enumeration specifying the field domain (without domain objects)
 * Also used as bit flags.
 */
enum cmzn_field_domain_type
{
	CMZN_FIELD_DOMAIN_TYPE_INVALID = 0,
		/*!< Unspecified domain type */
	CMZN_FIELD_DOMAIN_TYPE_POINT = 1,
		/*!< A single point for the region */
	CMZN_FIELD_DOMAIN_TYPE_NODES = 2,
		/*!< The set of node points */
	CMZN_FIELD_DOMAIN_TYPE_DATAPOINTS = 4,
		/*!< The set of data points */
	CMZN_FIELD_DOMAIN_TYPE_MESH1D = 8,
		/*!< The set of 1-D elements and edge lines of 2-D or 3-D elements */
	CMZN_FIELD_DOMAIN_TYPE_MESH2D = 16,
		/*!< The set of 2-D elements and faces of 3-D elements */
	CMZN_FIELD_DOMAIN_TYPE_MESH3D = 32,
		/*!< The set of 3-D elements */
	CMZN_FIELD_DOMAIN_TYPE_MESH_HIGHEST_DIMENSION = 64
		/*!< The set of elements of highest dimension in region */
};

/**
 * Type for passing logical OR of #cmzn_field_domain_type
 */
typedef int cmzn_field_domain_types;

/**
 * The types of values fields may produce.
 * @see cmzn_field_get_value_type
 */
enum cmzn_field_value_type
{
	CMZN_FIELD_VALUE_TYPE_INVALID = 0,
	/*!< Unspecified value type */
	CMZN_FIELD_VALUE_TYPE_REAL = 1,
	/*!< Real value type */
	CMZN_FIELD_VALUE_TYPE_STRING = 2,
	/*!< String value type */
	CMZN_FIELD_VALUE_TYPE_MESH_LOCATION = 3
	/*!< Mesh location value type */
};

#endif
