/**
 * @file fieldalias.hpp
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDALIAS_HPP__
#define CMZN_FIELDALIAS_HPP__

#include "opencmiss/zinc/fieldalias.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldAlias : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldAlias(cmzn_field_id field_id) : Field(field_id)
	{ }

	friend FieldAlias Fieldmodule::createFieldAlias(const Field& sourceField);

public:

	FieldAlias() : Field(0)
	{ }

};

inline FieldAlias Fieldmodule::createFieldAlias(const Field& sourceField)
{
	return FieldAlias(cmzn_fieldmodule_create_field_alias(id, sourceField.getId()));
}

}  // namespace Zinc
}
#endif
