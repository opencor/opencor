/**
 * @file fieldmeshoperators.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDMESHOPERATORS_HPP__
#define CMZN_FIELDMESHOPERATORS_HPP__

#include "opencmiss/zinc/fieldmeshoperators.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/element.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldMeshIntegral : public Field
{
private:

	inline cmzn_field_mesh_integral_id getDerivedId()
	{
		return reinterpret_cast<cmzn_field_mesh_integral_id>(id);
	}

public:

	FieldMeshIntegral() : Field()
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldMeshIntegral(cmzn_field_mesh_integral_id field_mesh_integral_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_mesh_integral_id))
	{	}

	int getNumbersOfPoints(int valuesCount, int *valuesOut)
	{
		return cmzn_field_mesh_integral_get_numbers_of_points(getDerivedId(),
			valuesCount, valuesOut);
	}

	int setNumbersOfPoints(int valuesCount, const int *valuesIn)
	{
		return cmzn_field_mesh_integral_set_numbers_of_points(getDerivedId(),
			valuesCount, valuesIn);
	}

	Element::QuadratureRule getElementQuadratureRule()
	{
		return static_cast<Element::QuadratureRule>(
			cmzn_field_mesh_integral_get_element_quadrature_rule(getDerivedId()));
	};

	int setElementQuadratureRule(Element::QuadratureRule quadratureRule)
	{
		return cmzn_field_mesh_integral_set_element_quadrature_rule(getDerivedId(),
			static_cast<cmzn_element_quadrature_rule>(quadratureRule));
	}
};

/**
 * Specialisation of FieldMeshIntegral that integrates the squares of
 * the components of the integrand field. Note that the volume/area/length and
 * weights are not squared in the integral.
 * This field type supports least-squares optimisation by giving individual
 * terms being squared and summed.
 */
class FieldMeshIntegralSquares : public FieldMeshIntegral
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldMeshIntegralSquares(cmzn_field_mesh_integral_id field_mesh_integral_id) :
		FieldMeshIntegral(field_mesh_integral_id)
	{	}

	friend FieldMeshIntegralSquares Fieldmodule::createFieldMeshIntegralSquares(
		const Field& integrandField, const Field& coordinateField, const Mesh& mesh);

public:
	FieldMeshIntegralSquares()
	{	}
};


inline FieldMeshIntegral Fieldmodule::createFieldMeshIntegral(
	const Field& integrandField, const Field& coordinateField, const Mesh& mesh)
{
	return FieldMeshIntegral(reinterpret_cast<cmzn_field_mesh_integral_id>(
		cmzn_fieldmodule_create_field_mesh_integral(id, integrandField.getId(),
		coordinateField.getId(), mesh.getId())));
}

inline FieldMeshIntegral Field::castMeshIntegral()
{
	return FieldMeshIntegral(cmzn_field_cast_mesh_integral(id));
}

inline FieldMeshIntegralSquares Fieldmodule::createFieldMeshIntegralSquares(
	const Field& integrandField, const Field& coordinateField, const Mesh& mesh)
{
	return FieldMeshIntegralSquares(reinterpret_cast<cmzn_field_mesh_integral_id>(
		cmzn_fieldmodule_create_field_mesh_integral_squares(id, integrandField.getId(),
		coordinateField.getId(), mesh.getId())));
}

}  // namespace Zinc
}

#endif
