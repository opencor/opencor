/**
 * @file element.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_ELEMENT_HPP__
#define CMZN_ELEMENT_HPP__

#include "opencmiss/zinc/element.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/differentialoperator.hpp"
#include "opencmiss/zinc/node.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Elementbasis
{
private:

	cmzn_elementbasis_id id;

public:

	Elementbasis() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Elementbasis(cmzn_elementbasis_id element_basis_id) :
		id(element_basis_id)
	{ }

	Elementbasis(const Elementbasis& elementBasis) :
		id(cmzn_elementbasis_access(elementBasis.id))
	{ }

	Elementbasis& operator=(const Elementbasis& elementBasis)
	{
		cmzn_elementbasis_id temp_id = cmzn_elementbasis_access(elementBasis.id);
		if (0 != id)
		{
			cmzn_elementbasis_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Elementbasis()
	{
		if (0 != id)
		{
			cmzn_elementbasis_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	enum FunctionType
	{
		FUNCTION_TYPE_INVALID = CMZN_ELEMENTBASIS_FUNCTION_TYPE_INVALID,
		FUNCTION_TYPE_CONSTANT = CMZN_ELEMENTBASIS_FUNCTION_TYPE_CONSTANT,
		FUNCTION_TYPE_LINEAR_LAGRANGE = CMZN_ELEMENTBASIS_FUNCTION_TYPE_LINEAR_LAGRANGE,
		FUNCTION_TYPE_QUADRATIC_LAGRANGE = CMZN_ELEMENTBASIS_FUNCTION_TYPE_QUADRATIC_LAGRANGE,
		FUNCTION_TYPE_CUBIC_LAGRANGE = CMZN_ELEMENTBASIS_FUNCTION_TYPE_CUBIC_LAGRANGE,
		FUNCTION_TYPE_LINEAR_SIMPLEX = CMZN_ELEMENTBASIS_FUNCTION_TYPE_LINEAR_SIMPLEX,   /**< linked on 2 or more dimensions */
		FUNCTION_TYPE_QUADRATIC_SIMPLEX = CMZN_ELEMENTBASIS_FUNCTION_TYPE_QUADRATIC_SIMPLEX, /**< linked on 2 or more dimensions */
		FUNCTION_TYPE_CUBIC_HERMITE = CMZN_ELEMENTBASIS_FUNCTION_TYPE_CUBIC_HERMITE
	};

	cmzn_elementbasis_id getId() const
	{
		return id;
	}

	int getDimension()
	{
		return cmzn_elementbasis_get_dimension(id);
	}

	enum FunctionType getFunctionType(int chartComponent)
	{
		return static_cast<FunctionType>(cmzn_elementbasis_get_function_type(id, chartComponent));
	}

	int setFunctionType(int chartComponent, FunctionType functionType)
	{
		return cmzn_elementbasis_set_function_type(id, chartComponent,
			static_cast<cmzn_elementbasis_function_type>(functionType));
	}

	int getNumberOfNodes()
	{
		return cmzn_elementbasis_get_number_of_nodes(id);
	}

	int getNumberOfFunctions()
	{
		return cmzn_elementbasis_get_number_of_functions(id);
	}

	int getNumberOfFunctionsPerNode(int nodeNumber)
	{
		return cmzn_elementbasis_get_number_of_functions_per_node(id, nodeNumber);
	}

};

class Fieldmodule;
class Mesh;
class MeshGroup;
class Elementtemplate;

class Element
{
private:

	cmzn_element_id id;

public:

	Element() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Element(cmzn_element_id element_id) :
		id(element_id)
	{ }

	Element(const Element& element) :
		id(cmzn_element_access(element.id))
	{ }

	~Element()
	{
		if (0 != id)
		{
			cmzn_element_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	/**
	 * Element change bit flags returned by Meshchanges query functions.
	 */
	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_ELEMENT_CHANGE_FLAG_NONE,
		CHANGE_FLAG_ADD = CMZN_ELEMENT_CHANGE_FLAG_ADD,
		CHANGE_FLAG_REMOVE = CMZN_ELEMENT_CHANGE_FLAG_REMOVE,
		CHANGE_FLAG_IDENTIFIER = CMZN_ELEMENT_CHANGE_FLAG_IDENTIFIER,
		CHANGE_FLAG_DEFINITION = CMZN_ELEMENT_CHANGE_FLAG_DEFINITION,
		CHANGE_FLAG_FIELD = CMZN_ELEMENT_CHANGE_FLAG_FIELD
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 */
	typedef int ChangeFlags;

	enum FaceType
	{
		FACE_TYPE_INVALID = CMZN_ELEMENT_FACE_TYPE_INVALID,
		FACE_TYPE_ALL = CMZN_ELEMENT_FACE_TYPE_ALL,
		FACE_TYPE_ANY_FACE = CMZN_ELEMENT_FACE_TYPE_ANY_FACE,
		FACE_TYPE_NO_FACE = CMZN_ELEMENT_FACE_TYPE_NO_FACE,
		FACE_TYPE_XI1_0 = CMZN_ELEMENT_FACE_TYPE_XI1_0,
		FACE_TYPE_XI1_1 = CMZN_ELEMENT_FACE_TYPE_XI1_1,
		FACE_TYPE_XI2_0 = CMZN_ELEMENT_FACE_TYPE_XI2_0,
		FACE_TYPE_XI2_1 = CMZN_ELEMENT_FACE_TYPE_XI2_1,
		FACE_TYPE_XI3_0 = CMZN_ELEMENT_FACE_TYPE_XI3_0,
		FACE_TYPE_XI3_1 = CMZN_ELEMENT_FACE_TYPE_XI3_1
	};

	Element& operator=(const Element& element)
	{
		cmzn_element_id temp_id = cmzn_element_access(element.id);
		if (0 != id)
		{
			cmzn_element_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	enum ShapeType
	{
		SHAPE_TYPE_INVALID = CMZN_ELEMENT_SHAPE_TYPE_INVALID,        /**< unspecified shape of known dimension */
		SHAPE_TYPE_LINE = CMZN_ELEMENT_SHAPE_TYPE_LINE,              /**< 1-D: 0 <= xi1 <= 1 */
		SHAPE_TYPE_SQUARE = CMZN_ELEMENT_SHAPE_TYPE_SQUARE,          /**< 2-D: 0 <= xi1,xi2 <= 1 */
		SHAPE_TYPE_TRIANGLE = CMZN_ELEMENT_SHAPE_TYPE_TRIANGLE,      /**< 3-D: 0 <= xi1,xi2; xi1+xi2 <= 1 */
		SHAPE_TYPE_CUBE = CMZN_ELEMENT_SHAPE_TYPE_CUBE,              /**< 3-D: 0 <= xi1,xi2,xi3 <= 1 */
		SHAPE_TYPE_TETRAHEDRON = CMZN_ELEMENT_SHAPE_TYPE_TETRAHEDRON,/**< 3-D: 0 <= xi1,xi2,xi3; xi1+xi2+xi3 <= 1 */
		SHAPE_TYPE_WEDGE12 = CMZN_ELEMENT_SHAPE_TYPE_WEDGE12,        /**< 3-D: 0 <= xi1,xi2; xi1+xi2 <= 1; 0 <= xi3 <= 1 */
		SHAPE_TYPE_WEDGE13 = CMZN_ELEMENT_SHAPE_TYPE_WEDGE13,        /**< 3-D: 0 <= xi1,xi3; xi1+xi3 <= 1; 0 <= xi2 <= 1 */
		SHAPE_TYPE_WEDGE23 = CMZN_ELEMENT_SHAPE_TYPE_WEDGE23         /**< 3-D: 0 <= xi2,xi3; xi2+xi3 <= 1; 0 <= xi1 <= 1 */
	};

	enum PointSamplingMode
	{
		POINT_SAMPLING_MODE_INVALID = CMZN_ELEMENT_POINT_SAMPLING_MODE_INVALID,
		POINT_SAMPLING_MODE_CELL_CENTRES = CMZN_ELEMENT_POINT_SAMPLING_MODE_CELL_CENTRES,
		POINT_SAMPLING_MODE_CELL_CORNERS = CMZN_ELEMENT_POINT_SAMPLING_MODE_CELL_CORNERS,
		POINT_SAMPLING_MODE_CELL_POISSON = CMZN_ELEMENT_POINT_SAMPLING_MODE_CELL_POISSON,
		POINT_SAMPLING_MODE_SET_LOCATION = CMZN_ELEMENT_POINT_SAMPLING_MODE_SET_LOCATION,
		POINT_SAMPLING_MODE_GAUSSIAN_QUADRATURE = CMZN_ELEMENT_POINT_SAMPLING_MODE_GAUSSIAN_QUADRATURE
	};

	enum QuadratureRule
	{
		QUADRATURE_RULE_INVALID = CMZN_ELEMENT_QUADRATURE_RULE_INVALID,
		QUADRATURE_RULE_GAUSSIAN = CMZN_ELEMENT_QUADRATURE_RULE_GAUSSIAN,
		QUADRATURE_RULE_MIDPOINT = CMZN_ELEMENT_QUADRATURE_RULE_MIDPOINT
	};

	cmzn_element_id getId() const
	{
		return id;
	}

	int getDimension()
	{
		return cmzn_element_get_dimension(id);
	}

	int getIdentifier()
	{
		return cmzn_element_get_identifier(id);
	}

	int setIdentifier(int identifier)
	{
		return cmzn_element_set_identifier(id, identifier);
	}

	inline Mesh getMesh() const;

	enum ShapeType getShapeType()
	{
		return static_cast<ShapeType>(cmzn_element_get_shape_type(id));
	}

	inline int merge(const Elementtemplate& elementTemplate);

};

inline bool operator==(const Element& a, const Element& b)
{
	return a.getId() == b.getId();
}

class Elementtemplate
{
private:

	cmzn_elementtemplate_id id;

public:

	Elementtemplate() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Elementtemplate(cmzn_elementtemplate_id element_template_id) :
		id(element_template_id)
	{ }

	Elementtemplate(const Elementtemplate& elementTemplate) :
		id(cmzn_elementtemplate_access(elementTemplate.id))
	{ }

	Elementtemplate& operator=(const Elementtemplate& elementTemplate)
	{
		cmzn_elementtemplate_id temp_id = cmzn_elementtemplate_access(elementTemplate.id);
		if (0 != id)
		{
			cmzn_elementtemplate_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Elementtemplate()
	{
		if (0 != id)
		{
			cmzn_elementtemplate_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_elementtemplate_id getId() const
	{
		return id;
	}

	enum Element::ShapeType getElementShapeType()
	{
		return static_cast<Element::ShapeType>(cmzn_elementtemplate_get_element_shape_type(id));
	}

	int setElementShapeType(enum Element::ShapeType shapeType)
	{
		return cmzn_elementtemplate_set_element_shape_type(id,
			static_cast<cmzn_element_shape_type>(shapeType));
	}

	int getNumberOfNodes()
	{
		return cmzn_elementtemplate_get_number_of_nodes(id);
	}

	int setNumberOfNodes(int numberOfNodes)
	{
		return cmzn_elementtemplate_set_number_of_nodes(id, numberOfNodes);
	}

	int defineFieldElementConstant(const Field& field, int componentNumber)
	{
		return cmzn_elementtemplate_define_field_element_constant(
			id, field.getId(), componentNumber);
	}

	int defineFieldSimpleNodal(const Field& field, int componentNumber,
		const Elementbasis& basis, int nodeIndexesCount, const int *nodeIndexesIn)
	{
		return cmzn_elementtemplate_define_field_simple_nodal(
			id, field.getId(), componentNumber, basis.getId(),
			nodeIndexesCount, nodeIndexesIn);
	}

	int setMapNodeValueLabel(const Field& field, int componentNumber,
		int basisNodeIndex, int nodeFunctionIndex, Node::ValueLabel nodeValueLabel)
	{
		return cmzn_elementtemplate_set_map_node_value_label(id, field.getId(),
			componentNumber, basisNodeIndex, nodeFunctionIndex,
			static_cast<cmzn_node_value_label>(nodeValueLabel));
	}

	int setMapNodeVersion(const Field& field, int componentNumber,
		int basisNodeIndex, int nodeFunctionIndex, int versionNumber)
	{
		return cmzn_elementtemplate_set_map_node_version(id, field.getId(),
			componentNumber, basisNodeIndex, nodeFunctionIndex, versionNumber);
	}

	Node getNode(int localNodeIndex)
	{
		return Node(cmzn_elementtemplate_get_node(id, localNodeIndex));
	}

	int setNode(int localNodeIndex, const Node& node)
	{
		return cmzn_elementtemplate_set_node(id, localNodeIndex, node.getId());
	}
};

class Elementiterator
{
private:

	cmzn_elementiterator_id id;

public:

	Elementiterator() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Elementiterator(cmzn_elementiterator_id element_iterator_id) :
		id(element_iterator_id)
	{ }

	Elementiterator(const Elementiterator& elementIterator) :
		id(cmzn_elementiterator_access(elementIterator.id))
	{ }

	Elementiterator& operator=(const Elementiterator& elementIterator)
	{
		cmzn_elementiterator_id temp_id = cmzn_elementiterator_access(elementIterator.id);
		if (0 != id)
		{
			cmzn_elementiterator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Elementiterator()
	{
		if (0 != id)
		{
			cmzn_elementiterator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Element next()
	{
		return Element(cmzn_elementiterator_next(id));
	}
};

class Mesh
{

protected:
	cmzn_mesh_id id;

public:

	Mesh() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Mesh(cmzn_mesh_id mesh_id) :	id(mesh_id)
	{ }

	Mesh(const Mesh& mesh) :
		id(cmzn_mesh_access(mesh.id))
	{ }

	~Mesh()
	{
		if (0 != id)
		{
			cmzn_mesh_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Mesh& operator=(const Mesh& mesh)
	{
		cmzn_mesh_id temp_id = cmzn_mesh_access(mesh.id);
		if (0 != id)
		{
			cmzn_mesh_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	cmzn_mesh_id getId() const
	{
		return id;
	}

	inline MeshGroup castGroup();

	bool containsElement(const Element& element)
	{
		return cmzn_mesh_contains_element(id, element.getId());
	}

	Elementtemplate createElementtemplate()
	{
		return Elementtemplate(cmzn_mesh_create_elementtemplate(id));
	}

	Element createElement(int identifier, const Elementtemplate& elementTemplate)
	{
		return Element(cmzn_mesh_create_element(id, identifier, elementTemplate.getId()));
	}

	Elementiterator createElementiterator()
	{
		return Elementiterator(cmzn_mesh_create_elementiterator(id));
	}

	int defineElement(int identifier, const Elementtemplate& elementTemplate)
	{
		return cmzn_mesh_define_element(id, identifier, elementTemplate.getId());
	}

	int destroyAllElements()
	{
		return cmzn_mesh_destroy_all_elements(id);
	}

	int destroyElement(const Element& element)
	{
		 return cmzn_mesh_destroy_element(id, element.getId());
	}

	int destroyElementsConditional(const Field& conditionalField)
	{
		return cmzn_mesh_destroy_elements_conditional(id,
			conditionalField.getId());
	}

	Element findElementByIdentifier(int identifier)
	{
		return Element(cmzn_mesh_find_element_by_identifier(id, identifier));
	}

	Differentialoperator getChartDifferentialoperator(int order, int term)
	{
		return Differentialoperator(cmzn_mesh_get_chart_differentialoperator(
			id, order, term));
	}

	int getDimension()
	{
		return cmzn_mesh_get_dimension(id);
	}

	inline Fieldmodule getFieldmodule() const;

	Mesh getMasterMesh()
	{
		return Mesh(cmzn_mesh_get_master_mesh(id));
	}

	char *getName()
	{
		return cmzn_mesh_get_name(id);
	}

	int getSize()
	{
		return cmzn_mesh_get_size(id);
	}

};

inline bool operator==(const Mesh& a, const Mesh& b)
{
	return cmzn_mesh_match(a.getId(), b.getId());
}

inline Mesh Element::getMesh() const
{
	return Mesh(cmzn_element_get_mesh(id));
}

class MeshGroup : public Mesh
{

public:

	// takes ownership of C handle, responsibility for destroying it
	explicit MeshGroup(cmzn_mesh_group_id mesh_id) : Mesh(reinterpret_cast<cmzn_mesh_id>(mesh_id))
	{ }

	MeshGroup()
	{ }

	cmzn_mesh_group_id getId() const
	{
		return (cmzn_mesh_group_id)(id);
	}

	int addElement(const Element& element)
	{
		return cmzn_mesh_group_add_element(
			reinterpret_cast<cmzn_mesh_group_id>(id), element.getId());
	}

	int addElementsConditional(const Field& conditionalField)
	{
		return cmzn_mesh_group_add_elements_conditional(
			reinterpret_cast<cmzn_mesh_group_id>(id), conditionalField.getId());
	}

	int removeAllElements()
	{
		return cmzn_mesh_group_remove_all_elements(reinterpret_cast<cmzn_mesh_group_id>(id));
	}

	int removeElement(const Element& element)
	{
		return cmzn_mesh_group_remove_element(reinterpret_cast<cmzn_mesh_group_id>(id),
			element.getId());
	}

	int removeElementsConditional(const Field& conditionalField)
	{
		return cmzn_mesh_group_remove_elements_conditional(
			reinterpret_cast<cmzn_mesh_group_id>(id), conditionalField.getId());
	}

};

inline MeshGroup Mesh::castGroup()
{
	return MeshGroup(cmzn_mesh_cast_group(id));
}

class Meshchanges
{
private:

	cmzn_meshchanges_id id;

public:

	Meshchanges() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Meshchanges(cmzn_meshchanges_id meshchanges_id) :
		id(meshchanges_id)
	{ }

	Meshchanges(const Meshchanges& meshchanges) :
		id(cmzn_meshchanges_access(meshchanges.id))
	{ }

	Meshchanges& operator=(const Meshchanges& meshchanges)
	{
		cmzn_meshchanges_id temp_id = cmzn_meshchanges_access(meshchanges.id);
		if (0 != id)
			cmzn_meshchanges_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Meshchanges()
	{
		if (0 != id)
			cmzn_meshchanges_destroy(&id);
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Element::ChangeFlags getElementChangeFlags(const Element& element)
	{
		return cmzn_meshchanges_get_element_change_flags(id, element.getId());
	}

	int getNumberOfChanges()
	{
		return cmzn_meshchanges_get_number_of_changes(id);
	}

	Element::ChangeFlags getSummaryElementChangeFlags()
	{
		return cmzn_meshchanges_get_summary_element_change_flags(id);
	}
};

inline int Element::merge(const Elementtemplate& elementTemplate)
{
	return cmzn_element_merge(id, elementTemplate.getId());
}

}  // namespace Zinc
}

#endif /* CMZN_ELEMENT_HPP__ */
