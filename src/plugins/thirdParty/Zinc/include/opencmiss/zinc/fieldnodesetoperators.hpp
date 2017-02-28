/**
 * @file fieldnodesetoperators.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDNODESETOPERATORS_HPP__
#define CMZN_FIELDNODESETOPERATORS_HPP__

#include "opencmiss/zinc/fieldnodesetoperators.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/node.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldNodesetSum : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodesetSum(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodesetSum Fieldmodule::createFieldNodesetSum(const Field& sourceField, const Nodeset& nodeset);

public:

	FieldNodesetSum() : Field(0)
	{	}

};

class FieldNodesetMean : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodesetMean(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodesetMean Fieldmodule::createFieldNodesetMean(const Field& sourceField,
		const Nodeset& nodeset);

public:

	FieldNodesetMean() : Field(0)
	{	}

};

class FieldNodesetSumSquares : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodesetSumSquares(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodesetSumSquares Fieldmodule::createFieldNodesetSumSquares(
		const Field& sourceField, const Nodeset& nodeset);

public:

	FieldNodesetSumSquares() : Field(0)
	{	}

};

class FieldNodesetMeanSquares : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodesetMeanSquares(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodesetMeanSquares Fieldmodule::createFieldNodesetMeanSquares(
		const Field& sourceField, const Nodeset& nodeset);

public:

	FieldNodesetMeanSquares() : Field(0)
	{	}

};

class FieldNodesetMinimum : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodesetMinimum(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodesetMinimum Fieldmodule::createFieldNodesetMinimum(
		const Field& sourceField, const Nodeset& nodeset);

public:

	FieldNodesetMinimum() : Field(0)
	{	}

};

class FieldNodesetMaximum : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNodesetMaximum(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNodesetMaximum Fieldmodule::createFieldNodesetMaximum(
		const Field& sourceField, const Nodeset& nodeset);

public:

	FieldNodesetMaximum() : Field(0)
	{	}

};

inline FieldNodesetSum Fieldmodule::createFieldNodesetSum(const Field& sourceField, const Nodeset& nodeset)
{
	return FieldNodesetSum(cmzn_fieldmodule_create_field_nodeset_sum(id,
		sourceField.getId(), nodeset.getId()));
}

inline FieldNodesetMean Fieldmodule::createFieldNodesetMean(const Field& sourceField, const Nodeset& nodeset)
{
	return FieldNodesetMean(cmzn_fieldmodule_create_field_nodeset_mean(id,
		sourceField.getId(), nodeset.getId()));
}

inline FieldNodesetSumSquares Fieldmodule::createFieldNodesetSumSquares(
	const Field& sourceField, const Nodeset& nodeset)
{
	return FieldNodesetSumSquares(cmzn_fieldmodule_create_field_nodeset_sum_squares(id,
		sourceField.getId(), nodeset.getId()));
}

inline FieldNodesetMeanSquares Fieldmodule::createFieldNodesetMeanSquares(
	const Field& sourceField, const Nodeset& nodeset)
{
	return FieldNodesetMeanSquares(cmzn_fieldmodule_create_field_nodeset_mean_squares(id,
		sourceField.getId(), nodeset.getId()));
}

inline FieldNodesetMinimum Fieldmodule::createFieldNodesetMinimum(
	const Field& sourceField, const Nodeset& nodeset)
{
	return FieldNodesetMinimum(cmzn_fieldmodule_create_field_nodeset_minimum(id,
		sourceField.getId(), nodeset.getId()));
}

inline FieldNodesetMaximum Fieldmodule::createFieldNodesetMaximum(
	const Field& sourceField, const Nodeset& nodeset)
{
	return FieldNodesetMaximum(cmzn_fieldmodule_create_field_nodeset_maximum(id,
		sourceField.getId(), nodeset.getId()));
}

}  // namespace Zinc
}

#endif
