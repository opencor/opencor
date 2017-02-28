/**
 * context.h
 *
 * API to access the main root structure of cmgui.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_CONTEXT_H__
#define CMZN_CONTEXT_H__

#include "types/contextid.h"
#include "types/fontid.h"
#include "types/glyphid.h"
#include "types/lightid.h"
#include "types/loggerid.h"
#include "types/materialid.h"
#include "types/regionid.h"
#include "types/scenefilterid.h"
#include "types/sceneviewerid.h"
#include "types/spectrumid.h"
#include "types/tessellationid.h"
#include "types/timekeeperid.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a new cmgui context with an id <id>.
 *
 * @param id  The identifier given to the new context.
 * @return  Handle to new context, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_context_id cmzn_context_create(const char *id);

/**
 * Returns a new handle to the context with reference count incremented.
 *
 * @param context  The context to obtain a new handle to.
 * @return  New handle to context, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_context_id cmzn_context_access(cmzn_context_id context);

/**
 * Destroy a context.
 *
 * @param context_address  The address to the handle of the context
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_context_destroy(cmzn_context_id *context_address);

/**
 * Returns the default region in the context. A convenience for applications that
 * need only one region tree.
 *
 * @param context  Handle to a context object.
 * @return  Handle to region, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_context_get_default_region(cmzn_context_id context);

/**
 * Sets the default region in the context. A convenience for applications that
 * need only one region tree.
 *
 * @param context  Handle to a context object.
 * @param region  The new default region.
 * @return  CMZN_OK on success, otherwise any other status code.
 */
ZINC_API int cmzn_context_set_default_region(cmzn_context_id context,
	cmzn_region_id region);

/**
 * Create a new region and return a handle to it. Use this function to create
 * a region forming the root of an independent region tree. Equivalent to region
 * create region method. Compare with region create child/subregion methods which
 * create and add a region to the tree.
 * @see cmzn_region_create_region
 * @see cmzn_region_create_child
 * @see cmzn_region_create_subregion
 *
 * @param context  Handle to a context object.
 * @return  Handle to new region, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_region_id cmzn_context_create_region(cmzn_context_id context);

/**
 * Get the font module which manages fonts for rendering text in graphics.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the font module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fontmodule_id cmzn_context_get_fontmodule(
	cmzn_context_id context);

/**
 * Get the glyph module which stores static graphics for visualising points,
 * vectors, axes etc. Note on startup no glyphs are defined and glyph module
 * functions need to be called to define standard glyphs.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the glyph module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphmodule_id cmzn_context_get_glyphmodule(
	cmzn_context_id context);

/**
 * Return the light module which manages lights used to calculate the
 * final colour of vertices in combination with material colour.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the light module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_lightmodule_id cmzn_context_get_lightmodule(
	cmzn_context_id context);

/**
 * Return the logger which logs debug messages.
 *
 * @param context  The context to request the logger from.
 * @return  Handle to the logger, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_logger_id cmzn_context_get_logger(cmzn_context_id context);

/**
 * Return the material module which manages materials used to colour, texture
 * and shade graphics. Note on startup only materials "default" and
 * "default_selected" are defined, as white and red, respectively. Additional
 * standard and custom materials can be defined using material module functions.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the material module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_materialmodule_id cmzn_context_get_materialmodule(
	cmzn_context_id context);

/**
 * Get the scene filter module which manages scenefilter objects for filtering
 * contents of scenes with scenepicker and sceneviewer etc.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the scene filter module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_scenefiltermodule_id cmzn_context_get_scenefiltermodule(
	cmzn_context_id context);

/**
 * Returns a handle to a sceneviewer module which manages sceneviewer objects
 * for rendering 3-D scenes into rectangular windows or canvases using OpenGL.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the sceneviewer module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_sceneviewermodule_id cmzn_context_get_sceneviewermodule(
	cmzn_context_id context);

/**
 * Get the spectrum module which manages spectrum objects controlling how
 * graphics data fields are converted into colours.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the spectrum module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrummodule_id cmzn_context_get_spectrummodule(
	cmzn_context_id context);

/**
 * Get the tessellation module which manages objects controlling how curves are
 * approximated by line segments in graphics.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the tesselation module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_tessellationmodule_id cmzn_context_get_tessellationmodule(
	cmzn_context_id context);

/**
 * Get the timekeeper module which manages objects for synchronising time across
 * zinc objects.
 *
 * @param context  The context to request the module from.
 * @return  Handle to the timekeeper module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_timekeepermodule_id cmzn_context_get_timekeepermodule(
	cmzn_context_id context);

/**
 * Get the version number of this Zinc library. It will return the major version,
 * minor version and patch version in a 3 component integer array.
 *
 * @param context  Handle to the context.
 * @param version_out  Array of size 3 to hold the values of the version number,
 * 	they are the major version, minor version and patch version respectively.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_context_get_version(cmzn_context_id context, int *version_out);

/**
 * Get the revision number of this Zinc library.
 *
 * @param context  Handle to the context.
 * @return  Revision of this Zinc library on success, 0 on failure.
 */
ZINC_API const char* cmzn_context_get_revision(cmzn_context_id context);

/**
 * Get the version string of this Zinc library. The version string will
 * be output in the following format
 * "[major version].[minor version].[patch version].r[revision(M)](.Debug)"
 * (M) indicates this binary contains local modifications that are not included
 * in the revision, and (.Debug) indicates this binary is not optimised.
 *
 * @param context  Handle to the context.
 * @return  on success : allocated string containing version details. Up to caller to
 * free using cmzn_deallocate().
 */
ZINC_API char *cmzn_context_get_version_string(cmzn_context_id context);

#ifdef __cplusplus
}
#endif

#endif /* CMZN_CONTEXT_H__ */
