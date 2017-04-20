/**
 * @file streamid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_STREAMID_H__
#define CMZN_STREAMID_H__

/**
 * @brief Base type listing resources and options for stream I/O.
 *
 * Base stream information type. The stream information maintains, creates and
 * adds details to streamresources for reading from or writing to external
 * resources. Users can create a type-specific stream information for each
 * object with read and write functionality, currently image fields and region.
 * @see cmzn_field_image_create_streaminformation_image
 * @see cmzn_region_create_streaminformation_region
 */
struct cmzn_streaminformation;
typedef struct cmzn_streaminformation *cmzn_streaminformation_id;

/**
 * @brief A description of a resource for reading from or writing to.
 *
 * A description of a resource for reading from or writing to, such as a file or
 * memory buffer. These are created through the stream information object.
 *
 * @see cmzn_streaminformation_id
 * @see cmzn_streamresource_file_id
 * @see cmzn_streamresource_memory_id
 * @see cmzn_streaminformation_create_streamresource_file
 * @see cmzn_streaminformation_create_streamresource_memory
 * @see cmzn_streaminformation_create_streamresource_memory_buffer
 * @see cmzn_streaminformation_create_streamresource_memory_buffer_copy
 */
struct cmzn_streamresource;
typedef struct cmzn_streamresource *cmzn_streamresource_id;

/**
 * @brief A derived streamresource describing a file.
 *
 * A derived streamresource describing a file.
 *
 * @see cmzn_streaminformation_create_streamresource_file
 */
struct cmzn_streamresource_file;
typedef struct cmzn_streamresource_file *cmzn_streamresource_file_id;

/**
 * @brief A derived stream resource describing a block of memory.
 *
 * A derived stream resource describing a block of memory.
 *
 * @see cmzn_streaminformation_create_streamresource_memory
 * @see cmzn_streaminformation_create_streamresource_memory_buffer
 */
struct cmzn_streamresource_memory;
typedef struct cmzn_streamresource_memory *cmzn_streamresource_memory_id;

/**
 * Describes data compression scheme used in a stream resource.
 */
enum cmzn_streaminformation_data_compression_type
{
	CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_INVALID = 0,
	/*!< Unspecified data compression type */
	CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_DEFAULT = 1,
	/*!< default data compression
	 * This is the default compression for streamresource.
	 * When compression type for streamresource is set to default,
	 * the DATA_COMPRESSION_TYPE set on the owning
	 * streaminformation will be used. This type cannot be set
	 * for streaminformation.
	 */
	CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_NONE = 2,
	/*!< No data compression
	 * This is the default compression for streaminformation.
	 * This specifies the resource(s) to not have any type of compression.
	 */
	CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_GZIP = 3,
	/*!< Gzip data compression
	 * This specifies the resource(s) is compressed using gzip. This mode
	 * is supported in all region resource(s) and analyze image format.
	 * Analyze image format expects the resource(s) in a gzip compressed tar
	 * containing an analyze header file and analyze image file.
	 */
	CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_BZIP2 = 4,
	/*!< Bzip2 data compression
	 * This specifies the resource(s) is compressed using bzip2. This mode
	 * is supported in all region resource(s)and analyze image format.
	 * Analyze image format expects the resource(s) in a bzip2 compressed tar
	 * containing analyze header file and analyze image file.
	 */
};

#endif
