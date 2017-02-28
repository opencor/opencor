/**
 * @file fieldconditional.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDCONDITIONAL_HPP__
#define CMZN_FIELDCONDITIONAL_HPP__

#include "opencmiss/zinc/fieldconditional.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldIf : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldIf(cmzn_field_id field_id) : Field(field_id)
	{ }

	friend FieldIf Fieldmodule::createFieldIf(const Field& sourceField1, const Field& sourceField2, const Field& sourceField3);

public:

	FieldIf() : Field(0)
	{	}

};

inline FieldIf Fieldmodule::createFieldIf(const Field& sourceField1, const Field& sourceField2, const Field& sourceField3)
{
	return FieldIf(cmzn_fieldmodule_create_field_if(id,
		sourceField1.getId(), sourceField2.getId(), sourceField3.getId()));
}

}  // namespace Zinc
}

#endif
