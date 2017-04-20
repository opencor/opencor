/**
 * @file loggerid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_LOGGERID_H__
#define CMZN_LOGGERID_H__

#include "opencmiss/zinc/zincsharedobject.h"

/**
 * Enumeration of available logger message types.
 */
enum cmzn_logger_message_type
{
	CMZN_LOGGER_MESSAGE_TYPE_INVALID = 0,
	/*!< Unspecified message type */
	CMZN_LOGGER_MESSAGE_TYPE_ERROR = 1,
	/*!< Message for critical errors */
	CMZN_LOGGER_MESSAGE_TYPE_WARNING = 2,
	/*!< Message for warnings */
	CMZN_LOGGER_MESSAGE_TYPE_INFORMATION = 3
	/*!< Message for non-critical informations.  */
};

enum cmzn_logger_change_flags
{
	CMZN_LOGGER_CHANGE_FLAG_NONE = 0,
	/*!< logger(s) not changed */
	CMZN_LOGGER_CHANGE_FLAG_NEW_MESSAGE = 1,
	/*!< logger(s) not changed */
	CMZN_LOGGER_CHANGE_FLAG_FINAL = 32768
	/*!< final notification: owning logger has been destroyed */
};

/**
 * @brief Font object controlling attributes of rendering text.
 *
 * Font object incorporating typeface, size and other attributes for rendering
 * text in graphics.
 */
struct cmzn_logger;
typedef struct cmzn_logger *cmzn_logger_id;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Manages individual user notification of changes with a logger.
 *
 * Manages individual user notification of changes with a logger.
 */
struct cmzn_loggernotifier;
typedef struct cmzn_loggernotifier *cmzn_loggernotifier_id;

/**
 * @brief Information about changes to messages in the logger.
 *
 * Information about changes to messages in the logger,
 * sent with each callback from the logger notifier.
 */
struct cmzn_loggerevent;
typedef struct cmzn_loggerevent *cmzn_loggerevent_id;

/**
 * The type used for logger notifier callback. It is a pointer to a function which
 * takes the same arguments.
 *
 * @param cmzn_loggerevent_id  Handle to cmzn_logger_event.
 * @param User_data  any data user want to pass into the callback function.
 */

typedef void (*cmzn_loggernotifier_callback_function)(
	cmzn_loggerevent_id event, void *client_data);


#ifdef __cplusplus
}
#endif

#endif
