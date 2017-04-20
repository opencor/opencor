/**
 * @file lightid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_LIGHTID_H__
#define CMZN_LIGHTID_H__

/**
 * @brief Module managing all light objects.
 *
 * Module managing all light objects. It maintains separate default lights for
 * directional head-lights and for ambient lights (background lighting from all
 * directions), which are automatically added to new scene viewers.
 */
struct cmzn_lightmodule;
typedef struct cmzn_lightmodule * cmzn_lightmodule_id;

/**
 * @brief The light controls how vertices will be lit on a sceneviewer.
 *
 * The light controls how vertices will be lit on a sceneviewer. Lighting and
 * colouring of graphics are affected by the light colours, direction, position
 * and type of lighting, in combination with material colours and other
 * parameters.
 */
struct cmzn_light;
typedef struct cmzn_light * cmzn_light_id;

/**
 * @brief An iterator for looping through all the lights in a light module.
 *
 * An iterator for looping through all the lights in a light module.
 */
struct cmzn_lightiterator;
typedef struct cmzn_lightiterator * cmzn_lightiterator_id;

/**
 * The type of light. Various types imitate real-world light sources.
 */
enum cmzn_light_type
{
	CMZN_LIGHT_TYPE_INVALID = 0,
	/*!< Unspecified light type */
	CMZN_LIGHT_TYPE_AMBIENT = 1,
	/*!< Ambient lighting from all directions. This multiplies the ambient colour
	 * of materials, giving a minimum colour where no other lights reach. */
	CMZN_LIGHT_TYPE_DIRECTIONAL = 2,
	/*!< Directional light with parallel light rays from a source at infinity,
	 * hence also known as infinite light. */
	CMZN_LIGHT_TYPE_POINT = 3,
	/*!< Positional light i.e. light radiating from a point. Takes into account
	 * attenuation parameters. */
	CMZN_LIGHT_TYPE_SPOT = 4
	/*!< Spot light taking position, direction, cut-off angle and attenuation
	 * into account when calculating the final colour. */
};

#endif
