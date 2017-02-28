/**
 * @file timekeeper.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TIMEKEEPER_HPP__
#define CMZN_TIMEKEEPER_HPP__

#include "opencmiss/zinc/timekeeper.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Timenotifier;
class TimenotifierRegular;

class Timekeeper
{
protected:
	cmzn_timekeeper_id id;

public:

	Timekeeper() : id(0)
	{   }

	// takes ownership of C handle, responsibility for destroying it
	explicit Timekeeper(cmzn_timekeeper_id in_timekeeper_id) :
		id(in_timekeeper_id)
	{  }

	Timekeeper(const Timekeeper& timeKeeper) :
		id(cmzn_timekeeper_access(timeKeeper.id))
	{  }

	Timekeeper& operator=(const Timekeeper& timeKeeper)
	{
		cmzn_timekeeper_id temp_id = cmzn_timekeeper_access(timeKeeper.id);
		if (0 != id)
		{
			cmzn_timekeeper_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Timekeeper()
	{
		if (0 != id)
		{
			cmzn_timekeeper_destroy(&id);
		}
	}

	enum PlayDirection
	{
		PLAY_DIRECTION_INVALID = CMZN_TIMEKEEPER_PLAY_DIRECTION_INVALID,
		PLAY_DIRECTION_FORWARD = CMZN_TIMEKEEPER_PLAY_DIRECTION_FORWARD,
		PLAY_DIRECTION_REVERSE = CMZN_TIMEKEEPER_PLAY_DIRECTION_REVERSE
	};

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_timekeeper_id getId() const
	{
		return id;
	}

	inline TimenotifierRegular createTimenotifierRegular(double updateFrequency, double timeOffset);

	double getMaximumTime()
	{
		return cmzn_timekeeper_get_maximum_time(id);
	}

	int setMaximumTime(double maximumTime)
	{
		return cmzn_timekeeper_set_maximum_time(id, maximumTime);
	}

	double getMinimumTime()
	{
		return cmzn_timekeeper_get_minimum_time(id);
	}

	int setMinimumTime(double minimumTime)
	{
		return cmzn_timekeeper_set_minimum_time(id, minimumTime);
	}

	double getTime()
	{
		return cmzn_timekeeper_get_time(id);
	}

	int setTime(double time)
	{
		return cmzn_timekeeper_set_time(id, time);
	}

	double getNextCallbackTime(enum PlayDirection playDirection)
	{
		return cmzn_timekeeper_get_next_callback_time(id,
			static_cast<cmzn_timekeeper_play_direction>(playDirection));
	}

};

class Timekeepermodule
{
protected:
	cmzn_timekeepermodule_id id;

public:

	Timekeepermodule() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Timekeepermodule(cmzn_timekeepermodule_id in_timekeepermodule_id) :
		id(in_timekeepermodule_id)
	{  }

	Timekeepermodule(const Timekeepermodule& timekeeperModule) :
		id(cmzn_timekeepermodule_access(timekeeperModule.id))
	{  }

	Timekeepermodule& operator=(const Timekeepermodule& timekeeperModule)
	{
		cmzn_timekeepermodule_id temp_id = cmzn_timekeepermodule_access(
			timekeeperModule.id);
		if (0 != id)
			cmzn_timekeepermodule_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Timekeepermodule()
	{
		if (0 != id)
			cmzn_timekeepermodule_destroy(&id);
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_timekeepermodule_id getId() const
	{
		return id;
	}

	Timekeeper getDefaultTimekeeper()
	{
		return Timekeeper(cmzn_timekeepermodule_get_default_timekeeper(id));
	}

 	int readDescription(const char *description)
 	{
 		return cmzn_timekeepermodule_read_description(this->id, description);
 	}

 	char *writeDescription()
 	{
 		return cmzn_timekeepermodule_write_description(this->id);
 	}

};

inline Timekeepermodule Context::getTimekeepermodule()
{
	return Timekeepermodule(cmzn_context_get_timekeepermodule(id));
}

}  // namespace Zinc
}

#endif
