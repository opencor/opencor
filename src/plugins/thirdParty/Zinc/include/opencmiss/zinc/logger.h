/**
 * @file logger.h
 *
 * Public interface to the logger object, it provides APIs to access messages
 * emitted from the library.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_LOGGER_H__
#define CMZN_LOGGER_H__

#include "types/loggerid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Return the number of messages currently in the logger.
 *
 * @param logger  Handle to the logger.
 * @return  number of message, or 0 on failure.
 */
ZINC_API int cmzn_logger_get_number_of_messages(cmzn_logger_id logger);

/**
 * Return text of the message at index, oldest message index being 1 and newest
 * message index being number_of_messages;
 *
 * @param logger  Handle to the logger.
 * @param index  index of the message to enquire.
 * @return  text of message at index, otherwise 0.
 */
ZINC_API char *cmzn_logger_get_message_text_at_index(
	cmzn_logger_id logger, int index);

/**
 * Return type of the message at index, oldest message index being 1 and newest
 * message index being number_of_messages;
 *
 * @see cmzn_logger_message_type
 *
 * @param logger  Handle to the logger.
 * @param index  index of the message to enquire.
 * @return  type of message at index, otherwise.
 */
ZINC_API enum cmzn_logger_message_type cmzn_logger_get_message_type_at_index(
	cmzn_logger_id logger, int index);

/**
 * Set the maximum number of messages stored in the logger. The default number
 * is 500.
 *
 * @param logger  Handle to the logger.
 * @param number  maximum number of messages to set to.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_logger_set_maximum_number_of_messages(
	cmzn_logger_id logger, int number);

/**
 * Remove all messages in logger. This will clear all messages.
 *
 * @param logger  Handle to the logger.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_logger_remove_all_messages(cmzn_logger_id logger);

/**
 * Returns a new handle to the logger with reference count incremented.
 *
 * @param logger  The logger to obtain a new handle to.
 * @return  New handle to logger, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_logger_id cmzn_logger_access(cmzn_logger_id logger);

/**
 * Destroys the handle to the logger and resets it to NULL.
 *
 * @param logger_address  Address of handle to graphics to be destroyed.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_logger_destroy(cmzn_logger_id *logger_address);

/**
 * Create a notifier for getting callbacks for changes to the messages in the
 * logger.
 *
 * @param logger  Handle to the logger to get notifications for.
 * @return  Handle to new logger notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_loggernotifier_id cmzn_logger_create_loggernotifier(
	cmzn_logger_id logger);

/**
 * Returns a new handle to the logger notifier with reference count
 * incremented.
 *
 * @param notifier  The logger notifier to obtain a new handle to.
 * @return  New handle to logger notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_loggernotifier_id cmzn_loggernotifier_access(
	cmzn_loggernotifier_id notifier);

/**
 * Destroys handle to the logger notifier and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param notifier_address  Address of logger notifier handle to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_loggernotifier_destroy(cmzn_loggernotifier_id *notifier_address);

/**
 * Stop and clear logger callback. This will stop the callback and also
 * remove the callback function from the logger notifier.
 *
 * @param notifier  Handle to the logger notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_loggernotifier_clear_callback(
	cmzn_loggernotifier_id notifier);

/**
 * Assign the callback function and user data for the logger notifier.
 * This function also starts the callback.
 *
 * @see cmzn_loggernotifier_callback_function
 * @param notifier  Handle to the logger notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User must ensure this
 * object's lifetime exceeds the duration for which callbacks are active.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_loggernotifier_set_callback(cmzn_loggernotifier_id notifier,
	cmzn_loggernotifier_callback_function function, void *user_data_in);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_loggernotifier_set_callback
 *
 * @see cmzn_loggernotifier_set_callback
 * @param notifier  Handle to the logger notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_loggernotifier_get_callback_user_data(
 cmzn_loggernotifier_id notifier);

/**
 * Returns a new handle to the logger event with reference count incremented.
 *
 * @param event  The logger event to obtain a new handle to.
 * @return  New handle to logger event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_loggerevent_id cmzn_loggerevent_access(
	cmzn_loggerevent_id event);

/**
 * Destroys this handle to the logger event and sets it to NULL.
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param event_address  Address of logger event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_loggerevent_destroy(cmzn_loggerevent_id *event_address);

/**
 * Get logical OR of flags indicating how the logger has changed.
 * @see cmzn_logger_change_flag
 *
 * @param event  Handle to the logger module event to query.
 * @param logger  The logger to query about.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_logger_change_flag values. Returns
 * CMZN_LOGGER_CHANGE_FLAG_NONE in case of invalid arguments.
 */
ZINC_API cmzn_logger_change_flags cmzn_loggerevent_get_change_flags(
	cmzn_loggerevent_id event);

/**
 * Get the type of the message associated with this event.
 * @see cmzn_logger_message_type
 *
 * @param event  Handle to the logger event.
 * @return  The type of the message associated with this event.
 */
ZINC_API cmzn_logger_message_type cmzn_loggerevent_get_message_type(
	cmzn_loggerevent_id event);

/**
 * Get the message assoicated with this event. This may not neccessarily be
 * the last message on the logger.
 *
 * @param event  Handle to the logger event.
 * @return  The message associated with this event.
 */
ZINC_API char *cmzn_loggerevent_get_message_text(cmzn_loggerevent_id event);

/**
 * Get the logger which creates this event.
 *
 * @param event  Handle to the logger event.
 * @return valid logger associated with this event, otherwise 0.
 */
ZINC_API cmzn_logger_id cmzn_loggerevent_get_logger(cmzn_loggerevent_id event);

#ifdef __cplusplus
}
#endif

#endif
