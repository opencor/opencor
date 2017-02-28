/**
 * @file tessellationid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_TESSELLATIONID_H__
#define CMZN_TESSELLATIONID_H__

/**
 * @brief Module managing all tessellation objects.
 *
 * Module managing all tessellation objects. It maintains separate default
 * tessellations for points and continuous graphics, the default points
 * tessellation having only 1 point in each direction.
 */
struct cmzn_tessellationmodule;
typedef struct cmzn_tessellationmodule * cmzn_tessellationmodule_id;

/**
 * @brief The tessellation controls the number of polygons or line segments.
 *
 * The tessellation controls the number of polygons or line segments used to
 * draw element surfaces and lines, and circular forms in graphics; the density
 * of point sampling and the piecewise linear approximation of elements
 * generally.
 */
struct cmzn_tessellation;
typedef struct cmzn_tessellation * cmzn_tessellation_id;

/**
 * @brief An iterator for looping through all the tessellations in a
 * tessellation module.
 *
 * An iterator for looping through all the tessellations in a tessellation
 * module.
 */
struct cmzn_tessellationiterator;
typedef struct cmzn_tessellationiterator * cmzn_tessellationiterator_id;


/**
 * Bit flags summarising changes to a tessellation or tessellations in a
 * tessellationmodule.
 */
enum cmzn_tessellation_change_flag
{
	CMZN_TESSELLATION_CHANGE_FLAG_NONE = 0,
		/*!< tessellation(s) not changed */
	CMZN_TESSELLATION_CHANGE_FLAG_ADD = 1,
		/*!< one or more tessellations added */
	CMZN_TESSELLATION_CHANGE_FLAG_REMOVE = 2,
		/*!< one or more tessellations removed */
	CMZN_TESSELLATION_CHANGE_FLAG_IDENTIFIER = 4,
		/*!< tessellation identifier changed */
	CMZN_TESSELLATION_CHANGE_FLAG_DEFINITION = 8,
		/*!< change to tessellation attributes other than identifier. */
	CMZN_TESSELLATION_CHANGE_FLAG_FULL_RESULT = 16,
		/*!< all resultant values of tessellation changed. */
	CMZN_TESSELLATION_CHANGE_FLAG_FINAL = 32768
		/*!< final notification: owning tessellation module i.e. tessellation module
		 * has been destroyed */
};

/**
 * Type for passing logical OR of #cmzn_tessellation_change_flag
 */
typedef int cmzn_tessellation_change_flags;

/**
 * @brief Manages individual user notification of changes with a tessellation module.
 *
 * Manages individual user notification of changes with a tessellation module.
 */
struct cmzn_tessellationmodulenotifier;
typedef struct cmzn_tessellationmodulenotifier *cmzn_tessellationmodulenotifier_id;

/**
 * @brief Information about changes to tessellations in the tessellation module.
 *
 * Information about changes to tessellations in the tessellation module,
 * sent with each callback from the tessellationmodule notifier.
 */
struct cmzn_tessellationmoduleevent;
typedef struct cmzn_tessellationmoduleevent *cmzn_tessellationmoduleevent_id;

typedef void (*cmzn_tessellationmodulenotifier_callback_function)(
	cmzn_tessellationmoduleevent_id event, void *client_data);

#endif
