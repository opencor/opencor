/**
 * @file light.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_LIGHT_HPP__
#define CMZN_LIGHT_HPP__

#include "opencmiss/zinc/light.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Light
{
protected:
	cmzn_light_id id;

public:

	Light() : id(0)
	{  }

	// takes ownership of C handle, responsibility for desLIGHTexplicit Light(cmzn_light_id in_light_id) :
	explicit Light(cmzn_light_id light_id) :
		id(light_id)
	{  }

	Light(const Light& light) :
		id(cmzn_light_access(light.id))
	{  }

	Light& operator=(const Light& light)
	{
		cmzn_light_id temp_id = cmzn_light_access(light.id);
		if (0 != id)
		{
			cmzn_light_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Light()
	{
		if (0 != id)
		{
			cmzn_light_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	enum Type
	{
		TYPE_INVALID = CMZN_LIGHT_TYPE_INVALID,
		TYPE_AMBIENT = CMZN_LIGHT_TYPE_AMBIENT,
		TYPE_DIRECTIONAL = CMZN_LIGHT_TYPE_DIRECTIONAL,
		TYPE_POINT = CMZN_LIGHT_TYPE_POINT,
		TYPE_SPOT = CMZN_LIGHT_TYPE_SPOT
	};

	cmzn_light_id getId() const
	{
		return id;
	}

	bool isManaged()
	{
		return cmzn_light_is_managed(id);
	}

	int setManaged(bool value)
	{
		return cmzn_light_set_managed(id, value);
	}

	enum Type getType()
	{
		return static_cast<Type>(cmzn_light_get_type(id));
	}

	int setType(enum Type type)
	{
		return cmzn_light_set_type(id, static_cast<cmzn_light_type>(type));
	}

	char *getName()
	{
		return cmzn_light_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_light_set_name(id, name);
	}

	double getConstantAttenuation()
	{
		return cmzn_light_get_constant_attenuation(id);
	}

	int setConstantAttenuation(double constantAttenuation)
	{
		return cmzn_light_set_constant_attenuation(id, constantAttenuation);
	}

	double getLinearAttenuation()
	{
		return cmzn_light_get_linear_attenuation(id);
	}

	int setLinearAttenuation(double linearAttenuation)
	{
		return cmzn_light_set_linear_attenuation(id, linearAttenuation);
	}

	double getQuadraticAttenuation()
	{
		return cmzn_light_get_quadratic_attenuation(id);
	}

	int setQuadraticAttenuation(double quadraticAttenuation)
	{
		return cmzn_light_set_quadratic_attenuation(id, quadraticAttenuation);
	}

	int getColourRGB(double *valuesOut3)
	{
		return cmzn_light_get_colour_rgb(id, valuesOut3);
	}

	int setColourRGB(const double *valuesIn3)
	{
		return cmzn_light_set_colour_rgb(id, valuesIn3);
	}

	int getDirection(double *valuesOut3)
	{
		return cmzn_light_get_direction(id, valuesOut3);
	}

	int setDirection(const double *valuesIn3)
	{
		return cmzn_light_set_direction(id, valuesIn3);
	}

	int getPosition(double *valuesOut3)
	{
		return cmzn_light_get_position(id, valuesOut3);
	}

	int setPosition(const double *valuesIn3)
	{
		return cmzn_light_set_position(id, valuesIn3);
	}

	double getSpotCutoff()
	{
		return cmzn_light_get_spot_cutoff(id);
	}

	int setSpotCutoff(double cutoff)
	{
		return cmzn_light_set_spot_cutoff(id, cutoff);
	}

	double getSpotExponent()
	{
		return cmzn_light_get_spot_exponent(id);
	}

	int setSpotExponent(double exponent)
	{
		return cmzn_light_set_spot_exponent(id, exponent);
	}

};

inline bool operator==(const Light& a, const Light& b)
{
	return a.getId() == b.getId();
}


class Lightiterator
{
private:

	cmzn_lightiterator_id id;

public:

	Lightiterator() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Lightiterator(cmzn_lightiterator_id iterator_id) :
		id(iterator_id)
	{ }

	Lightiterator(const Lightiterator& lightiterator) :
		id(cmzn_lightiterator_access(lightiterator.id))
	{ }

	Lightiterator& operator=(const Lightiterator& lightiterator)
	{
		cmzn_lightiterator_id temp_id = cmzn_lightiterator_access(lightiterator.id);
		if (0 != id)
		{
			cmzn_lightiterator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Lightiterator()
	{
		if (0 != id)
		{
			cmzn_lightiterator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Light next()
	{
		return Light(cmzn_lightiterator_next(id));
	}
};

class Lightmodule
{
protected:
	cmzn_lightmodule_id id;

public:

	Lightmodule() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Lightmodule(cmzn_lightmodule_id in_lightmodule_id) :
		id(in_lightmodule_id)
	{  }

	Lightmodule(const Lightmodule& lightModule) :
		id(cmzn_lightmodule_access(lightModule.id))
	{  }

	Lightmodule& operator=(const Lightmodule& lightModule)
	{
		cmzn_lightmodule_id temp_id = cmzn_lightmodule_access(
			lightModule.id);
		if (0 != id)
		{
			cmzn_lightmodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Lightmodule()
	{
		if (0 != id)
		{
			cmzn_lightmodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_lightmodule_id getId() const
	{
		return id;
	}

	Light createLight()
	{
		return Light(cmzn_lightmodule_create_light(id));
	}

	Lightiterator createLightiterator()
	{
		return Lightiterator(cmzn_lightmodule_create_lightiterator(id));
	}

	Light findLightByName(const char *name)
	{
		return Light(cmzn_lightmodule_find_light_by_name(id, name));
	}

	int beginChange()
	{
		return cmzn_lightmodule_begin_change(id);
	}

	int endChange()
	{
		return cmzn_lightmodule_end_change(id);
	}

	Light getDefaultLight()
	{
		return Light(cmzn_lightmodule_get_default_light(id));
	}

	int setDefaultLight(const Light& light)
	{
		return cmzn_lightmodule_set_default_light(id, light.getId());
	}

	Light getDefaultAmbientLight()
	{
		return Light(cmzn_lightmodule_get_default_ambient_light(id));
	}

	int setDefaultAmbientLight(const Light& light)
	{
		return cmzn_lightmodule_set_default_ambient_light(id, light.getId());
	}
};

inline Lightmodule Context::getLightmodule()
{
	return Lightmodule(cmzn_context_get_lightmodule(id));
}

}  // namespace Zinc

}
#endif
