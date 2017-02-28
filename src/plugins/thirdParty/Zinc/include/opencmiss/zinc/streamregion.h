/**
 * @file streamregion.h
 *
 * Region-specific file stream objects.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_STREAMREGION_H__
#define CMZN_STREAMREGION_H__

#include "types/fieldid.h"
#include "types/streamid.h"
#include "types/regionid.h"

#include "opencmiss/zinc/zincsharedobject.h"

/*
Global functions
----------------
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_streaminformation_region_attribute
	cmzn_streaminformation_region_attribute_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param attribute  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_streaminformation_region_attribute_enum_to_string(
	enum cmzn_streaminformation_region_attribute attribute);

/**
 * Creates a stream information object for specifying files/resources and options
 * for reading and writing field data to/from this region and child regions.
 *
 * @return  Handle to new stream information, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_streaminformation_id cmzn_region_create_streaminformation_region(
	cmzn_region_id region);

/**
 * If the streaminformation is of region type, then this function returns
 * the region specific handle.
 *
 * @param streaminformation  Handle to streaminformation to cast.
 * @return  Handle to derived stream information region, or NULL/invalid handle if
 * wrong type or failed.
 */
ZINC_API cmzn_streaminformation_region_id cmzn_streaminformation_cast_region(
	cmzn_streaminformation_id streaminformation);

/**
 * Cast cmzn_streaminformation_region back to its base streaminformation and
 * return the streaminformation.
 * IMPORTANT NOTE: Returned streaminformation does not have incremented
 * reference count and must not be destroyed. Use cmzn_streaminformation_access()
 * to add a reference if maintaining returned handle beyond the lifetime of the
 * streaminformation_region argument.
 *
 * @param streaminformation  Handle to the streaminformation_region to cast.
 * @return  Non-accessed handle to the base stream information or NULL if failed.
 */
ZINC_C_INLINE cmzn_streaminformation_id cmzn_streaminformation_region_base_cast(
	cmzn_streaminformation_region_id streaminformation_region)
{
	return (cmzn_streaminformation_id)(streaminformation_region);
}

/**
 * Destroys a stream information region object.
 *
 * @param streaminformation_address  Address of handle to streaminformation
 * region to destroy; handle is reset to NULL.
 * @return  Status CMZN_OK if the operation is successful, any other value on
 * failure.
 */
ZINC_API int cmzn_streaminformation_region_destroy(
	cmzn_streaminformation_region_id *streaminformation_address);

/**
 * Check either an attribute of streaminformation has been set or
 * not.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param attribute  The identifier of the real attribute to get.
 * @return  1 if attribute has been set.
 */
ZINC_API bool cmzn_streaminformation_region_has_attribute(
	cmzn_streaminformation_region_id streaminformation,
	enum cmzn_streaminformation_region_attribute attribute);

/**
 * Get a real value of an attribute of streaminformation.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param attribute  The identifier of the real attribute to get.
 * @return  Value of the attribute.
 */
ZINC_API double cmzn_streaminformation_region_get_attribute_real(
	cmzn_streaminformation_region_id streaminformation,
	enum cmzn_streaminformation_region_attribute attribute);

/**
 * Set a double attribute of the region stream information valid for
 * all resources unless overridden for individual resources.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param attribute  The identifier of the double attribute to set.
 * @param value  The new value for the attribute.
 *
 * @return  status CMZN_OK if attribute successfully set, any other value if
 * failed or attribute not valid or unable to be set for this
 * cmzn_streaminformation_region.
 */
ZINC_API int cmzn_streaminformation_region_set_attribute_real(
	cmzn_streaminformation_region_id streaminformation,
	enum cmzn_streaminformation_region_attribute attribute,
	double value);

/**
 * Check either an attribute of a stream in streaminformation has been set or
 * not.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param attribute  The identifier of the real attribute to get.
 * @return  1 if attribute has been set.
 */
ZINC_API bool cmzn_streaminformation_region_has_resource_attribute(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource,
	enum cmzn_streaminformation_region_attribute attribute);

/**
 * Get a real value of an attribute of a stream in streaminformation.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param attribute  The identifier of the real attribute to get.
 * @return  Value of the attribute.
 */
ZINC_API double cmzn_streaminformation_region_get_resource_attribute_real(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource,
	enum cmzn_streaminformation_region_attribute attribute);

/**
 * Set a double attribute for a resource in the region stream information.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param attribute  The identifier of the double attribute to set.
 * @param value  The new value for the attribute.
 *
 * @return   status CMZN_OK if attribute successfully set, any other value if
 * failed or attribute not valid or unable to be set for this
 * cmzn_streaminformation_region.
 */
ZINC_API int cmzn_streaminformation_region_set_resource_attribute_real(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource,
	enum cmzn_streaminformation_region_attribute attribute,
	double value);

/**
 * Gets the format for region field data to be read/written using this
 * stream information.
 *
 * @param streaminformation  The region stream information object.
 * @return  The file format, or FILE_FORMAT_INVALID on failure.
 */
ZINC_API enum cmzn_streaminformation_region_file_format
	cmzn_streaminformation_region_get_file_format(
		cmzn_streaminformation_region_id streaminformation);

/**
 * Specifies the format for region field data to be read/written using this
 * stream information. Applies to all resources managed by stream information.
 *
 * @param streaminformation  The region stream information object.
 * @param file_format  The region stream file format.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_region_set_file_format(
	cmzn_streaminformation_region_id streaminformation,
	enum cmzn_streaminformation_region_file_format file_format);

/**
 * Get the specified domain types for a stream resource in streaminformation.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @return  Bitmasks for specified domain types for stream resource,
 * 	CMZN_FIELD_DOMAIN_TYPE_INVALID if failed or unset
 */
ZINC_API cmzn_field_domain_types cmzn_streaminformation_region_get_resource_domain_types(
	cmzn_streaminformation_region_id streaminformation, cmzn_streamresource_id resource);

/**
 * Set the domain types to be exported from the region this stream information is
 * created in. The current default setting will output all domain types in region.
 * On import, the domain type also specifies nodesets without a specified domain
 * type in exformat file to be imported as nodes or datapoints domain. If unset,
 * unspecified nodesets will be imported as nodes.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param domain_types  Bitmasks for the domain type to be set for output. It currently supports
 *   the following domains:
 *   CMZN_FIELD_DOMAIN_TYPE_POINT - Only output the region name if this is the only bit set
 *   CMZN_FIELD_DOMAIN_TYPE_NODES - Enable output of nodes
 *   CMZN_FIELD_DOMAIN_TYPE_DATAPOINTS - Enable output of datapoints
 *   CMZN_FIELD_DOMAIN_TYPE_MESH1D - Enable output of 1D mesh
 *   CMZN_FIELD_DOMAIN_TYPE_MESH2D - Enable output of 2D mesh
 *   CMZN_FIELD_DOMAIN_TYPE_MESH3D - Enable output of 3D mesh
 *   CMZN_FIELD_DOMAIN_TYPE_MESH_HIGHEST_DIMENSION - Enable output of mesh with highest
 *   dimension possible
 *
 * @return   status CMZN_OK if domain types successfully set, any other value if
 *   failed or domain type not valid or unable to be set for this
 *   cmzn_streaminformation_region.
 */
ZINC_API int cmzn_streaminformation_region_set_resource_domain_types(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource, cmzn_field_domain_types domain_types);

/**
 * Use this function to choose the fields to be read/written by specifying
 * name of the fields, this will affect all resources in this streaminformation
 * unless field names have been set for individual resource.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param number_of_names  Number of field names provided.
 * @param fieldNames  The identifier of the double attribute to set.
 *
 * @return   status CMZN_OK if successfully set the field names,
 *   any other value on failure.
 */
ZINC_API int cmzn_streaminformation_region_set_field_names(
		cmzn_streaminformation_region_id streaminformation,
		int number_of_names, const char **field_names);

/**
 * Use this function to choose the fields to be read/written by specifying
 * name of the fields for this resource.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param number_of_names  Number of field names provided.
 * @param fieldNames  The identifier of the double attribute to set.
 *
 * @return   status CMZN_OK if field names are successfully set,
 *   any other value if failed or attribute not valid or unable to be set
 *   for this cmzn_streaminformation_region.
 */
ZINC_API int cmzn_streaminformation_region_set_resource_field_names(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource, int number_of_names, const char **field_names);

/**
 * Use this function to select either streaminformation should export
 * the region and its children or just the region itself. This applies
 * to all streaminformation resources unless this mode is set for
 * individuals resources.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param recursion_mode  Recursion mode to be set for this streaminformation.
 *
 * @return   status CMZN_OK if recursion mode is successfully set,
 *   any other value if failed.
 */
ZINC_API int cmzn_streaminformation_region_set_recursion_mode(
	cmzn_streaminformation_region_id streaminformation,
	enum cmzn_streaminformation_region_recursion_mode recursion_mode);

/**
 * Use this function to select either streaminformation resource should export
 * the region and its children or just the region itself.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param recursion_mode  Recursion mode to be set for this streaminformation.
 *
 * @return   status CMZN_OK if recursion mode is successfully set,
 *   any other value if failed.
 */
ZINC_API int cmzn_streaminformation_region_set_resource_recursion_mode(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource,
	enum cmzn_streaminformation_region_recursion_mode recursion_mode);

/**
 * Use this function to get the group name set for stream resource.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 *
 * @return   Allocated string if group name is successfully returned,
 *   any other value if failed.
 */
ZINC_API char *cmzn_streaminformation_region_get_resource_group_name(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource);

/**
 * Use this function to set the group to be exported. If a group field with
 * the provided name is found in the relevant region(s), the fields
 * information in the group will be exported. If group name is NULL (default),
 * all field informations will be exported.
 *
 * @param streaminformation  Handle to the stream information_region.
 * @param resource  Handle to the stream resource.
 * @param group_name  Group name to be set for this streaminformation.
 *
 * @return   status CMZN_OK if group name is successfully set,
 *   any other value if failed.
 */
ZINC_API int cmzn_streaminformation_region_set_resource_group_name(
	cmzn_streaminformation_region_id streaminformation,
	cmzn_streamresource_id resource, const char *group_name);

#ifdef __cplusplus
}
#endif

#endif
