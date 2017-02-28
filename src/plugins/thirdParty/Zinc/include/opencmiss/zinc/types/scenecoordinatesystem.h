/**
 * @file scenecoordinatesystem.h
 *
 * Enumerated type for identifying scene and window coordinate systems.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENECOORDINATESYSTEM_H__
#define CMZN_SCENECOORDINATESYSTEM_H__


#include "opencmiss/zinc/zincsharedobject.h"

/**
 * Enumerated type for identifying scene and window coordinate systems.
 * Graphics are drawn in one of these coordinate systems.
 */
enum cmzn_scenecoordinatesystem
{
	CMZN_SCENECOORDINATESYSTEM_INVALID = 0,
	/*!< Unspecified coordinate system. */
	CMZN_SCENECOORDINATESYSTEM_LOCAL = 1,
	/*!< The local coordinate system of a scene, subject to its own
	 * transformation matrix and those of all parent scenes up to the root
	 * region of the scene in use, which are world coordinates.*/
	CMZN_SCENECOORDINATESYSTEM_WORLD = 2,
	/*!< The world coordinate system which scene viewer viewing parameters
	 * are specified in, and which scene transformations (giving local
	 * coordinates) are ultimately relative to.*/
	CMZN_SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FILL = 3,
	/*!< Distorted normalised window coordinate system which varies from
	 * -1 to +1 from left to right, bottom to top, and far to near of window.
	 * If window is non-square, graphics in this space appear stretched. */
	CMZN_SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_CENTRE = 4,
	/*!< Undistorted normalised window coordinate system which varies from
	 * -1 to +1 from far to near, and from -1 to +1 from left-to-right and
	 * bottom-to-top in largest square that fits in centre of window.*/
	CMZN_SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_LEFT = 5,
	/*!< Undistorted normalised window coordinate system which varies from
	 * -1 to +1 from far to near, and from -1 to +1 from left-to-right and
	 * bottom-to-top in largest square that fits in left of window.*/
	CMZN_SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_RIGHT = 6,
	/*!< Undistorted normalised window coordinate system which varies from
	 * -1 to +1 from far to near, and from -1 to +1 from left-to-right and
	 * bottom-to-top in largest square that fits in right of window.*/
	CMZN_SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_BOTTOM = 7,
	/*!< Undistorted normalised window coordinate system which varies from
	 * -1 to +1 from far to near, and from -1 to +1 from left-to-right and
	 * bottom-to-top in largest square that fits in bottom of window.*/
	CMZN_SCENECOORDINATESYSTEM_NORMALISED_WINDOW_FIT_TOP = 8,
	/*!< Undistorted normalised window coordinate system which varies from
	 * -1 to +1 from far to near, and from -1 to +1 from left-to-right and
	 * bottom-to-top in largest square that fits in top of window.*/
	CMZN_SCENECOORDINATESYSTEM_WINDOW_PIXEL_BOTTOM_LEFT = 9,
	/*!< Window coordinate system in pixel units with 0,0 at centre of the
	 * bottom-left pixel in display window, and depth ranging from far = -1 to
	 * near = +1.*/
	CMZN_SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT = 10
	/*!< Window coordinate system in pixel units with 0,0 at centre of the
	 * top-left pixel in display window, and depth ranging from far = -1 to
	 * near = +1. Y coordinates are negative going down the window. */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_scenecoordinatesystem
	cmzn_scenecoordinatesystem_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param system  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_scenecoordinatesystem_enum_to_string(
	enum cmzn_scenecoordinatesystem system);

#ifdef __cplusplus
}
#endif

#endif
