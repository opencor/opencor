/**
 * @file fieldsmoothing.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDSMOOTHING_HPP__
#define CMZN_FIELDSMOOTHING_HPP__

#include "opencmiss/zinc/fieldsmoothing.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Fieldsmoothing
{
protected:
	cmzn_fieldsmoothing_id id;

public:

	Fieldsmoothing() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Fieldsmoothing(cmzn_fieldsmoothing_id in_fieldsmoothing_id) :
		id(in_fieldsmoothing_id)
	{  }

	Fieldsmoothing(const Fieldsmoothing& fieldsmoothing) :
		id(cmzn_fieldsmoothing_access(fieldsmoothing.id))
	{  }

	Fieldsmoothing& operator=(const Fieldsmoothing& fieldsmoothing)
	{
		cmzn_fieldsmoothing_id temp_id = cmzn_fieldsmoothing_access(fieldsmoothing.id);
		if (0 != id)
			cmzn_fieldsmoothing_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Fieldsmoothing()
	{
		if (0 != id)
			cmzn_fieldsmoothing_destroy(&id);
	}

	enum Algorithm
	{
		ALGORITHM_INVALID = CMZN_FIELDSMOOTHING_ALGORITHM_INVALID,
		ALGORITHM_AVERAGE_DELTA_DERIVATIVES_UNSCALED = CMZN_FIELDSMOOTHING_ALGORITHM_AVERAGE_DELTA_DERIVATIVES_UNSCALED
	};

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_fieldsmoothing_id getId() const
	{
		return id;
	}

	int setAlgorithm(Algorithm algorithm)
	{
		return cmzn_fieldsmoothing_set_algorithm(getId(),
			static_cast<cmzn_fieldsmoothing_algorithm>(algorithm));
	}

	int setTime(double time)
	{
		return cmzn_fieldsmoothing_set_time(id, time);
	}
};

inline Fieldsmoothing Fieldmodule::createFieldsmoothing()
{
	return Fieldsmoothing(cmzn_fieldmodule_create_fieldsmoothing(id));
}

inline int Field::smooth(const Fieldsmoothing& fieldsmoothing)
{
	return cmzn_field_smooth(this->getId(), fieldsmoothing.getId());
}

} // namespace Zinc
} // namespace OpenCMISS

#endif /* CMZN_Fieldsmoothing_HPP__ */
