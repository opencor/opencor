/**
 * @file fieldfiniteelement.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDTYPESFINITEELEMENT_HPP__
#define CMZN_FIELDTYPESFINITEELEMENT_HPP__

#include "opencmiss/zinc/fieldfiniteelement.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldcache.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/node.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldFiniteElement : public Field
{
public:

	FieldFiniteElement() : Field(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldFiniteElement(cmzn_field_finite_element_id field_finite_element_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_finite_element_id))
	{	}

	inline cmzn_field_finite_element_id getDerivedId()
	{
		return reinterpret_cast<cmzn_field_finite_element_id>(this->id);
	}

	int getNodeParameters(const Fieldcache& cache, int componentNumber,
		Node::ValueLabel nodeValueLabel, int versionNumber, int valuesCount, double *valuesOut)
	{
		return cmzn_field_finite_element_get_node_parameters(this->getDerivedId(),
			cache.getId(), componentNumber, static_cast<cmzn_node_value_label>(nodeValueLabel),
			versionNumber, valuesCount, valuesOut);
	}

	int setNodeParameters(const Fieldcache& cache, int componentNumber,
		Node::ValueLabel nodeValueLabel, int versionNumber, int valuesCount, const double *valuesIn)
	{
		return cmzn_field_finite_element_set_node_parameters(this->getDerivedId(),
			cache.getId(), componentNumber, static_cast<cmzn_node_value_label>(nodeValueLabel),
			versionNumber, valuesCount, valuesIn);
	}
};

class FieldEdgeDiscontinuity : public Field
{
private:
	friend FieldEdgeDiscontinuity Fieldmodule::createFieldEdgeDiscontinuity(
		const Field& sourceField);

public:

	FieldEdgeDiscontinuity() : Field(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldEdgeDiscontinuity(cmzn_field_edge_discontinuity_id field_edge_discontinuity_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_edge_discontinuity_id))
	{	}

	inline cmzn_field_edge_discontinuity_id getDerivedId()
	{
		return reinterpret_cast<cmzn_field_edge_discontinuity_id>(id);
	}

	enum Measure
	{
		MEASURE_INVALID = CMZN_FIELD_EDGE_DISCONTINUITY_MEASURE_INVALID,
		MEASURE_C1 = CMZN_FIELD_EDGE_DISCONTINUITY_MEASURE_C1,
		MEASURE_G1 = CMZN_FIELD_EDGE_DISCONTINUITY_MEASURE_G1,
		MEASURE_SURFACE_NORMAL = CMZN_FIELD_EDGE_DISCONTINUITY_MEASURE_SURFACE_NORMAL
	};

	Field getConditionalField()
	{
		return Field(cmzn_field_edge_discontinuity_get_conditional_field(getDerivedId()));
	}

	int setConditionalField(const Field& conditionalField)
	{
		return cmzn_field_edge_discontinuity_set_conditional_field(getDerivedId(), conditionalField.getId());
	}

	Measure getMeasure()
	{
		return static_cast<Measure>(cmzn_field_edge_discontinuity_get_measure(getDerivedId()));
	}

	int setMeasure(Measure measure)
	{
		return cmzn_field_edge_discontinuity_set_measure(getDerivedId(),
			static_cast<cmzn_field_edge_discontinuity_measure>(measure));
	}

};

class FieldEmbedded : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldEmbedded(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldEmbedded Fieldmodule::createFieldEmbedded(const Field& sourceField,
		const Field& embeddedLocationField);

public:

	FieldEmbedded() : Field(0)
	{ }

};

class FieldFindMeshLocation : public Field
{
public:

	FieldFindMeshLocation() : Field(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldFindMeshLocation(cmzn_field_find_mesh_location_id field_find_mesh_location_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_find_mesh_location_id))
	{	}

	enum SearchMode
	{
		SEARCH_MODE_INVALID = CMZN_FIELD_FIND_MESH_LOCATION_SEARCH_MODE_INVALID,
		SEARCH_MODE_EXACT = CMZN_FIELD_FIND_MESH_LOCATION_SEARCH_MODE_EXACT,
		SEARCH_MODE_NEAREST = CMZN_FIELD_FIND_MESH_LOCATION_SEARCH_MODE_NEAREST
	};

	Mesh getMesh()
	{
		return Mesh(cmzn_field_find_mesh_location_get_mesh(
			reinterpret_cast<cmzn_field_find_mesh_location_id>(id)));
	}

	SearchMode getSearchMode()
	{
		return static_cast<SearchMode>(cmzn_field_find_mesh_location_get_search_mode(
			reinterpret_cast<cmzn_field_find_mesh_location_id>(id)));
	}

	int setSearchMode(SearchMode searchMode)
	{
		return cmzn_field_find_mesh_location_set_search_mode(
			reinterpret_cast<cmzn_field_find_mesh_location_id>(id),
			static_cast<cmzn_field_find_mesh_location_search_mode>(searchMode));
	}
};

class FieldNodeValue : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodeValue(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodeValue Fieldmodule::createFieldNodeValue(const Field& sourceField,
		Node::ValueLabel nodeValueLabel, int versionNumber);

public:

	FieldNodeValue() : Field(0)
	{ }

};

class FieldStoredMeshLocation : public Field
{
public:

	FieldStoredMeshLocation() : Field(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldStoredMeshLocation(cmzn_field_stored_mesh_location_id field_stored_mesh_location_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_stored_mesh_location_id))
	{	}
};

class FieldStoredString : public Field
{
public:

	FieldStoredString() : Field(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldStoredString(cmzn_field_stored_string_id field_stored_string_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_stored_string_id))
	{	}
};

class FieldIsExterior : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldIsExterior(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldIsExterior Fieldmodule::createFieldIsExterior();

public:

	FieldIsExterior() : Field(0)
	{ }
};

class FieldIsOnFace : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldIsOnFace(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldIsOnFace Fieldmodule::createFieldIsOnFace(Element::FaceType face);

public:

	FieldIsOnFace() : Field(0)
	{ }
};

class FieldNodeLookup : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodeLookup(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodeLookup Fieldmodule::createFieldNodeLookup(const Field& sourceField,
		const Node& lookupNode);

public:

	FieldNodeLookup() : Field(0)
	{ }

};


inline FieldFiniteElement Fieldmodule::createFieldFiniteElement(int numberOfComponents)
{
	return FieldFiniteElement(reinterpret_cast<cmzn_field_finite_element_id>(
		cmzn_fieldmodule_create_field_finite_element(id,numberOfComponents)));
}

inline FieldFiniteElement Field::castFiniteElement()
{
	return FieldFiniteElement(cmzn_field_cast_finite_element(id));
}

inline FieldEdgeDiscontinuity Fieldmodule::createFieldEdgeDiscontinuity(
	const Field& sourceField)
{
	return FieldEdgeDiscontinuity(reinterpret_cast<cmzn_field_edge_discontinuity_id>(
		cmzn_fieldmodule_create_field_edge_discontinuity(id, sourceField.getId())));
}

inline FieldEdgeDiscontinuity Field::castEdgeDiscontinuity()
{
	return FieldEdgeDiscontinuity(cmzn_field_cast_edge_discontinuity(id));
}

inline FieldEmbedded Fieldmodule::createFieldEmbedded(const Field& sourceField, const Field& embeddedLocationField)
{
	return FieldEmbedded(cmzn_fieldmodule_create_field_embedded(id,
		sourceField.getId(), embeddedLocationField.getId()));
}

inline FieldFindMeshLocation Fieldmodule::createFieldFindMeshLocation(
	const Field& sourceField, const Field& meshField, const Mesh& mesh)
{
	return FieldFindMeshLocation(reinterpret_cast<cmzn_field_find_mesh_location_id>(
		cmzn_fieldmodule_create_field_find_mesh_location(id, sourceField.getId(), meshField.getId(), mesh.getId())));
}

inline FieldFindMeshLocation Field::castFindMeshLocation()
{
	return FieldFindMeshLocation(cmzn_field_cast_find_mesh_location(id));
}

inline FieldNodeValue Fieldmodule::createFieldNodeValue(const Field& sourceField,
	Node::ValueLabel nodeValueLabel, int versionNumber)
{
	return FieldNodeValue(cmzn_fieldmodule_create_field_node_value(id,
		sourceField.getId(), static_cast<cmzn_node_value_label>(nodeValueLabel),
		versionNumber));
}

inline FieldStoredMeshLocation Fieldmodule::createFieldStoredMeshLocation(const Mesh& mesh)
{
	return FieldStoredMeshLocation(reinterpret_cast<cmzn_field_stored_mesh_location_id>(
		cmzn_fieldmodule_create_field_stored_mesh_location(id, mesh.getId())));
}

inline FieldStoredMeshLocation Field::castStoredMeshLocation()
{
	return FieldStoredMeshLocation(cmzn_field_cast_stored_mesh_location(id));
}

inline FieldStoredString Fieldmodule::createFieldStoredString()
{
	return FieldStoredString(reinterpret_cast<cmzn_field_stored_string_id>(
		cmzn_fieldmodule_create_field_stored_string(id)));
}

inline FieldStoredString Field::castStoredString()
{
	return FieldStoredString(cmzn_field_cast_stored_string(id));
}

inline FieldIsExterior Fieldmodule::createFieldIsExterior()
{
	return FieldIsExterior(cmzn_fieldmodule_create_field_is_exterior(id));
}

inline FieldIsOnFace Fieldmodule::createFieldIsOnFace(Element::FaceType face)
{
	return FieldIsOnFace(cmzn_fieldmodule_create_field_is_on_face(
		id, static_cast<cmzn_element_face_type>(face)));
}

inline FieldNodeLookup Fieldmodule::createFieldNodeLookup(const Field& sourceField,
	const Node& lookupNode)
{
	return FieldNodeLookup(cmzn_fieldmodule_create_field_node_lookup(
		id, sourceField.getId(), lookupNode.getId()));
}

}  // namespace Zinc
}
#endif /* CMZN_FIELD_TYPES_FINITE_ELEMENT_HPP__ */
