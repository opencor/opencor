/**
 * @file scenepicker.h
 *
 * Public interface to the scene picker which represents a tool for
 * graphics primitives picking.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENEPICKER_H__
#define CMZN_SCENEPICKER_H__

#include "types/graphicsid.h"
#include "types/scenefilterid.h"
#include "types/fieldgroupid.h"
#include "types/elementid.h"
#include "types/nodeid.h"
#include "types/sceneid.h"
#include "types/scenecoordinatesystem.h"
#include "types/scenepickerid.h"
#include "types/sceneviewerid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the scene picker with reference count incremented.
 *
 * @param scenepicker  The scenepicker to obtain a new handle to.
 * @return  New handle to scenepicker, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenepicker_id cmzn_scenepicker_access(
	cmzn_scenepicker_id scenepicker);

/**
 * Destroys handle to the scenepicker (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param scenepicker_address  The address to the handle of the scenepicker
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_destroy(cmzn_scenepicker_id *scenepicker_address);

/**
 * Get the scene set for the scene picker to pick from.
 *
 * @param scenepicker  The scene picker to get the scene from.
 * @return  Handle to scene, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scene_id cmzn_scenepicker_get_scene(cmzn_scenepicker_id scenepicker);

/**
 * Set the scene for the scene picker to pick from.
 *
 * @param scenepicker  The scene picker to be modified.
 * @param scene  The scene to pick from.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_set_scene(cmzn_scenepicker_id scenepicker,
	cmzn_scene_id scene);

/**
 * Get the scene filter for the scene picker.
 *
 * @param scenepicker  The scene picker to get the scene filter from.
 * @return  Handle to scene filter, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_scenepicker_get_scenefilter(
	cmzn_scenepicker_id scenepicker);

/**
 * Set the scene filter for the scene picker. This filters which graphics
 * are pickable from the scene.
 *
 * @param scenepicker  The scene picker to be modified.
 * @param filter  The scene filter to use, or NULL if none (all pickable).
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_set_scenefilter(cmzn_scenepicker_id scenepicker,
	cmzn_scenefilter_id filter);

/**
 * Set the bounding box of scene picker. Scene viewer will provide the preset
 * modelview and projection matrix from itself.
 * (x1,y1) and (x2,y2) are diagonally opposite corners of the rectangle.
 *
 * @param scenepicker  The scene picker to be modified.
 * @param sceneviewer_in  Scene viewer to get the modelview and projection
 * matrix from.
 * @param coordinate_system_in
 * @param x1 and y1 specify the location of a corner of the rectangle
 * @param x1 and y2 specify tthe location of he diagonally opposite corners
 * to (x1, y1) of the rectangle.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_set_sceneviewer_rectangle(
	cmzn_scenepicker_id scenepicker, cmzn_sceneviewer_id sceneviewer_in,
	enum cmzn_scenecoordinatesystem coordinate_system_in, double x1,
		double y1, double x2, double y2);

/**
 * Get the nearest element in the defined bounding box on scene.
 *
 * @param scenepicker  The scene picker to pick the nearest element.
 * @return  Handle to nearest element, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_element_id cmzn_scenepicker_get_nearest_element(
	cmzn_scenepicker_id scenepicker);

/**
 * Get the nearest node in the defined bounding box on scene.
 *
 * @param scenepicker  The scene picker to pick the nearest node.
 * @return  Handle to nearest node, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_node_id cmzn_scenepicker_get_nearest_node(
	cmzn_scenepicker_id scenepicker);

/**
 * Get the nearest element graphics in the defined bounding box on scene.
 *
 * @param scenepicker  The scene picker to pick the nearest element graphics.
 * @return  Handle to graphics showing nearest element, or NULL/invalid handle
 * if none or failed.
 */
ZINC_API cmzn_graphics_id cmzn_scenepicker_get_nearest_element_graphics(
	cmzn_scenepicker_id scenepicker);

/**
 * Get the nearest node graphics in the defined bounding box on scene.
 *
 * @param scenepicker  The scene picker to pick the nearest node graphics.
 * @return  Handle to graphics showing nearest node, or NULL/invalid handle
 * if none or failed.
 */
ZINC_API cmzn_graphics_id cmzn_scenepicker_get_nearest_node_graphics(
	cmzn_scenepicker_id scenepicker);

/**
 * Get the nearest graphics in the defined bounding box on scene.
 *
 * @param scenepicker  The scene picker to pick the nearest graphics.
 * @return  Handle to nearest graphics, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_graphics_id cmzn_scenepicker_get_nearest_graphics(
	cmzn_scenepicker_id scenepicker);

/**
 * Picked nodes belong to group field's owning region tree will
 * be added to group.
 * Additional subregion fields will be created when required.
 *
 * @param scenepicker  The scene picker to pick the nearest node.
 * @param group  nodes will be added to this group field
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_add_picked_nodes_to_field_group(
	cmzn_scenepicker_id scenepicker, cmzn_field_group_id group);

/**
 * Picked elements belong to group field's owning region tree will
 * be added to group.
 * Additional subregion fields will be created when required.
 *
 * @param scenepicker  The scene picker to pick the nearest element.
 * @param group  elements will be added to this group field
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_add_picked_elements_to_field_group(
	cmzn_scenepicker_id scenepicker, cmzn_field_group_id group);

/**
 * Return the centre of the picking volume in world coordinates.
 *
 * @param scenepicker  The scene picker to pick the nearest element.
 * @param coordinateValuesOut3 Array of size 3 to hold the values of
 * 	the coordinates.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_scenepicker_get_picking_volume_centre(
		cmzn_scenepicker_id scenepicker, double *coordinateValuesOut3);

#ifdef __cplusplus
}
#endif

#endif
