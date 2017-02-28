/**
 * @file glyph.h
 *
 * Public interface to glyph static graphics objects used to visualise
 * points in the scene.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_GLYPH_H__
#define CMZN_GLYPH_H__

#include "types/glyphid.h"
#include "types/graphicsid.h"
#include "types/materialid.h"
#include "types/spectrumid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the glyph module with reference count
 * incremented.
 *
 * @param glyphmodule  The glyph module to obtain a new handle to.
 * @return  New handle to glyph module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphmodule_id cmzn_glyphmodule_access(
	cmzn_glyphmodule_id glyphmodule);

/**
 * Destroys handle to the glyph module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param glyphmodule_address  Address of handle to glyph module to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyphmodule_destroy(
	cmzn_glyphmodule_id *glyphmodule_address);

/**
 * Begin caching or increment cache level for this glyph module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end_change after completing changes.
 * Can be nested.
 * @see cmzn_glyphmodule_end_change
 *
 * @param glyphmodule  The glyph module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyphmodule_begin_change(cmzn_glyphmodule_id glyphmodule);

/**
 * Decrement cache level or end caching of changes for the glyph module.
 * Call glyphmodule begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_glyphmodule_begin_change
 *
 * @param glyphmodule  The glyph module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_glyphmodule_end_change(cmzn_glyphmodule_id glyphmodule);

/**
 * Create a glyph iterator object for iterating through the glyphs in the
 * glyph module, in alphabetical order of name. The iterator initially
 * points at the position before the first glyph, so the first call to the
 * glyph iterator next() method returns the first glyph and advances the
 * iterator. Iterator becomes invalid if glyphs are added, removed or
 * renamed while in use.
 * @see cmzn_glyphiterator_next
 *
 * @param glyphmodule  Handle to the glyph module whose glyphs are to
 * be iterated over.
 * @return  Handle to glyph iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphiterator_id cmzn_glyphmodule_create_glyphiterator(
	cmzn_glyphmodule_id glyphmodule);

/**
 * Defines a selection of standard glyphs for visualising points, vectors etc.
 * Graphics for all standard glyphs fit in a unit cube which:
 * 1. for orientable glyphs e.g. line, arrow, cylinder: span [0,1] on axis 1,
 *    and [-0.5,0.5] on axes 2 and 3 (except line which has no width).
 * 2. are otherwise centred at 0,0,0 for all other glyphs.
 * These consist of:
 * "arrow", "arrow_solid" = line and solid arrows from (0,0,0) to (1,0,0) with
 *     arrowhead 1/3 of length and unit width. Shaft diameter is 1/3 unit.
 * "axis", "axis_solid" = variants of arrows with arrowhead 1/10 of length.
 *     Shaft diameter is 1/3 unit.
 * "cone", "cone_solid" = cone descending from unit diameter circle in 2-3 plane
 *      at (0,0,0) to a point at (1,0,0). Solid variant has base closed.
 * "cross" = lines from -0.5 to +0.5 on each axis through (0,0,0).
 * "cube_solid", "cube_wireframe" = solid and wireframe (line) unit cubes
 *     aligned with primary axes and centred at (0,0,0).
 * "cylinder", "cylinder_solid" = a unit diameter cylinder, centre line from
 *     (0,0,0) to (1,0,0). Solid variant has ends closed.
 * "diamond" = an octohedron created as a sphere with 4 divisions around, so
 *     lit like a sphere rather than faceted.
 * "line" = a line from (0,0,0) to (1,0,0).
 * "point" = a single pixel at (0,0,0).
 * "sheet" = a unit square surface in 1-2 plane, centred at (0,0,0).
 * "sphere" = a unit diameter sphere centred at (0,0,0).
 * In addition there are 8 glyphs showing unit 3-D axes with various labels:
 * "axes", "axes_123", "axes_xyz", "axes_colour" - using glyph "axis" and
 * axis width 0.1.
 * "axes_solid", "axes_solid_123", "axes_solid_xyz", "axes_solid_colour"
 * - using glyph "arrow_solid" and axis width 0.25.
 * Note that the colour axes find standard materials "red", "green" and "blue"
 * for the 3 axes but are not created if those materials are not defined.
 * @see cmzn_materialmodule_define_standard_materials
 * All the above standard glyphs also have unique types enumeration.
 * @see cmzn_glyph_shape_type
 * Note if any glyphs of the predefined name already exist prior to calling this
 * function, the standard glyph is not created.
 * All glyphs created by this function have IS_MANAGED set to 1.
 * Circular glyphs including sphere, cylinders, cones and solid arrow and axis
 * get their circle divisions from the tessellation specified in the graphics.
 * If not already set, the default glyph is set to "point" by this function.
 *
 * @param glyphmodule  The glyph module to create the glyph in.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyphmodule_define_standard_glyphs(
	cmzn_glyphmodule_id glyphmodule);

/**
 * Find the glyph with the specified name, if any.
 *
 * @param glyphmodule  Glyph module to search.
 * @param name  The name of the glyph.
 * @return  Handle to glyph, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_glyph_id cmzn_glyphmodule_find_glyph_by_name(
	cmzn_glyphmodule_id glyphmodule, const char *name);

/**
 * Find the glyph with the specified type, if any.
 *
 * @param glyphmodule  Glyph module to search.
 * @param glyph_type  A glyph type identifier.
 * @return  Handle to glyph, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_glyph_id cmzn_glyphmodule_find_glyph_by_glyph_shape_type(
	cmzn_glyphmodule_id glyphmodule, enum cmzn_glyph_shape_type glyph_type);

/**
 * Get the default glyph used for new point graphics, if any.
 *
 * @param glyphmodule  Glyph module to query.
 * @return  Handle to glyph, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_glyph_id cmzn_glyphmodule_get_default_point_glyph(
	cmzn_glyphmodule_id glyphmodule);

/**
 * Set the default glyph used for new point graphics.
 *
 * @param glyphmodule  Glyph module to modify.
 * @param glyph  The glyph to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyphmodule_set_default_point_glyph(
	cmzn_glyphmodule_id glyphmodule, cmzn_glyph_id glyph);

/**
 * Returns a new handle to the glyph with reference count incremented.
 *
 * @param glyph  The glyph to obtain a new handle to.
 * @return  New handle to glyph, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyph_id cmzn_glyph_access(cmzn_glyph_id glyph);

/**
 * Destroys handle to the glyph (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param glyph_address  The address to the handle of the glyph to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_glyph_destroy(cmzn_glyph_id *glyph_address);

/**
 * Get managed status of glyph in its owning glyph module.
 * @see cmzn_glyph_set_managed
 *
 * @param glyph  The glyph to query.
 * @return  true if glyph is managed, otherwise 0 false.
 */
ZINC_API bool cmzn_glyph_is_managed(cmzn_glyph_id glyph);

/**
 * Set managed status of glyph in its owning glyph module.
 * If set (managed) the glyph will remain indefinitely in the glyph module even
 * if no external references are held.
 * If not set (unmanaged) the glyph will be automatically removed from the
 * module when no longer referenced externally, effectively marking it as
 * pending destruction.
 * All new objects are unmanaged unless stated otherwise.
 *
 * @param glyph  The glyph to modify.
 * @param value  The new value for the managed flag: true or false.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_set_managed(cmzn_glyph_id glyph, bool value);

/**
 * Return an allocated string containing glyph name.
 *
 * @param glyph  The glyph to query.
 * @return  Allocated string containing glyph name, or NULL on failure.
 * Up to caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_glyph_get_name(cmzn_glyph_id glyph);

/**
 * Set name of the glyph.
 *
 * @param glyph  The glyph to modify.
 * @param name  Name to be set for the glyph.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_glyph_set_name(cmzn_glyph_id glyph, const char *name);

/**
 * Create a glyph which drawing 3-D axes repeating the supplied axis glyph
 * on three axes with the given axis width.
 *
 * @param glyphmodule  The glyph module to create the glyph in.
 * @param axis_glyph  A glyph to repeat on each of 3 axes.
 * @param axis_width  The width of each axis glyph when drawn at unit length;
 * a fraction of 1 to give an appropriate arrow head size. Typically 0.1 for
 * glyph 'axis', 0.25 for glyph 'arrow_solid'. Must be non-negative.
 * @return  Handle to new glyph, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyph_id cmzn_glyphmodule_create_glyph_axes(
	cmzn_glyphmodule_id glyphmodule, cmzn_glyph_id axis_glyph,
	double axis_width);

/**
 * If the glyph is type axes, returns the type-specific handle.
 *
 * @param glyph  The glyph to be cast.
 * @return  Handle to derived axes glyph, or NULL/invalid handle if wrong type
 * or failed.
 */
ZINC_API cmzn_glyph_axes_id cmzn_glyph_cast_axes(cmzn_glyph_id glyph);

/**
 * Cast axes glyph back to the base glyph type and return it.
 * IMPORTANT NOTE: Returned glyph does not have incremented reference count and
 * must not be destroyed. Use cmzn_glyph_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the axes glyph argument.
 *
 * @param axes  Handle to the axes glyph to cast.
 * @return  Non-accessed handle to the base glyph or NULL if failed.
 */
ZINC_C_INLINE cmzn_glyph_id cmzn_glyph_axes_base_cast(cmzn_glyph_axes_id axes)
{
	return (cmzn_glyph_id)(axes);
}

/**
 * Destroys handle to the axes glyph (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param axes_address  Address of handle to the axes glyph.
 * @return  Status CMZN_OK if successfully destroyed the axes glyph handle,
 * any other value on failure.
 */
ZINC_API int cmzn_glyph_axes_destroy(cmzn_glyph_axes_id *axes_address);

/**
 * Gets the width of each axis relative to unit length.
 *
 * @param axes  The axes glyph to query.
 * @return  The axis width, or 0.0 if error.
 */
ZINC_API double cmzn_glyph_axes_get_axis_width(cmzn_glyph_axes_id axes);

/**
 * Sets the width of each axis relative to unit length.
 * The default axis width is 0.1 i.e. 10% of the length and suitable for
 * glyph 'axis' and 'axis_solid'.
 *
 * @param axes  The axes glyph to modify.
 * @param axis_width  The new axis width. Must be non-negative.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_axes_set_axis_width(cmzn_glyph_axes_id axes,
	double axis_width);

/**
 * Get the label to be drawn at the end of the given axis.
 *
 * @param axes  The axes glyph to query.
 * @param axis_number  The axis number from 1 to 3.
 * @return  Allocated string containing label, or NULL if none or error.
 * Up to caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_glyph_axes_get_axis_label(cmzn_glyph_axes_id axes,
	int axis_number);

/**
 * Set the label to be drawn at the end of the given axis.
 *
 * @param axes  The axes glyph to modify.
 * @param axis_number  The axis number from 1 to 3.
 * @param label  The label, or NULL for none.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_axes_set_axis_label(cmzn_glyph_axes_id axes,
	int axis_number, const char *label);

/**
 * Get the material an axis is drawn with.
 *
 * @param axes  The axes glyph to query.
 * @param axis_number  The axis number from 1 to 3.
 * @return  Handle to material, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_material_id cmzn_glyph_axes_get_axis_material(
	cmzn_glyph_axes_id axes, int axis_number);

/**
 * Set the material an axis is drawn with. Note if the material is NULL for
 * any axis the default material for the graphics is used for that axis.
 *
 * @param axes  The axes glyph to modify.
 * @param axis_number  The axis number from 1 to 3.
 * @param material  The material, or NULL to use the default material from the
 * graphics.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_axes_set_axis_material(cmzn_glyph_axes_id axes,
	int axis_number, cmzn_material_id material);

/**
 * Create a glyph which draws a colour bar for the spectrum with ticks and
 * value labels. The glyph dynamically updates to match the current range and
 * definition of the spectrum. Note it only shows a single component.
 *
 * @param glyphmodule  The glyph module to create the glyph in.
 * @param spectrum  The spectrum to be displayed on the colour bar.
 * @return  Handle to new glyph, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyph_id cmzn_glyphmodule_create_glyph_colour_bar(
	cmzn_glyphmodule_id glyphmodule, cmzn_spectrum_id spectrum);

/**
 * If the glyph is type colour bar, returns the type-specific handle.
 *
 * @param glyph  The glyph to be cast.
 * @return  Handle to derived colour bar glyph, or NULL/invalid handle if wrong
 * type or failed.
 */
ZINC_API cmzn_glyph_colour_bar_id cmzn_glyph_cast_colour_bar(cmzn_glyph_id glyph);

/**
 * Cast colour bar glyph back to the base glyph type and return it.
 * IMPORTANT NOTE: Returned glyph does not have incremented reference count and
 * must not be destroyed. Use cmzn_glyph_access() to add a reference if
 * maintaining returned handle beyond the lifetime of the colour_bar glyph argument.
 *
 * @param colour_bar  Handle to the colour_bar glyph to cast.
 * @return  Non-accessed handle to the base glyph or NULL if failed.
 */
ZINC_C_INLINE cmzn_glyph_id cmzn_glyph_colour_bar_base_cast(cmzn_glyph_colour_bar_id colour_bar)
{
	return (cmzn_glyph_id)(colour_bar);
}

/**
 * Destroys handle to the colour_bar glyph (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param colour_bar_address  Address of handle to the colour_bar glyph.
 * @return  Status CMZN_OK if successfully destroyed the colour_bar glyph handle,
 * any other value on failure.
 */
ZINC_API int cmzn_glyph_colour_bar_destroy(cmzn_glyph_colour_bar_id *colour_bar_address);

/**
 * Gets the vector defining the main axis of the colour bar.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @param valuesCount  The size of valuesOut array. Gets maximum of 3 values.
 * @param valuesOut  Array to receive axis vector.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_get_axis(
	cmzn_glyph_colour_bar_id colour_bar, int valuesCount, double *valuesOut);

/**
 * Sets the vector defining the main axis of the colour bar. The magnitude of
 * this vector gives the length of the bar without the extend length.
 * The default axis is (0,1,0) for vertical orientation in window coordinates.
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param valuesCount  The size of valuesIn array. Sets maximum of 3 values.
 * @param valuesIn  Array containing axis vector. If fewer than 3 values then
 * assumes zero for remaining components.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_axis(
	cmzn_glyph_colour_bar_id colour_bar, int valuesCount, const double *valuesIn);

/**
 * Gets the centre position of the colour bar.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @param valuesCount  The size of valuesOut array. Gets maximum of 3 values.
 * @param valuesOut  Array to receive centre position.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_get_centre(
	cmzn_glyph_colour_bar_id colour_bar, int valuesCount, double *valuesOut);

/**
 * Sets the centre position of the colour bar.
 * The default centre is (0,0,0). It is recommended that this not be changed
 * and instead use the graphics point attributes glyph offset.
 * @see cmzn_graphicspointattributes_set_glyph_offset
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param valuesCount  The size of valuesIn array. Sets maximum of 3 values.
 * @param valuesIn  Array containing centre position. If fewer than 3 values
 * then assumes zero for remaining components.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_centre(
	cmzn_glyph_colour_bar_id colour_bar, int valuesCount, const double *valuesIn);

/**
 * Gets the extend length used at each end of the colour bar to show values
 * outside the spectrum range.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @return  The extend length, or 0.0 if error.
 */
ZINC_API double cmzn_glyph_colour_bar_get_extend_length(
	cmzn_glyph_colour_bar_id colour_bar);

/**
 * Sets the extend length used at each end of the colour bar to show values
 * outside the spectrum range.
 * The default extend length is 0.05.
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param extendLength  The new extend length. Must be non-negative.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_extend_length(
	cmzn_glyph_colour_bar_id colour_bar, double extendLength);

/**
 * Gets the number of divisions between labels.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @return  The number of label divisions, or 0 if error.
 */
ZINC_API int cmzn_glyph_colour_bar_get_label_divisions(
	cmzn_glyph_colour_bar_id colour_bar);

/**
 * Sets the number of divisions between labels. This is one less than the
 * number of labels/ticks.
 * The default label divisions is 10.
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param labelDivisions  The new number of divisions, at least 1.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_label_divisions(
	cmzn_glyph_colour_bar_id colour_bar, int labelDivisions);

/**
 * Gets the material used for colour bar labels and ticks. Can be NULL.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @return  Handle to material, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_material_id cmzn_glyph_colour_bar_get_label_material(
	cmzn_glyph_colour_bar_id colour_bar);

/**
 * Sets the material used for colour bar labels and ticks. Can be NULL.
 * Default is none i.e. use the same material as for the colour bar
 * itself, which is supplied by the graphics.
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param material  The new label material; can be NULL to clear.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_label_material(
	cmzn_glyph_colour_bar_id colour_bar, cmzn_material_id material);

/**
 * Get the number format used to write value labels on the colour bar.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @return  Allocated string containing number format, or NULL on failure.
 * Up to caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_glyph_colour_bar_get_number_format(
	cmzn_glyph_colour_bar_id colour_bar);

/**
 * Set the number format used to write value labels on the colour bar.
 * This is a C printf format string with a single numerical format in the form:
 * %[+/-/0][length][.precision](e/E/f/F/g/G)
 * Other characters can be added before or after the number format.
 * Note a literal % is entered by repeating it i.e. %%.
 * The default format is "%+.4e".
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param numberFormat  The printf number format used for value labels.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_number_format(
	cmzn_glyph_colour_bar_id colour_bar, const char *numberFormat);

/**
 * Gets the vector defining the side/tick axis of the colour bar.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @param valuesCount  The size of valuesOut array. Gets maximum of 3 values.
 * @param valuesOut  Array to receive side axis vector.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_get_side_axis(
	cmzn_glyph_colour_bar_id colour_bar, int valuesCount, double *valuesOut);

/**
 * Gets the spectrum shown by the colour bar.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @return  Handle to spectrum, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_spectrum_id cmzn_glyph_colour_bar_get_spectrum(
	cmzn_glyph_colour_bar_id colour_bar);

/**
 * Sets the vector defining the side/tick axis of the colour bar. The magnitude
 * of this vector gives the diameter of the bar.
 * The default side axis is (0.1,0,0) for vertical bar and horizontal ticks in
 * window coordinates.
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param valuesCount  The size of valuesIn array. Sets maximum of 3 values.
 * @param valuesIn  Array containing side axis vector. If fewer than 3 values
 * then assumes zero for remaining components.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_side_axis(
	cmzn_glyph_colour_bar_id colour_bar, int valuesCount, const double *valuesIn);

/**
 * Gets the tick length.
 *
 * @param colour_bar  The colour bar glyph to query.
 * @return  The tick length, or 0.0 if error.
 */
ZINC_API double cmzn_glyph_colour_bar_get_tick_length(
	cmzn_glyph_colour_bar_id colour_bar);

/**
 * Sets the tick length measured from outside radius of the colour bar, in the
 * direction of the side axis.
 * The default tick length is 0.05.
 *
 * @param colour_bar  The colour bar glyph to modify.
 * @param tickLength  The new tick length. Must be non-negative.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyph_colour_bar_set_tick_length(
	cmzn_glyph_colour_bar_id colour_bar, double tickLength);

/**
 * Returns a new handle to the iterator with reference count incremented.
 *
 * @param iterator  The glyph iterator to obtain a new handle to.
 * @return  New handle to glyph iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphiterator_id cmzn_glyphiterator_access(
	cmzn_glyphiterator_id iterator);

/**
 * Destroys this handle to the glyph iterator and sets it to NULL.
 *
 * @param iterator_address  Address of handle to glyph_iterator to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyphiterator_destroy(cmzn_glyphiterator_id *iterator_address);

/**
 * Returns a handle to the next glyph in the container being iterated over then
 * advances the iterator position. The caller is required to destroy the
 * returned glyph handle.
 *
 * @param iterator  Glyph iterator to query and advance.
 * @return  Handle to the next glyph, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_glyph_id cmzn_glyphiterator_next(cmzn_glyphiterator_id iterator);

/**
 * Create and return a handle to the glyph which is a static copy of the vertices
 * of the specified graphics.
 *
 * @param glyphmodule  The glyph module to create the glyph in.
 * @param graphics  Handle to the graphics which vertices will be copied into
 * 	the newly created glyph.
 * @return  Handle to the newly created glyph on success, otherwise 0.
 */
ZINC_API cmzn_glyph_id cmzn_glyphmodule_create_static_glyph_from_graphics(
	cmzn_glyphmodule_id glyphmodule, cmzn_graphics_id graphics);

/**
 * Create a notifier for getting callbacks for changes to the glyphs in the
 * glyph module.
 *
 * @param glyphmodule  Handle to the glyph module to get notifications for.
 * @return  Handle to new glyph module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphmodulenotifier_id cmzn_glyphmodule_create_glyphmodulenotifier(
	cmzn_glyphmodule_id glyphmodule);

/**
 * Returns a new handle to the glyph module notifier with reference count
 * incremented.
 *
 * @param notifier  The glyph module notifier to obtain a new handle to.
 * @return  New handle to glyph module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphmodulenotifier_id cmzn_glyphmodulenotifier_access(
	cmzn_glyphmodulenotifier_id notifier);

/**
 * Destroys handle to the glyph module notifier and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param notifier_address  Address of glyph module notifier handle to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_glyphmodulenotifier_destroy(
	cmzn_glyphmodulenotifier_id *notifier_address);

/**
 * Stop and clear glyph module callback. This will stop the callback and also
 * remove the callback function from the glyphmodule notifier.
 *
 * @param notifier  Handle to the glyphmodule notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_glyphmodulenotifier_clear_callback(
	cmzn_glyphmodulenotifier_id notifier);

/**
 * Assign the callback function and user data for the glyphmodule notifier.
 * This function also starts the callback.
 *
 * @see cmzn_glyphmodulenotifier_callback_function
 * @param notifier  Handle to the glyphmodule notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User must ensure this
 * object's lifetime exceeds the duration for which callbacks are active.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_glyphmodulenotifier_set_callback(
	cmzn_glyphmodulenotifier_id notifier,
	cmzn_glyphmodulenotifier_callback_function function, void *user_data_in);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_glyphmodulenotifier_set_callback
 *
 * @see cmzn_glyphmodulenotifier_set_callback
 * @param notifier  Handle to the glyph module notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_glyphmodulenotifier_get_callback_user_data(
 cmzn_glyphmodulenotifier_id notifier);

/**
 * Returns a new handle to the glyphmodule event with reference count incremented.
 *
 * @param event  The glyph module event to obtain a new handle to.
 * @return  New handle to glyph module event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_glyphmoduleevent_id cmzn_glyphmoduleevent_access(
	cmzn_glyphmoduleevent_id event);

/**
 * Destroys this handle to the glyphmodule event and sets it to NULL.
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param event_address  Address of glyph module event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_glyphmoduleevent_destroy(cmzn_glyphmoduleevent_id *event_address);

/**
 * Get logical OR of flags indicating how glyphs in the glyph module have changed.
 * @see cmzn_glyph_change_flag
 *
 * @param event  Handle to the glyph module event.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_glyph_change_flag values.
 */
ZINC_API cmzn_glyph_change_flags cmzn_glyphmoduleevent_get_summary_glyph_change_flags(
	cmzn_glyphmoduleevent_id event);

/**
 * Get logical OR of flags indicating how the glyph has changed.
 * @see cmzn_glyph_change_flag
 *
 * @param event  Handle to the glyph module event to query.
 * @param glyph  The glyph to query about.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_glyph_change_flag values. Returns
 * CMZN_SPECTRUM_CHANGE_FLAG_NONE in case of invalid arguments.
 */
ZINC_API cmzn_glyph_change_flags cmzn_glyphmoduleevent_get_glyph_change_flags(
	cmzn_glyphmoduleevent_id event, cmzn_glyph_id glyph);

#ifdef __cplusplus
}
#endif

#endif /* ZINC_GLYPH_H */
