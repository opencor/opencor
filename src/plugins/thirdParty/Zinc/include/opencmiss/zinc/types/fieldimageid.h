/**
 * @file fieldimageid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_FIELDIMAGEID_H__
#define CMZN_FIELDIMAGEID_H__

/**
 * @brief An image-based field giving the pixel colour/intensity values.
 *
 * An image-based field giving the pixel colour/intensity values as a function
 * of its domain texture coordinates, interpolated between pixels according to
 * the filter mode. 2-D and 3-D images are supported.
 * Graphics materials can have image fields attached to perform texturing.
 */
struct cmzn_field_image;
typedef struct cmzn_field_image *cmzn_field_image_id;

/**
 * @brief An image-specific stream information object.
 *
 * An image-specific stream information object, used to specify one or more
 * image files/resources for an image field to read from or write to, with any
 * attributes specified for them such as format, size etc.
 *
 * @see cmzn_field_image_create_streaminformation_image
 */
struct cmzn_streaminformation_image;
typedef struct cmzn_streaminformation_image *cmzn_streaminformation_image_id;

/**
 * Describes the blending of the texture with the texture constant colour and
 * the underlying material/fragment colour.
 *
 * Ct = texture colour, Cf = material colour, Cv = colour produced
 * At = texture alpha, Af = material alpha, alpha = colour produced
 *
 */
enum cmzn_field_image_combine_mode
{
	CMZN_FIELD_IMAGE_COMBINE_MODE_INVALID = 0,
		/*!< Unspecified combine mode */
	CMZN_FIELD_IMAGE_COMBINE_MODE_BLEND = 1,
		/*!< Blending the material and texture colour
		 * Cv = (1 - Ct) * Cf
		 * Av = At * Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_DECAL = 2,
		/*!< Default combine mode, uses the image's alpha value to
		 * determine how the material colour is to combine with the
		 * texture colour. Material alpha value will be used as the
		 * resulting alpha.
		 * Cv = (1 - At) *Ct + At * C
		 * Av = Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_MODULATE = 3,
		/*!< Colour will be the product of texture and material colour.
		 * Alpha will be the product of texture and material alph.
		 * Cv = Ct * Cf
		 * Av = At * Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_ADD = 4,
		/*!< Colour will be the sum of texture and material colour.
		 * Alpha will be the sum of texture and material alph.
		 * Cv = Ct + Cf
		 * Av = At + Af
		 * */
	CMZN_FIELD_IMAGE_COMBINE_MODE_ADD_SIGNED = 5,
		/*!< Add the value and subtract 0.5 so the texture value
		  * effectively ranges from -0.5 to 0.5
		  * Cv = Ct + Cf - 0.5
		  * Av = At + Af - 0.5
		  */
	CMZN_FIELD_IMAGE_COMBINE_MODE_MODULATE_SCALE_4 = 6,
		/*!< Multiply and then scale by 4, so that we can
		 * scale down or up.
		 * Cv = (Ct * Cf) * 4
		 * Av = At * Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_BLEND_SCALE_4 = 7,
		/*!< Same as blend with a 4 * scaling
		 * Cv = ( (1 - Ct) * Cf ) * 4
		 * Av = At * Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_SUBTRACT = 8,
		/*!< Colour will be the difference of texture and material colour.
		 * Alpha will be the difference of texture and material alph.
		 * Cv = Ct - Cf
		 * Av = At - Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_ADD_SCALE_4 = 9,
		/*!< Same as ADD with a 4 * scaling
		 * Cv = (Ct + Cf) * 4
		 * Av = At + Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_SUBTRACT_SCALE_4 = 10,
		/*!< Same as SUBTRACT with a 4 * scaling
		 * Cv = (Ct - Cf) * 4
		 * Av = At - Af
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_INVERT_ADD_SCALE_4 = 11,
		/*!< Texture colour will be added to 1 - material colour and then
		 * multiplied by 4.
		 * Cv = ((1 - Cf) + Ct) * 4
		 * Av = Af + At
		 */
	CMZN_FIELD_IMAGE_COMBINE_MODE_INVERT_SUBTRACT_SCALE_4 = 12
		/*!< Texture colour will be subtracted frim 1 - material colour
		 * then multiplied by 4.
		 * Cv = ((1 - Cf) - Ct) * 4
		 * Av = Af - At
		 */
};

/**
 * Specifies how the graphics hardware rasterises the texture onto the screen,
 * mainly its interpolation mode.
 */
enum cmzn_field_image_filter_mode
{
	CMZN_FIELD_IMAGE_FILTER_MODE_INVALID = 0,
		/*!< Unspecified filter mode */
	CMZN_FIELD_IMAGE_FILTER_MODE_NEAREST = 1,
		/*!< Default combine mode, texel with coordinates nearest the centre
		 * of the pixel is used for both magnification and minification */
	CMZN_FIELD_IMAGE_FILTER_MODE_LINEAR = 2,
		/*!< A weighted linear average of the 2 x 2 arrays of texels
		 * that lie nearest to the centre of the pixel is used for
		 * both magnification and minification */
	CMZN_FIELD_IMAGE_FILTER_MODE_NEAREST_MIPMAP_NEAREST = 3,
		/*!< Within an individual mipmap, nearest texel value can be
		 * used using this mode.
		 */
	CMZN_FIELD_IMAGE_FILTER_MODE_LINEAR_MIPMAP_NEAREST = 4,
		/*!< Within an individual mipmap, linearly interpolated value can be
		 * used using this mode.
		 */
	CMZN_FIELD_IMAGE_FILTER_MODE_LINEAR_MIPMAP_LINEAR = 5
		/*!< This mode uses linear interpolation to compute the value
		 * in each of two maps and then interpolates linearly between
		 * these two values.
		 */
};

/**
 * Whether the image is compressed when used for texturing.
 *
 * @future  Could add hardware/driver specific compression formats.
 */
enum cmzn_field_image_hardware_compression_mode
{
	CMZN_FIELD_IMAGE_HARDWARE_COMPRESSION_MODE_INVALID = 0,
		/*!< Uncompressed compression mode */
	CMZN_FIELD_IMAGE_HARDWARE_COMPRESSION_MODE_UNCOMPRESSED = 1,
		/*!< No compression. The default hardware compression mode */
	CMZN_FIELD_IMAGE_HARDWARE_COMPRESSION_MODE_AUTOMATIC = 2
		/*!< allow the hardware to choose the compression */
};

/**
 * Describes how the image is to be wrapped when texture coordinate is assigned
 * outside the range [0,1], you can choose to have them clamp or repeat.
 */
enum cmzn_field_image_wrap_mode
{
	CMZN_FIELD_IMAGE_WRAP_MODE_INVALID = 0,
		/*!< Unspecified wrap mode */
	CMZN_FIELD_IMAGE_WRAP_MODE_CLAMP = 1,
		/*!< Clamp to a blend of the pixel edge and border colour */
	CMZN_FIELD_IMAGE_WRAP_MODE_REPEAT = 2,
		/*!< Default wrap mode. Repeat texture cylically in multiples of the
		  texture coordinate range */
	CMZN_FIELD_IMAGE_WRAP_MODE_EDGE_CLAMP = 3,
		/*!< Always ignore the border, texels at or near the edge of the texure are
		  used for texturing */
	CMZN_FIELD_IMAGE_WRAP_MODE_BORDER_CLAMP = 4,
		/*!< Clamp to the border colour when outside the texture coordinate range. */
	CMZN_FIELD_IMAGE_WRAP_MODE_MIRROR_REPEAT = 5
		/*!< Repeat but mirror every second multiple of the texture coordinates
		  range. Texture may appear up-right in coordinate range[0,1] but
		  upside-down in coordinate range[1,2] */
};

/**
 * Describes the format for image storage.
 * @see cmzn_streaminformation_image_set_file_format
 */
enum cmzn_streaminformation_image_file_format
{
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_INVALID = 0,
	/*!< Unspecified image file format. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_BMP = 1,
	/*!< Specify the file to be reading in or writing to as
	 * bitmap image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_DICOM = 2,
	/*!< Specify the file to be reading in or writing to as
	 * dicom image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_JPG = 3,
	/*!< Specify the file to be reading in or writing to as
	 * jpeg image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_GIF = 4,
	/*!< Specify the file to be reading in or writing to as
	 * gif image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_PNG = 5,
	/*!< Specify the file to be reading in or writing to as
	 * png image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_SGI = 6,
	/*!< Specify the file to be reading in or writing to as
	 * sgi image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_TIFF = 7,
	/*!< Specify the file to be reading in or writing to as
	 * tiff image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_ANALYZE = 8,
	/*!< Specify the file to be reading in or writing to as
	 * Analyze image. */
	CMZN_STREAMINFORMATION_IMAGE_FILE_FORMAT_ANALYZE_OBJECT_MAP = 9
	/*!< Specify the file to be reading in or writing to as
	 * Analyze object map. */
};

/**
 * Optional information used to describe the binary data supplied to the images.
 */
enum cmzn_streaminformation_image_pixel_format
{
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_INVALID = 0,
	/*!< Unspecified image pixel format. */
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_LUMINANCE = 1,
	/*!< Specify the binary data of image to be single component
	 * grey scale. */
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_LUMINANCE_ALPHA = 2,
	/*!< Specify the binary data of image to be two components:
	 * grey scale and alpha. */
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_RGB = 3,
	/*!< Specify the binary data of image to be three components:
	 * red, green and blue. */
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_RGBA = 4,
	/*!< Specify the binary data of image to be four components:
	 * red, green, blue and alpha. */
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_ABGR = 5,
	/*!< Specify the binary data of image to be four components:
	 * alpha, blue, green and red. */
	CMZN_STREAMINFORMATION_IMAGE_PIXEL_FORMAT_BGR = 6
	/*!< Specify the binary data of image to be three components:
	 * blue, green and red. */
};

/**
 * Enumeration of attributes that can be set by generic stream information image
 * methods.
 */
enum cmzn_streaminformation_image_attribute
{
	CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_RAW_WIDTH_PIXELS = 1,
	/*!< Integer specifies the pixel width for binary data reading in using this
	 * stream information.
	 */
	CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_RAW_HEIGHT_PIXELS = 2,
	/*!< Integer specifies the pixel height for binary data reading in using this
	 * stream information.
	 */
	CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_BITS_PER_COMPONENT = 3,
	/*!< Integer specifies the number of bytes per component for binary data using
	 * this stream information. Only 8 and 16 bits are supported at the moment.
	 */
	CMZN_STREAMINFORMATION_IMAGE_ATTRIBUTE_COMPRESSION_QUALITY = 4
	/*!< Real number specifies the quality for binary data using this stream information.
	 * This parameter controls compression for compressed lossy formats,
	 * where a quality of 1.0 specifies the least lossy output for a given format and a
	 * quality of 0.0 specifies the most compression.
	 */
};

#endif
