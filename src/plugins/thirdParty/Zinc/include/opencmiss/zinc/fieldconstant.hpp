/**
 * @file fieldconstant.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDCONSTANT_HPP__
#define CMZN_FIELDCONSTANT_HPP__

#include "opencmiss/zinc/fieldconstant.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldConstant : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldConstant(cmzn_field_id field_id) : Field(field_id)
	{ }

	friend FieldConstant Fieldmodule::createFieldConstant(int valuesCount, const double *valuesIn);

public:

	FieldConstant() : Field(0)
	{ }

};

class FieldStringConstant : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldStringConstant(cmzn_field_id field_id) : Field(field_id)
	{ }

	friend FieldStringConstant Fieldmodule::createFieldStringConstant(const char *stringConstant);

public:

	FieldStringConstant() : Field(0)
	{ }

};

inline FieldConstant Fieldmodule::createFieldConstant(int valuesCount, const double *valuesIn)
{
	return FieldConstant(cmzn_fieldmodule_create_field_constant(id,
		valuesCount, valuesIn));
}

inline FieldStringConstant Fieldmodule::createFieldStringConstant(const char *stringConstant)
{
	return FieldStringConstant(cmzn_fieldmodule_create_field_string_constant(id,
		stringConstant));
}

}  // namespace Zinc
}

#endif
