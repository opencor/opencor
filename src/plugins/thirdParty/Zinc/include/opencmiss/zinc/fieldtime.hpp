/**
 * @file fieldtime.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDTIME_HPP__
#define CMZN_FIELDTIME_HPP__

#include "opencmiss/zinc/fieldtime.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/timekeeper.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldTimeLookup : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldTimeLookup(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldTimeLookup Fieldmodule::createFieldTimeLookup(const Field& sourceField,
		const Field& timeField);

public:

	FieldTimeLookup() : Field(0)
	{	}

};

class FieldTimeValue : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldTimeValue(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldTimeValue Fieldmodule::createFieldTimeValue(const Timekeeper& timeKeeper);

public:

	FieldTimeValue() : Field(0)
	{	}

};

inline FieldTimeLookup Fieldmodule::createFieldTimeLookup(const Field& sourceField, const Field& timeField)
{
	return FieldTimeLookup(cmzn_fieldmodule_create_field_time_lookup(id,
		sourceField.getId(), timeField.getId()));
}

inline FieldTimeValue Fieldmodule::createFieldTimeValue(const Timekeeper& timeKeeper)
{
	return FieldTimeValue(cmzn_fieldmodule_create_field_time_value(id, timeKeeper.getId()));
}

}  // namespace Zinc
}

#endif /* CMZN_FIELDTIME_HPP__ */
