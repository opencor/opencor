/**
 * @file fieldcomposite.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDCOMPOSITE_HPP__
#define CMZN_FIELDCOMPOSITE_HPP__


#include "opencmiss/zinc/fieldcomposite.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldIdentity : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldIdentity(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldIdentity Fieldmodule::createFieldIdentity(const Field& sourceField);

public:

	FieldIdentity() : Field(0)
	{	}

};

class FieldComponent : public Field
{
public:

	FieldComponent() : Field(0)
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldComponent(cmzn_field_component_id field_component_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_component_id))
	{	}

	inline cmzn_field_component_id getDerivedId()
	{
		return reinterpret_cast<cmzn_field_component_id>(id);
	}

	int getComponentIndex()
	{
		return cmzn_field_component_get_component_index(getDerivedId());
	}

	int setComponentIndex(int componentIndex)
	{
		return cmzn_field_component_set_component_index(getDerivedId(), componentIndex);
	}

	int getSourceComponentIndex(int index)
	{
		return cmzn_field_component_get_source_component_index(getDerivedId(), index);
	}

	int setSourceComponentIndex(int index, int sourceComponentIndex)
	{
		return cmzn_field_component_set_source_component_index(getDerivedId(), index, sourceComponentIndex);
	}

};

class FieldConcatenate : public Field
{
private:

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldConcatenate(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldConcatenate Fieldmodule::createFieldConcatenate(int fieldsCount, const Field *sourceFields);

public:

	FieldConcatenate() : Field(0)
	{	}

};

inline FieldIdentity Fieldmodule::createFieldIdentity(const Field& sourceField)
{
	return FieldIdentity(cmzn_fieldmodule_create_field_identity(id, sourceField.getId()));
}

inline FieldComponent Fieldmodule::createFieldComponent(const Field& sourceField, int sourceComponentIndex)
{
	return FieldComponent(reinterpret_cast<cmzn_field_component_id>(
		cmzn_fieldmodule_create_field_component(this->id, sourceField.getId(), sourceComponentIndex)));
}

inline FieldComponent Fieldmodule::createFieldComponent(const Field& sourceField,
	int sourceComponentIndexesCount, const int *sourceComponentIndexesIn)
{
	return FieldComponent(reinterpret_cast<cmzn_field_component_id>(
		cmzn_fieldmodule_create_field_component_multiple(this->id, sourceField.getId(),
			sourceComponentIndexesCount, sourceComponentIndexesIn)));
}

inline FieldComponent Field::castComponent()
{
	return FieldComponent(cmzn_field_cast_component(id));
}

inline FieldConcatenate Fieldmodule::createFieldConcatenate(int fieldsCount, const Field *sourceFields)
{
	cmzn_field_id concatenateField = 0;
	if (fieldsCount > 0)
	{
		cmzn_field_id *source_fields = new cmzn_field_id[fieldsCount];
		for (int i = 0; i < fieldsCount; i++)
		{
			source_fields[i] = sourceFields[i].getId();
		}
		concatenateField = cmzn_fieldmodule_create_field_concatenate(id, fieldsCount, source_fields);
		delete[] source_fields;
	}
	return FieldConcatenate(concatenateField);
}

}  // namespace Zinc
}

#endif
