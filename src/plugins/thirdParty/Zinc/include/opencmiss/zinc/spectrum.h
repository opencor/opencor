/**
 * @file spectrum.h
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SPECTRUM_H__
#define CMZN_SPECTRUM_H__

#include "types/scenefilterid.h"
#include "types/sceneid.h"
#include "types/spectrumid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the spectrum module with reference count
 * incremented.
 *
 * @param spectrummodule  Handle to spectrum module.
 * @return  New handle to spectrum module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrummodule_id cmzn_spectrummodule_access(
	cmzn_spectrummodule_id spectrummodule);

/**
 * Destroys handle to the spectrum module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param spectrummodule_address  Address of handle to spectrum module
 *   to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrummodule_destroy(
	cmzn_spectrummodule_id *spectrummodule_address);

/**
 * Create and return a new spectrum.
 *
 * @param spectrummodule  The handle to the spectrum module the
 * spectrum will belong to.
 * @return  Handle to new spectrum, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrum_id cmzn_spectrummodule_create_spectrum(
	cmzn_spectrummodule_id spectrummodule);

/**
 * Create a spectrum iterator object for iterating through the
 * spectrums in the spectrum module, in alphabetical order of name. The
 * iterator initially points at the position before the first spectrum, so
 * the first call to the iterator next() method returns the first spectrum
 * and advances the iterator. The iterator becomes invalid if spectrums are
 * added, removed or renamed while in use.
 * @see cmzn_spectrumiterator_next
 *
 * @param spectrummodule  Handle to the spectrum module whose
 * spectrums are to be iterated over.
 * @return  Handle to spectrum iterator, or NULL/invalid handle on failure.
  */
ZINC_API cmzn_spectrumiterator_id cmzn_spectrummodule_create_spectrumiterator(
	cmzn_spectrummodule_id spectrummodule);

/**
 * Begin caching or increment cache level for this spectrum module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end_change after completing changes.
 * Can be nested.
 * @see cmzn_spectrummodule_end_change
 *
 * @param spectrummodule  The spectrum module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrummodule_begin_change(cmzn_spectrummodule_id spectrummodule);

/**
 * Decrement cache level or end caching of changes for the spectrum module.
 * Call spectrum module begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_spectrummodule_begin_change
 *
 * @param spectrummodule  The spectrum module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrummodule_end_change(cmzn_spectrummodule_id spectrummodule);

/**
 * Find the spectrum with the specified name, if any.
 *
 * @param spectrummodule  spectrum module to search.
 * @param name  The name of the spectrum.
 * @return  Handle to spectrum, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_spectrum_id cmzn_spectrummodule_find_spectrum_by_name(
	cmzn_spectrummodule_id spectrummodule, const char *name);

/**
 * Get the default spectrum, if any. By default, a single component spectrum
 * with COLOUR_MAPPING_TYPE_RAINBOW is returned.
 * @see cmzn_spectrummodule_set_default_spectrum
 * @see cmzn_spectrumcomponent_colour_mapping_type
 *
 * @param spectrummodule  spectrum module to query.
 * @return  Handle to default spectrum, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_spectrum_id cmzn_spectrummodule_get_default_spectrum(
	cmzn_spectrummodule_id spectrummodule);

/**
 * Set the default spectrum.
 *
 * @param spectrummodule  spectrum module to modify
 * @param spectrum  The spectrum to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrummodule_set_default_spectrum(
	cmzn_spectrummodule_id spectrummodule, cmzn_spectrum_id spectrum);

/**
 * Read the json description to the spectrum module. This will change
 * the spectrums in the spectrum module.
 *
 * @param spectrummodule  Handle to the spectrum module.
 * @description  The string containing json description
 * @return  CMZN_OK on success, otherwise ERROR status.
 */
ZINC_API int cmzn_spectrummodule_read_description(
	cmzn_spectrummodule_id spectrummodule, const char *description);

/**
 * Write the json file describing the spectrums in the spectrum module, which can
 * be used to store the current spectrum settings.
 *
 * @param spectrummodule  Handle to the spectrum module.
 * @return  c string containing the json description of spectrum module, otherwise 0;
 */
ZINC_API char *cmzn_spectrummodule_write_description(cmzn_spectrummodule_id spectrummodule);

/**
 * Get new handle to spectrum. Increments the reference count.
 *
 * @param spectrum  Handle to spectrum.
 * @return  New handle to spectrum, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrum_id cmzn_spectrum_access(cmzn_spectrum_id spectrum);

/**
 * Destroy the spectrum.
 *
 * @param spectrum  address to the handle to the "to be destroyed"
 *   zinc spectrum.
 * @return  status CMZN_OK if successfully destroy spectrum, any other value
 * on failure.
 */
ZINC_API int cmzn_spectrum_destroy(cmzn_spectrum_id *spectrum);

/**
 * Get if a spectrum is managed. See cmzn_spectrum_set_managed for
 * more information about managed.
 *
 *  @param spectrum  The spectrum to get the managed value from.
 *  @return 1 if spectrum is managed, otherwise false.
 */
ZINC_API bool cmzn_spectrum_is_managed(cmzn_spectrum_id spectrum);

/**
 * When the managed status is 0 (default) spectrum is destroyed when no longer
 * in use, i.e. when number of external references to it drops to
 * zero. Set to 1 to manage spectrum object indefinitely, or until this
 * attribute is reset to zero, effectively marking it as pending destruction.
 *
 * @param spectrum  Handle to the zinc spectrum.
 * @param value  The new value for the managed flag: true or false.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrum_set_managed(cmzn_spectrum_id spectrum, bool value);

/**
 * Return an allocated string containing spectrum name.
 *
 * @param spectrum  handle to the zinc spectrum.
 * @return  allocated string containing spectrum name, otherwise NULL. Up to
 * caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_spectrum_get_name(cmzn_spectrum_id spectrum);

/**
 * Set/change name for <spectrum>.
 *
 * @param spectrum  The handle to zinc graphical spectrum.
 * @param name  name to be set to the spectrum
 * @return  status CMZN_OK if successfully set/change name for spectrum,
 * any other value on failure.
 */
ZINC_API int cmzn_spectrum_set_name(cmzn_spectrum_id spectrum, const char *name);


/**
 * Get the overwrite material flag for spectrum. When overwrite flag is true,
 * the spectrum will clear any material rgba on primitive before applying
 * its own rgba. If the flag is false, rgba from the spectrum will be added
 * to the primitives on top of the one given by material.
 *
 * @param spectrum  Handle to a spectrum object.
 * @return  true if overwrite flag is set for spectrum, false if the flag
 * 	is not set or on failure.
 */
ZINC_API bool cmzn_spectrum_is_material_overwrite(cmzn_spectrum_id spectrum);

/**
 * Set the overwrite material flag for spectrum.
 *
 * @param spectrum  Handle to a spectrum object.
 *
 * @return  CMZN_OK if successfully set, any other value on failure.
 */
ZINC_API int cmzn_spectrum_set_material_overwrite(cmzn_spectrum_id spectrum, bool overwrite);

/**
 * Call this function before making multiple changes on the spectrum, to stop
 * spectrum from  notifying clients about every change. Call spectrum end change
 * method afterwards to send notifications of change made. Can be nested.
 * @see cmzn_spectrum_end_change
 *
 * @param scene  The handle to the spectrum.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrum_begin_change(cmzn_spectrum_id spectrum);

/**
 * Call spectrum begin change before making multiple changes on the spectrum,
 * and this function afterwards to send a single notification of any changes
 * to clients.
 * @see cmzn_spectrum_begin_change
 *
 * @param spectrum  The handle to the spectrum.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrum_end_change(cmzn_spectrum_id spectrum);

/**
 * Returns the number of components in spectrum.
 *
 * @param spectrum  The handle to the spectrum
 * @return  Returns the number of components in spectrum.
 */
ZINC_API int cmzn_spectrum_get_number_of_spectrumcomponents(cmzn_spectrum_id spectrum);

/**
 * Create a component for spectrum. Used to colour graphics.
 *
 * @param spectrum  Handle to spectrum the spectrum component is created in.
 * @return  Handle to new spectrum component, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrumcomponent_id cmzn_spectrum_create_spectrumcomponent(
	cmzn_spectrum_id spectrum);

/**
 * Get the first spectrum component on the spectrum list of <component>.

 * @param spectrum  Handle to a spectrum object.
 * @return  Handle to first spectrum component, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_spectrumcomponent_id cmzn_spectrum_get_first_spectrumcomponent(
	cmzn_spectrum_id spectrum);

/**
 * Get the next spectrum component after ref_component from list in spectrum.

 * @param spectrum  Handle to a spectrum object.
 * @param ref_component  Handle to a spectrum component object.
 * @return  Handle to next spectrum component, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_spectrumcomponent_id cmzn_spectrum_get_next_spectrumcomponent(
	cmzn_spectrum_id spectrum, cmzn_spectrumcomponent_id ref_component);

/**
 * Get the component before <ref_component> on the components list of <spectrum>.

 * @param spectrum  Handle to a spectrum object.
 * @param ref_component  Handle to a spectrum component object.
 * @return  Handle to previous spectrum component, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_spectrumcomponent_id cmzn_spectrum_get_previous_spectrumcomponent(
	cmzn_spectrum_id spectrum, cmzn_spectrumcomponent_id ref_component);

/**
 * Move an existing component in spectrum before ref_component. Both <component> and
 * <ref_component> must be from the same spectrum.
 *
 * @param spectrum  The handle to the spectrum.
 * @param component  spectrum component to be moved.
 * @param ref_component  <component> will be moved into the current position of this
 * 		spectrum component.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrum_move_spectrumcomponent_before(
	cmzn_spectrum_id spectrum, cmzn_spectrumcomponent_id component,
	cmzn_spectrumcomponent_id ref_component);

/**
 * Removes <component> from <spectrum> and decrements the position
 * of all subsequent graphics.
 *
 * @param spectrum  The handle to the spectrum of which the component is removed
 *   from.
 * @param component  The handle to a spectrum component object which will be
 *   removed from the spectrum.
 * @return  Status CMZN_OK if successfully remove component from spectrum,
 * any other value on failure.
 */
ZINC_API int cmzn_spectrum_remove_spectrumcomponent(cmzn_spectrum_id spectrum,
	cmzn_spectrumcomponent_id component);

/**
 * Removes all components from the spectrum.
 *
 * @param spectrum  The handle to the spectrum of which the component is removed
 *   from.
 * @return  Status CMZN_OK if successfully remove all components from spectrum,
 * any other value on failure.
 */
ZINC_API int cmzn_spectrum_remove_all_spectrumcomponents(cmzn_spectrum_id spectrum);

/**
 * Set the minimum and maximum value for the specified spectrum to
 * match the minimum and maximum field value in scene's grpahics
 * which are coloured with the specified spectrum.
 *
 * @param spectrum  Handle to the spectrum.
 * @param scene  scene to get the autorange data from.
 * @param  filter filter for the scene
 * @return  Status CMZN_OK if successfully autorange, any other value
 * on failure.
 */
ZINC_API int cmzn_spectrum_autorange(cmzn_spectrum_id spectrum,
	cmzn_scene_id scene, cmzn_scenefilter_id filter);

/**
 * Returns a new handle to the spectrum component with reference count
 * incremented.
 *
 * @param component  Handle to spectrum component.
 * @return  New handle to spectrum component, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrumcomponent_id cmzn_spectrumcomponent_access(
	cmzn_spectrumcomponent_id component);

/**
 * Destroys the spectrum component handle and resets it to NULL.
 *
 * @param component_address  Address of handle to the spectrum component.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_destroy(
	cmzn_spectrumcomponent_id *component_address);

/**
 * Get the minimum value of the range this spectrum component will
 * lookup to on targeted field. Primitives with this field value will
 * display the minimum colour value of the component colour type
 *
 * @param component  The handle of the spectrum component.
 * @return  value of range minimum on success.
 */
ZINC_API double cmzn_spectrumcomponent_get_range_minimum(
	cmzn_spectrumcomponent_id component);

/**
 * Set the minimum value of the range this spectrum component will
 * lookup to on targeted field. Primitives with this field value will
 * display the minimum colour value of the component colour type
 *
 * @param component  The handle of the spectrum component.
 * @value  the value to be set for range minimum
 *
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_range_minimum(
	cmzn_spectrumcomponent_id component,	double value);

/**
 * Get the maximum value of the range this spectrum component will
 * lookup to on targeted field. Primitives with this field value will
 * display the maximum colour value of the component colour type
 *
 * @param component  The handle of the spectrum component.
 * @return  value of range maximum on success.
 */
ZINC_API double cmzn_spectrumcomponent_get_range_maximum(
	cmzn_spectrumcomponent_id component);

/**
 * Set the maximum value of the range this spectrum component will
 * lookup to on targeted field. Primitives with this field value will
 * display the maximum colour value of the component colour type
 *
 * @param component  The handle of the spectrum component.
 * @value  the value to be set for range maximum
 *
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_range_maximum(
	cmzn_spectrumcomponent_id component,	double value);

/**
 * Get the colour value mapped to the minimum spectrum component data range.
 * This is a normalised value from 0.0 to 1.0.
 *
 * @param component  The handle to the spectrum component.
 * @return  Minimum colour value from 0.0 to 1.0, or 0.0 if invalid component.
 */
ZINC_API double cmzn_spectrumcomponent_get_colour_minimum(
	cmzn_spectrumcomponent_id component);

/**
 * Set the colour value mapped to the minimum spectrum component data range.
 * This is a normalised value from 0.0 to 1.0, mapping to a position in the
 * colour map. For simple colour mappings red, green, blue and alpha, this
 * gives the exact colour component value at the minimum data range, which is
 * interpolated to the maximum colour at the maximum data range.
 * Note that setting 'reverse' colours reverses the association of colour range
 * and data range; it is recommended the colour minimum and maximum be swapped
 * instead of using reverse.
 * @see cmzn_spectrumcomponent_set_colour_reverse
 * Changing this value does not affect COLOUR_MAPPING_TYPE_BANDED which is
 * only affected by the number and ratio of bands.
 *
 * @param component  The handle to the spectrum component.
 * @value  The minimum colour value to set, from 0.0 to 1.0.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_colour_minimum(
	cmzn_spectrumcomponent_id component, double value);

/**
 * Get the colour value mapped to the maximum spectrum component data range.
 * This is a normalised value from 0.0 to 1.0.
 *
 * @param component  The handle to the spectrum component.
 * @return  Maximum colour value from 0.0 to 1.0, or 0.0 if invalid component.
 */
ZINC_API double cmzn_spectrumcomponent_get_colour_maximum(
	cmzn_spectrumcomponent_id component);

/**
 * Set the colour value mapped to the maximum spectrum component data range.
 * This is a normalised value from 0.0 to 1.0, mapping to a position in the
 * colour map. For simple colour mappings red, green, blue and alpha, this
 * gives the exact colour component value at the maximum data range, which is
 * interpolated to the minimum colour at the minimum data range.
 * Note that setting 'reverse' colours reverses the association of colour range
 * and data range; it is recommended the colour minimum and maximum be swapped
 * instead of using reverse.
 * Changing this value does not affect COLOUR_MAPPING_TYPE_BANDED which is
 * only affected by the number and ratio of bands.
 *
 * @param component  The handle to the spectrum component.
 * @value  The maximum colour value to set, from 0.0 to 1.0.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_colour_maximum(
	cmzn_spectrumcomponent_id component, double value);

/**
 * Get the step value of a spectrum component. The step spectrum
 * defines the boundary between the red and blue colour of the
 * COLOUR_MAPPING_TYPE_STEP spectrum compomemt.
 *
 * @param component  The handle of the spectrum component.
 *
 * @return  step value of the spectrum component on success.
 */
ZINC_API double cmzn_spectrumcomponent_get_step_value(
	cmzn_spectrumcomponent_id component);

/**
 * Set the step value of a spectrum component. The step spectrum
 * defines the boundary between the red and blue colour of the
 * COLOUR_MAPPING_TYPE_STEP spectrum component.
 *
 * @param component  The handle of the spectrum component.
 * @value  the step value to be set
 *
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_step_value(
	cmzn_spectrumcomponent_id component,	double value);

/**
 * Get the value which alters the colour progression when scale type
 * is set to SCALE_TYPE_LOG.
 * @see CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_LOG
 *
 * @param component  The handle of the spectrum component.
 * @return  The exaggeration value of the spectrum, or zero on failure.
 */
ZINC_API double cmzn_spectrumcomponent_get_exaggeration(
	cmzn_spectrumcomponent_id component);

/**
 * Set the value which alters the colour progression when scale type
 * is set to SCALE_TYPE_LOG
 * @see CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_LOG
 *
 * @param component  The handle of the spectrum component.
 * @param value  The exaggeration value to be set.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_exaggeration(
	cmzn_spectrumcomponent_id component,	double value);

/**
 * Get the value determining the proportion of band present on each section, number of
 * sections in a spectrum is determined by number of bands, value must be larger
 * than 0.0 and must not exceed 1.0.
 *
 * @param component  The handle of the spectrum component.
 *
 * @return  banded ratio of the spectrum component on success.
 */
ZINC_API double cmzn_spectrumcomponent_get_banded_ratio(
	cmzn_spectrumcomponent_id component);

/**
 * Set the value determining the proportion of band present on each section, number of
 * sections in a spectrum is determined by number of bands, value must be larger
 * than 0.0 and must not exceed 1.0.
 *
 * @param component  The handle of the spectrum component.
 * @value  the banded ratio to be set
 *
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_banded_ratio(
	cmzn_spectrumcomponent_id component,	double value);

/**
 * Get the active state of a spectrum component, only active spectrum component
 * will be rendered
 *
 * @param component  Handle to the zinc spectrum component.
 * @return  true if spectrum component is active, false if
 * failed or spectrum component is not active
 */
ZINC_API bool cmzn_spectrumcomponent_is_active(
	cmzn_spectrumcomponent_id component);

/**
 * Set the active state of a spectrum component, only active spectrum component
 * will be rendered
 *
 * @param component  Handle to the zinc spectrum component.
 * @param active  Value to be set to the zinc spectrum component.
 * @return  CMZN_OK if value is set successfully, any other value if
 * failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_active(
	cmzn_spectrumcomponent_id component, bool active);

/**
 * Get the reverse flag of a spectrum component, reverse spectrum component will
 * have the colour rendered reversely.
 *
 * @param component  Handle to the zinc spectrum component.
 * @return  true if spectrum component is reverse, false if
 * 	failed or spectrum component is not reverse
 */
ZINC_API bool cmzn_spectrumcomponent_is_colour_reverse(
	cmzn_spectrumcomponent_id component);

/**
 * Set the reverse flag of a spectrum component, which if set maps the colour
 * maximum to the data range minimum, and the colour minimum to the data range
 * maximum. This is primarily intended to reverse the rainbow colour map.
 * With most colour maps, swapping the colour minimum and maximum values is
 * easier to explain and achieves the same result.
 *
 * @param component  Handle to the zinc spectrum component.
 * @param reverse  Value to be set to the zinc spectrum component.
 * @return  CMZN_OK if value is set successfully, any other value if
 * failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_colour_reverse(
	cmzn_spectrumcomponent_id component, bool reverse);

/**
 * Get the 'extend above' flag of a spectrum component, which controls
 * whether values above the maximum are rendered for this component.
 * @see cmzn_spectrumcomponent_set_extend_above
 *
 * @param component  Handle to the zinc spectrum component.
 * @return  true if spectrum component extends above, false if
 * 	failed or spectrum component does not extend above
 */
ZINC_API bool cmzn_spectrumcomponent_is_extend_above(
	cmzn_spectrumcomponent_id component);

/**
 * Set the 'extend above' flag of a spectrum component.
 * When on (true), values above the maximum are rendered with the same colour
 * as the maximum value. When off, no colour is set for this component for
 * values above the maximum value.
 * By default, extend above is on (true).
 *
 * @param component  Handle to the zinc spectrum component.
 * @param extend_above  Value to be set to the zinc spectrum component.
 * @return  CMZN_OK if value is set successfully, any other value if
 * failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_extend_above(
	cmzn_spectrumcomponent_id component, bool extend_above);

/**
 * Get the 'extend below' flag of a spectrum component, which controls
 * whether values below the minimum are rendered for this component.
 * @see cmzn_spectrumcomponent_set_extend_below
 *
 * @param component  Handle to the zinc spectrum component.
 * @return  true if spectrum component extends below, false if
 * 	failed or spectrum component does not extend below
 */
ZINC_API bool cmzn_spectrumcomponent_is_extend_below(
	cmzn_spectrumcomponent_id component);

/**
 * Set the 'extend below' flag of a spectrum component.
 * When on (true), values below the minimum are rendered with the same colour
 * as the minimum value. When off, no colour is set for this component for
 * values below the minimum value.
 * By default, extend below is on (true).
 *
 * @param component  Handle to the zinc spectrum component.
 * @param extend_below  Value to be set to the zinc spectrum component.
 * @return  CMZN_OK if value is set successfully, any other value if
 * failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_extend_below(
	cmzn_spectrumcomponent_id component, bool extend_below);

/**
 * Get the field component lookup number of a spectrum component, this value
 * determines which of the field component this spectrum component will look up on.
 *
 * @see cmzn_graphics_set_data_field
 *
 * @param component  Handle to the zinc spectrum component.
 * @return  positive integer of the field component number to look up to.
 *   Any other value if failed or value is not set correctly.
 */
ZINC_API int cmzn_spectrumcomponent_get_field_component(
	cmzn_spectrumcomponent_id component);

/**
 * Set the field component lookup number of a spectrum component, this value
 * determines which of the field component this spectrum component will look up on.
 *
 * @see cmzn_graphics_set_data_field
 *
 * @param component  Handle to the zinc spectrum component.
 * @param component_number  field component number for this spectrum to lookup.
 *
 * @return  CMZN_OK if value is set successfully, any other value if
 * failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_field_component(
	cmzn_spectrumcomponent_id component,	int component_number);

/**
 * Query whether this component's data range maximum is marked as fixed.
 * @see cmzn_spectrumcomponent_set_fix_maximum
 *
 * @param component  The spectrum component to query.
 * @return  Boolean true if the maximum is fixed, otherwise false.
 */
ZINC_API bool cmzn_spectrumcomponent_is_fix_maximum(
	cmzn_spectrumcomponent_id component);

/**
 * Set hint for whether to fix this component's data range maximum when
 * fitting the spectrum ranges to the range of data in the model visualisation.
 * If true, the maximum should be fixed at its current value. If false, it can
 * change e.g. scale in proportion to other data ranges.
 * This attribute is metadata i.e. it does not affect the spectrum result.
 * Its default value is false i.e. the maximum is not fixed.
 *
 * @param component  The spectrum component to modify.
 * @param fix_maximum  The new state of the fix maximum flag.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_fix_maximum(
	cmzn_spectrumcomponent_id component, bool fix_maximum);

/**
 * Query whether this component's data range minimum is marked as fixed.
 * @see cmzn_spectrumcomponent_set_fix_minimum
 *
 * @param component  The spectrum component to query.
 * @return  Boolean true if the minimum is fixed, otherwise false.
 */
ZINC_API bool cmzn_spectrumcomponent_is_fix_minimum(
	cmzn_spectrumcomponent_id component);

/**
 * Set hint for whether to fix this component's data range minimum when
 * fitting the spectrum ranges to the range of data in the model visualisation.
 * If true, the minimum should be fixed at its current value. If false, it can
 * change e.g. scale in proportion to other data ranges.
 * This attribute is metadata i.e. it does not affect the spectrum result.
 * Its default value is false i.e. the minimum is not fixed.
 *
 * @param component  The spectrum component to modify.
 * @param fix_minimum  The new state of the fix minimum flag.
 * @return  CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumcomponent_set_fix_minimum(
	cmzn_spectrumcomponent_id component, bool fix_minimum);

/**
 * Get the number of bands this component contains within its range in
 * CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_BANDED
 *
 * @param component  Handle to the zinc spectrum component.
 *
 * @return  positive integer of nuymber of bands set for this components.
 *   Any other value if failed or value is not set correctly
 */
ZINC_API int cmzn_spectrumcomponent_get_number_of_bands(cmzn_spectrumcomponent_id component);

/**
 * Set the number of bands this component contains within its range in
 * CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_BANDED
 *
 * @param component  Handle to the zinc spectrum component.
 * @param number of bands  number of bands for this component
 *
 * @return  CMZN_OK if value is set successfully, any other value if
 * failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_number_of_bands(cmzn_spectrumcomponent_id component,
	int number_of_bands);

/**
 * Get the interpolation_mode of this component.
 *
 * @param component  Handle to the zinc spectrum component.
 *
 * @return  interpolation_mode set for this spectrum.
 *   CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_INVALID if failed or
 *   mode is not set correctly
 */
ZINC_API enum cmzn_spectrumcomponent_scale_type
	cmzn_spectrumcomponent_get_scale_type(cmzn_spectrumcomponent_id component);

/**
 * Set the interpolation_mode of this component.
 *
 * @param component  Handle to the zinc spectrum component.
 * @param interpolation_mode  Interpolation mode to be set for spectrum component
 *
 * @return  CMZN_OK if value is set successfully, any other value if
 * 	failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_scale_type(
	cmzn_spectrumcomponent_id component,
	enum cmzn_spectrumcomponent_scale_type scale_type);

/**
 * Convert a short attribute name into an enum if the attribute name matches
 * any of the members in the enum.
 *
 * @param attribute_name  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_spectrumcomponent_colour_mapping_type
	cmzn_spectrumcomponent_colour_mapping_type_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param attribute  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_spectrumcomponent_colour_mapping_type_enum_to_string(
	enum cmzn_spectrumcomponent_colour_mapping_type component_colour);

/**
 * Get the colour_mapping_type of this component.
 *
 * @param component  Handle to the zinc spectrum component.
 *
 * @return  colour_mapping_type of the spectrum component.
 *   CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_INVALID if failed or
 *   mode is not set correctly.
 */
ZINC_API enum cmzn_spectrumcomponent_colour_mapping_type
cmzn_spectrumcomponent_get_colour_mapping_type(cmzn_spectrumcomponent_id component);

/**
 * Set the colour_mapping_type of this component.
 *
 * @param component  Handle to the zinc spectrum component.
 * @param colour_mapping_type  colour_mapping_type to be set for spectrum component
 *
 * @return  CMZN_OK if value is set successfully, any other value if
 * 	failed.
 */
ZINC_API int cmzn_spectrumcomponent_set_colour_mapping_type(
	cmzn_spectrumcomponent_id component,	enum cmzn_spectrumcomponent_colour_mapping_type type);

/**
 * Returns a new handle to the iterator with reference count incremented.
 *
 * @param iterator  The spectrum iterator to obtain a new handle to.
 * @return  New handle to spectrum iterator, or NULL/invalid handle on
 * failure.
 */
ZINC_API cmzn_spectrumiterator_id cmzn_spectrumiterator_access(
	cmzn_spectrumiterator_id iterator);

/**
 * Destroys this handle to the spectrum iterator and sets it to NULL.
 *
 * @param iterator_address  Address of handle to spectrumiterator to
 * destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrumiterator_destroy(cmzn_spectrumiterator_id *iterator_address);

/**
 * Returns a handle to the next spectrum in the container being iterated
 * over then advances the iterator position. The caller is required to destroy
 * the returned spectrum handle.
 *
 * @param iterator  Spectrum iterator to query and advance.
 * @return  Handle to the next spectrum, or NULL/invalid handle if none or
 * failed.
 */
ZINC_API cmzn_spectrum_id cmzn_spectrumiterator_next(cmzn_spectrumiterator_id iterator);

/**
 * Create a notifier for getting callbacks for changes to the spectrums in the
 * spectrum module.
 *
 * @param spectrummodule  Handle to the spectrum module to get notifications for.
 * @return  Handle to new spectrum module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrummodulenotifier_id cmzn_spectrummodule_create_spectrummodulenotifier(
	cmzn_spectrummodule_id spectrummodule);

/**
 * Returns a new handle to the spectrum module notifier with reference count
 * incremented.
 *
 * @param notifier  The spectrum module notifier to obtain a new handle to.
 * @return  New handle to spectrum module notifier, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrummodulenotifier_id cmzn_spectrummodulenotifier_access(
	cmzn_spectrummodulenotifier_id notifier);

/**
 * Destroys handle to the spectrum module notifier and sets it to NULL.
 * Internally this decrements the reference count.
 *
 * @param notifier_address  Address of spectrum module notifier handle to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_spectrummodulenotifier_destroy(cmzn_spectrummodulenotifier_id *notifier_address);

/**
 * Stop and clear spectrum module callback. This will stop the callback and also
 * remove the callback function from the spectrummodule notifier.
 *
 * @param notifier  Handle to the spectrummodule notifier.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrummodulenotifier_clear_callback(cmzn_spectrummodulenotifier_id notifier);

/**
 * Assign the callback function and user data for the spectrummodule notifier.
 * This function also starts the callback.
 *
 * @see cmzn_spectrummodulenotifier_callback_function
 * @param notifier  Handle to the spectrummodule notifier.
 * @param function  function to be called when event is triggered.
 * @param user_data_in  Void pointer to user object. User must ensure this
 * object's lifetime exceeds the duration for which callbacks are active.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrummodulenotifier_set_callback(cmzn_spectrummodulenotifier_id notifier,
	cmzn_spectrummodulenotifier_callback_function function, void *user_data_in);

/**
 * Get the user data set when establishing the callback.
 * @see cmzn_spectrummodulenotifier_set_callback
 *
 * @see cmzn_spectrummodulenotifier_set_callback
 * @param notifier  Handle to the spectrum module notifier.
 * @return  user data or NULL on failure or not set.
 */
ZINC_API void *cmzn_spectrummodulenotifier_get_callback_user_data(
 cmzn_spectrummodulenotifier_id notifier);

/**
 * Returns a new handle to the spectrummodule event with reference count incremented.
 *
 * @param event  The spectrum module event to obtain a new handle to.
 * @return  New handle to spectrum module event, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_spectrummoduleevent_id cmzn_spectrummoduleevent_access(
	cmzn_spectrummoduleevent_id event);

/**
 * Destroys this handle to the spectrummodule event and sets it to NULL.
 * Internally this decrements the reference count.
 * Note: Do not destroy the event argument passed to the user callback function.
 *
 * @param event_address  Address of spectrum module event handle to destroy.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_spectrummoduleevent_destroy(cmzn_spectrummoduleevent_id *event_address);

/**
 * Get logical OR of flags indicating how spectrums in the spectrum module have changed.
 * @see cmzn_spectrum_change_flag
 *
 * @param event  Handle to the spectrum module event.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_spectrum_change_flag values.
 */
ZINC_API cmzn_spectrum_change_flags cmzn_spectrummoduleevent_get_summary_spectrum_change_flags(
	cmzn_spectrummoduleevent_id event);

/**
 * Get logical OR of flags indicating how the spectrum has changed.
 * @see cmzn_spectrum_change_flag
 *
 * @param event  Handle to the spectrum module event to query.
 * @param spectrum  The spectrum to query about.
 * @return  The change flags summarising the change: logical OR of
 * enum cmzn_spectrum_change_flag values. Returns
 * CMZN_SPECTRUM_CHANGE_FLAG_NONE in case of invalid arguments.
 */
ZINC_API cmzn_spectrum_change_flags cmzn_spectrummoduleevent_get_spectrum_change_flags(
	cmzn_spectrummoduleevent_id event, cmzn_spectrum_id spectrum);


#ifdef __cplusplus
}
#endif

#endif
