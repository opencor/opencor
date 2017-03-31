/**
 * @file fieldmatrixoperators.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDMATRIXOPERATORS_HPP__
#define CMZN_FIELDMATRIXOPERATORS_HPP__

#include "opencmiss/zinc/fieldmatrixoperators.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldDeterminant : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldDeterminant(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldDeterminant Fieldmodule::createFieldDeterminant(const Field& sourceField);

public:

	FieldDeterminant() : Field(0)
	{	}

};

class FieldEigenvalues : public Field
{
private:

	friend FieldEigenvalues Fieldmodule::createFieldEigenvalues(const Field& sourceField);

public:

	explicit FieldEigenvalues(cmzn_field_eigenvalues_id field_eigenvalues_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_eigenvalues_id))
	{	}

	FieldEigenvalues() : Field(0)
	{	}

};

class FieldEigenvectors : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldEigenvectors(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldEigenvectors Fieldmodule::createFieldEigenvectors(
		const FieldEigenvalues& eigenvaluesField);

public:

	FieldEigenvectors() : Field(0)
	{	}

};

class FieldMatrixInvert : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldMatrixInvert(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldMatrixInvert Fieldmodule::createFieldMatrixInvert(const Field& sourceField);

public:

	FieldMatrixInvert() : Field(0)
	{	}

};

class FieldMatrixMultiply : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldMatrixMultiply(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldMatrixMultiply Fieldmodule::createFieldMatrixMultiply(int numberOfRows,
		const Field& sourceField1, const Field& sourceField2);

public:

	FieldMatrixMultiply() : Field(0)
	{	}

};

class FieldProjection : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldProjection(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldProjection Fieldmodule::createFieldProjection(const Field& sourceField,
		const Field& projectionMatrixField);

public:

	FieldProjection() : Field(0)
	{	}

};

class FieldTranspose : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldTranspose(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldTranspose Fieldmodule::createFieldTranspose(int sourceNumberOfRows,
		const Field& sourceField);

public:

	FieldTranspose() : Field(0)
	{	}

};

inline FieldDeterminant Fieldmodule::createFieldDeterminant(const Field& sourceField)
{
	return FieldDeterminant(cmzn_fieldmodule_create_field_determinant(id,
		sourceField.getId()));
}

inline FieldEigenvalues Field::castEigenvalues()
{
	return FieldEigenvalues(cmzn_field_cast_eigenvalues(id));
}

inline FieldEigenvalues Fieldmodule::createFieldEigenvalues(const Field& sourceField)
{
	return FieldEigenvalues(reinterpret_cast<cmzn_field_eigenvalues_id>(
		cmzn_fieldmodule_create_field_eigenvalues(id,sourceField.getId())));
}

inline FieldEigenvectors Fieldmodule::createFieldEigenvectors(const FieldEigenvalues& eigenValuesField)
{
	return FieldEigenvectors(cmzn_fieldmodule_create_field_eigenvectors(id,
		eigenValuesField.getId()));
}

inline FieldMatrixInvert Fieldmodule::createFieldMatrixInvert(const Field& sourceField)
{
	return FieldMatrixInvert(cmzn_fieldmodule_create_field_matrix_invert(id,
		sourceField.getId()));
}

inline FieldMatrixMultiply Fieldmodule::createFieldMatrixMultiply(int numberOfRows,
	const Field& sourceField1, const Field& sourceField2)
{
	return FieldMatrixMultiply(cmzn_fieldmodule_create_field_matrix_multiply(id,
		numberOfRows, sourceField1.getId(), sourceField2.getId()));
}

inline FieldProjection Fieldmodule::createFieldProjection(const Field& sourceField,
	const Field& projectionMatrixField)
{
	return FieldProjection(cmzn_fieldmodule_create_field_projection(id,
		sourceField.getId(), projectionMatrixField.getId()));
}

inline FieldTranspose Fieldmodule::createFieldTranspose(int sourceNumberOfRows,
	const Field& sourceField)
{
	return FieldTranspose(cmzn_fieldmodule_create_field_transpose(id,
		sourceNumberOfRows, sourceField.getId()));
}

}  // namespace Zinc
}

#endif
