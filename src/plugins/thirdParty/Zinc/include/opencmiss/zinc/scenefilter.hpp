/**
 * @file scenefilter.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SCENEFILTER_HPP__
#define CMZN_SCENEFILTER_HPP__

#include "opencmiss/zinc/scenefilter.h"
#include "opencmiss/zinc/context.hpp"
#include "opencmiss/zinc/graphics.hpp"
#include "opencmiss/zinc/region.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class ScenefilterOperator;

class Scenefilter
{
protected:
	cmzn_scenefilter_id id;

public:

	Scenefilter() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Scenefilter(cmzn_scenefilter_id in_filter_id) :
		id(in_filter_id)
	{  }

	Scenefilter(const Scenefilter& scenefilter) :
		id(cmzn_scenefilter_access(scenefilter.id))
	{  }

	Scenefilter& operator=(const Scenefilter& scenefilter)
	{
		cmzn_scenefilter_id temp_id = cmzn_scenefilter_access(scenefilter.id);
		if (0 != id)
		{
			cmzn_scenefilter_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Scenefilter()
	{
		if (0 != id)
		{
			cmzn_scenefilter_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_scenefilter_id getId() const
	{
		return id;
	}

	bool isManaged()
	{
		return cmzn_scenefilter_is_managed(id);
	}

	int setManaged(bool value)
	{
		return cmzn_scenefilter_set_managed(id, value);
	}

	bool evaluateGraphics(const Graphics& graphics)
	{
		return cmzn_scenefilter_evaluate_graphics(id, graphics.getId());
	}

	bool isInverse()
	{
		return cmzn_scenefilter_is_inverse(id);
	}

	int setInverse(bool value)
	{
		return cmzn_scenefilter_set_inverse(id, value);
	}

	char *getName()
	{
		return cmzn_scenefilter_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_scenefilter_set_name(id, name);
	}

	inline ScenefilterOperator castOperator();
};

inline bool operator==(const Scenefilter& a, const Scenefilter& b)
{
	return a.getId() == b.getId();
}

class ScenefilterOperator : public Scenefilter
{
public:

	ScenefilterOperator() : Scenefilter()
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit ScenefilterOperator(cmzn_scenefilter_operator_id operator_filter_id) :
		Scenefilter(reinterpret_cast<cmzn_scenefilter_id>(operator_filter_id))
	{ }

	cmzn_scenefilter_operator_id getDerivedId()
	{
		return reinterpret_cast<cmzn_scenefilter_operator_id>(id);
	}

	int appendOperand(const Scenefilter& operand)
	{
		return cmzn_scenefilter_operator_append_operand(getDerivedId(), operand.getId());
	}

	Scenefilter getFirstOperand()
	{
		return Scenefilter(cmzn_scenefilter_operator_get_first_operand(getDerivedId()));
	}

	Scenefilter getNextOperand(const Scenefilter& refOperand)
	{
		return Scenefilter(cmzn_scenefilter_operator_get_next_operand(getDerivedId(), refOperand.getId()));
	}

	bool isOperandActive(const Scenefilter& operand)
	{
		return cmzn_scenefilter_operator_is_operand_active(getDerivedId(), operand.getId());
	}

	int setOperandActive(const Scenefilter& operand, bool isActive)
	{
		return cmzn_scenefilter_operator_set_operand_active(getDerivedId(), operand.getId(), isActive);
	}

	int insertOperandBefore(const Scenefilter& operand, const Scenefilter& refOperand)
	{
		return cmzn_scenefilter_operator_insert_operand_before(getDerivedId(), operand.getId(), refOperand.getId());
	}

	int removeOperand(const Scenefilter& operand)
	{
		return cmzn_scenefilter_operator_remove_operand(getDerivedId(), operand.getId());
	}
};

inline ScenefilterOperator Scenefilter::castOperator()
{
	return ScenefilterOperator(cmzn_scenefilter_cast_operator(id));
}

class Scenefiltermodule
{
protected:
	cmzn_scenefiltermodule_id id;

public:

	Scenefiltermodule() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Scenefiltermodule(cmzn_scenefiltermodule_id in_filtermodule_id) :
		id(in_filtermodule_id)
	{  }

	Scenefiltermodule(const Scenefiltermodule& scenefiltermodule) :
		id(cmzn_scenefiltermodule_access(scenefiltermodule.id))
	{  }

	Scenefiltermodule& operator=(const Scenefiltermodule& scenefiltermodule)
	{
		cmzn_scenefiltermodule_id temp_id = cmzn_scenefiltermodule_access(
			scenefiltermodule.id);
		if (0 != id)
		{
			cmzn_scenefiltermodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Scenefiltermodule()
	{
		if (0 != id)
		{
			cmzn_scenefiltermodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_scenefiltermodule_id getId() const
	{
		return id;
	}

	Scenefilter createScenefilterVisibilityFlags()
	{
		return Scenefilter(cmzn_scenefiltermodule_create_scenefilter_visibility_flags(id));
	}

	Scenefilter createScenefilterFieldDomainType(Field::DomainType domainType)
	{
		return Scenefilter(cmzn_scenefiltermodule_create_scenefilter_field_domain_type(id,
			static_cast<cmzn_field_domain_type>(domainType)));
	}

	Scenefilter createScenefilterGraphicsName(const char *matchName)
	{
		return Scenefilter(cmzn_scenefiltermodule_create_scenefilter_graphics_name(id, matchName));
	}

	Scenefilter createScenefilterGraphicsType(Graphics::Type graphicsType)
	{
		return Scenefilter(cmzn_scenefiltermodule_create_scenefilter_graphics_type(id,
			static_cast<cmzn_graphics_type>(graphicsType)));
	}

	Scenefilter createScenefilterRegion(const Region& matchRegion)
	{
		return Scenefilter(cmzn_scenefiltermodule_create_scenefilter_region(
			id, matchRegion.getId()));
	}

	ScenefilterOperator createScenefilterOperatorAnd()
	{
		return ScenefilterOperator(reinterpret_cast<cmzn_scenefilter_operator_id>(
			cmzn_scenefiltermodule_create_scenefilter_operator_and(id)));
	}

	ScenefilterOperator createScenefilterOperatorOr()
	{
		return ScenefilterOperator(reinterpret_cast<cmzn_scenefilter_operator_id>(
			cmzn_scenefiltermodule_create_scenefilter_operator_or(id)));
	}

	Scenefilter findScenefilterByName(const char *name)
	{
		return Scenefilter(cmzn_scenefiltermodule_find_scenefilter_by_name(id, name));
	}

	int beginChange()
	{
		return cmzn_scenefiltermodule_begin_change(id);
	}

	int endChange()
	{
		return cmzn_scenefiltermodule_end_change(id);
	}

	Scenefilter getDefaultScenefilter()
	{
		return Scenefilter(cmzn_scenefiltermodule_get_default_scenefilter(id));
	}

	int setDefaultScenefilter(const Scenefilter& filter)
	{
		return cmzn_scenefiltermodule_set_default_scenefilter(id, filter.getId());
	}
};

inline Scenefiltermodule Context::getScenefiltermodule()
{
	return Scenefiltermodule(cmzn_context_get_scenefiltermodule(id));
}

}  // namespace Zinc
}

#endif
