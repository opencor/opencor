/**
 * @file material.h
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_MATERIAL_H__
#define CMZN_MATERIAL_H__

#include "types/fieldid.h"
#include "types/materialid.h"

#include "opencmiss/zinc/zincsharedobject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a new handle to the material module with reference count
 * incremented.
 *
 * @param materialmodule  The material module to obtain a new handle to.
 * @return  New handle to material module, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_materialmodule_id cmzn_materialmodule_access(
	cmzn_materialmodule_id materialmodule);

/**
 * Destroys handle to the material module (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param materialmodule_address  Address of handle to material module
 *   to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_materialmodule_destroy(
	cmzn_materialmodule_id *materialmodule_address);

/**
 * Create and return a new material.
 *
 * @param materialmodule  The handle to the material module the
 * material will belong to.
 * @return  Handle to new material, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_material_id cmzn_materialmodule_create_material(
	cmzn_materialmodule_id materialmodule);

/**
 * Create a material iterator object for iterating through the materials in the
 * material module, in alphabetical order of name. The iterator initially
 * points at the position before the first material, so the first call to the
 * material iterator next() method returns the first material and advances the
 * iterator. Iterator becomes invalid if materials are added, removed or
 * renamed while in use.
 * @see cmzn_materialiterator_next
 *
 * @param materialmodule  Handle to the material module whose materials are to
 * be iterated over.
 * @return  Handle to material iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_materialiterator_id cmzn_materialmodule_create_materialiterator(
	cmzn_materialmodule_id materialmodule);

/**
 * Begin caching or increment cache level for this material module. Call this
 * function before making multiple changes to minimise number of change messages
 * sent to clients. Must remember to end_change after completing changes.
 * Can be nested.
 * @see cmzn_materialmodule_end_change
 *
 * @param materialmodule  The material module to begin change cache on.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_materialmodule_begin_change(
	cmzn_materialmodule_id materialmodule);

/**
 * Decrement cache level or end caching of changes for the material module.
 * Call matching begin change method before making multiple changes
 * and call this afterwards. When change level is restored to zero,
 * cached change messages are sent out to clients.
 * @see cmzn_materialmodule_begin_change
 *
 * @param materialmodule  The material module to end change cache on.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_materialmodule_end_change(
	cmzn_materialmodule_id materialmodule);

/**
 * Find the material with the specified name, if any.
 *
 * @param materialmodule  material module to search.
 * @param name  The name of the material.
 * @return  Handle to material, or NULL/invalid handle if not found or failed.
 */
ZINC_API cmzn_material_id cmzn_materialmodule_find_material_by_name(
	cmzn_materialmodule_id materialmodule, const char *name);

/**
 * Get the default material, if any.
 * @see cmzn_materialmodule_set_default_material
 *
 * @param materialmodule  material module to query.
 * @return  Handle to material, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_material_id cmzn_materialmodule_get_default_material(
	cmzn_materialmodule_id materialmodule);

/**
 * Set the default material. This material is used as the default material for
 * new graphics, but note if the default surface material is set it is used
 * instead for new surface and contours graphics.
 * @see cmzn_materialmodule_set_default_surface_material
 *
 * @param materialmodule  Material module to modify.
 * @param material  The material to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_materialmodule_set_default_material(
	cmzn_materialmodule_id materialmodule, cmzn_material_id material);

/**
 * Get the default selected material, if any. This material is used as the
 * default selected material for any new graphics.
 *
 * @param materialmodule  material module to query.
 * @return  Handle to material, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_material_id cmzn_materialmodule_get_default_selected_material(
	cmzn_materialmodule_id materialmodule);

/**
 * Set the default selected material.
 *
 * @param materialmodule  material module to modify
 * @param material  The material to set as default.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_materialmodule_set_default_selected_material(
	cmzn_materialmodule_id materialmodule,
	cmzn_material_id material);

/**
 * Get the default surface material, if any.
 * @see cmzn_materialmodule_set_default_surface_material
 *
 * @param materialmodule  material module to query.
 * @return  Handle to material, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_material_id cmzn_materialmodule_get_default_surface_material(
	cmzn_materialmodule_id materialmodule);

/**
 * Set the default surface material set for new surface and contours graphics.
 * If this is not set, the default material is used instead.
 *
 * @param materialmodule  Material module to modify.
 * @param material  The material to set as default for surfaces. Pass NULL or
 * unvalid material to clear.
 * @return  CMZN_OK on success otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_materialmodule_set_default_surface_material(
	cmzn_materialmodule_id materialmodule, cmzn_material_id material);

/**
 * Define a list of standard cmgui materials and store them as they are managed
 * by graphics module.
 *
 * @param materialmodule  Handle to material module to modify.
 * @return  Status CMZN_OK if successfully create a list of standard materials
 * into material module, any other value on failure.
 */
ZINC_API int cmzn_materialmodule_define_standard_materials(
	cmzn_materialmodule_id materialmodule);

/**
 * Convert a short attribute name into an enum if the attribute name matches
 * any of the members in the enum.
 *
 * @param attribute_name  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_material_attribute
	cmzn_material_attribute_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param attribute  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_material_attribute_enum_to_string(
	enum cmzn_material_attribute attribute);

/**
 * Get managed status of material in its owning material module.
 * @see cmzn_material_set_managed
 *
 * @param material  The material to query.
 * @return  true if material is managed, otherwise false.
 */
ZINC_API bool cmzn_material_is_managed(cmzn_material_id material);

/**
 * Set managed status of material in its owning material module.
 * If set (managed) the material will remain indefinitely in the
 * material module even if no external references are held.
 * If not set (unmanaged) the material will be automatically removed from the
 * module when no longer referenced externally, effectively marking it as
 * pending destruction.
 * All new objects are unmanaged unless stated otherwise.
 *
 * @param material  The material to modify.
 * @param value  The new value for the managed flag: true or false.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_material_set_managed(cmzn_material_id material,
	bool value);

/**
 * Get a new handle to the material, incrementing its reference count.
 *
 * @param material  Handle to the material.
 * @return  New handle to the material, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_material_id cmzn_material_access(cmzn_material_id material);

/**
 * Destroys handle to the material (and sets it to NULL).
 * Internally this decrements the reference count.
 *
 * @param material  address to the handle to the "to be destroyed"
 *   zinc material.
 * @return  Status CMZN_OK if successfully destroy material, any other value on
 * failure.
 */
ZINC_API int cmzn_material_destroy(cmzn_material_id *material);

/**
 * Get a real value of an attribute of the material.
 *
 * @param material  Handle to the zinc material.
 * @param attribute  The identifier of the real attribute to get.
 * @return  Value of the attribute.
 */
ZINC_API double cmzn_material_get_attribute_real(cmzn_material_id material,
	enum cmzn_material_attribute attribute);

/**
 * Set a real value for an attribute of the material.
 *
 * @param material  Handle to the zinc material.
 * @param attribute  The identifier of the real attribute to set.
 * @param value  The new value for the attribute.
 * @return  Status CMZN_OK if attribute successfully set, any other value if
 * failed or attribute not valid or unable to be set for this material object.
 */
ZINC_API int cmzn_material_set_attribute_real(cmzn_material_id material,
	enum cmzn_material_attribute attribute, double value);

/**
 * Get a 3 components vectors of an attribute of the material.
 * <values> should be allocated with enough space for 3 components.
 *
 * @param material  Handle to the zinc material.
 * @param attribute  The identifier of the vectors attribute to get.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_material_get_attribute_real3(cmzn_material_id material,
	enum cmzn_material_attribute attribute, double *values);

/**
 * Set a 3 components vectors of an attribute for the material.
 * <values> should be a vectors with 3 components containg valid values.
 *
 * @param material  Handle to the zinc material.
 * @param attribute  The identifier of the vectors attribute to get.
 * @return  Status CMZN_OK if attribute successfully set, any other value if
 * failed or attribute not valid or unable to be set for this material object.
 */
ZINC_API int cmzn_material_set_attribute_real3(cmzn_material_id material,
	enum cmzn_material_attribute attribute, const double *values);

/**
 * Return an allocated string containing material name.
 *
 * @param material  handle to the zinc material.
 * @return  allocated string containing material name, otherwise NULL. Up to
 * caller to free using cmzn_deallocate().
 */
ZINC_API char *cmzn_material_get_name(cmzn_material_id material);

/**
 * Set/change name for <material>.
 *
 * @param material  The handle to the zinc material.
 * @param name  name to be set to the material
 * @return  Status CMZN_OK if successfully set/change name for material,
 * any other value on failure.
 */
ZINC_API int cmzn_material_set_name(
	cmzn_material_id material, const char *name);

/**
 * Get field containing an image used for texturing the material.
 *
 * @param material  The material to query.
 * @param texture_number  The number of the texture to get, from 1 to 4.
 * @return  Handle to the field, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_field_id cmzn_material_get_texture_field(
	cmzn_material_id material, int texture_number);

/**
 * Set field containing an image used for texturing the material.
 * Up to 4 textures can be used with a material.
 *
 * @param material  The material to modify.
 * @param texture_number  The number of the texture to set, from 1 to 4.
 * Texture 1 is used for texture mapping with the legacy OpenGL shader model.
 * Custom material shaders can use any of the textures explicitly.
 * @param texture_field  The field supplying the texture image. Must be of type
 * field_image.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_material_set_texture_field(cmzn_material_id material,
	int texture_number, cmzn_field_id texture_field);

/**
 * Returns a new handle to the iterator with reference count incremented.
 *
 * @param iterator  The material iterator to obtain a new handle to.
 * @return  New handle to material iterator, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_materialiterator_id cmzn_materialiterator_access(
	cmzn_materialiterator_id iterator);

/**
 * Destroys this handle to the material iterator and sets it to NULL.
 *
 * @param iterator_address  Address of handle to material_iterator to destroy.
 * @return  Status CMZN_OK on success, otherwise CMZN_ERROR_ARGUMENT.
 */
ZINC_API int cmzn_materialiterator_destroy(cmzn_materialiterator_id *iterator_address);

/**
 * Returns a handle to the next material in the container being iterated over then
 * advances the iterator position. The caller is required to destroy the
 * returned material handle.
 *
 * @param iterator  Material iterator to query and advance.
 * @return  Handle to the next material, or NULL/invalid handle if none or failed.
 */
ZINC_API cmzn_material_id cmzn_materialiterator_next(cmzn_materialiterator_id iterator);

#ifdef __cplusplus
}
#endif

#endif
