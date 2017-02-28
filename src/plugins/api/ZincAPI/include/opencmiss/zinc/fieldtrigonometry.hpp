/**
 * @file fieldtrigonometry.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDTRIGONOMETRY_HPP__
#define CMZN_FIELDTRIGONOMETRY_HPP__

#include "opencmiss/zinc/fieldtrigonometry.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldSin : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldSin(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldSin Fieldmodule::createFieldSin(const Field& sourceField);

public:

	FieldSin() : Field(0)
	{	}

};

class FieldCos : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldCos(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldCos Fieldmodule::createFieldCos(const Field& sourceField);

public:

	FieldCos() : Field(0)
	{	}

};

class FieldTan : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldTan(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldTan Fieldmodule::createFieldTan(const Field& sourceField);

public:

	FieldTan() : Field(0)
	{	}

};

class FieldAsin : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldAsin(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldAsin Fieldmodule::createFieldAsin(const Field& sourceField);

public:

	FieldAsin() : Field(0)
	{	}

};

class FieldAcos : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldAcos(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldAcos Fieldmodule::createFieldAcos(const Field& sourceField);

public:

	FieldAcos() : Field(0)
	{	}

};

class FieldAtan : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldAtan(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldAtan Fieldmodule::createFieldAtan(const Field& sourceField);

public:

	FieldAtan() : Field(0)
	{	}

};

class FieldAtan2 : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldAtan2(cmzn_field_id field_id) : Field(field_id)
	{	}

public:

	FieldAtan2() : Field(0)
	{	}

	friend FieldAtan2 Fieldmodule::createFieldAtan2(const Field& sourceField1,
		const Field& sourceField2);

};

inline FieldSin Fieldmodule::createFieldSin(const Field& sourceField)
{
	return FieldSin(cmzn_fieldmodule_create_field_sin(id, sourceField.getId()));
}

inline FieldCos Fieldmodule::createFieldCos(const Field& sourceField)
{
	return FieldCos(cmzn_fieldmodule_create_field_cos(id, sourceField.getId()));
}

inline FieldTan Fieldmodule::createFieldTan(const Field& sourceField)
{
	return FieldTan(cmzn_fieldmodule_create_field_tan(id, sourceField.getId()));
}

inline FieldAsin Fieldmodule::createFieldAsin(const Field& sourceField)
{
	return FieldAsin(cmzn_fieldmodule_create_field_asin(id, sourceField.getId()));
}

inline FieldAcos Fieldmodule::createFieldAcos(const Field& sourceField)
{
	return FieldAcos(cmzn_fieldmodule_create_field_acos(id, sourceField.getId()));
}

inline FieldAtan Fieldmodule::createFieldAtan(const Field& sourceField)
{
	return FieldAtan(cmzn_fieldmodule_create_field_atan(id, sourceField.getId()));
}

inline FieldAtan2 Fieldmodule::createFieldAtan2(const Field& sourceField1, const Field& sourceField2)
{
	return FieldAtan2(cmzn_fieldmodule_create_field_atan2(id, sourceField1.getId(),
		sourceField2.getId()));
}

}  // namespace Zinc
}

#endif
