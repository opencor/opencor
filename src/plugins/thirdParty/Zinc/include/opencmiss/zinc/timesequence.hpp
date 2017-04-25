/**
 * @file timesequence.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TIMESEQUENCE_HPP__
#define CMZN_TIMESEQUENCE_HPP__

#include "opencmiss/zinc/timesequence.h"

namespace OpenCMISS
{
namespace Zinc
{

class Timesequence
{
protected:
	cmzn_timesequence_id id;

public:

	Timesequence() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Timesequence(cmzn_timesequence_id in_time_sequence_id) :
		id(in_time_sequence_id)
	{  }

	Timesequence(const Timesequence& timeSequence) :
		id(cmzn_timesequence_access(timeSequence.id))
	{  }

	Timesequence& operator=(const Timesequence& timeSequence)
	{
		cmzn_timesequence_id temp_id = cmzn_timesequence_access(timeSequence.id);
		if (0 != id)
		{
			cmzn_timesequence_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Timesequence()
	{
		if (0 != id)
		{
			cmzn_timesequence_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_timesequence_id getId() const
	{
		return id;
	}

	int getNumberOfTimes()
	{
		return cmzn_timesequence_get_number_of_times(id);
	}

	double getTime(int timeIndex)
	{
		return cmzn_timesequence_get_time(id, timeIndex);
	}

	int setTime(int timeIndex, double time)
	{
		return cmzn_timesequence_set_time(id, timeIndex, time);
	}

};

inline bool operator==(const Timesequence& a, const Timesequence& b)
{
	return a.getId() == b.getId();
}

}  // namespace Zinc
}

#endif
