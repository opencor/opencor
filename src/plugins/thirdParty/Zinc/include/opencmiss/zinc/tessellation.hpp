/**
 * @file tessellation.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_TESSELLATION_HPP__
#define CMZN_TESSELLATION_HPP__

#include "opencmiss/zinc/tessellation.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Tessellationmodulenotifier;

class Tessellation
{
protected:
	cmzn_tessellation_id id;

public:

	Tessellation() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Tessellation(cmzn_tessellation_id in_tessellation_id) :
		id(in_tessellation_id)
	{  }

	Tessellation(const Tessellation& tessellation) :
		id(cmzn_tessellation_access(tessellation.id))
	{  }

	Tessellation& operator=(const Tessellation& tessellation)
	{
		cmzn_tessellation_id temp_id = cmzn_tessellation_access(tessellation.id);
		if (0 != id)
		{
			cmzn_tessellation_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Tessellation()
	{
		if (0 != id)
		{
			cmzn_tessellation_destroy(&id);
		}
	}

	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_TESSELLATION_CHANGE_FLAG_NONE,
		CHANGE_FLAG_ADD = CMZN_TESSELLATION_CHANGE_FLAG_ADD,
		CHANGE_FLAG_REMOVE = CMZN_TESSELLATION_CHANGE_FLAG_REMOVE,
		CHANGE_FLAG_IDENTIFIER = CMZN_TESSELLATION_CHANGE_FLAG_IDENTIFIER,
		CHANGE_FLAG_DEFINITION = CMZN_TESSELLATION_CHANGE_FLAG_DEFINITION,
		CHANGE_FLAG_FULL_RESULT = CMZN_TESSELLATION_CHANGE_FLAG_FULL_RESULT,
		CHANGE_FLAG_FINAL = CMZN_TESSELLATION_CHANGE_FLAG_FINAL
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 * @see Tessellationmoduleevent::getTessellationChangeFlags
	 */
	typedef int ChangeFlags;

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_tessellation_id getId() const
	{
		return id;
	}

	bool isManaged()
	{
		return cmzn_tessellation_is_managed(id);
	}

	int setManaged(bool value)
	{
		return cmzn_tessellation_set_managed(id, value);
	}

	int getCircleDivisions()
	{
		return cmzn_tessellation_get_circle_divisions(id);
	}

	int setCircleDivisions(int circleDivisions)
	{
		return cmzn_tessellation_set_circle_divisions(id, circleDivisions);
	}

	char *getName()
	{
		return cmzn_tessellation_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_tessellation_set_name(id, name);
	}

	int getMinimumDivisions(int valuesCount, int *valuesOut)
	{
		return cmzn_tessellation_get_minimum_divisions(id, valuesCount, valuesOut);
	}

	int setMinimumDivisions(int valuesCount, const int *valuesIn)
	{
		return cmzn_tessellation_set_minimum_divisions(id, valuesCount, valuesIn);
	}

	int getRefinementFactors(int valuesCount, int *valuesOut)
	{
		return cmzn_tessellation_get_refinement_factors(id, valuesCount, valuesOut);
	}

	int setRefinementFactors(int valuesCount, const int *valuesIn)
	{
		return cmzn_tessellation_set_refinement_factors(id, valuesCount, valuesIn);
	}

};

inline bool operator==(const Tessellation& a, const Tessellation& b)
{
	return a.getId() == b.getId();
}


class Tessellationiterator
{
private:

	cmzn_tessellationiterator_id id;

public:

	Tessellationiterator() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Tessellationiterator(cmzn_tessellationiterator_id iterator_id) :
		id(iterator_id)
	{ }

	Tessellationiterator(const Tessellationiterator& tessellationiterator) :
		id(cmzn_tessellationiterator_access(tessellationiterator.id))
	{ }

	Tessellationiterator& operator=(const Tessellationiterator& tessellationiterator)
	{
		cmzn_tessellationiterator_id temp_id = cmzn_tessellationiterator_access(tessellationiterator.id);
		if (0 != id)
		{
			cmzn_tessellationiterator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Tessellationiterator()
	{
		if (0 != id)
		{
			cmzn_tessellationiterator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Tessellation next()
	{
		return Tessellation(cmzn_tessellationiterator_next(id));
	}
};

class Tessellationmodule
{
protected:
	cmzn_tessellationmodule_id id;

public:

	Tessellationmodule() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Tessellationmodule(cmzn_tessellationmodule_id in_tessellationmodule_id) :
		id(in_tessellationmodule_id)
	{  }

	Tessellationmodule(const Tessellationmodule& tessellationModule) :
		id(cmzn_tessellationmodule_access(tessellationModule.id))
	{  }

	Tessellationmodule& operator=(const Tessellationmodule& tessellationModule)
	{
		cmzn_tessellationmodule_id temp_id = cmzn_tessellationmodule_access(
			tessellationModule.id);
		if (0 != id)
		{
			cmzn_tessellationmodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Tessellationmodule()
	{
		if (0 != id)
		{
			cmzn_tessellationmodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_tessellationmodule_id getId() const
	{
		return id;
	}

	Tessellation createTessellation()
	{
		return Tessellation(cmzn_tessellationmodule_create_tessellation(id));
	}

	Tessellationiterator createTessellationiterator()
	{
		return Tessellationiterator(cmzn_tessellationmodule_create_tessellationiterator(id));
	}

	Tessellation findTessellationByName(const char *name)
	{
		return Tessellation(cmzn_tessellationmodule_find_tessellation_by_name(id, name));
	}

	int beginChange()
	{
		return cmzn_tessellationmodule_begin_change(id);
	}

	int endChange()
	{
		return cmzn_tessellationmodule_end_change(id);
	}

	Tessellation getDefaultTessellation()
	{
		return Tessellation(cmzn_tessellationmodule_get_default_tessellation(id));
	}

	int setDefaultTessellation(const Tessellation& tessellation)
	{
		return cmzn_tessellationmodule_set_default_tessellation(id, tessellation.getId());
	}

	Tessellation getDefaultPointsTessellation()
	{
		return Tessellation(cmzn_tessellationmodule_get_default_points_tessellation(id));
	}

	int setDefaultPointsTessellation(const Tessellation& tessellation)
	{
		return cmzn_tessellationmodule_set_default_points_tessellation(id, tessellation.getId());
	}

 	int readDescription(const char *description)
 	{
 		return cmzn_tessellationmodule_read_description(this->id, description);
 	}

 	char *writeDescription()
 	{
 		return cmzn_tessellationmodule_write_description(this->id);
 	}

 	inline Tessellationmodulenotifier createTessellationmodulenotifier();
};


class Tessellationmoduleevent
{
protected:
	cmzn_tessellationmoduleevent_id id;

public:

	Tessellationmoduleevent() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Tessellationmoduleevent(cmzn_tessellationmoduleevent_id in_tessellationmodule_event_id) :
		id(in_tessellationmodule_event_id)
	{  }

	Tessellationmoduleevent(const Tessellationmoduleevent& tessellationmoduleEvent) :
		id(cmzn_tessellationmoduleevent_access(tessellationmoduleEvent.id))
	{  }

	Tessellationmoduleevent& operator=(const Tessellationmoduleevent& tessellationmoduleEvent)
	{
		cmzn_tessellationmoduleevent_id temp_id = cmzn_tessellationmoduleevent_access(tessellationmoduleEvent.id);
		if (0 != id)
			cmzn_tessellationmoduleevent_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Tessellationmoduleevent()
	{
		if (0 != id)
		{
			cmzn_tessellationmoduleevent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_tessellationmoduleevent_id getId() const
	{
		return id;
	}

	Tessellation::ChangeFlags getTessellationChangeFlags(const Tessellation& tessellation) const
	{
		return cmzn_tessellationmoduleevent_get_tessellation_change_flags(id, tessellation.getId());
	}

	Tessellation::ChangeFlags getSummaryTessellationChangeFlags() const
	{
		return cmzn_tessellationmoduleevent_get_summary_tessellation_change_flags(id);
	}

};

/**
 * @brief Base class functor for tessellation module notifier callbacks
 *
 * Base class functor for tessellation module notifier callbacks:
 * - Derive from this class adding any user data required.
 * - Implement virtual operator()(const Tessellationmoduleevent&) to handle callback.
 * @see Tessellationmodulenotifier::setCallback()
 */
class Tessellationmodulecallback
{
friend class Tessellationmodulenotifier;
private:
	Tessellationmodulecallback(const Tessellationmodulecallback&); // not implemented
	Tessellationmodulecallback& operator=(const Tessellationmodulecallback&); // not implemented

	static void C_callback(cmzn_tessellationmoduleevent_id tessellationmoduleevent_id, void *callbackVoid)
	{
		Tessellationmoduleevent tessellationmoduleevent(cmzn_tessellationmoduleevent_access(tessellationmoduleevent_id));
		Tessellationmodulecallback *callback = reinterpret_cast<Tessellationmodulecallback *>(callbackVoid);
		(*callback)(tessellationmoduleevent);
	}

  virtual void operator()(const Tessellationmoduleevent &tessellationmoduleevent) = 0;

protected:
	Tessellationmodulecallback()
	{ }

public:
	virtual ~Tessellationmodulecallback()
	{ }
};

class Tessellationmodulenotifier
{
protected:
	cmzn_tessellationmodulenotifier_id id;

public:

	Tessellationmodulenotifier() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Tessellationmodulenotifier(cmzn_tessellationmodulenotifier_id in_tessellationmodulenotifier_id) :
		id(in_tessellationmodulenotifier_id)
	{  }

	Tessellationmodulenotifier(const Tessellationmodulenotifier& tessellationmoduleNotifier) :
		id(cmzn_tessellationmodulenotifier_access(tessellationmoduleNotifier.id))
	{  }

	Tessellationmodulenotifier& operator=(const Tessellationmodulenotifier& tessellationmoduleNotifier)
	{
		cmzn_tessellationmodulenotifier_id temp_id = cmzn_tessellationmodulenotifier_access(tessellationmoduleNotifier.id);
		if (0 != id)
		{
			cmzn_tessellationmodulenotifier_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Tessellationmodulenotifier()
	{
		if (0 != id)
		{
			cmzn_tessellationmodulenotifier_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_tessellationmodulenotifier_id getId() const
	{
		return id;
	}

	int setCallback(Tessellationmodulecallback& callback)
	{
		return cmzn_tessellationmodulenotifier_set_callback(id, callback.C_callback, static_cast<void*>(&callback));
	}

	int clearCallback()
	{
		return cmzn_tessellationmodulenotifier_clear_callback(id);
	}
};

inline Tessellationmodulenotifier Tessellationmodule::createTessellationmodulenotifier()
{
	return Tessellationmodulenotifier(cmzn_tessellationmodule_create_tessellationmodulenotifier(id));
}


inline Tessellationmodule Context::getTessellationmodule()
{
	return Tessellationmodule(cmzn_context_get_tessellationmodule(id));
}

}  // namespace Zinc
}

#endif
