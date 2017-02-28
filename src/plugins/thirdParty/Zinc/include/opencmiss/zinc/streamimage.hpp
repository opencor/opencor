/**
 * @file streamimage.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_STREAMIMAGE_HPP__
#define CMZN_STREAMIMAGE_HPP__

#include "opencmiss/zinc/streamimage.h"
#include "opencmiss/zinc/fieldimage.hpp"
#include "opencmiss/zinc/stream.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class StreaminformationImage : public Streaminformation
{
public:
	StreaminformationImage() : Streaminformation()
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit StreaminformationImage(cmzn_streaminformation_image_id streaminformation_image_id) :
		Streaminformation(reinterpret_cast<cmzn_streaminformation_id>(streaminformation_image_id))
	{ }

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_streaminformation_image_id getDerivedId() const
	{
		return reinterpret_cast<cmzn_streaminformation_image_id>(id);
	}

	enum Attribute
	{
		ATTRIBUTE_RAW_WIDTH_PIXELS = CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_RAW_WIDTH_PIXELS,
		ATTRIBUTE_RAW_HEIGHT_PIXELS = CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_RAW_HEIGHT_PIXELS,
		ATTRIBUTE_BITS_PER_COMPONENT = CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_BITS_PER_COMPONENT,
		ATTRIBUTE_COMPRESSION_QUALITY = CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_COMPRESSION_QUALITY
	};

	enum FileFormat
	{
		FILE_FORMAT_INVALID = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_INVALID,
		FILE_FORMAT_BMP = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_BMP,
		FILE_FORMAT_DICOM = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_DICOM,
		FILE_FORMAT_JPG = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_JPG,
		FILE_FORMAT_GIF = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_GIF,
		FILE_FORMAT_PNG = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_PNG,
		FILE_FORMAT_SGI = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_SGI,
		FILE_FORMAT_TIFF = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_TIFF,
		FILE_FORMAT_ANALYZE = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_ANALYZE,
		FILE_FORMAT_ANALYZE_OBJECT_MAP = CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_ANALYZE_OBJECT_MAP
	};

	enum PixelFormat
	{
		PIXEL_FORMAT_INVALID = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_INVALID,
		PIXEL_FORMAT_LUMINANCE = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_LUMINANCE,
		PIXEL_FORMAT_LUMINANCE_ALPHA = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_LUMINANCE_ALPHA,
		PIXEL_FORMAT_RGB = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_RGB,
		PIXEL_FORMAT_RGBA = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_RGBA,
		PIXEL_FORMAT_ABGR = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_ABGR,
		PIXEL_FORMAT_BGR = CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_BGR
	};

	int setAttributeInteger(Attribute attribute, int value)
	{
		return cmzn_streaminformation_image_set_attribute_integer(getDerivedId(),
			static_cast<cmzn_streaminformation_image_attribute>(attribute), value);
	}

	int setAttributeReal(Attribute attribute, double value)
	{
		return cmzn_streaminformation_image_set_attribute_real(getDerivedId(),
			static_cast<cmzn_streaminformation_image_attribute>(attribute), value);
	}

	int setFileFormat(FileFormat imageFileFormat)
	{
		return cmzn_streaminformation_image_set_file_format(getDerivedId(),
			static_cast<cmzn_streaminformation_image_file_format>(imageFileFormat));
	}

	int setPixelFormat(PixelFormat imagePixelFormat)
	{
		return cmzn_streaminformation_image_set_pixel_format(getDerivedId(),
			static_cast<cmzn_streaminformation_image_pixel_format>(imagePixelFormat));
	}

};

inline StreaminformationImage Streaminformation::castImage()
{
	return StreaminformationImage(cmzn_streaminformation_cast_image(id));
}

inline StreaminformationImage FieldImage::createStreaminformationImage()
{
  return StreaminformationImage(
    reinterpret_cast<cmzn_streaminformation_image_id>(
      cmzn_field_image_create_streaminformation_image(getDerivedId())));
}

inline int FieldImage::read(const StreaminformationImage& streaminformationImage)
{
  return cmzn_field_image_read(getDerivedId(), streaminformationImage.getDerivedId());
}

inline int FieldImage::write(const StreaminformationImage& streaminformationImage)
{
  return cmzn_field_image_write(getDerivedId(), streaminformationImage.getDerivedId());
}

} // namespace Zinc
}
#endif
