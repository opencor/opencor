/**
 * @file fieldlogicaloperators.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDLOGICALOPERATORS_HPP__
#define CMZN_FIELDLOGICALOPERATORS_HPP__

#include "opencmiss/zinc/fieldlogicaloperators.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldAnd : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldAnd(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldAnd Fieldmodule::createFieldAnd(const Field& sourceField1,
		const Field& sourceField2);

public:

	FieldAnd() : Field(0)
	{	}

};

inline FieldAnd operator&&(const Field& operand1, const Field& operand2)
{
	return operand1.getFieldmodule().createFieldAnd(operand1, operand2);
}

class FieldEqualTo : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldEqualTo(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldEqualTo Fieldmodule::createFieldEqualTo(const Field& sourceField1,
		const Field& sourceField2);

public:

	FieldEqualTo() : Field(0)
	{	}

};

class FieldGreaterThan : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldGreaterThan(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldGreaterThan Fieldmodule::createFieldGreaterThan(const Field& sourceField1,
		const Field& sourceField2);

public:

	FieldGreaterThan() : Field(0)
	{	}

};

inline FieldGreaterThan operator>(const Field& operand1, const Field& operand2)
{
	return operand1.getFieldmodule().createFieldGreaterThan(operand1, operand2);
}

class FieldIsDefined : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldIsDefined(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldIsDefined Fieldmodule::createFieldIsDefined(const Field& sourceField);

public:
	FieldIsDefined() : Field(0)
	{	}
};

class FieldLessThan : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldLessThan(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldLessThan Fieldmodule::createFieldLessThan(const Field& sourceField1, const Field& sourceField2);

public:

	FieldLessThan() : Field(0)
	{	}

};

inline FieldLessThan operator<(const Field& operand1, const Field& operand2)
{
	return operand1.getFieldmodule().createFieldLessThan(operand1, operand2);
}

class FieldOr : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldOr(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldOr Fieldmodule::createFieldOr(const Field& sourceField1,
		const Field& sourceField2);

public:

	FieldOr() : Field(0)
	{	}

};

inline FieldOr operator||(const Field& operand1, const Field& operand2)
{
	return operand1.getFieldmodule().createFieldOr(operand1, operand2);
}

class FieldNot : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldNot(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldNot Fieldmodule::createFieldNot(const Field& sourceField);

public:

	FieldNot() : Field(0)
	{	}

};

inline FieldNot operator!(const Field& operand)
{
	return operand.getFieldmodule().createFieldNot(operand);
}

class FieldXor : public Field
{
private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldXor(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldXor Fieldmodule::createFieldXor(const Field& sourceField1,
		const Field& sourceField2);

public:

	FieldXor() : Field(0)
	{	}

};

inline FieldAnd Fieldmodule::createFieldAnd(const Field& sourceField1, const Field& sourceField2)
{
	return FieldAnd(cmzn_fieldmodule_create_field_and(id,
		sourceField1.getId(), sourceField2.getId()));
}

inline FieldEqualTo Fieldmodule::createFieldEqualTo(const Field& sourceField1, const Field& sourceField2)
{
	return FieldEqualTo(cmzn_fieldmodule_create_field_equal_to(id,
		sourceField1.getId(), sourceField2.getId()));
}

inline FieldGreaterThan Fieldmodule::createFieldGreaterThan(const Field& sourceField1, const Field& sourceField2)
{
	return FieldGreaterThan(cmzn_fieldmodule_create_field_greater_than(id,
		sourceField1.getId(), sourceField2.getId()));
}

inline FieldIsDefined Fieldmodule::createFieldIsDefined(const Field& sourceField)
{
	return FieldIsDefined(cmzn_fieldmodule_create_field_is_defined(id, sourceField.getId()));
}

inline FieldLessThan Fieldmodule::createFieldLessThan(const Field& sourceField1, const Field& sourceField2)
{
	return FieldLessThan(cmzn_fieldmodule_create_field_less_than(id,
		sourceField1.getId(), sourceField2.getId()));
}

inline FieldOr Fieldmodule::createFieldOr(const Field& sourceField1, const Field& sourceField2)
{
	return FieldOr(cmzn_fieldmodule_create_field_or(id,
		sourceField1.getId(), sourceField2.getId()));
}

inline FieldNot Fieldmodule::createFieldNot(const Field& sourceField)
{
	return FieldNot(cmzn_fieldmodule_create_field_not(id, sourceField.getId()));
}

inline FieldXor Fieldmodule::createFieldXor(const Field& sourceField1, const Field& sourceField2)
{
	return FieldXor(cmzn_fieldmodule_create_field_xor(id,
		sourceField1.getId(), sourceField2.getId()));
}

}  // namespace Zinc
}

#endif
