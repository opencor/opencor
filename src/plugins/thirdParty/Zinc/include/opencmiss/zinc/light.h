/**
 * @file light.h
 *
 * Public interface to light objects. These describe vertices will be lit in
 * scene_viewer.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_LIGHT_H__
#define CMZN_LIGHT_H__

#include "types/lightid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the light module with reference count
 * incremented.
 *
 * @param lightmodule  Handle to light module.
 * @return  New handle to light module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_lightmodule_id cmzn_lightmodule_access(
	cmzn_lightmodule_id lightmodule);

/**
 * Destroys this handle to the light module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param lightmodule_address  Address of handle to light module
 *   to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_lightmodule_destroy(
	cmzn_lightmodule_id *lightmodule_address);

/**
 * Create a new light, initialised with default settings.
 * Following creation, the caller is expected to set the type and settings
 * such as colour, direction and/or position as appropriate for the type.
 *
 * @param lightmodule  The light module the new light will belong to.
 * @return  Handle to new light, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_light_id cmzn_lightmodule_create_light(
	cmzn_lightmodule_id lightmodule);

/**
 * Create a light iterator object for iterating through the
 * lights in the light module, in alphabetical order of name. The
 * iterator initially points at the position before the first light, so
 * the first call to the iterator next() method returns the first light
 * and advances the iterator. The iterator becomes invalid if lights are
 * added, removed or renamed while in use.
 * @see cmzn_lightiterator_next
 *
 * @param lightmodule  Handle to the light module whose
 * lights are to be iterated over.
 * @return  Handle to light iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_lightiterator_id cmzn_lightmodule_create_lightiterator(
	cmzn_lightmodule_id lightmodule);

/**
 * Begin caching or increment cache level for this light module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end_change after completing changes.
 * Can be nested.
 * @see cmzn_lightmodule_end_change
 *
 * @param lightmodule  The light module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_lightmodule_begin_change(cmzn_lightmodule_id lightmodule);

/**
 * Decrement cache level or end caching of changes for the light module.
 * Call light module begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_lightmodule_begin_change
 *
 * @param lightmodule  The light module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_lightmodule_end_change(cmzn_lightmodule_id lightmodule);

/**
 * Find the light with the specified name, if any.
 *
 * @param lightmodule  cmzn_light module to search.
 * @param name  The name of the light.
 * @return  Handle to light, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_light_id cmzn_lightmodule_find_light_by_name(
	cmzn_lightmodule_id lightmodule, const char *name);

/**
 * Get the default light to be used in sceneviewer. If there is none,
 * a default directional light is automatically created with RGB value of
 * [0.8, 0.8, 0.8] and [0.0, -0.5, -1.0] for its direction i.e. into the
 * screen and slightly down.
 * Note that the colour of this light and the default ambient light sum to 1.0
 * to ensure colours of graphics are not oversaturated with default settings.
 *
 * @param lightmodule  cmzn_light module to query.
 * @return  Handle to default light, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_light_id cmzn_lightmodule_get_default_light(
	cmzn_lightmodule_id lightmodule);

/**
 * Set the default light, this default light will be used by any
 * sceneviewer created afterward. Typically this is expected to be a
 * directional light; not expected to be of ambient type.
 *
 * @param lightmodule  cmzn_light module to modify.
 * @param light  The light to set as the default light.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_lightmodule_set_default_light(
	cmzn_lightmodule_id lightmodule, cmzn_light_id light);

/**
 * Get the default ambient light to be used in new scene viewers. If none,
 * one is automatically created with RGB colour value of [0.2, 0.2, 0.2].
 * Note that the colour of this light and the default light sum to 1.0 to
 * ensure colours of graphics are not oversaturated with default settings.
 *
 * @param lightmodule  cmzn_light module to query.
 * @return  Handle to default ambient light, or NULL/invalid handle if
 * none or failed.
 */
ZINC_API cmzn_light_id cmzn_lightmodule_get_default_ambient_light(
	cmzn_lightmodule_id lightmodule);

/**
 * Set the default ambient light, this default ambient light will be
 * used by any scene viewers created afterward.
 *
 * @param lightmodule  cmzn_light module to modify.
 * @param light  The light to set as default ambient light.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_lightmodule_set_default_ambient_light(
	cmzn_lightmodule_id lightmodule, cmzn_light_id light);

/**
 * Returns a new handle to the light with reference count incremented.
 *
 * @param light  Handle to light.
 * @return  New handle to light, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_light_id cmzn_light_access(cmzn_light_id light);

/**
 * Destroys handle to the light (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param light_address  The address to the handle of the light
 *    to be destroyed.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_destroy(cmzn_light_id *light_address);

/**
 * Get managed status of light in its owning light module.
 * @see cmzn_light_set_managed
 *
 * @param light  The light to query.
 * @return  True if light is managed, otherwise false.
 */
ZINC_API bool cmzn_light_is_managed(cmzn_light_id light);

/**
 * Set managed status of light in its owning light module.
 * If true (managed) the light will remain indefinitely in the light
 * module even if no external references are held.
 * If false (unmanaged) the light will be automatically removed from the
 * module when no longer referenced externally, effectively marking it as
 * pending destruction.
 * All new objects are unmanaged unless stated otherwise.
 *
 * @param light  The light to modify.
 * @param value  The new value for the managed flag: true or false.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_light_set_managed(cmzn_light_id light, bool value);

/**
 * Return an allocated string containing light name.
 *
 * @param light  handle to the cmzn light.
 * @return  allocated string containing light name, or NULL on failure.
 * Up to caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_light_get_name(cmzn_light_id light);

/**
 * Set/change name for light. Must be unique in the light module.
 *
 * @param light  The handle to zinc light.
 * @param name  name to be set to the light
 * @return  status CMZN_OK if successfully set/change name for light,
 * any other value on failure.
 */
ZINC_API int cmzn_light_set_name(cmzn_light_id light, const char *name);

/**
 * Returns the value of the constant attenuation. Attenuation is the loss
 * of light intensity over a distance. It only applies to spot and point lights.
 *
 * attenuation factor: 1 / (c + k * l + k * q * q) where c, l, q and k is
 * constant, linear, quadratic attenuation and distance respectively.
 *
 * @param light  The light to query.
 * @return  Value of the constant attenuation on success, 0 on failure.
 */
ZINC_API double cmzn_light_get_constant_attenuation(cmzn_light_id light);

/**
 * Set the value of the constant attenuation. Attenuation is the loss
 * of light intensity over a distance. It only applies to spot and point lights.
 *
 * attenuation factor: 1 / (c + k * l + k * q * q) where c, l, q and k is
 * constant, linear, quadratic attenuation and distance respectively.
 *
 * @param light  The light to modify.
 * @param constant_attenuation  Non-negative float representing the value
 *   of constant attenuation to be set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_light_set_constant_attenuation(cmzn_light_id light,
	double constant_attenuation);

/**
 * Returns the value of the linear attenuation. Attenuation is the loss
 * of light intensity over a distance. It only applies to spot and point lights.
 *
 * attenuation factor: 1 / (c + k * l + k * q * q) where c, l, q and k is
 * constant, linear, quadratic attenuation and distance respectively.
 *
 * @param light  The light to query.
 * @return  Value of the linear attenuation on success, 0 on failure.
 */
ZINC_API double cmzn_light_get_linear_attenuation(cmzn_light_id light);

/**
 * Set the value of the linear attenuation. Attenuation is the loss
 * of light intensity over a distance. It only applies to spot and point lights.
 *
 * attenuation factor: 1 / (c + k * l + k * q * q) where c, l, q and k is
 * constant, linear, quadratic attenuation and distance respectively.
 *
 * @param light  The light to modify.
 * @param linear_attenuation  non-negative float representing the value
 *   of linear attenuation to be set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_light_set_linear_attenuation(cmzn_light_id light,
	double linear_attenuation);

/**
 * Returns the value of the quadratic attenuation. Attenuation is the loss
 * of light intensity over a distance. It only applies to spot and point lights.
 *
 * attenuation factor: 1 / (c + k * l + k * q * q) where c, l, q and k is
 * constant, linear, quadratic attenuation and distance respectively.
 *
 * @param light  The light to query.
 * @return  Value of the quadratic attenuation on success, 0 on failure.
 */
ZINC_API double cmzn_light_get_quadratic_attenuation(cmzn_light_id light);

/**
 * Set the value of the quadratic attenuation. Attenuation is the loss
 * of light intensity over a distance. It only applies to spot and point lights.
 *
 * attenuation factor: 1 / (c + k * l + k * q * q) where c, l, q and k is
 * constant, linear, quadratic attenuation and distance respectively.
 *
 * @param light  The light to modify.
 * @param quadratic_attenuation  Non-negative float representing the value
 *   of quadratic attenuation to be set.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_light_set_quadratic_attenuation(cmzn_light_id light,
	double quadratic_attenuation);

/**
 * Gets the type of the light: AMBIENT, DIRECTIONAL, POINT or SPOT.
 *
 * @param light  The light to query.
 * @return  type of the light, otherwise CMZN_LIGHT_TYPE_INVALID.
 */
ZINC_API enum cmzn_light_type cmzn_light_get_type(cmzn_light_id light);

/**
 * Sets the type of the light: AMBIENT, DIRECTIONAL, POINT or SPOT.
 *
 * @param light  The light to modify.
 * @param light_type  type to be set for the specified light.
 *
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_light_set_type(cmzn_light_id light,
	enum cmzn_light_type light_type);

/**
 * Get the 3 component real vector giving the RGB colour of the light.
 *
 * @param light  The light to query.
 * @param colour  Values of the colour will be output into this
 *   3 component vector. It should be allocated with enough space
 *   for 3 components.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_get_colour_rgb(cmzn_light_id light, double *colour);

/**
 * Set the colour of the light using a 3 component vector of the RGB values.
 * For ambient light type, the colour applies to ambient lighting calculations,
 * blending with the ambient colour of materials.
 * For all other light types, the colour applies to diffuse and specular
 * lighting, blending with materials' diffuse and specular colours.
 *
 * @param light  The light to modify.
 * @param colour  Values of the colour to be set, it must be a 3 component
 *   vector storing the RGB value. Colour values range from 0.0 to 1.0 (max).
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_set_colour_rgb(cmzn_light_id light, const double *colour);

/**
 * Get the 3 component vector giving the direction of the light. It
 * only applies to infinite and spot lights.
 *
 * @param light  The light to query.
 * @param direction  Values of the direction will be output into this
 *   3 component vector. It should be allocated with enough space
 *   for 3 components.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_get_direction(cmzn_light_id light,
	double *direction);

/**
 * Set the direction of the light using a 3 component vector. It
 * only applies to infinite and spot lights.
 *
 * @param light  The light to modify.
 * @param direction  Values of the direction to set, must be a 3 component
 *   vector storing the direction value.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_set_direction(cmzn_light_id light,
	const double *direction);

/**
 * Get the 3 component vector giving the position of the light. It
 * only applies to point and spot lights.
 *
 * @param light  The light to query.
 * @param position  Values of the position will be output into this
 *   3 component vector. It should be allocated with enough space
 *   for 3 components.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_get_position(cmzn_light_id light, double *position);

/**
 * Set the position of the light using a 3 component vector. It
 * only applies to point and spot lights.
 *
 * @param light  The light to modify.
 * @param position  Values of the position to set, must be a 3 component
 *   vector storing the position value.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_set_position(cmzn_light_id light,
	const double *position);

/**
 * Gets the spotlight cutoff angle in degrees from 0 to 90.
 * It only applies to spot light.
 *
 * @param light  The light to query.
 * @return  Value of the spot cutoff on success, 0 on failure.
 */
ZINC_API double cmzn_light_get_spot_cutoff(cmzn_light_id light);

/**
 * Sets the spotlight cutoff angle in degrees from 0 to 90.
 * It only applies to spot light.
 *
 * @param light  The light to modify.
 * @param spot_cutoff  New value to set, the value must be between
 *   0.0 and 90.0.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_set_spot_cutoff(cmzn_light_id light,
	double spot_cutoff);

/**
 * Gets the spotlight exponent which controls how concentrated the spotlight
 * becomes as one approaches its axis. A value of 0.0 gives even illumination
 * throughout the cutoff angle. It only applies to spot light.
 *
 * @param light  The light to query.
 * @return  Value of the spot exponent on success, 0 on failure.
 */
ZINC_API double cmzn_light_get_spot_exponent(cmzn_light_id light);

/**
 * Sets the spotlight exponent which controls how concentrated the spotlight
 * becomes as one approaches its axis. A value of 0.0 gives even illumination
 * throughout the cutoff angle. It only applies to spot light.
 *
 * @param light  The light to modify.
 * @param spot_exponent  New spot exponent value to be set for light.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_light_set_spot_exponent(cmzn_light_id light,
	double spot_exponent);

/**
 * Returns a new handle to the iterator with reference count incremented.
 *
 * @param iterator  The light iterator to obtain a new handle to.
 * @return  New handle to light iterator, or NULL/invalid handle on
 * failure.
 */
ZINC_API cmzn_lightiterator_id cmzn_lightiterator_access(
	cmzn_lightiterator_id iterator);

/**
 * Destroys this handle to the light iterator and sets it to NULL.
 *
 * @param iterator_address  Address of handle to light_iterator to
 * destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_lightiterator_destroy(cmzn_lightiterator_id *iterator_address);

/**
 * Returns a handle to the next light in the container being iterated
 * over then advances the iterator position. The caller is required to destroy
 * the returned light handle.
 *
 * @param iterator  Light iterator to query and advance.
 * @return  Handle to the next light, or NULL/invalid handle if none or
 * failed.
 */
ZINC_API cmzn_light_id cmzn_lightiterator_next(cmzn_lightiterator_id iterator);

#ifdef __cplusplus
}
#endif

#endif
