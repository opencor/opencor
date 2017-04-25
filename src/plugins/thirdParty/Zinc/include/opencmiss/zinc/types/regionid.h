/**
 * @file regionid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_REGIONID_H__
#define CMZN_REGIONID_H__

/**
 * @brief A hierarchical block/namespace owning domains and fields.
 *
 * A region represents a block of a hierarchical model. It owns a local set of
 * domains and fields describing the model (each accessed from its fieldmodule),
 * and a scene for building graphics to visualise the model.
 * Each region has any number of uniquely named child regions, allowing
 * tree-like models to be constructed.
 */
struct cmzn_region;
typedef struct cmzn_region * cmzn_region_id;

/**
 * @brief A region-specific stream information object.
 *
 * A region-specific stream information object, used to specify one or more
 * model files/resources for a region to read from or write to, with attributes
 * specified for them either globally or per-resource.
 *
 * @see cmzn_region_create_streaminformation_region
 */
struct cmzn_streaminformation_region;
typedef struct cmzn_streaminformation_region * cmzn_streaminformation_region_id;

/**
 * Enumeration of attributes that can be set by generic stream information region
 * methods.
 */
enum cmzn_streaminformation_region_attribute
{
	CMZN_STREAMINFORMATION_REGION_ATTRIBUTE_INVALID = 0,
	/*!< Unspecified attribute */
	CMZN_STREAMINFORMATION_REGION_ATTRIBUTE_TIME = 1
	/*!< Attribute used to specify at what time the stream resource(s) to be
	 * reading in or writing to.*/
};

/**
 * Describes the format for serialisation of region field data.
 * @see cmzn_streaminformation_region_set_file_format
 */
enum cmzn_streaminformation_region_file_format
{
	CMZN_STREAMINFORMATION_REGION_FILE_FORMAT_INVALID = 0,
	/*!< Invalid file format */
	CMZN_STREAMINFORMATION_REGION_FILE_FORMAT_AUTOMATIC = 1,
	/*!< Automatically choose file format. This is the default option.
	 * On read: determine from internal characteristics
	 * On write: determine from file extension (case insensitive):
	 * .ex* -> EX format; .fieldml -> FieldML */
	CMZN_STREAMINFORMATION_REGION_FILE_FORMAT_EX = 2,
	/*!< Zinc/Cmgui EX format */
	CMZN_STREAMINFORMATION_REGION_FILE_FORMAT_FIELDML = 3
	/*!< Latest supported FieldML format */
};

enum cmzn_streaminformation_region_recursion_mode
{
	CMZN_STREAMINFORMATION_REGION_RECURSION_MODE_INVALID = 0,
	/*!< Invalid recursion mode */
	CMZN_STREAMINFORMATION_REGION_RECURSION_MODE_ON = 1,
	/*!< Region will be exported recursively */
	CMZN_STREAMINFORMATION_REGION_RECURSION_MODE_OFF = 2,
	/*!< Region will not be exported recursively */

};

#endif /* CMZN_REGION_ID_H */
