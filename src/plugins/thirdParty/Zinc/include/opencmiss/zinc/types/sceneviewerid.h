/**
 * @file sceneviewerid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SCENEVIEWERID_H__
#define CMZN_SCENEVIEWERID_H__

/**
 * @brief Module object for creating and managing scene viewers.
 *
 * Module object for creating and managing scene viewers.
 */
struct cmzn_sceneviewermodule;
typedef struct cmzn_sceneviewermodule * cmzn_sceneviewermodule_id;

/**
 * @brief The Zinc sceneviewer is responsible for rendering the graphical Scene.
 *
 * The Zinc sceneviewer is responsible for rendering the graphical Scene using
 * OpenGL. It has methods to set its top scene and scene filter, and to get and
 * set attributes controlling the view orientation, field of view, clipping
 * planes and more.
 * The client is responsible for creating the OpenGL-capable canvas with a
 * sceneviewer, informing the sceneviewer of its size including on resize
 * events, making the OpenGL context current and invoking the render scene
 * method of the sceneviewer.
 */
struct cmzn_sceneviewer;
typedef struct cmzn_sceneviewer *cmzn_sceneviewer_id;

/**
 * Scene viewer GL blending mode.
 */
enum cmzn_sceneviewer_blending_mode
{
	CMZN_SCENEVIEWER_BLENDING_MODE_INVALID = 0,
	CMZN_SCENEVIEWER_BLENDING_MODE_NORMAL,
		/*!< src=GL_SRC_ALPHA and dest=GL_ONE_MINUS_SRC_ALPHA */
	CMZN_SCENEVIEWER_BLENDING_MODE_NONE,
		/*!<  */
	CMZN_SCENEVIEWER_BLENDING_MODE_TRUE_ALPHA
		/*!< Available for OpenGL version 1.4 and above:
		 * src=GL_SRC_ALPHA and dest=GL_ONE_MINUS_SRC_ALPHA for rgb,
		 * src=GL_ONE and dest=GL_ONE_MINUS_SRC_ALPHA for alpha,
		 * which results in the correct final alpha value in a saved image. */
};

/**
 * Describes the buffering mode of the scene viewer.
 * The special modes RENDER_OFFSCREEN_AND_COPY and RENDER_OFFSCREEN_AND_BLEND
 * are used when an OpenGL context cannot be activated directly on the supplied
 * window, such as when the graphics are to be composited by an external program.
 * These are currently only implemeneted for winapi.
 * The graphics will be drawn offscreen and only rendered on screen when
 * requested. The COPY version will overwrite any existing pixels when drawing
 * and the BLEND version will use the alpha channel of the rendered scene to
 * blend itself with the existing pixels.
 */
enum cmzn_sceneviewer_buffering_mode
{
	CMZN_SCENEVIEWER_BUFFERING_MODE_INVALID = 0,
	/*!< Invalid scene viewer buffering mode. */
	CMZN_SCENEVIEWER_BUFFERING_MODE_DEFAULT,
	/*!< BUFFERING_MODE_DEFAULT will mean that with SINGLE_BUFFER or DOUBLE_BUFFER mode may
	 * be selected depending on the other requirements of the scene viewer. */
	CMZN_SCENEVIEWER_BUFFERING_MODE_SINGLE,
	/*!< allow you a greater colour depth or
	 * other features unavailable on a single buffer scene viewer. */
	CMZN_SCENEVIEWER_BUFFERING_MODE_DOUBLE,
	/*!< allows the graphics to be drawn offscreen before being displayed all at once,
	 * reducing the apparent flicker. */
	CMZN_SCENEVIEWER_BUFFERING_MODE_RENDER_OFFSCREEN_AND_COPY,
	/*!< A special modes, used when an OpenGL context cannot be activated directly
	 * on the supplied window, such as when the graphics are to be composited by an
	 * external program. These are currently only implemeneted for winapi.
	 * The graphics will be drawn offscreen and only rendered on screen when
	 * requested. This mode will overwrite any existing pixels when drawing.
	 */
	CMZN_SCENEVIEWER_BUFFERING_MODE_RENDER_OFFSCREEN_AND_BLEND
	/*!< Similar to RENDER_OFFSCREEN_AND_COPY, however instead of overwriting existing pixels,
	 * this mode will use the alpha channel of the rendered scene to
	 * blend itself with the existing pixels.
	 */
};

/**
 * Controls the way the mouse and keyboard are used to interact with the scene viewer.
 */
enum cmzn_sceneviewer_interact_mode
{
	CMZN_SCENEVIEWER_INTERACT_MODE_INVALID = 0,
	/*!< Unspecified scene viewer interaction mode. */
	CMZN_SCENEVIEWER_INTERACT_MODE_STANDARD,
		/*!< CMZN_SCENEVIEWER_INTERACT_MODE_STANDARD is the traditional cmgui mode.
		 *   Rotate: Left mouse button
		 *   Translate: Middle mouse button
		 *   Zoom: Right mouse button */
	CMZN_SCENEVIEWER_INTERACT_MODE_2D
		/*!< CMZN_SCENEVIEWER_INTERACT_MODE_2D is a mode more suitable for 2D use
		 *   Translate: Left mouse button
		 *   Rotate: Middle mouse button
		 *   Zoom: Right mouse button */
};

/**
 * Specifies the sort of projection matrix used to render the 3D scene.
 */
enum cmzn_sceneviewer_projection_mode
{
	CMZN_SCENEVIEWER_PROJECTION_MODE_INVALID = 0,
	/*!< Unspecified scene viewer projection mode. */
	CMZN_SCENEVIEWER_PROJECTION_MODE_PARALLEL,
	/*!< Parallel (orthographic) projection transformations will be used when drawing. */
	CMZN_SCENEVIEWER_PROJECTION_MODE_PERSPECTIVE
	/*!< Perspective  projection transformations will be used when drawing. */
};

/**
 * Specifies whether a STEREO capable scene viewer is required.  This will
 * have to work in cooperation with your window manager and hardware.
 */
enum cmzn_sceneviewer_stereo_mode
{
	CMZN_SCENEVIEWER_STEREO_MODE_INVALID = 0,
	/*!< Unspecified scene viewer stereo mode. */
	CMZN_SCENEVIEWER_STEREO_MODE_DEFAULT,
	/*!< either STEREO or MONO depending on other scene viewer requirements */
	CMZN_SCENEVIEWER_STEREO_MODE_MONO,
	/*!< Normal 2-D Monoscopic display */
	CMZN_SCENEVIEWER_STEREO_MODE_STEREO
	/*!< Stereoscopic display */
};

/**
 * Controls the way partially transparent objects are rendered in scene viewer.
 */
enum cmzn_sceneviewer_transparency_mode
{
	CMZN_SCENEVIEWER_TRANSPARENCY_MODE_INVALID = 0,
	/*!< Unspecified scene viewer transparency mode. */
	CMZN_SCENEVIEWER_TRANSPARENCY_MODE_FAST = 1,
	/*!< CMZN_CMZN_SCENEVIEWER_TRANSPARENCY_MODE_FAST just includes
	 * transparent objects in the normal render, this causes them
	 * to obscure other objects behind if they are drawn first.
	 */
	CMZN_SCENEVIEWER_TRANSPARENCY_MODE_SLOW = 2,
	/*!< CMZN_CMZN_SCENEVIEWER_TRANSPARENCY_MODE_SLOW puts out all the
	 * opaque geometry first and then ignores the depth test while
	 * drawing all partially transparent objects, this ensures everything
	 * is drawn but multiple layers of transparency will always draw
	 * on top of each other which means a surface that is behind another
	 * may be drawn over the top of one that is supposed to be in front.
	 */
	CMZN_SCENEVIEWER_TRANSPARENCY_MODE_ORDER_INDEPENDENT = 3
	/*!< CMZN_CMZN_SCENEVIEWER_TRANSPARENCY_MODE_ORDER_INDEPENDENT uses
	 * some Nvidia extensions to implement a full back to front perl pixel
	 * fragment sort correctly rendering transparency with a small number
	 * of passes, specified by "transparency layers". This uses all the
	 * texturing resources of the current Nvidia hardware and so
	 * no materials used in the scene can contain textures.
	 */
};

/**
 * Specifies the behaviour of the NDC coordinates with respect to the size of the
 * viewport.
 */
enum cmzn_sceneviewer_viewport_mode
{
	CMZN_SCENEVIEWER_VIEWPORT_MODE_INVALID = 0,
	/*!< Unspecified scene viewer viewport mode. */
	CMZN_SCENEVIEWER_VIEWPORT_MODE_ABSOLUTE,
		/*!< viewport_pixels_per_unit values are used to give and exact mapping from
		 *   user coordinates to pixels. */
	CMZN_SCENEVIEWER_VIEWPORT_MODE_RELATIVE,
		/*!< the intended viewing volume is made as large as possible in the physical
		 *   viewport while maintaining the aspect ratio from NDC_width and NDC_height. */
	CMZN_SCENEVIEWER_VIEWPORT_MODE_DISTORTING_RELATIVE
		/*!< the intended viewing volume is made as large as possible in the physical
		 *   viewport, and the aspect ratio may be changed. */
};

/**
 * @brief Manages individual user notification of changes with a scene viewer.
 *
 * Manages individual user notification of changes with a scene viewer.
 */
struct cmzn_sceneviewernotifier;
typedef struct cmzn_sceneviewernotifier *cmzn_sceneviewernotifier_id;

/**
 * @brief Information about changes to fields and other objects in the scene viewer.
 *
 * Information about changes to fields and other objects in the scene viewer,
 * sent with each callback from the sceneviewer notifier.
 */
struct cmzn_sceneviewerevent;
typedef struct cmzn_sceneviewerevent *cmzn_sceneviewerevent_id;

typedef void (*cmzn_sceneviewernotifier_callback_function)(
	cmzn_sceneviewerevent_id event, void *client_data);

/**
 * Bit flags summarising changes to fields in the field module.
 */
enum cmzn_sceneviewerevent_change_flag
{
	CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_NONE = 0,
		/*!< object not changed */
	CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_REPAINT_REQUIRED = 1,
		/*!< repaint required evenet */
	CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_TRANSFORM = 2,
		/*!< sceneviewer receive input */
	CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_FINAL = 32768
		/*!< sceneviewer is currently being destroyed */
};

/**
 * Type for passing logical OR of #cmzn_sceneviewerevent_change_flag
 */
typedef int cmzn_sceneviewerevent_change_flags;

#endif
