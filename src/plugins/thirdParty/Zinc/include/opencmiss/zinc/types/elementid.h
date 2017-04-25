/**
 * @file elementid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_ELEMENTID_H__
#define CMZN_ELEMENTID_H__

/**
 * @brief A finite element mesh consisting of a set of elements of fixed dimension.
 *
 * A finite element mesh consisting of a set of elements of fixed dimension.
 * Note that Zinc elements are not iso-parametric, meaning each field must
 * be individually defined on them, specifying the basis and parameter mapping.
 */
struct cmzn_mesh;
typedef struct cmzn_mesh *cmzn_mesh_id;

/**
 * @brief A subset of a master mesh.
 *
 * A specialised mesh consisting of a subset of elements from a master mesh.
 */
struct cmzn_mesh_group;
typedef struct cmzn_mesh_group *cmzn_mesh_group_id;

/**
 * @brief A description of element shape and field definitions.
 *
 * A description of element shape and field definitions (incl. basis, parameter
 * mappings), used as a template for creating new elements in a mesh, or merging
 * into an element to define additional fields on it.
 */
struct cmzn_elementtemplate;
typedef struct cmzn_elementtemplate *cmzn_elementtemplate_id;

/**
 * @brief A single finite element from a mesh.
 *
 * A single finite element from a mesh. Represents a local coordinate chart
 * of prescribed shape/bounds, over which fields can be defined.
 */
struct cmzn_element;
typedef struct cmzn_element *cmzn_element_id;

/**
 * An enumeration for selecting the faces of elements.
 */
enum cmzn_element_face_type
{
	CMZN_ELEMENT_FACE_TYPE_INVALID = 0,  /*!< invalid element face type */
	CMZN_ELEMENT_FACE_TYPE_ALL = 1,      /*!< match all elements whether faces of parent elements or not */
	CMZN_ELEMENT_FACE_TYPE_ANY_FACE = 2, /*!< element on any face of a higher dimension element */
	CMZN_ELEMENT_FACE_TYPE_NO_FACE = 3,  /*!< not a face of a higher dimension element; top-level elements only */
	CMZN_ELEMENT_FACE_TYPE_XI1_0 = 4,    /*!< element on face where top-level xi1 = 0 */
	CMZN_ELEMENT_FACE_TYPE_XI1_1 = 5,    /*!< element on face where top-level xi1 = 1 */
	CMZN_ELEMENT_FACE_TYPE_XI2_0 = 6,    /*!< element on face where top-level xi2 = 0 */
	CMZN_ELEMENT_FACE_TYPE_XI2_1 = 7,    /*!< element on face where top-level xi2 = 1 */
	CMZN_ELEMENT_FACE_TYPE_XI3_0 = 8,    /*!< element on face where top-level xi3 = 0 */
	CMZN_ELEMENT_FACE_TYPE_XI3_1 = 9     /*!< element on face where top-level xi3 = 1 */
};

/**
 * Common element shape enumeration.
 */
enum cmzn_element_shape_type
{
	CMZN_ELEMENT_SHAPE_TYPE_INVALID = 0,/**< unspecified shape of known dimension */
	CMZN_ELEMENT_SHAPE_TYPE_LINE = 1,        /**< 1-D: 0 <= xi1 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_SQUARE = 2,      /**< 2-D: 0 <= xi1,xi2 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_TRIANGLE = 3,    /**< 3-D: 0 <= xi1,xi2; xi1+xi2 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_CUBE = 4,        /**< 3-D: 0 <= xi1,xi2,xi3 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_TETRAHEDRON = 5, /**< 3-D: 0 <= xi1,xi2,xi3; xi1+xi2+xi3 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_WEDGE12 = 6,     /**< 3-D: 0 <= xi1,xi2; xi1+xi2 <= 1; 0 <= xi3 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_WEDGE13 = 7,     /**< 3-D: 0 <= xi1,xi3; xi1+xi3 <= 1; 0 <= xi2 <= 1 */
	CMZN_ELEMENT_SHAPE_TYPE_WEDGE23 = 8      /**< 3-D: 0 <= xi2,xi3; xi2+xi3 <= 1; 0 <= xi1 <= 1 */
};

/**
 * @brief An iterator for looping through all the elements in a mesh.
 *
 * An iterator for looping through all the elements in a mesh.
 */
struct cmzn_elementiterator;
typedef struct cmzn_elementiterator * cmzn_elementiterator_id;

/**
 * @brief A set of basis functions that can apply over an element of a given dimension.
 *
 * A set of basis functions that can apply over an element of a given dimension.
 * The element basis can be a tensor product of different basis function types,
 * with the parameters of the resulting basis cycling over lower element 'xi'
 * coordinates fastest.
 */
struct cmzn_elementbasis;
typedef struct cmzn_elementbasis *cmzn_elementbasis_id;

/**
 * Common 1-D or linked-dimension basis function types.
 *
 * @see cmzn_elementbasis_id
 */
enum cmzn_elementbasis_function_type
{
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_INVALID = 0,
	/*!< Invalid or unspecified basis function type */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_CONSTANT = 1,
	/*!< Constant value, 1 parameter */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_LINEAR_LAGRANGE = 2,
	/*!< Linear Lagrange interpolation over xi in [0,1] with 2 parameters
	     ordered in increasing xi: xi=0, xi=1 */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_QUADRATIC_LAGRANGE = 3,
	/*!< Quadratic Lagrange interpolation over xi in [0,1] with 3 parameters
	     ordered in increasing xi: xi=0, xi=0.5, xi=1 */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_CUBIC_LAGRANGE = 4,
	/*!< Cubic Lagrange interpolation over xi in [0,1] with 4 parameters
	     ordered in increasing xi: xi=0, xi=1/3, xi=2/3, xi=1 */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_LINEAR_SIMPLEX = 5,
	/*!< Linear Lagrange simplex basis linked on 2 or more dimensions over
	     chart xi >= 0, and sum of linked xi coordinates <= 1.
	     2 linked dimensions gives a linear triangle with 3 parameters with
	     lowest xi cycling fastest: xi (0,0) (1,0) (0,1)
	     3 linked dimensions gives a linear tetrahedron with 4 parameters with
	     lowest xi cycling fastest: xi (0,0,0) (1,0,0) (0,1,0) (0,0,1) */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_QUADRATIC_SIMPLEX = 6,
	/*!< Quadratic Lagrange simplex basis linked on 2 or more dimensions over
	     chart xi >= 0, and sum of linked xi coordinates <= 1.
	     2 linked dimensions gives a quadratic triangle with 6 parameters with
	     lowest xi cycling fastest: xi (0,0) (0.5,0) (1,0) (0,0.5) (0.5,0.5) (0,1)
	     3 linked dimensions gives a quadratic tetrahedron with 10 parameters with
	     lowest xi cycling fastest: xi (0,0,0) (0.5,0,0) (1,0,0) (0,0.5,0)
	     (0.5,0.5,0) (0,1,0) (0,0,0.5) (0.5,0,0.5) (0,0.5,0.5) (0,0,1) */
	CMZN_ELEMENTBASIS_FUNCTION_TYPE_CUBIC_HERMITE = 7
	/*!< Cubic Hermite basis over xi in [0,1] with 4 parameters: x at xi=0,
	     dx/dxi at xi=0, x at xi=1. dx/dxi at xi=1. */
};

/**
 * Mode controlling how points are sampled from elements.
 */
enum cmzn_element_point_sampling_mode
{
	CMZN_ELEMENT_POINT_SAMPLING_MODE_INVALID = 0,
	  /*!< Unspecified point sampling mode */
	CMZN_ELEMENT_POINT_SAMPLING_MODE_CELL_CENTRES = 1,
		/*!< Sample points at centres of element or tessellation cells */
	CMZN_ELEMENT_POINT_SAMPLING_MODE_CELL_CORNERS = 2,
		/*!< Sample points at corners of element or tessellation cells */
	CMZN_ELEMENT_POINT_SAMPLING_MODE_CELL_POISSON = 3,
		/*!< Sample points randomly within each tessellation cell according to a
		     Poisson distribution with expected number given by:
		     sample density field * cell volume, area or length, depending on dimension.
		     The sample density field should be evaluated at the cell centre. */
	CMZN_ELEMENT_POINT_SAMPLING_MODE_SET_LOCATION = 4,
		/*!< One point at a specified location in the element chart. */
	CMZN_ELEMENT_POINT_SAMPLING_MODE_GAUSSIAN_QUADRATURE = 5
		/*!< Sample at Gaussian quadrature points for the element. Currently
		     limited to a maximum of 4 points in each element direction.
		     Triangles and tetrahedra have symmetric point arrangements for an
		     equal polynomial degree in each axis. */
};

/**
 * Quadrature rule for numerically integrating over elements.
 */
enum cmzn_element_quadrature_rule
{
	CMZN_ELEMENT_QUADRATURE_RULE_INVALID = 0,
		/*!< Unspecified or invalid quarature rule */
	CMZN_ELEMENT_QUADRATURE_RULE_GAUSSIAN = 1,
		/*!< Gaussian quadrature. Gives optimal/exact integration of polynomials of
		     degree up to 2n - 1, when n is the number of Gauss points chosen.
		     Currently limited to a maximum of 4 points in each element direction.
		     Triangles and tetrahedra have symmetric point arrangements for an
		     equal polynomial degree in each axis. */
	CMZN_ELEMENT_QUADRATURE_RULE_MIDPOINT = 2
		/*!< Sample at mid-points of equal-sized cells in element local xi chart,
		     with equal weights. Also called the rectangle rule. */
};

/**
 * @brief Object describing changes to a mesh in a fieldmoduleevent
 *
 * Object describing changes to a mesh in a fieldmoduleevent
 */
struct cmzn_meshchanges;
typedef struct cmzn_meshchanges *cmzn_meshchanges_id;

/**
 * Bit flags summarising changes to an element or elements in a mesh.
 */
enum cmzn_element_change_flag
{
	CMZN_ELEMENT_CHANGE_FLAG_NONE = 0,
		/*!< element(s) not changed */
	CMZN_ELEMENT_CHANGE_FLAG_ADD = 1,
		/*!< element(s) added */
	CMZN_ELEMENT_CHANGE_FLAG_REMOVE = 2,
		/*!< element(s) removed */
	CMZN_ELEMENT_CHANGE_FLAG_IDENTIFIER = 4,
		/*!< element(s') identifier changed */
	CMZN_ELEMENT_CHANGE_FLAG_DEFINITION = 8,
		/*!< element(s') definition other than identifier changed e.g. shape */
	CMZN_ELEMENT_CHANGE_FLAG_FIELD = 16
		/*!< change to field values mapped to element(s) */
};

/**
 * Type for passing logical OR of #cmzn_element_change_flag
 */
typedef int cmzn_element_change_flags;

#endif
