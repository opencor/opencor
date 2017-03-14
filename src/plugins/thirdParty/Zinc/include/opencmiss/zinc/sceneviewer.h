/**
 * @file sceneviewer.h
 *
 * The public interface to the sceneviewer object for rendering scenes.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SCENEVIEWER_H__
#define CMZN_SCENEVIEWER_H__

#include "types/fieldid.h"
#include "types/fieldimageid.h"
#include "types/lightid.h"
#include "types/scenefilterid.h"
#include "types/sceneid.h"
#include "types/sceneviewerid.h"
#include "types/sceneid.h"
#include "types/scenecoordinatesystem.h"
#include "types/sceneviewerinputid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
Global functions
----------------
*/

/**
 * Returns a new handle to the scene viewer with reference count incremented.
 *
 * @param sceneviewer  The scene viewer to obtain a new handle to.
 * @return  New handle to sceneviewer, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewer_id cmzn_sceneviewer_access(cmzn_sceneviewer_id sceneviewer);

/**
 * Destroys handle to the scene viewer (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param sceneviewer_address  The address to the handle of the scene viewer
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_destroy(cmzn_sceneviewer_id *sceneviewer_address);

/**
 * Gets the mouse and keyboard interaction mode of the scene viewer.
 * @see cmzn_sceneviewer_interact_mode
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The interact mode or CMZN_SCENEVIEWER_INTERACT_MODE_INVALID on error.
 */
ZINC_API enum cmzn_sceneviewer_interact_mode cmzn_sceneviewer_get_interact_mode(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Call this function before making multiple changes on the sceneviewer, this
 * will stop sceneviewer from notifying clients of every change. After multiple
 * changes have been made, call the sceneviewer end change method to restart
 * notifications and notify clients of changes that have happened. Can be nested.
 * @see cmzn_sceneviewer_end_change
 *
 * @param sceneviewer  The handle to the sceneviewer.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_begin_change(cmzn_sceneviewer_id sceneviewer);

/**
 * Call sceneviewer begin change method before making multiple changes on the
 * sceneviewer, to stop sceneviewer from notifying clients of every change.
 * After multiple changes have been made, call this method to restart
 * notifications and notify clients of changes that have happened.
 * @see cmzn_sceneviewer_begin_change
 *
 * @param scene  The handle to the sceneviewer.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_end_change(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the mouse and keyboard interaction mode of the scene viewer.
 * @see cmzn_sceneviewer_interact_mode
 *
 * @param sceneviewer  The scene viewer to modify.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_interact_mode(cmzn_sceneviewer_id sceneviewer,
	enum cmzn_sceneviewer_interact_mode interact_mode);

/**
 * Set the eye position of the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param eyeValuesIn3  Array of three values containing the new eye position.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_set_eye_position(cmzn_sceneviewer_id sceneviewer,
	const double *eyeValuesIn3);

/**
 * Get the eye position of the scene viewer.
 *
 * @param  sceneviewer  Handle to the scene viewer.
 * @param  eyeValuesOut3  Array of size 3 to hold the values of the eye position.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_get_eye_position(cmzn_sceneviewer_id sceneviewer,
	double *eyeValuesOut3);

/**
 * Set the lookat position of the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param lookatValuesIn3  Array of three values containing the new lookat position.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_set_lookat_position(cmzn_sceneviewer_id sceneviewer,
	const double *lookatValuesIn3);

/**
 * Get the lookat position of the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param lookatValuesOut3  Array of size 3 to hold the values of the lookat position.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_get_lookat_position(cmzn_sceneviewer_id sceneviewer,
	double *lookatValuesOut3);

/**
 * Set the up vector of the scene viewer. Internally this is always converted
 * into a unit vector.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param upVectorValuesIn3  Array of three values containing the new up vector.
 * @return Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_set_up_vector(cmzn_sceneviewer_id sceneviewer,
	const double *upVectorValuesIn3);

/**
 * Get the up vector of the scene viewer.
 *
 * @param  sceneviewer  Handle to the scene viewer.
 * @param  upVectorValuesOut3  Array of size 3 to hold the values of the up vector.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_get_up_vector(cmzn_sceneviewer_id sceneviewer,
	double *upVectorValuesOut3);

/**
 * Gets the 3 main viewing parameters of the scene viewer: eye point, lookat
 * point and up vector in a single call.
 *
 * @param sceneviewer  The scene viewer to query.
 * @param eyeValuesOut3  Array of size 3 to receive the coordinates of the point
 * you are looking from.
 * @param lookatValuesOut3  Array of size 3 to receive the coordinates of the
 * point you are looking at, often also called the interest point.
 * @param upVectorValuesOut3  Array of size 3 to receive the vector giving the
 * up orientation in the scene viewer.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_get_lookat_parameters(
	cmzn_sceneviewer_id sceneviewer, double *eyeValuesOut3,
	double *lookatValuesOut3, double *upVectorValuesOut3);

/**
 * Sets the 3 main viewing parameters of the scene viewer: eye point, lookat
 * point and up vector in a single call. This function ensures the up vector
 * is orthogonal to the view direction so the projection is not skew.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param eyeValuesIn3  The 3 coordinates of the point you are looking from.
 * It is an error if this is at the same as the lookat point.
 * @param lookatValuesIn3  The 3 coordinates of the point you are looking at,
 * often also called the interest point.
 * @param upVectorValuesIn3  A vector which gives the up orientation in the
 * scene viewer. It is an error if this vector is colinear with the view
 * direction line from the eye to the lookat point. This vector is
 * automatically converted to a unit vector orthogonal to the view direction.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_lookat_parameters_non_skew(
	cmzn_sceneviewer_id sceneviewer, const double *eyeValuesIn3,
	const double *lookatValuesIn3, const double *upVectorValuesIn3);

/**
 * Gets the distance from the eye_point to the near clipping plane in the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @return distance from the eye_point to the near clipping plane on success,
 * 	any other value on failure.
 */
ZINC_API double cmzn_sceneviewer_get_near_clipping_plane(cmzn_sceneviewer_id sceneviewer);

/**
 * Gets the distance from the eye_point to the far clipping plane in the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @return distance from the eye_point to the far clipping plane on success,
 * 	any other value on failure.
 */
ZINC_API double cmzn_sceneviewer_get_far_clipping_plane(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the distance from the eye_point to the far clipping plane in the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param far_palne  distance from the eye_point to the far clipping plane
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_set_far_clipping_plane(cmzn_sceneviewer_id sceneviewer,
	double far_clipping_plane);

/**
 * Sets the distance from the eye_point to the near clipping plane in the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param near_plane  distance from the eye_point to the near clipping plane
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_set_near_clipping_plane(cmzn_sceneviewer_id sceneviewer,
	double near_clipping_plane);

/**
 * Gets the viewport mode (absolute/relative/distorting relative) for the
 * scene viewer.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The viewport mode or CMZN_SCENEVIEWER_VIEWPORT_MODE_INVALID on error.
 */
ZINC_API enum cmzn_sceneviewer_viewport_mode cmzn_sceneviewer_get_viewport_mode(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the viewport mode(absolute/relative/distorting relative) for the
 * scene viewer.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_viewport_mode(cmzn_sceneviewer_id sceneviewer,
	enum cmzn_sceneviewer_viewport_mode viewport_mode);

/**
 * Sets the width and height of the scene viewer's drawing area.
 */
ZINC_API int cmzn_sceneviewer_set_viewport_size(cmzn_sceneviewer_id sceneviewer,
	int width, int height);

/**
 * Gets the projection mode - parallel/perspective - of the scene viewer.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The viewport mode or CMZN_SCENEVIEWER_PROJECTION_MODE_INVALID on error.
 */
ZINC_API enum cmzn_sceneviewer_projection_mode
	cmzn_sceneviewer_get_projection_mode(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the projection mode - parallel/perspective - of the scene viewer.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param projection_mode  The new projection mode.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_projection_mode(cmzn_sceneviewer_id sceneviewer,
	enum cmzn_sceneviewer_projection_mode projection_mode);

/**
 * Gets the blending mode of the scene viewer.
 * @see cmzn_sceneviewer_blending_mode
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The blending mode or CMZN_SCENEVIEWER_BLENDING_MODE_INVALID on error.
 */
ZINC_API enum cmzn_sceneviewer_blending_mode cmzn_sceneviewer_get_blending_mode(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the blending mode of the scene viewer.
 * @see cmzn_sceneviewer_blending_mode
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param blending_mode  The new blending mode.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_blending_mode(cmzn_sceneviewer_id sceneviewer,
	enum cmzn_sceneviewer_blending_mode blending_mode);

/**
 * Gets the diagonal view angle, in radians, of the scene viewer.
 * View angle is measured across the largest square which fits inside the viewing
 * window.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The view angle in radians, or 0 if invalid arguments.
 */
ZINC_API double cmzn_sceneviewer_get_view_angle(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the diagonal view angle, in radians, of the scene viewer.
 * View angle is measured across the largest square which fits inside the viewing
 * window.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param view_angle  The view angle in radians, > 0.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_view_angle(cmzn_sceneviewer_id sceneviewer,
	double view_angle);

/**
 * Gets the number of jitter samples used to antialias the scene viewer.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The number of jitter samples used to antialias the graphics, or
 * 0 if antialiasing is off or bad argument.
 */
ZINC_API int cmzn_sceneviewer_get_antialias_sampling(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the number of jitter samples used to antialias the scene viewer.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param number_of_samples  The number of jitter samples used to antialias the
 * graphics. Only 2, 4 or 8 samples are supported. Zero disables antialiasing.
 * @return  On success CMZN_OK, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_antialias_sampling(
	cmzn_sceneviewer_id sceneviewer, int number_of_samples);

/**
 * Get the depth of field and focal depth of the scene viewer.
 */
ZINC_API int cmzn_sceneviewer_get_depth_of_field(cmzn_sceneviewer_id sceneviewer,
	double *depth_of_field, double *focal_depth);

/**
 * Set a simulated <depth_of_field> for the scene viewer.
 * If <depth_of_field> is 0, then this is disabled, essentially an infinite depth.
 * Otherwise, <depth_of_field> is a normalised length in z space, so 1 is a
 * significant value, 0.1 is a small value causing significant distortion.
 * The <focal_depth> is depth in normalised device coordinates, -1 at near plane
 * and +1 at far plane.  At this <focal_depth> the image is in focus no matter
 * how small the <depth_of_field>.
 */
ZINC_API int cmzn_sceneviewer_set_depth_of_field(cmzn_sceneviewer_id sceneviewer,
	double depth_of_field, double focal_depth);

/**
 * Query whether lines are perturbed to appear in front of surfaces at the same
 * depth to avoid visual stitching artefacts.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  Boolean true if perturbing lines, otherwise false.
 */
ZINC_API bool cmzn_sceneviewer_get_perturb_lines_flag(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Set whether lines are perturbed to appear in front of surfaces at the same
 * depth to avoid visual stitching artefacts. Uses GL_EXT_polygon_offset.
 * Note that quality of surface-only graphics can be impacted with this mode.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param value  New value of perturb lines flag.
 * @return  On success CMZN_OK, otherwise CMZN_ERROR_ARGUMENT.
*/
ZINC_API int cmzn_sceneviewer_set_perturb_lines_flag(
	cmzn_sceneviewer_id sceneviewer, bool value);

/**
 * Returns the background_colour of the scene viewer.
 * The component order in the array is [red, green, blue]
 *
 * @param sceneviewer The handle to the scene viewer
 * @param valuesOut3 The rgb components of the colour with values between [0, 1.0]
 * @return CMZN_OK if successful, any other value otherwise
 */
ZINC_API int cmzn_sceneviewer_get_background_colour_rgb(
	cmzn_sceneviewer_id sceneviewer, double *valuesOut3);

/**
 * Returns the background_colour of the scene viewer.
 * The component order in the array is [red, green, blue, alpha]
 *
 * @param sceneviewer The handle to the scene viewer
 * @param valuesOut4 The rgba components of the colour with values between [0, 1.0]
 * @return CMZN_OK if successful, any other value otherwise
 */
ZINC_API int cmzn_sceneviewer_get_background_colour_rgba(
	cmzn_sceneviewer_id sceneviewer, double *valuesOut4);

/**
 * Sets the background_colour of the scene viewer by individual component.
 * Each component should be in the range [0, 1.0].
 *
 * @param sceneviewer The handle to the scene viewer
 * @param red The red component value between [0, 1.0]
 * @param green The green component value between [0, 1.0]
 * @param blue The blue component value between [0, 1.0]
 * @return CMZN_OK if successful, any other value otherwise
 */
ZINC_API int cmzn_sceneviewer_set_background_colour_component_rgb(
	cmzn_sceneviewer_id sceneviewer, double red, double green, double blue);

/**
 * Sets the background_colour of the scene viewer by individual component.
 * Each component should be in the range [0, 1.0].
 *
 * @param sceneviewer The handle to the scene viewer
 * @param red The red component value between [0, 1.0]
 * @param green The green component value between [0, 1.0]
 * @param blue The blue component value between [0, 1.0]
 * @param alpha The alpha component value between [0, 1.0]
 * @return CMZN_OK if successful, any other value otherwise
 */
ZINC_API int cmzn_sceneviewer_set_background_colour_component_rgba(
	cmzn_sceneviewer_id sceneviewer, double red, double green, double blue, double alpha);

/**
 * Sets the background_colour of the scene viewer.
 * Each component should be in the range [0, 1.0].
 * The component order is [red, green, blue]
 *
 * @param sceneviewer The handle to the scene viewer
 * @param valuesIn3 The rgb components of the colour
 * @return CMZN_OK if successful, any other value otherwise
 */
ZINC_API int cmzn_sceneviewer_set_background_colour_rgb(
	cmzn_sceneviewer_id sceneviewer, const double *valuesIn3);

/**
 * Sets the background_colour of the scene viewer.
 * Each component should be in the range [0, 1.0].
 * The component order is [red, green, blue, alpha]
 *
 * @param sceneviewer The handle to the scene viewer
 * @param valuesIn4 The rgba components of the colour
 * @return CMZN_OK if successful, any other value otherwise
 */
ZINC_API int cmzn_sceneviewer_set_background_colour_rgba(
	cmzn_sceneviewer_id sceneviewer, const double *valuesIn4);

/**
 * If there is a background_texture in the scene viewer, these values specify the
 * top,left corner, in user coordinates, where it will be displayed, while the
 * next two parameters specify the size it will have in these coordinates.
 * If the bk_texture_undistort_on flag is set, radial distortion parameters from the background texture are un-distorted when the
 * texture is displayed. It does this by drawing it as a collection of polygons;
 * the last parameter controls the size of polygons used to do this.
 */
ZINC_API int cmzn_sceneviewer_set_background_texture_info(cmzn_sceneviewer_id sceneviewer,
	double bk_texture_left,double bk_texture_top,
	double bk_texture_width,double bk_texture_height,
	int bk_texture_undistort_on,double bk_texture_max_pixels_per_polygon);

/**
 * Finds the x, y and z ranges from the scene and sets the view parameters so
 * that everything can be seen, and with window's std_view_angle. Also adjusts
 * near and far clipping planes; if specific values are required, should follow
 * with commands for setting these.
 */
ZINC_API int cmzn_sceneviewer_view_all(cmzn_sceneviewer_id sceneviewer);

/**
 * Forces a redraw of the given scene viewer to take place immediately
 */
ZINC_API int cmzn_sceneviewer_render_scene(cmzn_sceneviewer_id sceneviewer);

/**
 * Gets the rate at which translation occurs in relation to mouse movement using
 * standard sceneviewerinput transformation processing.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The translation rate, or 0 if invalid arguments.
 */
ZINC_API double cmzn_sceneviewer_get_translation_rate(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the rate at which translation occurs in relation to mouse movement using
 * standard sceneviewerinput transformation processing.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param translation_rate  The translation rate;
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_translation_rate(cmzn_sceneviewer_id sceneviewer,
	double translation_rate);

/**
 * Gets the rate at which tumble occurs in relation to mouse movement using
 * standard sceneviewerinput transformation processing.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The tumble rate, or 0 if invalid arguments.
 */
ZINC_API double cmzn_sceneviewer_get_tumble_rate(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the rate at which tumble occurs in relation to mouse movement using
 * standard sceneviewerinput transformation processing.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param tumble_rate  The tumble rate;
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_tumble_rate(cmzn_sceneviewer_id sceneviewer,
	double tumble_rate);

/**
 * Gets the rate at which zoom occurs in relation to mouse movement using
 * standard sceneviewerinput transformation processing.
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  The zoom rate, or 0 if invalid arguments.
 */
ZINC_API double cmzn_sceneviewer_get_zoom_rate(cmzn_sceneviewer_id sceneviewer);

/**
 * Sets the rate at which zoom occurs in relation to mouse movement using
 * standard sceneviewerinput transformation processing.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param zoom_rate  The zoom rate;
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_zoom_rate(cmzn_sceneviewer_id sceneviewer,
	double zoom_rate);

/**
 * Returns the scene viewer freespin tool tumble angle.
 */
ZINC_API int cmzn_sceneviewer_get_freespin_tumble_angle(cmzn_sceneviewer_id sceneviewer,
	double *tumble_angle);

/**
 * Sets the scene viewer freespin tool tumble angle.
 */
ZINC_API int cmzn_sceneviewer_set_freespin_tumble_angle(cmzn_sceneviewer_id sceneviewer,
	double tumble_angle);

/**
 * Gets the scene viewer tumble axis.  The <tumble_axis> is the vector
 * about which the scene is turning relative to its lookat point.
 */
ZINC_API int cmzn_sceneviewer_get_freespin_tumble_axis(cmzn_sceneviewer_id sceneviewer,
	double *tumble_axis);

/**
 * Sets the scene viewer spinning in idle time.  The <tumble_axis> is the vector
 * about which the scene is turning relative to its lookat point and the
 * <tumble_angle> controls how much it turns on each redraw.
 */
ZINC_API int cmzn_sceneviewer_start_freespin(cmzn_sceneviewer_id sceneviewer,
	double *tumble_axis, double tumble_angle);

/**
 * Tells the scene viewer to stop all automatic informations that it produces,
 * eg. automatic tumble.
 */
ZINC_API int cmzn_sceneviewer_stop_animations(cmzn_sceneviewer_id sceneviewer);

/**
 * Writes the view in the scene viewer to the specified filename.
 * If <preferred_width>, <preferred_height>, <preferred_antialias> or
 * <preferred_transparency_layers> are non zero then they attempt to override the
 * default values for just this write.  The width and height cannot be overridden
 * when the <force_onscreen> flag is set.
 */
ZINC_API int cmzn_sceneviewer_write_image_to_file(cmzn_sceneviewer_id sceneviewer,
	const char *file_name, int force_onscreen, int preferred_width,
	int preferred_height, int preferred_antialias, int preferred_transparency_layers);

/**
 * Gets the NDC information.
 */
ZINC_API int cmzn_sceneviewer_get_NDC_info(cmzn_sceneviewer_id sceneviewer,
	double *NDC_left,double *NDC_top,double *NDC_width,double *NDC_height);

/**
 * Gets the NDC information.
 */
ZINC_API int cmzn_sceneviewer_set_NDC_info(cmzn_sceneviewer_id sceneviewer,
	double NDC_left,double NDC_top,double NDC_width,double NDC_height);

/**
 * Gets the viewing volume of the scene viewer.
 */
ZINC_API int cmzn_sceneviewer_get_viewing_volume(cmzn_sceneviewer_id sceneviewer,
	double *left,double *right,double *bottom,double *top,double *near_plane,
	double *far_plane);

/**
 * Sets the viewing volume of the scene viewer. Unless the viewing volume is the
 * same shape as the window, taking into account the aspect, the scene viewer will
 * enlarge it to maintain the desired aspect ratio. Hence, the values specified
 * represent the minimum viewing volume. The left, right, bottom and top values
 * are at the lookat point, not on the near plane as OpenGL assumes. This gives a
 * similar sized viewing_volume for both parallel and perspective projections.
 * The viewing volume can be made unsymmetric to create special effects such as
 * rendering a higher resolution image in parts.
 */
ZINC_API int cmzn_sceneviewer_set_viewing_volume(cmzn_sceneviewer_id sceneviewer,
	double left,double right,double bottom,double top,double near_plane,double far_plane);

/**
 * Returns the contents of the scene viewer as pixels.  <width> and <height>
 * will be respected if the window is drawn offscreen and they are non zero,
 * otherwise they are set in accordance with current size of the scene viewer.
 * If <preferred_antialias> or <preferred_transparency_layers> are non zero then they
 * attempt to override the default values for just this call.
 * If <force_onscreen> is non zero then the pixels will always be grabbed from the
 * scene viewer on screen.
 */
ZINC_API int cmzn_sceneviewer_get_frame_pixels(cmzn_sceneviewer_id  sceneviewer,
	enum cmzn_streaminformation_image_pixel_format storage, int *width, int *height,
	int preferred_antialias, int preferred_transparency_layers,
	unsigned char **frame_data, int force_onscreen);

/**
 * Destroys this handle to the scene viewer inpit, and sets it to NULL.
 *
 * @param input_address  The address to the handle of the input
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
//-- ZINC_API int cmzn_sceneviewerinput_destroy(cmzn_sceneviewerinput_id *input_address);

/**
 * Manually calls the scene viewer's list of input callbacks with the supplied
 * input data.
 *
 * @param sceneviewer  Handle to sceneviewer object.
 * @param input_data  Description of the input event.
 * @return  Status CMZN_OK on success, any other value if failed.
 */
ZINC_API int cmzn_sceneviewer_process_sceneviewerinput(cmzn_sceneviewer_id sceneviewer,
	cmzn_sceneviewerinput_id input_data);

/**
 * Set the top scene for the scene viewer. Only graphics for this scene and
 * its descendents, filtered by the scene filter, are displayed.
 * @see cmzn_sceneviewer_set_scenefilter
 *
 * @param sceneviewer  The sceneviewer to modify.
 * @param scene  The top scene to display.
 * @return  Status CMZN_OK on success, any other value if failed.
 */
ZINC_API int cmzn_sceneviewer_set_scene(cmzn_sceneviewer_id sceneviewer,
	cmzn_scene_id scene);

/**
 * Get the top scene for the scene viewer.
 *
 * @param sceneviewer  The sceneviewer to query.
 * @return  Handle to scene, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scene_id cmzn_sceneviewer_get_scene(cmzn_sceneviewer_id sceneviewer);

/**
 * Get the filter currently used in scene viewer.
 *
 * @param sceneviewer  scene viewer to get the filters from.
 * @return  Handle to scene filter, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_scenefilter_id cmzn_sceneviewer_get_scenefilter(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Set the filter to be used in scene viewer. All graphics will be shown
 * if no filter is set.
 *
 * @param sceneviewer  Scene viewer to set filter for.
 * @param filter  Scene filter to set for scene viewer, or NULL to show all
 * graphics in scene.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_set_scenefilter(cmzn_sceneviewer_id sceneviewer,
	cmzn_scenefilter_id filter);

/**
 * Returns a count of the number of scene viewer redraws.
 */
ZINC_API unsigned int cmzn_sceneviewer_get_frame_count(cmzn_sceneviewer_id sceneviewer);

/**
 * Returns a handle to a scene viewer object.  The scene viewer attributes for
 * buffering mode and stereo mode are also set.
 *
 * @param sceneviewermodule  Handle to a scene viewer module object.
 * @param buffer_mode  The buffering mode in use for the OpenGL context.
 * @param stereo_mode  The stereo mode in use for the OpenGL context.
 * @return  Handle to new sceneviewer, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewer_id cmzn_sceneviewermodule_create_sceneviewer(
	cmzn_sceneviewermodule_id sceneviewermodule,
	enum cmzn_sceneviewer_buffering_mode buffer_mode,
	enum cmzn_sceneviewer_stereo_mode stereo_mode);

/**
* Gets the default background colour for new scene viewers created from this
* scene viewer module.
* The component order in the array is [red, green, blue]
*
* @param sceneviewermodule  The handle to the scene viewer module.
* @param valuesOut3  The rgb components of the colour with values in [0, 1.0].
* @return  CMZN_OK if successful, otherwise CMZN_ERROR_ARGUMENT.
*/
ZINC_API int cmzn_sceneviewermodule_get_default_background_colour_rgb(
	cmzn_sceneviewermodule_id sceneviewermodule, double *valuesOut3);

/**
* Gets the default background colour for new scene viewers created from this
* scene viewer module.
* The component order in the array is [red, green, blue, alpha]
*
* @param sceneviewermodule  The handle to the scene viewer module.
* @param valuesOut4  The rgba components of the colour with values in [0, 1.0].
* @return  CMZN_OK if successful, otherwise CMZN_ERROR_ARGUMENT.
*/
ZINC_API int cmzn_sceneviewermodule_get_default_background_colour_rgba(
	cmzn_sceneviewermodule_id sceneviewermodule, double *valuesOut4);

/**
* Sets the default background colour for new scene viewers created from this
* scene viewer module.
* Each component should be in the range [0, 1.0].
* The component order is [red, green, blue].
*
* @param sceneviewermodule  The handle to the scene viewer module.
* @param valuesIn3  The RGB components of the colour.
* @return  CMZN_OK if successful, otherwise CMZN_ERROR_ARGUMENT.
*/
ZINC_API int cmzn_sceneviewermodule_set_default_background_colour_rgb(
	cmzn_sceneviewermodule_id sceneviewermodule, const double *valuesIn3);

/**
* Sets the default background colour for new scene viewers created from this
* scene viewer module.
* Each component should be in the range [0, 1.0].
* The component order is [red, green, blue, alpha].
*
* @param sceneviewermodule  The handle to the scene viewer module.
* @param valuesIn4  The RGBA components of the colour.
* @return  CMZN_OK if successful, otherwise CMZN_ERROR_ARGUMENT.
*/
ZINC_API int cmzn_sceneviewermodule_set_default_background_colour_rgba(
	cmzn_sceneviewermodule_id sceneviewermodule, const double *valuesIn4);

/**
 * Returns a new handle to the scene viewer module with the reference counter
 * incremented.
 *
 * @param sceneviewermodule  The scene viewer module to obtain a reference to.
 * @return  New handle to sceneviewer module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewermodule_id cmzn_sceneviewermodule_access(cmzn_sceneviewermodule_id sceneviewermodule);

/**
 * Destroys the scene viewer module and resets it to NULL.
 *
 * @param sceneviewermodule_address  Address of handle of the scene viewer module.
 * @return  Status CMZN_OK if handle successfully destroyed, otherwise any other value.
 */
ZINC_API int cmzn_sceneviewermodule_destroy(cmzn_sceneviewermodule_id *sceneviewermodule_address);

/**
 * Get the transparency_mode of the scene viewer. In fast transparency mode,
 * the scene is drawn as it is, with depth buffer writing even for semi-transparent
 * objects. In slow transparency mode, opaque objects are rendered first, then
 * semi-transparent objects are rendered without writing the depth buffer. Hence,
 * you can even see through the first semi-transparent surface drawn.
 *
 * @see cmzn_sceneviewer_transparency_mode
 * @see cmzn_sceneviewer_set_transparency_mode
 *
 * @param sceneviewer  Handle to the scene viewer.
 *
 * @return  transparency_mode set for this sceneviewer.
 *   CMZN_SCENEVIEWER_TRANSPARENCY_MODE_ORDER_INDEPENDENT if failed or
 *   mode is not set correctly
 */
ZINC_API enum cmzn_sceneviewer_transparency_mode cmzn_sceneviewer_get_transparency_mode(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Set the transparency_mode of the scene viewer.
 *
 * @see cmzn_sceneviewer_transparency_mode
 * @see cmzn_sceneviewer_get_transparency_mode
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param transparency_mode  Transparency mode to be set for sceneviewer
 *
 * @return  CMZN_OK if value is set successfully, any other value if
 * 	failed.
 */
ZINC_API int cmzn_sceneviewer_set_transparency_mode(cmzn_sceneviewer_id sceneviewer,
	enum cmzn_sceneviewer_transparency_mode transparency_mode);

/**
 * Get the number of layers used in the TRANSPARENCY_MODE_ORDER_INDEPENDENT
 * transparency mode.
 * @see cmzn_sceneviewer_transparency_mode
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  number of layers for this scene viewer. Any otehr value if failed or
 *   it is not set correctly.
 */
ZINC_API int cmzn_sceneviewer_get_transparency_layers(cmzn_sceneviewer_id sceneviewer);

/**
 * Set the number of layers used in the TRANSPARENCY_MODE_ORDER_INDEPENDENT
 * transparency_mode.
 * @see cmzn_sceneviewer_transparency_mode
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param layers  number of layers to be set for this scene viewer.
 *
 * @return  CMZN_OK if value is set successfully, any other value if
 * 	failed.
 */
ZINC_API int cmzn_sceneviewer_set_transparency_layers(cmzn_sceneviewer_id sceneviewer,
	int layers);

/**
 * Adds a light to the Scene_viewer list of lights.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param light  Handle to the light to be added to scene viewer.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_add_light(cmzn_sceneviewer_id sceneviewer,
	cmzn_light_id light);

/**
 * Check if a light is on Scene_viewer list of lights.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param light  Handle to the light to be checked.
 * @return  1 if light is on the list, 0 otherwise.
 */
ZINC_API bool cmzn_sceneviewer_has_light(cmzn_sceneviewer_id sceneviewer,
	cmzn_light_id light);

/**
 * Remove a light from the Scene_viewer list of lights.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @param light  Handle to the light to be removed from scene viewer.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewer_remove_light(cmzn_sceneviewer_id sceneviewer,
	cmzn_light_id light);

/**
 * Queries whether local viewer lighting is used by the scene viewer.
 * @see cmzn_sceneviewer_set_lighting_local_viewer
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  Boolean true if local viewer lighting is set, false if not or bad
 *   argument.
 */
ZINC_API bool cmzn_sceneviewer_is_lighting_local_viewer(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Sets whether local viewer lighting is used by the scene viewer.
 * If true, the angle of view from the eye to the graphics vertex is used to
 * give more realistic lighting, at slightly greater rendering expense.
 * If false (default) infinite lighting is assumed, which gives faster
 * rendering.
 * The difference becomes apparent when viewing a plane close up with specular
 * colour: with local viewer lighting (and sufficient tessellation divisions)
 * rounded specular highlighting can be seen; with infinite viewer lighting
 * the specular highlighting is even across the plane.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param value  The new state of the local viewer lighting flag.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_lighting_local_viewer(
	cmzn_sceneviewer_id sceneviewer, bool value);

/**
 * Queries whether two-sided lighting is used for rendering polygons.
 * @see cmzn_sceneviewer_set_lighting_two_sided
 *
 * @param sceneviewer  The scene viewer to query.
 * @return  Boolean true if lighting is on, false if not or bad argument.
 */
ZINC_API bool cmzn_sceneviewer_is_lighting_two_sided(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Sets whether two-sided lighting is used for rendering polygons.
 * If true (default) then back surfaces are lit with reversed normals.
 * If false i.e. one-sided then back surfaces are only lit by ambient lights.
 * One-sided lighting is useful for finding problems with element definitions
 * in 3-D: exterior faces should have outward normals, so if these are not
 * lit on the outside with one-sided lighting, then the element has a
 * left-handed coordinate system i.e. negative volume. Note that interior
 * faces' normals should only be outward with respect to their first parent
 * element, and inward with respect to their second parent.
 *
 * @param sceneviewer  The scene viewer to modify.
 * @param value  The new state of the two-sided lighting flag.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewer_set_lighting_two_sided(
	cmzn_sceneviewer_id sceneviewer, bool value);

/**
 * Transforms coordinates between scene coordinate systems relative to this
 * scene viewer.
 * Note: this function only works once there is an OpenGL rendering context!
 * @param sceneviewer  The scene viewer to query.
 * @param in_coordinate_system  The coordinate system of the input values.
 * @param out_coordinate_system  The coordinate system of the output values.
 * @param local_scene  Optional local scene, relative to top scene of scene
 * viewer from which the local-to-world transformation is obtained. If omitted
 * the identity transformation is assumed. Only used with scene local
 * coordinate systems, relative to world.
 * @param inValues3  The input coordinates, 3-component.
 * @param outValues3  The output coordinates to set, 3-component.
 * @return  Status CMZN_OK on success, otherwise any error code.
 */
ZINC_API int cmzn_sceneviewer_transform_coordinates(
	cmzn_sceneviewer_id sceneviewer,
  enum cmzn_scenecoordinatesystem in_coordinate_system,
  enum cmzn_scenecoordinatesystem out_coordinate_system,
	cmzn_scene_id local_scene, const double *valuesIn3, double *valuesOut3);

/**
 * Read the json description to the scene viewer. This will change
 * the settings of the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @description  The string containing json description
 * @return  CMZN_OK on success, otherwise ERROR status.
 */
ZINC_API int cmzn_sceneviewer_read_description(
	cmzn_sceneviewer_id sceneviewer, const char *description);

/**
 * Write the json file describing settings of the scene viewer, which can
 * be used to store the current settings.
 *
 * @param sceneviewer  Handle to the scene viewer.
 * @return  c string containing the json description of scene viewer, otherwise 0;
 */
ZINC_API char *cmzn_sceneviewer_write_description(cmzn_sceneviewer_id sceneviewer);

/**
 * Create a notifier for getting callbacks for changes to the scene viewer.
 *
 * @param sceneviewer  Handle to the scene viewer to get notifications for.
 * @return  Handle to new sceneviewer notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewernotifier_id cmzn_sceneviewer_create_sceneviewernotifier(
	cmzn_sceneviewer_id sceneviewer);

/**
 * Returns a new handle to the scene viewer notifier with reference count
 * incremented.
 *
 * @param notifier  The scene viewer notifier to obtain a new handle to.
 * @return  Handle to scene viewer notifier with incremented reference count.
 * @return  New handle to sceneviewer notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewernotifier_id cmzn_sceneviewernotifier_access(
	cmzn_sceneviewernotifier_id notifier);

/**
 * Destroys handle to the scene viewer notifier and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param notifier_address  Address of scene viewer notifier handle to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_sceneviewernotifier_destroy(cmzn_sceneviewernotifier_id *notifier_address);

/**
 * Stop and clear scene viewer callback. This will stop the callback and also
 * remove the callback function from the sceneviewer notifier.
 *
 * @param notifier  Handle to the sceneviewer notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewernotifier_clear_callback(cmzn_sceneviewernotifier_id notifier);

/**
 * Assign the callback function and user data for the sceneviewer notifier.
 * This function also starts the callback.
 *
 * @see cmzn_sceneviewernotifier_callback_function
 * @param notifier  Handle to the sceneviewer notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User must ensure this
 * object's lifetime exceeds the duration for which callbacks are active.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewernotifier_set_callback(cmzn_sceneviewernotifier_id notifier,
	cmzn_sceneviewernotifier_callback_function function, void *user_data_in);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_sceneviewernotifier_set_callback
 *
 * @see cmzn_sceneviewernotifier_set_callback
 * @param notifier  Handle to the scene viewer notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_sceneviewernotifier_get_callback_user_data(
 cmzn_sceneviewernotifier_id notifier);

/**
 * Returns a new handle to the sceneviewer event with reference count incremented.
 *
 * @param event  The scene viewer event to obtain a new handle to.
 * @return  New handle to sceneviewer event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewerevent_id cmzn_sceneviewerevent_access(
	cmzn_sceneviewerevent_id event);

/**
 * Destroys this handle to the sceneviewer event and sets it to NULL.
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param event_address  Address of scene viewer event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_sceneviewerevent_destroy(cmzn_sceneviewerevent_id *event_address);

/**
 * Get logical OR of flags indicating how fields in the scene viewer have changed.
 * @see cmzn_sceneviewerevent_change_flag
 *
 * @param event  Handle to the scene viewer event.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_sceneviewerevent_change_flag values.
 */
ZINC_API cmzn_sceneviewerevent_change_flags cmzn_sceneviewerevent_get_change_flags(
	cmzn_sceneviewerevent_id event);

#ifdef __cplusplus
}
#endif

#endif
