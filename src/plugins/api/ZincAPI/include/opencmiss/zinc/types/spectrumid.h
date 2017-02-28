/**
 * @file spectrumid.h
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_SPECTRUMID_H__
#define CMZN_SPECTRUMID_H__

/**
 * @brief Maps a single component of a data field to colour in a spectrum.
 *
 * A spectrumcomponent object maps a single component of a data field to one of
 * several colour ramps, rainbow, alpha ramp, contour bands or a step function.
 * A spectrum may blend multiple components to give the overall colouring.
 * @see cmzn_spectrum_id
 */
struct cmzn_spectrumcomponent;
typedef struct cmzn_spectrumcomponent *cmzn_spectrumcomponent_id;

/**
 * @brief Zinc Spectrum maps values of graphics data fields to colours.
 *
 * A Zinc Spectrum maps values of graphics data fields to colours. It consists
 * of a list of spectrumcomponent objects which combine to give the overall
 * colouring.
 * @see cmzn_spectrumcomponent_id
 */
struct cmzn_spectrum;
typedef struct cmzn_spectrum *cmzn_spectrum_id;

/**
 * @brief Module managing all spectrums.
 *
 * Module managing all spectrums.
 */
struct cmzn_spectrummodule;
typedef struct cmzn_spectrummodule *cmzn_spectrummodule_id;

/**
 * @brief An iterator for looping through all the spectrums in a
 * spectrum module.
 *
 * An iterator for looping through all the spectrums in a spectrum
 * module.
 */
struct cmzn_spectrumiterator;
typedef struct cmzn_spectrumiterator * cmzn_spectrumiterator_id;

/**
 * Controls the mapping of field value to colour in a spectrum component.
 */
enum cmzn_spectrumcomponent_scale_type
{
    CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_INVALID = 0,
    /*!< Unspecified scale type */
    CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_LINEAR = 1,
    /*!< default scale type
     * The colour value on spectrum will be interpolated linearly in range when
     * this mode is chosen.
     */
    CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_LOG = 2
    /*!< Logarithm scale type for spectrum component.
     * The colour value on spectrum will be interpolated logarithmically in range
     * when this mode is chosen.
     */
};

/**
 * Colour mapping mode for specctrum component. Appearances of these mappings
 * can be altered by the various APIs provided in spectrum and spectrum components
 * APIs.
 */
enum cmzn_spectrumcomponent_colour_mapping_type
{
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_INVALID = 0,
    /*!< Unspecified colour mapping type */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_ALPHA = 1,
    /*!< This colour mapping alters the alpha (transparency value) for
     * primitives.
     * This mode does not alter the rgb value and
     * should be used with other spectrum component or with
     * overwrite_material set to 0 in spectrum.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_BANDED = 2,
    /*!< This colour mapping create non-coloured strips/bands.
     *  The appearance is controlled by the number of bands and the banded ratio.
     *  This mode does not alter the rgb value except at the bands and
     *  should be used with other spectrum component or with
     *  overwrite_material set to 0 in spectrum.
     * @see cmzn_spectrumcomponent_set_banded_ratio
     * @see cmzn_spectrumcomponent_get_number_of_bands
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_BLUE = 3,
    /*!< This colour mapping create a colour spectrum from black to blue.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_GREEN = 4,
    /*!< This colour mapping create a colour spectrum from black to green.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_MONOCHROME = 5,
    /*!< This colour mapping create a monochrome (grey scale) spectrum.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_RAINBOW = 6,
    /*!< default colour mapping type
     * This colour mapping create a spectrum from blue to red, similar
     * to the colour of a rainbow.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_RED = 7,
    /*!< This colour mapping create a colour spectrum from black to red.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_STEP = 8,
    /*!< This colour mapping create a spectrum with only two colours, red and green.
     * The boundary between red and green can be altered by
     * CMZN_SPECTRUMCOMPONENT_ATTRIBUTE_STEP_VALUE.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_WHITE_TO_BLUE = 9,
    /*!< This colour mapping create a colour spectrum from black to blue.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_WHITE_TO_RED = 10,
    /*!< This colour mapping create a colour spectrum from black to red.
     */
    CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_WHITE_TO_GREEN = 11
    /*!< This colour mapping create a colour spectrum from black to green.
     */
};

/**
 * Bit flags summarising changes to a spectrum or spectrums in a spectrummodule.
 */
enum cmzn_spectrum_change_flag
{
	CMZN_SPECTRUM_CHANGE_FLAG_NONE = 0,
		/*!< spectrum(s) not changed */
	CMZN_SPECTRUM_CHANGE_FLAG_ADD = 1,
		/*!< one or more spectrums added */
	CMZN_SPECTRUM_CHANGE_FLAG_REMOVE = 2,
		/*!< one or more spectrums removed */
	CMZN_SPECTRUM_CHANGE_FLAG_IDENTIFIER = 4,
		/*!< spectrum identifier changed */
	CMZN_SPECTRUM_CHANGE_FLAG_DEFINITION = 8,
		/*!< change to spectrum attributes other than identifier. */
	CMZN_SPECTRUM_CHANGE_FLAG_FULL_RESULT = 16,
		/*!< all resultant values of spectrum changed. */
	CMZN_SPECTRUM_CHANGE_FLAG_FINAL = 32768
		/*!< final notification: owning spectrum module i.e. spectrum module
		 * has been destroyed */
};

/**
 * Type for passing logical OR of #cmzn_spectrum_change_flag
 */
typedef int cmzn_spectrum_change_flags;

/**
 * @brief Manages individual user notification of changes with a spectrum module.
 *
 * Manages individual user notification of changes with a spectrum module.
 */
struct cmzn_spectrummodulenotifier;
typedef struct cmzn_spectrummodulenotifier *cmzn_spectrummodulenotifier_id;

/**
 * @brief Information about changes to spectrums in the spectrum module.
 *
 * Information about changes to spectrums in the spectrum module,
 * sent with each callback from the spectrummodule notifier.
 */
struct cmzn_spectrummoduleevent;
typedef struct cmzn_spectrummoduleevent *cmzn_spectrummoduleevent_id;

typedef void (*cmzn_spectrummodulenotifier_callback_function)(
	cmzn_spectrummoduleevent_id event, void *client_data);

#endif
