/**
 * @file field.hpp
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELD_HPP__
#define CMZN_FIELD_HPP__

#include "opencmiss/zinc/field.h"

namespace OpenCMISS
{
namespace Zinc
{

class Differentialoperator;
class Element;
class Fieldcache;
class FieldComponent;
class FieldEdgeDiscontinuity;
class FieldEigenvalues;
class FieldElementGroup;
class FieldFindMeshLocation;
class FieldFiniteElement;
class FieldGroup;
class FieldImage;
class FieldImagefilterBinaryThreshold;
class FieldImagefilterDiscreteGaussian;
class FieldImagefilterHistogram;
class FieldImagefilterThreshold;
class FieldMeshIntegral;
class FieldNodeGroup;
class FieldStoredMeshLocation;
class FieldStoredString;
class Fieldmodule;
class Fieldsmoothing;

class Field
{
protected:

	cmzn_field_id id;

public:

	Field() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Field(cmzn_field_id field_id) : id(field_id)
	{ }

	Field(const Field& field) : id(cmzn_field_access(field.id))
	{ }

	Field& operator=(const Field& field)
	{
		cmzn_field_id temp_id = cmzn_field_access(field.id);
		if (0 != id)
		{
			cmzn_field_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Field()
	{
		if (0 != id)
		{
			cmzn_field_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_field_id getId() const
	{
		return id;
	}

	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_FIELD_CHANGE_FLAG_NONE,
		CHANGE_FLAG_ADD = CMZN_FIELD_CHANGE_FLAG_ADD,
		CHANGE_FLAG_REMOVE = CMZN_FIELD_CHANGE_FLAG_REMOVE,
		CHANGE_FLAG_IDENTIFIER = CMZN_FIELD_CHANGE_FLAG_IDENTIFIER,
		CHANGE_FLAG_DEFINITION = CMZN_FIELD_CHANGE_FLAG_DEFINITION,
		CHANGE_FLAG_FULL_RESULT = CMZN_FIELD_CHANGE_FLAG_FULL_RESULT,
		CHANGE_FLAG_PARTIAL_RESULT = CMZN_FIELD_CHANGE_FLAG_PARTIAL_RESULT,
		CHANGE_FLAG_RESULT = CMZN_FIELD_CHANGE_FLAG_RESULT,
		CHANGE_FLAG_FINAL = CMZN_FIELD_CHANGE_FLAG_FINAL
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 * @see Fieldmoduleevent::getFieldChangeFlags
	 */
	typedef int ChangeFlags;

	enum CoordinateSystemType
	{
		COORDINATE_SYSTEM_TYPE_INVALID = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_INVALID,
		COORDINATE_SYSTEM_TYPE_RECTANGULAR_CARTESIAN = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_RECTANGULAR_CARTESIAN ,
		COORDINATE_SYSTEM_TYPE_CYLINDRICAL_POLAR = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_CYLINDRICAL_POLAR,
		COORDINATE_SYSTEM_TYPE_SPHERICAL_POLAR = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_SPHERICAL_POLAR,
		COORDINATE_SYSTEM_TYPE_PROLATE_SPHEROIDAL = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_PROLATE_SPHEROIDAL,
			/*!< uses ATTRIBUTE_COORDINATE_SYSTEM_FOCUS */
		COORDINATE_SYSTEM_TYPE_OBLATE_SPHEROIDAL = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_OBLATE_SPHEROIDAL,
			/*!< uses ATTRIBUTE_COORDINATE_SYSTEM_FOCUS */
		COORDINATE_SYSTEM_TYPE_FIBRE = CMZN_FIELD_COORDINATE_SYSTEM_TYPE_FIBRE
			/*!< For Euler angles specifying fibre axes orientation from default
			 * aligned with element xi coordinates. */
	};

	enum DomainType
	{
		DOMAIN_TYPE_INVALID = CMZN_FIELD_DOMAIN_TYPE_INVALID,
		DOMAIN_TYPE_POINT = CMZN_FIELD_DOMAIN_TYPE_POINT,
		DOMAIN_TYPE_NODES = CMZN_FIELD_DOMAIN_TYPE_NODES,
		DOMAIN_TYPE_DATAPOINTS = CMZN_FIELD_DOMAIN_TYPE_DATAPOINTS,
		DOMAIN_TYPE_MESH1D = CMZN_FIELD_DOMAIN_TYPE_MESH1D,
		DOMAIN_TYPE_MESH2D = CMZN_FIELD_DOMAIN_TYPE_MESH2D,
		DOMAIN_TYPE_MESH3D = CMZN_FIELD_DOMAIN_TYPE_MESH3D,
		DOMAIN_TYPE_MESH_HIGHEST_DIMENSION = CMZN_FIELD_DOMAIN_TYPE_MESH_HIGHEST_DIMENSION
	};

	/**
	 * Type for passing logical OR of #DomainType
	 */
	typedef int DomainTypes;

	enum ValueType
	{
		VALUE_TYPE_INVALID = CMZN_FIELD_VALUE_TYPE_INVALID,
		VALUE_TYPE_REAL = CMZN_FIELD_VALUE_TYPE_REAL,
		VALUE_TYPE_STRING = CMZN_FIELD_VALUE_TYPE_STRING,
		VALUE_TYPE_MESH_LOCATION = CMZN_FIELD_VALUE_TYPE_MESH_LOCATION
	};

	bool isManaged()
	{
		return cmzn_field_is_managed(id);
	}

	int setManaged(bool value)
	{
		return cmzn_field_set_managed(id, value);
	}

	char *getComponentName(int componentNumber)
	{
		return cmzn_field_get_component_name(id, componentNumber);
	}

	int setComponentName(int componentNumber, const char *name)
	{
		return cmzn_field_set_component_name(id, componentNumber, name);
	}

	double getCoordinateSystemFocus()
	{
		return cmzn_field_get_coordinate_system_focus(id);
	}

	int setCoordinateSystemFocus(double focus)
	{
		 return cmzn_field_set_coordinate_system_focus(id, focus);
	}

	CoordinateSystemType getCoordinateSystemType()
	{
		return static_cast<CoordinateSystemType>(
			cmzn_field_get_coordinate_system_type(id));
	}

	int setCoordinateSystemType(CoordinateSystemType coordinateSystemType)
	{
		 return cmzn_field_set_coordinate_system_type(id,
			 static_cast<cmzn_field_coordinate_system_type>(coordinateSystemType));
	}

	int getNumberOfComponents()
	{
		return cmzn_field_get_number_of_components(id);
	}

	char *getName()
	{
		return cmzn_field_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_field_set_name(id, name);
	}

	int getNumberOfSourceFields()
	{
		return cmzn_field_get_number_of_source_fields(id);
	}

	Field getSourceField(int index)
	{
		return Field(cmzn_field_get_source_field(id, index));
	}

	bool isTypeCoordinate()
	{
		return cmzn_field_is_type_coordinate(id);
	}

	int setTypeCoordinate(bool value)
	{
		return cmzn_field_set_type_coordinate(id, value);
	}

	ValueType getValueType()
	{
		return static_cast<ValueType>(cmzn_field_get_value_type(id));
	}

	inline Fieldmodule getFieldmodule() const;

	inline int assignMeshLocation(const Fieldcache& cache, const Element& element,
		int coordinatesCount, const double *coordinatesIn);

	inline int assignReal(const Fieldcache& cache, int valuesCount, const double *valuesIn);

	inline int assignString(const Fieldcache& cache, const char *stringValue);

	inline Element evaluateMeshLocation(const Fieldcache& cache, int coordinatesCount,
		double *coordinatesOut);

	inline int evaluateReal(const Fieldcache& cache, int valuesCount, double *valuesOut);

	inline char *evaluateString(const Fieldcache& cache);

	inline int evaluateDerivative(const Differentialoperator& differentialOperator,
		const Fieldcache& cache, int valuesCount, double *valuesOut);

	inline bool isDefinedAtLocation(const Fieldcache& cache);

	inline int smooth(const Fieldsmoothing& fieldsmoothing);

	// casting functions: must check isValid()
	inline FieldComponent castComponent();
	inline FieldEdgeDiscontinuity castEdgeDiscontinuity();
	inline FieldEigenvalues castEigenvalues();
	inline FieldElementGroup castElementGroup();
	inline FieldFindMeshLocation castFindMeshLocation();
	inline FieldFiniteElement castFiniteElement();
	inline FieldGroup castGroup();
	inline FieldImage castImage();
	inline FieldImagefilterBinaryThreshold castImagefilterBinaryThreshold();
	inline FieldImagefilterDiscreteGaussian castImagefilterDiscreteGaussian();
	inline FieldImagefilterHistogram castImagefilterHistogram();
	inline FieldImagefilterThreshold castImagefilterThreshold();
	inline FieldMeshIntegral castMeshIntegral();
	inline FieldNodeGroup castNodeGroup();
	inline FieldStoredMeshLocation castStoredMeshLocation();
	inline FieldStoredString castStoredString();
};

inline bool operator==(const Field& a, const Field& b)
{
	return a.getId() == b.getId();
}

class Fielditerator
{
private:

	cmzn_fielditerator_id id;

public:

	Fielditerator() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Fielditerator(cmzn_fielditerator_id iterator_id) :
		id(iterator_id)
	{ }

	Fielditerator(const Fielditerator& fielditerator) :
		id(cmzn_fielditerator_access(fielditerator.id))
	{ }

	Fielditerator& operator=(const Fielditerator& fielditerator)
	{
		cmzn_fielditerator_id temp_id = cmzn_fielditerator_access(fielditerator.id);
		if (0 != id)
		{
			cmzn_fielditerator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Fielditerator()
	{
		if (0 != id)
		{
			cmzn_fielditerator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Field next()
	{
		return Field(cmzn_fielditerator_next(id));
	}
};

}  // namespace Zinc
}
#endif /* CMZN_FIELD_HPP__ */
