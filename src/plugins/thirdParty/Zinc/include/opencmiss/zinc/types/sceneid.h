/**
 * @file sceneid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENEID_H__
#define CMZN_SCENEID_H__

/**
 * @brief Container/manager for graphics visualising a region.
 *
 * Container/manager for the graphics making up the visualisation of the
 * region's domains and fields.
 */
struct cmzn_scene;
typedef struct cmzn_scene * cmzn_scene_id;

/**
 * @brief A scene-specific stream information object.
 *
 * A scene-specific stream information object, used to specify one or more
 * files/resources for a scene to write to, with attributes
 * specified for them globally.
 *
 * @see cmzn_scene_create_streaminformation_scene
 */
struct cmzn_streaminformation_scene;
typedef struct cmzn_streaminformation_scene * cmzn_streaminformation_scene_id;

/**
 * Enumeration to indicate what the data should written out as.
 */
enum cmzn_streaminformation_scene_io_data_type
{
	CMZN_STREAMINFORMATION_SCENE_IO_DATA_TYPE_INVALID = 0,
	/*!< Unspecified type */
	CMZN_STREAMINFORMATION_SCENE_IO_DATA_TYPE_COLOUR = 1,
	/*!< Export data as colour*/
	CMZN_STREAMINFORMATION_SCENE_IO_DATA_TYPE_PER_VERTEX_VALUE = 2,
	/*!< Export data as field value on per vertex basis*/
	CMZN_STREAMINFORMATION_SCENE_IO_DATA_TYPE_PER_FACE_VALUE = 3
	/*!< Export data as field value on per face basis*/
};

/**
 * Enumeration of attributes that can be set by generic stream information region
 * methods.
 */
enum cmzn_streaminformation_scene_io_format
{
	CMZN_STREAMINFORMATION_SCENE_IO_FORMAT_INVALID = 0,
	/*!< Unspecified attribute */
	CMZN_STREAMINFORMATION_SCENE_IO_FORMAT_THREEJS = 1,
	/*!< Export scene into ThreeJS compatible JSON file.*/
	CMZN_STREAMINFORMATION_SCENE_IO_FORMAT_DESCRIPTION = 2
	/*!< Import/export scene configurations into the scene */
};

#endif
