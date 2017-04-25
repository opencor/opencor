/**
 * @file fieldcache.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDCACHE_HPP__
#define CMZN_FIELDCACHE_HPP__

#include "opencmiss/zinc/field.h"
#include "opencmiss/zinc/fieldcache.h"
#include "opencmiss/zinc/differentialoperator.hpp"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/node.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Fieldcache
{
protected:
	cmzn_fieldcache_id id;

public:

	Fieldcache() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Fieldcache(cmzn_fieldcache_id in_field_cache_id) :
		id(in_field_cache_id)
	{  }

	Fieldcache(const Fieldcache& fieldCache) :
		id(cmzn_fieldcache_access(fieldCache.id))
	{  }

	Fieldcache& operator=(const Fieldcache& fieldCache)
	{
		cmzn_fieldcache_id temp_id = cmzn_fieldcache_access(fieldCache.id);
		if (0 != id)
		{
			cmzn_fieldcache_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Fieldcache()
	{
		if (0 != id)
		{
			cmzn_fieldcache_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_fieldcache_id getId() const
	{
		return id;
	}

	int clearLocation()
	{
		return cmzn_fieldcache_clear_location(id);
	}

	int setElement(const Element& element)
	{
		return cmzn_fieldcache_set_element(id, element.getId());
	}

	int setMeshLocation(const Element& element, int coordinatesCount,
		const double *coordinatesIn)
	{
		return cmzn_fieldcache_set_mesh_location(id, element.getId(),
			coordinatesCount, coordinatesIn);
	}

	int setFieldReal(const Field& referenceField, int valuesCount,
		const double *valuesIn)
	{
		return cmzn_fieldcache_set_field_real(id,
			referenceField.getId(), valuesCount, valuesIn);
	}

	int setNode(const Node& node)
	{
		return cmzn_fieldcache_set_node(id, node.getId());
	}

	int setTime(double time)
	{
		return cmzn_fieldcache_set_time(id, time);
	}
};

inline Fieldcache Fieldmodule::createFieldcache()
{
	return Fieldcache(cmzn_fieldmodule_create_fieldcache(id));
}

inline int Field::assignMeshLocation(const Fieldcache& cache, const Element& element,
	int coordinatesCount, const double *coordinatesIn)
{
	return cmzn_field_assign_mesh_location(id, cache.getId(), element.getId(),
		coordinatesCount, coordinatesIn);
}

inline int Field::assignReal(const Fieldcache& cache,	int valuesCount, const double *valuesIn)
{
	return cmzn_field_assign_real(id, cache.getId(), valuesCount, valuesIn);
}

inline int Field::assignString(const Fieldcache& cache, const char *stringValue)
{
	return cmzn_field_assign_string(id, cache.getId(), stringValue);
}

inline Element Field::evaluateMeshLocation(const Fieldcache& cache, int coordinatesCount,
	double *coordinatesOut)
{
	return Element(cmzn_field_evaluate_mesh_location(id,
		cache.getId(), coordinatesCount, coordinatesOut));
}

inline int Field::evaluateReal(const Fieldcache& cache, int valuesCount, double *valuesOut)
{
	return cmzn_field_evaluate_real(id, cache.getId(), valuesCount, valuesOut);
}

inline char *Field::evaluateString(const Fieldcache& cache)
{
	return cmzn_field_evaluate_string(id, cache.getId());
}

inline int Field::evaluateDerivative(const Differentialoperator& differentialOperator,
	const Fieldcache& cache, int valuesCount, double *valuesOut)
{
	return cmzn_field_evaluate_derivative(id, differentialOperator.getId(),
		cache.getId(), valuesCount, valuesOut);
}

inline bool Field::isDefinedAtLocation(const Fieldcache& cache)
{
	return cmzn_field_is_defined_at_location(id, cache.getId());
}

}  // namespace Zinc
}

#endif /* CMZN_FIELDCACHE_HPP__ */
