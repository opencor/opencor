/**
 * @file logger.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_LOGGER_HPP__
#define CMZN_LOGGER_HPP__

#include "opencmiss/zinc/logger.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Loggernotifier;

class Logger
{
protected:
	cmzn_logger_id id;

public:

	Logger() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Logger(cmzn_logger_id in_logger_id) :
		id(in_logger_id)
	{  }

	Logger(const Logger& logger) :
		id(cmzn_logger_access(logger.id))
	{  }

	Logger& operator=(const Logger& logger)
	{
		cmzn_logger_id temp_id = cmzn_logger_access(logger.id);
		if (0 != id)
		{
			cmzn_logger_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Logger()
	{
		if (0 != id)
		{
			cmzn_logger_destroy(&id);
		}
	}

	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_LOGGER_CHANGE_FLAG_NONE,
		CHANGE_FLAG_NEW_MESSAGE = CMZN_LOGGER_CHANGE_FLAG_NEW_MESSAGE,
		CHANGE_FLAG_FINAL = CMZN_LOGGER_CHANGE_FLAG_FINAL
	};

	enum MessageType
	{
		MESSAGE_TYPE_INVALID = CMZN_LOGGER_MESSAGE_TYPE_INVALID,
		MESSAGE_TYPE_ERROR = CMZN_LOGGER_MESSAGE_TYPE_ERROR,
		MESSAGE_TYPE_WARNING = CMZN_LOGGER_MESSAGE_TYPE_WARNING,
		MESSAGE_TYPE_INFORMATION = CMZN_LOGGER_MESSAGE_TYPE_INFORMATION
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 * @see Loggerevent::getChangeFlags
	 */
	typedef int ChangeFlags;

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_logger_id getId() const
	{
		return id;
	}

	int getNumberOfMessages()
	{
		return cmzn_logger_get_number_of_messages(id);
	}

	MessageType getMessageTypeAtIndex(int index)
	{
		return static_cast<MessageType>(cmzn_logger_get_message_type_at_index(
			id, index));
	}

	char *getMessageTextAtIndex(int index)
	{
		return cmzn_logger_get_message_text_at_index(id, index);
	}

	int setMaximumNumberOfMessages(int number)
	{
		return cmzn_logger_set_maximum_number_of_messages(id, number);
	}

	int removeAllMessages()
	{
		return cmzn_logger_remove_all_messages(id);
	}

	inline Loggernotifier createLoggernotifier();

};

inline bool operator==(const Logger& a, const Logger& b)
{
	return a.getId() == b.getId();
}

class Loggerevent
{
protected:
	cmzn_loggerevent_id id;

public:

	Loggerevent() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Loggerevent(cmzn_loggerevent_id in_logger_event_id) :
		id(in_logger_event_id)
	{  }

	Loggerevent(const Loggerevent& loggerEvent) :
		id(cmzn_loggerevent_access(loggerEvent.id))
	{  }

	Loggerevent& operator=(const Loggerevent& loggerEvent)
	{
		cmzn_loggerevent_id temp_id = cmzn_loggerevent_access(loggerEvent.id);
		if (0 != id)
			cmzn_loggerevent_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Loggerevent()
	{
		if (0 != id)
		{
			cmzn_loggerevent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_loggerevent_id getId() const
	{
		return id;
	}

	Logger::ChangeFlags getChangeFlags() const
	{
		return cmzn_loggerevent_get_change_flags(id);
	}

	Logger::MessageType getMessageType() const
	{
		return static_cast<Logger::MessageType>(cmzn_loggerevent_get_message_type(id));
	}

	char *getMessageText() const
	{
		return cmzn_loggerevent_get_message_text(id);
	}

	Logger getLogger() const
	{
		return Logger(cmzn_loggerevent_get_logger(id));
	}

};

/**
 * @brief Base class functor for logger notifier callbacks
 *
 * Base class functor for logger notifier callbacks:
 * - Derive from this class adding any user data required.
 * - Implement virtual operator()(const Loggerevent&) to handle callback.
 * @see Loggernotifier::setCallback()
 */
class Loggercallback
{
friend class Loggernotifier;
private:
	Loggercallback(const Loggercallback&); // not implemented
	Loggercallback& operator=(const Loggercallback&); // not implemented

	static void C_callback(cmzn_loggerevent_id loggerevent_id, void *callbackVoid)
	{
		Loggerevent loggerevent(cmzn_loggerevent_access(loggerevent_id));
		Loggercallback *callback = reinterpret_cast<Loggercallback *>(callbackVoid);
		(*callback)(loggerevent);
	}

	virtual void operator()(const Loggerevent &loggerevent) = 0;

protected:
	Loggercallback()
	{ }

public:
	virtual ~Loggercallback()
	{ }
};

class Loggernotifier
{
protected:
	cmzn_loggernotifier_id id;

public:

	Loggernotifier() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Loggernotifier(cmzn_loggernotifier_id in_loggernotifier_id) :
		id(in_loggernotifier_id)
	{  }

	Loggernotifier(const Loggernotifier& loggerNotifier) :
		id(cmzn_loggernotifier_access(loggerNotifier.id))
	{  }

	Loggernotifier& operator=(const Loggernotifier& loggerNotifier)
	{
		cmzn_loggernotifier_id temp_id = cmzn_loggernotifier_access(loggerNotifier.id);
		if (0 != id)
		{
			cmzn_loggernotifier_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Loggernotifier()
	{
		if (0 != id)
		{
			cmzn_loggernotifier_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_loggernotifier_id getId() const
	{
		return id;
	}

	int setCallback(Loggercallback& callback)
	{
		return cmzn_loggernotifier_set_callback(id, callback.C_callback, static_cast<void*>(&callback));
	}

	int clearCallback()
	{
		return cmzn_loggernotifier_clear_callback(id);
	}
};

inline Loggernotifier Logger::createLoggernotifier()
{
	return Loggernotifier(cmzn_logger_create_loggernotifier(id));
}

inline Logger Context::getLogger()
{
	return Logger(cmzn_context_get_logger(id));
}

}  // namespace Zinc
}

#endif
