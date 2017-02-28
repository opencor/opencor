/**
 * @file streamimage.h
 *
 * Public interface to image field stream objects.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_STREAMIMAGE_H__
#define CMZN_STREAMIMAGE_H__

#include "types/fieldimageid.h"
#include "types/streamid.h"

#include "opencmiss/zinc/zincsharedobject.h"

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
ZINC_API enum cmzn_streaminformation_image_file_format
	cmzn_streaminformation_image_file_format_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param format  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_streaminformation_image_file_format_enum_to_string(
	enum cmzn_streaminformation_image_file_format format);

/**
 * Convert a short name into an enum if the name matches any of the members in
 * the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_streaminformation_image_pixel_format
	cmzn_streaminformation_image_pixel_format_enum_from_string(
		const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param format  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_streaminformation_image_pixel_format_enum_to_string(
	enum cmzn_streaminformation_image_pixel_format format);

/**
 * Convert a short attribute name into an enum if the attribute name matches
 * any of the members in the enum.
 *
 * @param string  string of the short enumerator name
 * @return  the correct enum type if a match is found.
 */
ZINC_API enum cmzn_streaminformation_image_attribute
	cmzn_streaminformation_image_attribute_enum_from_string(const char *string);

/**
 * Return an allocated short name of the enum type from the provided enum.
 * User must call cmzn_deallocate to destroy the successfully returned string.
 *
 * @param attribute  enum to be converted into string
 * @return  an allocated string which stored the short name of the enum.
 */
ZINC_API char *cmzn_streaminformation_image_attribute_enum_to_string(
	enum cmzn_streaminformation_image_attribute attribute);

/**
 * Creates a stream information object for specifying files/resources and options
 * for reading and writing image data to/from an image field.
 *
 * @return  Handle to new stream information, or NULL/invalid handle on failure.
 */
ZINC_API cmzn_streaminformation_id cmzn_field_image_create_streaminformation_image(
	cmzn_field_image_id image_field);

/**
 * Destroys handle to stream information image.
 *
 * @param streaminformation_address  Address of handle to streaminformation
 * image. Handle will be reset to NULL.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_image_destroy(
	cmzn_streaminformation_image_id *streaminformation_address);

/**
 * If the streaminformation is of image type, then this function returns
 * the derived image stream information handle.
 *
 * @param streaminformation  Handle to streaminformation to cast.
 * @return  Handle to derived stream information image, or NULL/invalid handle if
 * wrong type or failed.
 */
ZINC_API cmzn_streaminformation_image_id cmzn_streaminformation_cast_image(
	cmzn_streaminformation_id streaminformation);

/**
 * Cast streaminformation_image back to its base streaminformation and
 * return the streaminformation.
 * IMPORTANT NOTE: Returned streaminformation does not have incremented
 * reference count and must not be destroyed. Use cmzn_streaminformation_access()
 * to add a reference if maintaining returned handle beyond the lifetime of the
 * streaminformation_image argument.
 *
 * @param streaminformation_image  Handle to the streaminformation_image to cast.
 * @return  Non-accessed handle to the base stream information or NULL if failed.
 */
ZINC_C_INLINE cmzn_streaminformation_id cmzn_streaminformation_image_base_cast(
	cmzn_streaminformation_image_id streaminformation_image)
{
	return (cmzn_streaminformation_id)(streaminformation_image);
}

/**
 * Set an integer or Boolean attribute of the streaminformation_image.
 *
 * @param streaminformation  Handle to the zinc streaminformation_image.
 * @param attribute  The identifier of the integer attribute to set.
 * @param value  The new value for the attribute. For Boolean values use 1 for
 * true.
 *
 * @return  Status CMZN_OK if attribute successfully set, any other value if
 *	failed or attribute not valid or unable to be set for this
 * 	streaminformation_image.
 */
ZINC_API int cmzn_streaminformation_image_set_attribute_integer(
	cmzn_streaminformation_image_id streaminformation,
	enum cmzn_streaminformation_image_attribute attribute, int value);

/**
 * Set an double attribute of the streaminformation_image.
 *
 * @param streaminformation  Handle to the zinc streaminformation_image.
 * @param attribute  The identifier of the double attribute to set.
 * @param value  The new value for the attribute.
 * @return   Status CMZN_OK if attribute successfully set, any other value if
 * failed or attribute not valid or unable to be set for this
 * streaminformation_image.
 */
ZINC_API int cmzn_streaminformation_image_set_attribute_real(
	cmzn_streaminformation_image_id streaminformation,
	enum cmzn_streaminformation_image_attribute attribute,
	double value);

/**
 * Specifies the format for binary data to be read/written using this
 * streaminformation.
 *
 * @param streaminformation  The storage information object.
 * @param file_format  The image file format.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_image_set_file_format(
	cmzn_streaminformation_image_id streaminformation,
	enum cmzn_streaminformation_image_file_format file_format);

/**
 * Specifies the pixel format for binary data of the images  to be read/written
 * using this streaminformation.
 *
 * @param streaminformation  The storage information object.
 * @param pixel_format  The pixel_format of the formatted data.
 * @return  Status CMZN_OK on success, any other value on failure.
 */
ZINC_API int cmzn_streaminformation_image_set_pixel_format(
	cmzn_streaminformation_image_id streaminformation,
	enum cmzn_streaminformation_image_pixel_format pixel_format);

#ifdef __cplusplus
}
#endif

#endif
