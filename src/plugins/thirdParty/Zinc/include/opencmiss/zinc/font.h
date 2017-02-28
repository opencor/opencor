/**
 * @file font.h
 *
 * Public interface to font class for rendering text with graphics.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FONT_H__
#define CMZN_FONT_H__

#include "types/fontid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the font module with reference count
 * incremented.
 *
 * @param fontmodule  The font module to obtain a new handle to.
 * @return  New handle to font module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_fontmodule_id cmzn_fontmodule_access(
	cmzn_fontmodule_id fontmodule);

/**
 * Destroys handle to the font module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param fontmodule_address  Address of handle to font module
 *   to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_fontmodule_destroy(
	cmzn_fontmodule_id *fontmodule_address);

/**
 * Create and return a new font.
 *
 * @param fontmodule  The handle to the font module the
 * font will belong to.
 * @return  Handle to new font, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_font_id cmzn_fontmodule_create_font(
	cmzn_fontmodule_id fontmodule);

/**
 * Begin caching or increment cache level for this font module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end change after completing changes.
 * Can be nested.
 * @see cmzn_fontmodule_end_change
 *
 * @param fontmodule  The font module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_fontmodule_begin_change(cmzn_fontmodule_id fontmodule);

/**
 * Decrement cache level or end caching of changes for the font module.
 * Call fontmodule begin change before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_fontmodule_begin_change
 *
 * @param fontmodule  The font module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_fontmodule_end_change(cmzn_fontmodule_id fontmodule);

/**
 * Find the font with the specified name, if any.
 *
 * @param fontmodule  font module to search.
 * @param name  The name of the font.
 * @return  Handle to font, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_font_id cmzn_fontmodule_find_font_by_name(
	cmzn_fontmodule_id fontmodule, const char *name);

/**
 * Get the default font, if any.
 *
 * @param fontmodule  font module to query.
 * @return  Handle to font, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_font_id cmzn_fontmodule_get_default_font(
	cmzn_fontmodule_id fontmodule);

/**
 * Set the default font.
 *
 * @param fontmodule  font module to modify
 * @param font  The font to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_fontmodule_set_default_font(
	cmzn_fontmodule_id fontmodule,
	cmzn_font_id font);

/**
 * Get new handle to font, incrementing reference count.
 *
 * @param font  handle to the "to be access" zinc font.
 * @return  New handle to font, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_font_id cmzn_font_access(cmzn_font_id font);

/**
 * Destroy the graphics font.
 *
 * @param font_address  address to the handle to the "to be destroyed"
 *   zinc graphics font.
 * @return  status CMZN_OK if successfully destroy graphics font, any other value
 * on failure.
 */
ZINC_API int cmzn_font_destroy(cmzn_font_id *font_address);

/**
 * Return an allocated string containing font name.
 *
 * @param spectrum  handle to the zinc graphics font.
 * @return  allocated string containing font name, otherwise NULL. Up to
 * caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_font_get_name(cmzn_font_id font);

/**
 * Set/change name for <font>.
 *
 * @param font  The handle to zinc graphics font.
 * @param name  name to be set to the font
 * @return  status CMZN_OK if successfully set/change name for font,
 * any other value on failure.
 */
ZINC_API int cmzn_font_set_name(cmzn_font_id font, const char *name);

/**
 * Get the True Type typeface type of the font.
 *
 * @param font  The font to query.
 * @return  The enumerated typeface type, or CMZN_FONT_TYPEFACE_TYPE_INVALID on error.
 */
ZINC_API enum cmzn_font_typeface_type cmzn_font_get_typeface_type(cmzn_font_id font);

/**
 * Set the True Type typeface type of the font.
 *
 * @param font  The font to modify.
 * @param typeface_type  The enumerator of the TrueType typeface to use for font.
 * @return  status CMZN_OK if successfully set the true type for font,
 * any other value on failure.
 */
ZINC_API int cmzn_font_set_typeface_type(cmzn_font_id font,
	enum cmzn_font_typeface_type typeface_type);

/**
 * Get the render type of the given font.
 *
 * @param font  The font to query.
 * @return The render type of font, or CMZN_FONT_RENDER_TYPE_INVALID on error.
 */
ZINC_API enum cmzn_font_render_type cmzn_font_get_render_type(
	cmzn_font_id font);

/**
 * Set the rendering type of the given font.
 *
 * @param font  The handle to zinc graphics font.
 * @param render_type  the render type to use for font.
 * @return  status CMZN_OK if successfully set the font type,
 * any other value on failure.
 */
ZINC_API int cmzn_font_set_render_type(cmzn_font_id font,
	enum cmzn_font_render_type render_type);

/**
 * Query whether the font is bold.
 *
 * @param font  The font to query.
 * @return  Boolean true if font is bold, otherwise false.
 */
ZINC_API bool cmzn_font_is_bold(cmzn_font_id font);

/**
 * Set whether the font is bold.
 *
 * @param font  The font to modify.
 * @param bold  New state of bold flag, true = bold, false = normal.
 * @return  On success CMZN_OK, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_font_set_bold(cmzn_font_id font, bool bold);

/**
 * Get the depth for extrude font type.
 *
 * @param font  The handle to zinc graphics font.
 * @return  depth of the font.
 */
ZINC_API double cmzn_font_get_depth(cmzn_font_id font);

/**
 * Set the depth for extrude font type.
 *
 * @param font  The handle to zinc graphics font.
 * @param depth  depth of the font to be set.
 * @return  status CMZN_OK if successfully set depth for font,
 * any other value on failure.
 */
ZINC_API int cmzn_font_set_depth(cmzn_font_id font, double depth);

/**
 * Query whether the font is italic.
 *
 * @param font  The font to query.
 * @return  Boolean true if font is italic, otherwise false.
 */
ZINC_API bool cmzn_font_is_italic(cmzn_font_id font);

/**
 * Set whether the font is italic.
 *
 * @param font  The font to modify.
 * @param italic  New state of italic flag, true = italic, false = normal.
 * @return  On success CMZN_OK, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_font_set_italic(cmzn_font_id font, bool italic);

/**
 * Get the point size for the font.
 *
 * @param font  The handle to zinc graphics font.
 * @return  Point size of the font.
 */
ZINC_API int cmzn_font_get_point_size(cmzn_font_id font);

/**
 * Set the point size for font type.
 *
 * @param font  The handle to zinc graphics font.
 * @param point_size  Point size of the font to be set, > 0.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_font_set_point_size(cmzn_font_id font, int point_size);

#ifdef __cplusplus
}
#endif

#endif
