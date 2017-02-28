/**
 * @file sceneviewerinput.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SCENEVIEWERINPUT_HPP__
#define CMZN_SCENEVIEWERINPUT_HPP__

#include "opencmiss/zinc/sceneviewerinput.h"
#include "opencmiss/zinc/sceneviewer.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Sceneviewerinput
{
protected:
	cmzn_sceneviewerinput_id id;

public:
	enum ButtonType
	{
		BUTTON_TYPE_INVALID = CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_INVALID,
		BUTTON_TYPE_LEFT = CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_LEFT,
		BUTTON_TYPE_MIDDLE = CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_MIDDLE,
		BUTTON_TYPE_RIGHT = CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_RIGHT,
		BUTTON_TYPE_SCROLL_DOWN = CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_SCROLL_DOWN,
		BUTTON_TYPE_SCROLL_UP = CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_SCROLL_UP
	};

	enum EventType
	{
		EVENT_TYPE_INVALID = CMZN_SCENEVIEWERINPUT_EVENT_TYPE_INVALID,
		EVENT_TYPE_MOTION_NOTIFY = CMZN_SCENEVIEWERINPUT_EVENT_TYPE_MOTION_NOTIFY,
		EVENT_TYPE_BUTTON_PRESS = CMZN_SCENEVIEWERINPUT_EVENT_TYPE_BUTTON_PRESS,
		EVENT_TYPE_BUTTON_RELEASE = CMZN_SCENEVIEWERINPUT_EVENT_TYPE_BUTTON_RELEASE,
		EVENT_TYPE_KEY_PRESS = CMZN_SCENEVIEWERINPUT_EVENT_TYPE_KEY_PRESS,
		EVENT_TYPE_KEY_RELEASE = CMZN_SCENEVIEWERINPUT_EVENT_TYPE_KEY_RELEASE
	};

	enum ModifierFlag
	{
		MODIFIER_FLAG_NONE = CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_NONE,
		MODIFIER_FLAG_SHIFT = CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_SHIFT,
		MODIFIER_FLAG_CONTROL = CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_CONTROL,
		MODIFIER_FLAG_ALT = CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_ALT,
		MODIFIER_FLAG_BUTTON1 = CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_BUTTON1
	};

	/**
	 * Type for passing logical OR of #ModifierFlag
	 * @see setModifierFlags
	 */
	typedef int ModifierFlags;

	Sceneviewerinput() : id(0)
	{  }

	// takes ownership of C-style region reference
	explicit Sceneviewerinput(cmzn_sceneviewerinput_id in_sceneviewerinput_id) :
		id(in_sceneviewerinput_id)
	{  }

	Sceneviewerinput(const Sceneviewerinput& sceneviewerinput) :
		id(cmzn_sceneviewerinput_access(sceneviewerinput.id))
	{  }

	Sceneviewerinput& operator=(const Sceneviewerinput& sceneviewerinput)
	{
		cmzn_sceneviewerinput_id temp_id = cmzn_sceneviewerinput_access(sceneviewerinput.id);
		if (0 != id)
		{
			cmzn_sceneviewerinput_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Sceneviewerinput()
	{
		if (0 != id)
		{
			cmzn_sceneviewerinput_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_sceneviewerinput_id getId() const
	{
		return id;
	}

	int setPosition(int x, int y)
	{
		return cmzn_sceneviewerinput_set_position(id, x, y);
	}

	int setButtonType(ButtonType buttonType)
	{
		return cmzn_sceneviewerinput_set_button_type(id, static_cast<cmzn_sceneviewerinput_button_type>(buttonType));
	}

	int setEventType(EventType eventType)
	{
		return cmzn_sceneviewerinput_set_event_type(id, static_cast<cmzn_sceneviewerinput_event_type>(eventType));
	}

	int setModifierFlags(ModifierFlags modifierFlags)
	{
		return cmzn_sceneviewerinput_set_modifier_flags(id, modifierFlags);
	}

};

inline Sceneviewerinput Sceneviewer::createSceneviewerinput()
{
	return Sceneviewerinput(cmzn_sceneviewer_create_sceneviewerinput(id));
}

inline int Sceneviewer::processSceneviewerinput(const Sceneviewerinput& input)
{
	return cmzn_sceneviewer_process_sceneviewerinput(id, input.getId());
}

}  // namespace Zinc
}

#endif
