/**
 * @file sceneviewerinputid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENEVIEWERINPUTID_H__
#define CMZN_SCENEVIEWERINPUTID_H__

/**
 * @brief  Describes a mouse event for processing by the sceneviewer.
 *
 * Describes a mouse event for processing by the sceneviewer to produce viewer
 * transformations: tumble, pan, zoom.
 * @see cmzn_sceneviewer_process_sceneviewerinput
 */
struct cmzn_sceneviewerinput;
typedef struct cmzn_sceneviewerinput *cmzn_sceneviewerinput_id;

/**
 * The type of scene viewer input button. It describes the type
 * of mouse buttons interaction to be processed on scene viewer.
 */
enum cmzn_sceneviewerinput_button_type
{
	CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_INVALID = -1,
	/*!< Unspecified scene viewer input button type. */
	CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_LEFT,
	/*!< Indicates left mouse click event. */
	CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_MIDDLE,
	/*!< Indicates middle mouse click event. */
	CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_RIGHT,
	/*!< Indicates right mouse click event. */
	CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_SCROLL_DOWN,
	/*!< Indicates scroll down event. */
	CMZN_SCENEVIEWERINPUT_BUTTON_TYPE_SCROLL_UP
	/*!< Indicates scroll up event. */
};

/**
 * Specifies the scene viewer input event type.
 */
enum cmzn_sceneviewerinput_event_type
{
	CMZN_SCENEVIEWERINPUT_EVENT_TYPE_INVALID = 0,
	/*!< Unspecified scene viewer input event type. */
	CMZN_SCENEVIEWERINPUT_EVENT_TYPE_MOTION_NOTIFY,
	/*!< indicates the cursor has moved. */
	CMZN_SCENEVIEWERINPUT_EVENT_TYPE_BUTTON_PRESS,
	/*!< indicates a button on the mouse has been pressed. */
	CMZN_SCENEVIEWERINPUT_EVENT_TYPE_BUTTON_RELEASE,
	/*!< indicates a button on the mouse has been released. */
	CMZN_SCENEVIEWERINPUT_EVENT_TYPE_KEY_PRESS,
	/*!< indicates a button on the keyboard has been pressed. */
	CMZN_SCENEVIEWERINPUT_EVENT_TYPE_KEY_RELEASE
	/*!< indicates a button on the mouse has been released. */
};

/**
 * Specifies the scene viewer input modifier flags.
 */
enum cmzn_sceneviewerinput_modifier_flag
{
	CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_NONE = 0,
	/*!< Unspecified scene viewer modifier type. */
	CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_SHIFT = 1,
	/*!< indicates shift on keyboard has been pressed. */
	CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_CONTROL = 2,
	/*!< indicates control on keyboard has been pressed. */
	CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_ALT = 4,
	/*!< indicates alt on keyboard has been pressed. */
	CMZN_SCENEVIEWERINPUT_MODIFIER_FLAG_BUTTON1 = 8
	/*!< indicates button1 on keyboard has been pressed. */
};

/**
 * Type for passing logical OR of #cmzn_sceneviewerinput_modifier_flag
 */
typedef int cmzn_sceneviewerinput_modifier_flags;

#endif
