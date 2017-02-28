/**
 * @file fieldgroup.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDGROUP_HPP__
#define CMZN_FIELDGROUP_HPP__

#include "opencmiss/zinc/fieldgroup.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/fieldsubobjectgroup.hpp"
#include "opencmiss/zinc/node.hpp"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/region.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldGroup : public Field
{
public:

	FieldGroup() : Field(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldGroup(cmzn_field_group_id field_group_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_group_id))
	{	}

	enum SubelementHandlingMode
	{
		SUBELEMENT_HANDLING_MODE_INVALID = CMZN_FIELD_GROUP_SUBELEMENT_HANDLING_MODE_INVALID,
		SUBELEMENT_HANDLING_MODE_NONE = CMZN_FIELD_GROUP_SUBELEMENT_HANDLING_MODE_NONE,
		SUBELEMENT_HANDLING_MODE_FULL = CMZN_FIELD_GROUP_SUBELEMENT_HANDLING_MODE_FULL
	};

	inline cmzn_field_group_id getDerivedId()
	{
		return reinterpret_cast<cmzn_field_group_id>(id);
	}

	bool isEmpty()
	{
		return cmzn_field_group_is_empty(getDerivedId());
	}

	bool isEmptyLocal()
	{
		return cmzn_field_group_is_empty_local(getDerivedId());
	}

	int clear()
	{
		return cmzn_field_group_clear(getDerivedId());
	}

	int clearLocal()
	{
		return cmzn_field_group_clear_local(getDerivedId());
	}

	int removeEmptySubgroups()
	{
		return cmzn_field_group_remove_empty_subgroups(getDerivedId());
	}

	int addLocalRegion()
	{
		return cmzn_field_group_add_local_region(getDerivedId());
	}

	bool containsLocalRegion()
	{
		return cmzn_field_group_contains_local_region(getDerivedId());
	}

	int removeLocalRegion()
	{
		return cmzn_field_group_remove_local_region(getDerivedId());
	}

	int addRegion(const Region& region)
	{
		return cmzn_field_group_add_region(getDerivedId(),
			region.getId());
	}

	int removeRegion(const Region& region)
	{
		return cmzn_field_group_remove_region(getDerivedId(), region.getId());
	}

	bool containsRegion(const Region& region)
	{
		return cmzn_field_group_contains_region(getDerivedId(), region.getId());
	}

	SubelementHandlingMode getSubelementHandlingMode()
	{
		return static_cast<SubelementHandlingMode>(
			cmzn_field_group_get_subelement_handling_mode(getDerivedId()));
	}

	int setSubelementHandlingMode(SubelementHandlingMode mode)
	{
		return cmzn_field_group_set_subelement_handling_mode(getDerivedId(),
			static_cast<enum cmzn_field_group_subelement_handling_mode>(mode));
	}

	FieldGroup createSubregionFieldGroup(const Region& region)
	{
		return FieldGroup(cmzn_field_group_create_subregion_field_group(
			getDerivedId(), region.getId()));
	}

	FieldGroup getSubregionFieldGroup(const Region& region)
	{
		return FieldGroup(cmzn_field_group_get_subregion_field_group(
			getDerivedId(), region.getId()));
	}

	FieldNodeGroup createFieldNodeGroup(const Nodeset& nodeset)
	{
		return FieldNodeGroup(cmzn_field_group_create_field_node_group(
			getDerivedId(), nodeset.getId()));
	}

	FieldNodeGroup getFieldNodeGroup(const Nodeset& nodeset)
	{
		return FieldNodeGroup(cmzn_field_group_get_field_node_group(
			getDerivedId(), nodeset.getId()));
	}

	FieldElementGroup createFieldElementGroup(const Mesh& mesh)
	{
		return FieldElementGroup(cmzn_field_group_create_field_element_group(
			getDerivedId(), mesh.getId()));
	}

	FieldElementGroup getFieldElementGroup(const Mesh& mesh)
	{
		return FieldElementGroup(cmzn_field_group_get_field_element_group(
			getDerivedId(), mesh.getId()));
	}

	Field getSubobjectGroupFieldForDomainField(const Field& domainField)
	{
		return Field(cmzn_field_group_get_subobject_group_field_for_domain_field(
			getDerivedId(), domainField.getId()));
	}

	FieldGroup getFirstNonEmptySubregionFieldGroup()
	{
		return FieldGroup(cmzn_field_group_get_first_non_empty_subregion_field_group(
			getDerivedId()));
	}

};

inline FieldGroup Fieldmodule::createFieldGroup()
{
	return FieldGroup(reinterpret_cast<cmzn_field_group_id>(
		cmzn_fieldmodule_create_field_group(id)));
}

inline FieldGroup Field::castGroup()
{
	return FieldGroup(cmzn_field_cast_group(id));
}

}  // namespace Zinc
}

#endif
