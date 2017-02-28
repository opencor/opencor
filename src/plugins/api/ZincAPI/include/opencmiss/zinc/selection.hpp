/**
 * @file selection.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SELECTION_HPP__
#define CMZN_SELECTION_HPP__

#include "opencmiss/zinc/selection.h"

namespace OpenCMISS
{
namespace Zinc
{

class Selectionevent
{
protected:
	cmzn_selectionevent_id id;

public:

	Selectionevent() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Selectionevent(cmzn_selectionevent_id in_selection_event_id) :
		id(in_selection_event_id)
	{  }

	Selectionevent(const Selectionevent& selectionEvent) :
		id(cmzn_selectionevent_access(selectionEvent.id))
	{  }

	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_SELECTIONEVENT_CHANGE_FLAG_NONE,
		CHANGE_FLAG_ADD = CMZN_SELECTIONEVENT_CHANGE_FLAG_ADD,
		CHANGE_FLAG_REMOVE = CMZN_SELECTIONEVENT_CHANGE_FLAG_REMOVE,
		CHANGE_FLAG_FINAL = CMZN_SELECTIONEVENT_CHANGE_FLAG_FINAL
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 * @see getChangeFlags
	 */
	typedef int ChangeFlags;

	Selectionevent& operator=(const Selectionevent& selectionEvent)
	{
		cmzn_selectionevent_id temp_id = cmzn_selectionevent_access(selectionEvent.id);
		if (0 != id)
		{
			cmzn_selectionevent_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Selectionevent()
	{
		if (0 != id)
		{
			cmzn_selectionevent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_selectionevent_id getId() const
	{
		return id;
	}

	ChangeFlags getChangeFlags() const
	{
		return static_cast<ChangeFlag>(cmzn_selectionevent_get_change_flags(id));
	}

};

/**
 * @brief Base class functor for Selection notifier callbacks:
 *
 * Base class functor for Selection notifier callbacks:
 * - Derive from this class adding any user data required.
 * - Implement virtual operator()(const Selectionevent&) to handle callback.
 * @see Selectionnotifier::setCallback()
 */
class Selectioncallback
{
friend class Selectionnotifier;
private:
	Selectioncallback(const Selectioncallback&); // not implemented
	Selectioncallback& operator=(const Selectioncallback&); // not implemented

	static void C_callback(cmzn_selectionevent_id selectionevent_id, void *callbackVoid)
	{
		Selectionevent selectionevent(cmzn_selectionevent_access(selectionevent_id));
		Selectioncallback *callback = reinterpret_cast<Selectioncallback *>(callbackVoid);
		(*callback)(selectionevent);
	}

  virtual void operator()(const Selectionevent &selectionevent) = 0;

protected:
	Selectioncallback()
	{ }

public:
	virtual ~Selectioncallback()
	{ }
};

class Selectionnotifier
{
protected:
	cmzn_selectionnotifier_id id;

public:

	Selectionnotifier() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Selectionnotifier(cmzn_selectionnotifier_id in_selectionnotifier_id) :
		id(in_selectionnotifier_id)
	{  }

	Selectionnotifier(const Selectionnotifier& selectionNotifier) :
		id(cmzn_selectionnotifier_access(selectionNotifier.id))
	{  }

	Selectionnotifier& operator=(const Selectionnotifier& selectionNotifier)
	{
		cmzn_selectionnotifier_id temp_id = cmzn_selectionnotifier_access(selectionNotifier.id);
		if (0 != id)
		{
			cmzn_selectionnotifier_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Selectionnotifier()
	{
		if (0 != id)
		{
			cmzn_selectionnotifier_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_selectionnotifier_id getId() const
	{
		return id;
	}

	int setCallback(Selectioncallback& callback)
	{
		return cmzn_selectionnotifier_set_callback(
			id, callback.C_callback, static_cast<void*>(&callback));
	}

	int clearCallback()
	{
		return cmzn_selectionnotifier_clear_callback(id);
	}
};

}  // namespace Zinc
}

#endif
