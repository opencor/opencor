/**
 * @file spectrum.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SPECTRUM_HPP__
#define CMZN_SPECTRUM_HPP__

#include "opencmiss/zinc/spectrum.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{
class Scene;
class Scenefilter;
class Spectrummodulenotifier;

class Spectrumcomponent
{
protected:
	cmzn_spectrumcomponent_id id;

public:

	Spectrumcomponent() :
		id(0)
	{
	}

	// takes ownership of C handle, responsibility for destroying it
	explicit Spectrumcomponent(
		cmzn_spectrumcomponent_id in_spectrumcomponent_id) :
		id(in_spectrumcomponent_id)
	{
	}

	Spectrumcomponent(const Spectrumcomponent& spectrumComponent) :
		id(cmzn_spectrumcomponent_access(spectrumComponent.id))
	{
	}

	Spectrumcomponent& operator=(const Spectrumcomponent& spectrumComponent)
	{
		cmzn_spectrumcomponent_id temp_id = cmzn_spectrumcomponent_access(
			spectrumComponent.id);
		if (0 != id)
		{
			cmzn_spectrumcomponent_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Spectrumcomponent()
	{
		if (0 != id)
		{
			cmzn_spectrumcomponent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	enum ScaleType
	{
		SCALE_TYPE_INVALID = CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_INVALID,
		SCALE_TYPE_LINEAR = CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_LINEAR,
		/*!< default ScaleType */
		SCALE_TYPE_LOG = CMZN_SPECTRUMCOMPONENT_SCALE_TYPE_LOG
	};

	enum ColourMappingType
	{
		COLOUR_MAPPING_TYPE_INVALID = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_INVALID,
		COLOUR_MAPPING_TYPE_ALPHA = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_ALPHA,
		COLOUR_MAPPING_TYPE_BANDED = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_BANDED,
		COLOUR_MAPPING_TYPE_BLUE = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_BLUE,
		COLOUR_MAPPING_TYPE_GREEN = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_GREEN,
		COLOUR_MAPPING_TYPE_MONOCHROME = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_MONOCHROME,
		COLOUR_MAPPING_TYPE_RAINBOW = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_RAINBOW,
		/*!< default ColourMappingType */
		COLOUR_MAPPING_TYPE_RED = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_RED,
		COLOUR_MAPPING_TYPE_STEP = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_STEP,
		COLOUR_MAPPING_TYPE_WHITE_TO_BLUE = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_WHITE_TO_BLUE,
		COLOUR_MAPPING_TYPE_WHITE_TO_RED = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_WHITE_TO_RED,
		COLOUR_MAPPING_TYPE_WHITE_TO_GREEN = CMZN_SPECTRUMCOMPONENT_COLOUR_MAPPING_TYPE_WHITE_TO_GREEN
	};

	cmzn_spectrumcomponent_id getId() const
	{
		return id;
	}

	double getRangeMinimum()
	{
		return cmzn_spectrumcomponent_get_range_minimum(id);
	}

	int setRangeMinimum(double value)
	{
		return cmzn_spectrumcomponent_set_range_minimum(id, value);
	}

	double getRangeMaximum()
	{
		return cmzn_spectrumcomponent_get_range_maximum(id);
	}

	int setRangeMaximum(double value)
	{
		return cmzn_spectrumcomponent_set_range_maximum(id, value);
	}

	double getColourMinimum()
	{
		return cmzn_spectrumcomponent_get_colour_minimum(id);
	}

	int setColourMinimum(double value)
	{
		return cmzn_spectrumcomponent_set_colour_minimum(id,value);
	}

	double getColourMaximum()
	{
		return cmzn_spectrumcomponent_get_colour_maximum(id);
	}

	int setColourMaximum(double value)
	{
		return cmzn_spectrumcomponent_set_colour_maximum(id, value);
	}

	double getStepValue()
	{
		return cmzn_spectrumcomponent_get_step_value(id);
	}

	int setStepValue(double value)
	{
		return cmzn_spectrumcomponent_set_step_value(id, value);
	}

	double getExaggeration()
	{
		return cmzn_spectrumcomponent_get_exaggeration(id);
	}

	int setExaggeration(double value)
	{
		return cmzn_spectrumcomponent_set_exaggeration(id, value);
	}

	double getBandedRatio()
	{
		return cmzn_spectrumcomponent_get_banded_ratio(id);
	}

	int setBandedRatio(double value)
	{
		return cmzn_spectrumcomponent_set_banded_ratio(id, value);
	}

	bool isActive()
	{
		return cmzn_spectrumcomponent_is_active(id);
	}

	int setActive(bool active)
	{
		return cmzn_spectrumcomponent_set_active(id, active);
	}

	bool isColourReverse()
	{
		return cmzn_spectrumcomponent_is_colour_reverse(id);
	}

	int setColourReverse(bool reverse)
	{
		return cmzn_spectrumcomponent_set_colour_reverse(id, reverse);
	}

	bool isExtendAbove()
	{
		return cmzn_spectrumcomponent_is_extend_above(id);
	}

	int setExtendAbove(bool extend_above)
	{
		return cmzn_spectrumcomponent_set_extend_above(id, extend_above);
	}

	bool isExtendBelow()
	{
		return cmzn_spectrumcomponent_is_extend_below(id);
	}

	int setExtendBelow(bool extend_below)
	{
		return cmzn_spectrumcomponent_set_extend_below(id, extend_below);
	}

	int getFieldComponent()
	{
		return cmzn_spectrumcomponent_get_field_component(id);
	}

	int setFieldComponent(int componentNumber)
	{
		return cmzn_spectrumcomponent_set_field_component(id,
			componentNumber);
	}

	bool isFixMaximum()
	{
		return cmzn_spectrumcomponent_is_fix_maximum(id);
	}

	int setFixMaximum(bool fixMaximum)
	{
		return cmzn_spectrumcomponent_set_fix_maximum(id, fixMaximum);
	}

	bool isFixMinimum()
	{
		return cmzn_spectrumcomponent_is_fix_minimum(id);
	}

	int setFixMinimum(bool fixMinimum)
	{
		return cmzn_spectrumcomponent_set_fix_minimum(id, fixMinimum);
	}

	int getNumberOfBands()
	{
		return cmzn_spectrumcomponent_get_number_of_bands(id);
	}

	int setNumberOfBands(int numberOfBands)
	{
		return cmzn_spectrumcomponent_set_number_of_bands(id, numberOfBands);
	}

	ScaleType getScaleType()
	{
		return static_cast<ScaleType>(cmzn_spectrumcomponent_get_scale_type(
			id));
	}

	int setScaleType(ScaleType scaleType)
	{
		return cmzn_spectrumcomponent_set_scale_type(id,
			static_cast<cmzn_spectrumcomponent_scale_type>(scaleType));
	}

	ColourMappingType getColourMappingType()
	{
		return static_cast<ColourMappingType>(cmzn_spectrumcomponent_get_colour_mapping_type(
			id));
	}

	int setColourMappingType(ColourMappingType colourMapping)
	{
		return cmzn_spectrumcomponent_set_colour_mapping_type(id,
			static_cast<cmzn_spectrumcomponent_colour_mapping_type>(colourMapping));
	}

};

class Spectrum
{
protected:
	cmzn_spectrum_id id;

public:

	Spectrum() :
		id(0)
	{
	}

	// takes ownership of C handle, responsibility for destroying it
	explicit Spectrum(cmzn_spectrum_id in_spectrum_id) :
		id(in_spectrum_id)
	{
	}

	Spectrum(const Spectrum& spectrum) :
		id(cmzn_spectrum_access(spectrum.id))
	{
	}

	Spectrum& operator=(const Spectrum& spectrum)
	{
		cmzn_spectrum_id temp_id = cmzn_spectrum_access(spectrum.id);
		if (0 != id)
		{
			cmzn_spectrum_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Spectrum()
	{
		if (0 != id)
		{
			cmzn_spectrum_destroy(&id);
		}
	}

	enum ChangeFlag
	{
	 CHANGE_FLAG_NONE = CMZN_SPECTRUM_CHANGE_FLAG_NONE,
	 CHANGE_FLAG_ADD = CMZN_SPECTRUM_CHANGE_FLAG_ADD,
	 CHANGE_FLAG_REMOVE = CMZN_SPECTRUM_CHANGE_FLAG_REMOVE,
	 CHANGE_FLAG_IDENTIFIER = CMZN_SPECTRUM_CHANGE_FLAG_IDENTIFIER,
	 CHANGE_FLAG_DEFINITION = CMZN_SPECTRUM_CHANGE_FLAG_DEFINITION,
	 CHANGE_FLAG_FULL_RESULT = CMZN_SPECTRUM_CHANGE_FLAG_FULL_RESULT,
	 CHANGE_FLAG_FINAL = CMZN_SPECTRUM_CHANGE_FLAG_FINAL
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 * @see Spectrummoduleevent::getSpectrumChangeFlags
	 */
	typedef int ChangeFlags;

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_spectrum_id getId() const
	{
		return id;
	}

	bool isManaged()
	{
		return cmzn_spectrum_is_managed(id);
	}

	int setManaged(bool value)
	{
		return cmzn_spectrum_set_managed(id, value);
	}

	int beginChange()
	{
		return cmzn_spectrum_begin_change(id);
	}

	int endChange()
	{
		return cmzn_spectrum_end_change(id);
	}

	char *getName()
	{
		return cmzn_spectrum_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_spectrum_set_name(id, name);
	}

	int getNumberOfSpectrumcomponents()
	{
		return cmzn_spectrum_get_number_of_spectrumcomponents(id);
	}

	Spectrumcomponent createSpectrumcomponent()
	{
		return Spectrumcomponent(cmzn_spectrum_create_spectrumcomponent(id));
	}

	Spectrumcomponent getFirstSpectrumcomponent()
	{
		return Spectrumcomponent(cmzn_spectrum_get_first_spectrumcomponent(id));
	}

	Spectrumcomponent getNextSpectrumcomponent(const Spectrumcomponent& refComponent)
	{
		return Spectrumcomponent(
			cmzn_spectrum_get_next_spectrumcomponent(id, refComponent.getId()));
	}

	Spectrumcomponent getPreviousSpectrumcomponent(const Spectrumcomponent& refComponent)
	{
		return Spectrumcomponent(
			cmzn_spectrum_get_previous_spectrumcomponent(id, refComponent.getId()));
	}

	int moveSpectrumcomponentBefore(const Spectrumcomponent& component,
		const Spectrumcomponent& refComponent)
	{
		return cmzn_spectrum_move_spectrumcomponent_before(id, component.getId(),
			refComponent.getId());
	}

	int removeSpectrumcomponent(const Spectrumcomponent& component)
	{
		return cmzn_spectrum_remove_spectrumcomponent(id, component.getId());
	}

	int removeAllSpectrumcomponents()
	{
		return cmzn_spectrum_remove_all_spectrumcomponents(id);
	}

	bool isMaterialOverwrite()
	{
		return cmzn_spectrum_is_material_overwrite(id);
	}

	int setMaterialOverwrite(bool overwrite)
	{
		return cmzn_spectrum_set_material_overwrite(id, overwrite);
	}

	inline int autorange(const Scene &scene, const Scenefilter &scenefilter);

};

inline bool operator==(const Spectrum& a, const Spectrum& b)
{
	return a.getId() == b.getId();
}

class Spectrumiterator
{
private:

	cmzn_spectrumiterator_id id;

public:

	Spectrumiterator() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Spectrumiterator(cmzn_spectrumiterator_id iterator_id) :
		id(iterator_id)
	{ }

	Spectrumiterator(const Spectrumiterator& spectrumiterator) :
		id(cmzn_spectrumiterator_access(spectrumiterator.id))
	{ }

	Spectrumiterator& operator=(const Spectrumiterator& spectrumiterator)
	{
		cmzn_spectrumiterator_id temp_id = cmzn_spectrumiterator_access(spectrumiterator.id);
		if (0 != id)
		{
			cmzn_spectrumiterator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Spectrumiterator()
	{
		if (0 != id)
		{
			cmzn_spectrumiterator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Spectrum next()
	{
		return Spectrum(cmzn_spectrumiterator_next(id));
	}
};

class Spectrummodule
{
protected:
	cmzn_spectrummodule_id id;

public:

	Spectrummodule() :
		id(0)
	{
	}

	// takes ownership of C handle, responsibility for destroying it
	explicit Spectrummodule(cmzn_spectrummodule_id in_spectrummodule_id) :
		id(in_spectrummodule_id)
	{
	}

	Spectrummodule(const Spectrummodule& spectrummodule) :
		id(cmzn_spectrummodule_access(spectrummodule.id))
	{
	}

	Spectrummodule& operator=(const Spectrummodule& spectrummodule)
	{
		cmzn_spectrummodule_id temp_id = cmzn_spectrummodule_access(
			spectrummodule.id);
		if (0 != id)
		{
			cmzn_spectrummodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Spectrummodule()
	{
		if (0 != id)
		{
			cmzn_spectrummodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_spectrummodule_id getId() const
	{
		return id;
	}

	Spectrum createSpectrum()
	{
		return Spectrum(cmzn_spectrummodule_create_spectrum(id));
	}

	Spectrumiterator createSpectrumiterator()
	{
		return Spectrumiterator(cmzn_spectrummodule_create_spectrumiterator(id));
	}

	Spectrum findSpectrumByName(const char *name)
	{
		return Spectrum(cmzn_spectrummodule_find_spectrum_by_name(id, name));
	}

	int beginChange()
	{
		return cmzn_spectrummodule_begin_change(id);
	}

	int endChange()
	{
		return cmzn_spectrummodule_end_change(id);
	}

	Spectrum getDefaultSpectrum()
	{
		return Spectrum(cmzn_spectrummodule_get_default_spectrum(id));
	}

	int setDefaultSpectrum(const Spectrum& spectrum)
	{
		return cmzn_spectrummodule_set_default_spectrum(id, spectrum.getId());
	}

	int readDescription(const char *description)
	{
		return cmzn_spectrummodule_read_description(this->id, description);
	}

	char *writeDescription()
	{
		return cmzn_spectrummodule_write_description(this->id);
	}

	inline Spectrummodulenotifier createSpectrummodulenotifier();

};

class Spectrummoduleevent
{
protected:
	cmzn_spectrummoduleevent_id id;

public:

	Spectrummoduleevent() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Spectrummoduleevent(cmzn_spectrummoduleevent_id in_spectrummodule_event_id) :
		id(in_spectrummodule_event_id)
	{  }

	Spectrummoduleevent(const Spectrummoduleevent& spectrummoduleEvent) :
		id(cmzn_spectrummoduleevent_access(spectrummoduleEvent.id))
	{  }

	Spectrummoduleevent& operator=(const Spectrummoduleevent& spectrummoduleEvent)
	{
		cmzn_spectrummoduleevent_id temp_id = cmzn_spectrummoduleevent_access(spectrummoduleEvent.id);
		if (0 != id)
			cmzn_spectrummoduleevent_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Spectrummoduleevent()
	{
		if (0 != id)
		{
			cmzn_spectrummoduleevent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_spectrummoduleevent_id getId() const
	{
		return id;
	}

	Spectrum::ChangeFlags getSpectrumChangeFlags(const Spectrum& spectrum) const
	{
		return cmzn_spectrummoduleevent_get_spectrum_change_flags(id, spectrum.getId());
	}

	Spectrum::ChangeFlags getSummarySpectrumChangeFlags() const
	{
		return cmzn_spectrummoduleevent_get_summary_spectrum_change_flags(id);
	}

};

/**
 * @brief Base class functor for spectrum module notifier callbacks
 *
 * Base class functor for spectrum module notifier callbacks:
 * - Derive from this class adding any user data required.
 * - Implement virtual operator()(const Spectrummoduleevent&) to handle callback.
 * @see Spectrummodulenotifier::setCallback()
 */
class Spectrummodulecallback
{
friend class Spectrummodulenotifier;
private:
	Spectrummodulecallback(const Spectrummodulecallback&); // not implemented
	Spectrummodulecallback& operator=(const Spectrummodulecallback&); // not implemented

	static void C_callback(cmzn_spectrummoduleevent_id spectrummoduleevent_id, void *callbackVoid)
	{
		Spectrummoduleevent spectrummoduleevent(cmzn_spectrummoduleevent_access(spectrummoduleevent_id));
		Spectrummodulecallback *callback = reinterpret_cast<Spectrummodulecallback *>(callbackVoid);
		(*callback)(spectrummoduleevent);
	}

  virtual void operator()(const Spectrummoduleevent &spectrummoduleevent) = 0;

protected:
	Spectrummodulecallback()
	{ }

public:
	virtual ~Spectrummodulecallback()
	{ }
};

class Spectrummodulenotifier
{
protected:
	cmzn_spectrummodulenotifier_id id;

public:

	Spectrummodulenotifier() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Spectrummodulenotifier(cmzn_spectrummodulenotifier_id in_spectrummodulenotifier_id) :
		id(in_spectrummodulenotifier_id)
	{  }

	Spectrummodulenotifier(const Spectrummodulenotifier& spectrummoduleNotifier) :
		id(cmzn_spectrummodulenotifier_access(spectrummoduleNotifier.id))
	{  }

	Spectrummodulenotifier& operator=(const Spectrummodulenotifier& spectrummoduleNotifier)
	{
		cmzn_spectrummodulenotifier_id temp_id = cmzn_spectrummodulenotifier_access(spectrummoduleNotifier.id);
		if (0 != id)
		{
			cmzn_spectrummodulenotifier_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Spectrummodulenotifier()
	{
		if (0 != id)
		{
			cmzn_spectrummodulenotifier_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_spectrummodulenotifier_id getId() const
	{
		return id;
	}

	int setCallback(Spectrummodulecallback& callback)
	{
		return cmzn_spectrummodulenotifier_set_callback(id, callback.C_callback, static_cast<void*>(&callback));
	}

	int clearCallback()
	{
		return cmzn_spectrummodulenotifier_clear_callback(id);
	}
};

inline Spectrummodulenotifier Spectrummodule::createSpectrummodulenotifier()
{
	return Spectrummodulenotifier(cmzn_spectrummodule_create_spectrummodulenotifier(id));
}

inline Spectrummodule Context::getSpectrummodule()
{
	return Spectrummodule(cmzn_context_get_spectrummodule(id));
}

} // namespace Zinc
}

#endif
