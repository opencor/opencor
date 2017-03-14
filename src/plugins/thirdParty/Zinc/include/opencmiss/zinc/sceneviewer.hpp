/**
 * @file sceneviewer.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SCENEVIEWER_HPP__
#define CMZN_SCENEVIEWER_HPP__

#include "opencmiss/zinc/sceneviewer.h"
#include "opencmiss/zinc/context.hpp"
#include "opencmiss/zinc/light.hpp"
#include "opencmiss/zinc/scene.hpp"
#include "opencmiss/zinc/scenefilter.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Sceneviewerinput;

class Sceneviewerevent
{
protected:
	cmzn_sceneviewerevent_id id;

public:

	Sceneviewerevent() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Sceneviewerevent(cmzn_sceneviewerevent_id in_sceneviewer_event_id) :
		id(in_sceneviewer_event_id)
	{  }

	Sceneviewerevent(const Sceneviewerevent& sceneviewerEvent) :
		id(cmzn_sceneviewerevent_access(sceneviewerEvent.id))
	{  }

	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_NONE,
		CHANGE_FLAG_REPAINT_REQUIRED = CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_REPAINT_REQUIRED,
		CHANGE_FLAG_TRANSFORM = CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_TRANSFORM,
		CHANGE_FLAG_FINAL = CMZN_SCENEVIEWEREVENT_CHANGE_FLAG_FINAL
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 * @see getChangeFlags
	 */
	typedef int ChangeFlags;

	Sceneviewerevent& operator=(const Sceneviewerevent& sceneviewerEvent)
	{
		cmzn_sceneviewerevent_id temp_id = cmzn_sceneviewerevent_access(sceneviewerEvent.id);
		if (0 != id)
		{
			cmzn_sceneviewerevent_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Sceneviewerevent()
	{
		if (0 != id)
		{
			cmzn_sceneviewerevent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_sceneviewerevent_id getId() const
	{
		return id;
	}

	ChangeFlags getChangeFlags() const
	{
		return static_cast<ChangeFlag>(cmzn_sceneviewerevent_get_change_flags(id));
	}

};

/**
 * @brief Base class functor for Sceneviewer notifier callbacks:
 *
 * Base class functor for Sceneviewer notifier callbacks:
 * - Derive from this class adding any user data required.
 * - Implement virtual operator()(const Sceneviewerevent&) to handle callback.
 * @see Sceneviewernotifier::setCallback()
 */
class Sceneviewercallback
{
friend class Sceneviewernotifier;
private:
	Sceneviewercallback(const Sceneviewercallback&); // not implemented
	Sceneviewercallback& operator=(const Sceneviewercallback&); // not implemented

	static void C_callback(cmzn_sceneviewerevent_id sceneviewerevent_id, void *callbackVoid)
	{
		Sceneviewerevent sceneviewerevent(cmzn_sceneviewerevent_access(sceneviewerevent_id));
		Sceneviewercallback *callback = reinterpret_cast<Sceneviewercallback *>(callbackVoid);
		(*callback)(sceneviewerevent);
	}

	virtual void operator()(const Sceneviewerevent &sceneviewerevent) = 0;

protected:
	Sceneviewercallback()
	{ }

public:
	virtual ~Sceneviewercallback()
	{ }
};

class Sceneviewernotifier
{
protected:
	cmzn_sceneviewernotifier_id id;

public:

	Sceneviewernotifier() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Sceneviewernotifier(cmzn_sceneviewernotifier_id in_sceneviewernotifier_id) :
		id(in_sceneviewernotifier_id)
	{  }

	Sceneviewernotifier(const Sceneviewernotifier& sceneviewerNotifier) :
		id(cmzn_sceneviewernotifier_access(sceneviewerNotifier.id))
	{  }

	Sceneviewernotifier& operator=(const Sceneviewernotifier& sceneviewerNotifier)
	{
		cmzn_sceneviewernotifier_id temp_id = cmzn_sceneviewernotifier_access(sceneviewerNotifier.id);
		if (0 != id)
		{
			cmzn_sceneviewernotifier_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Sceneviewernotifier()
	{
		if (0 != id)
		{
			cmzn_sceneviewernotifier_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_sceneviewernotifier_id getId() const
	{
		return id;
	}

	int setCallback(Sceneviewercallback& callback)
	{
		return cmzn_sceneviewernotifier_set_callback(
			id, callback.C_callback, static_cast<void*>(&callback));
	}

	int clearCallback()
	{
		return cmzn_sceneviewernotifier_clear_callback(id);
	}
};

class Sceneviewer
{
protected:
	cmzn_sceneviewer_id id;

public:

	enum BufferingMode
	{
		BUFFERING_MODE_INVALID = CMZN_SCENEVIEWER_BUFFERING_MODE_INVALID,
		BUFFERING_MODE_DEFAULT = CMZN_SCENEVIEWER_BUFFERING_MODE_DEFAULT,
		BUFFERING_MODE_SINGLE = CMZN_SCENEVIEWER_BUFFERING_MODE_SINGLE,
		BUFFERING_MODE_DOUBLE = CMZN_SCENEVIEWER_BUFFERING_MODE_DOUBLE,
		BUFFERING_MODE_RENDER_OFFSCREEN_AND_COPY = CMZN_SCENEVIEWER_BUFFERING_MODE_RENDER_OFFSCREEN_AND_COPY,
		BUFFERING_MODE_RENDER_OFFSCREEN_AND_BLEND = CMZN_SCENEVIEWER_BUFFERING_MODE_RENDER_OFFSCREEN_AND_BLEND
	};

	enum InteractMode
	{
		INTERACT_MODE_INVALID = CMZN_SCENEVIEWER_INTERACT_MODE_INVALID,
		INTERACT_MODE_STANDARD = CMZN_SCENEVIEWER_INTERACT_MODE_STANDARD,
		INTERACT_MODE_2D = CMZN_SCENEVIEWER_INTERACT_MODE_2D
	};

	enum ProjectionMode
	{
		PROJECTION_MODE_INVALID = CMZN_SCENEVIEWER_PROJECTION_MODE_INVALID,
		PROJECTION_MODE_PARALLEL = CMZN_SCENEVIEWER_PROJECTION_MODE_PARALLEL,
		PROJECTION_MODE_PERSPECTIVE = CMZN_SCENEVIEWER_PROJECTION_MODE_PERSPECTIVE
	};

	enum StereoMode
	{
		STEREO_MODE_INVALID = CMZN_SCENEVIEWER_STEREO_MODE_INVALID,
		STEREO_MODE_DEFAULT = CMZN_SCENEVIEWER_STEREO_MODE_DEFAULT,
		STEREO_MODE_MONO = CMZN_SCENEVIEWER_STEREO_MODE_MONO,
		STEREO_MODE_STEREO = CMZN_SCENEVIEWER_STEREO_MODE_STEREO
	};

	enum TransparencyMode
	{
		TRANSPARENCY_MODE_INVALID = CMZN_SCENEVIEWER_TRANSPARENCY_MODE_INVALID,
		TRANSPARENCY_MODE_FAST = CMZN_SCENEVIEWER_TRANSPARENCY_MODE_FAST,
		TRANSPARENCY_MODE_SLOW = CMZN_SCENEVIEWER_TRANSPARENCY_MODE_SLOW,
		TRANSPARENCY_MODE_ORDER_INDEPENDENT = CMZN_SCENEVIEWER_TRANSPARENCY_MODE_ORDER_INDEPENDENT
	};

	enum ViewportMode
	{
		VIEWPORT_MODE_INVALID = CMZN_SCENEVIEWER_VIEWPORT_MODE_INVALID,
		VIEWPORT_MODE_ABSOLUTE = CMZN_SCENEVIEWER_VIEWPORT_MODE_ABSOLUTE,
		VIEWPORT_MODE_RELATIVE = CMZN_SCENEVIEWER_VIEWPORT_MODE_RELATIVE,
		VIEWPORT_MODE_DISTORTING_RELATIVE = CMZN_SCENEVIEWER_VIEWPORT_MODE_DISTORTING_RELATIVE
	};

	Sceneviewer() : id(0)
	{  }

	// takes ownership of C-style region reference
	explicit Sceneviewer(cmzn_sceneviewer_id in_sceneviewer_id) :
		id(in_sceneviewer_id)
	{  }

	Sceneviewer(const Sceneviewer& sceneviewermodule) :
		id(cmzn_sceneviewer_access(sceneviewermodule.id))
	{  }

	Sceneviewer& operator=(const Sceneviewer& sceneviewer)
	{
		cmzn_sceneviewer_id temp_id = cmzn_sceneviewer_access(sceneviewer.id);
		if (0 != id)
		{
			cmzn_sceneviewer_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Sceneviewer()
	{
		if (0 != id)
		{
			cmzn_sceneviewer_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_sceneviewer_id getId() const
	{
		return id;
	}

	int beginChange()
	{
		return cmzn_sceneviewer_begin_change(id);
	}

	int endChange()
	{
		return cmzn_sceneviewer_end_change(id);
	}

	int renderScene()
	{
		return cmzn_sceneviewer_render_scene(id);
	}

	int setScene(const Scene& scene)
	{
		return cmzn_sceneviewer_set_scene(id, scene.getId());
	}

	Scene getScene()
	{
		return Scene(cmzn_sceneviewer_get_scene(id));
	}

	int setScenefilter(const Scenefilter& scenefilter)
	{
		return cmzn_sceneviewer_set_scenefilter(id, scenefilter.getId());
	}

	Scenefilter getScenefilter()
	{
		return Scenefilter(cmzn_sceneviewer_get_scenefilter(id));
	}

	int setViewportSize(int width, int height)
	{
		return cmzn_sceneviewer_set_viewport_size(id, width, height);
	}

	inline Sceneviewerinput createSceneviewerinput();

	inline int processSceneviewerinput(const Sceneviewerinput& input);

	int getAntialiasSampling()
	{
		return cmzn_sceneviewer_get_antialias_sampling(id);
	}

	int setAntialiasSampling(int numberOfSamples)
	{
		return cmzn_sceneviewer_set_antialias_sampling(id, numberOfSamples);
	}

	int getEyePosition(double *eyeValuesOut3)
	{
		return cmzn_sceneviewer_get_eye_position(id, eyeValuesOut3);
	}

	int setEyePosition(const double *eyeValuesIn3)
	{
		return cmzn_sceneviewer_set_eye_position(id, eyeValuesIn3);
	}

	InteractMode getInteractMode()
	{
		return static_cast<InteractMode>(cmzn_sceneviewer_get_interact_mode(id));
	}

	int setInteractMode(InteractMode interactMode)
	{
		return cmzn_sceneviewer_set_interact_mode(id,
			static_cast<cmzn_sceneviewer_interact_mode>(interactMode));
	}

	int getLookatPosition(double *lookatValuesOut3)
	{
		return cmzn_sceneviewer_get_lookat_position(id, lookatValuesOut3);
	}

	int setLookatPosition(const double *lookatValuesIn3)
	{
		return cmzn_sceneviewer_set_lookat_position(id, lookatValuesIn3);
	}

	bool getPerturbLinesFlag()
	{
		return cmzn_sceneviewer_get_perturb_lines_flag(id);
	}

	int setPerturbLinesFlag(bool value)
	{
		return cmzn_sceneviewer_set_perturb_lines_flag(id, value);
	}

	ProjectionMode getProjectionMode()
	{
		return static_cast<ProjectionMode>(cmzn_sceneviewer_get_projection_mode(id));
	}

	int setProjectionMode(ProjectionMode projectionMode)
	{
		return cmzn_sceneviewer_set_projection_mode(id,
			static_cast<cmzn_sceneviewer_projection_mode>(projectionMode));
	}

	double getTranslationRate()
	{
		return cmzn_sceneviewer_get_translation_rate(id);
	}

	int setTranslationRate(double translationRate)
	{
		return cmzn_sceneviewer_set_translation_rate(id, translationRate);
	}

	double getTumbleRate()
	{
		return cmzn_sceneviewer_get_tumble_rate(id);
	}

	int setTumbleRate(double tumbleRate)
	{
		return cmzn_sceneviewer_set_tumble_rate(id, tumbleRate);
	}

	double getZoomRate()
	{
		return cmzn_sceneviewer_get_zoom_rate(id);
	}

	int setZoomRate(double zoomRate)
	{
		return cmzn_sceneviewer_set_zoom_rate(id, zoomRate);
	}

	int getUpVector(double *upVectorValuesOut3)
	{
		return cmzn_sceneviewer_get_up_vector(id, upVectorValuesOut3);
	}

	int setUpVector(const double *upVectorValuesIn3)
	{
		return cmzn_sceneviewer_set_up_vector(id, upVectorValuesIn3);
	}

	int getLookatParameters(double *eyeValuesOut3, double *lookatValuesOut3, double *upVectorValuesOut3)
	{
		return cmzn_sceneviewer_get_lookat_parameters(id,
			eyeValuesOut3, lookatValuesOut3, upVectorValuesOut3);
	}

	int setLookatParametersNonSkew(const double *eyeValuesIn3, const double *lookatValuesIn3, const double *upVectorValuesIn3)
	{
		return cmzn_sceneviewer_set_lookat_parameters_non_skew(id,
			eyeValuesIn3, lookatValuesIn3, upVectorValuesIn3);
	}

	int getViewingVolume(double *left,double *right,double *bottom,double *top,
		double *near_plane, double *far_plane)
	{
		return cmzn_sceneviewer_get_viewing_volume(id, left, right, bottom, top,
			near_plane, far_plane);
	}

	int setViewingVolume(double left, double right, double bottom, double top,
		double near_plane, double far_plane)
	{
		return cmzn_sceneviewer_set_viewing_volume(id, left, right, bottom, top,
			near_plane, far_plane);
	}

	int setBackgroundColourComponentRGB(double red, double green, double blue)
	{
		return cmzn_sceneviewer_set_background_colour_component_rgb(id, red, green, blue);
	}

	int setBackgroundColourComponentRGBA(double red, double green, double blue, double alpha)
	{
		return cmzn_sceneviewer_set_background_colour_component_rgba(id, red, green, blue, alpha);
	}

	int setBackgroundColourRGB(const double *valuesIn3)
	{
		return cmzn_sceneviewer_set_background_colour_rgb(id, valuesIn3);
	}

	int setBackgroundColourRGBA(const double *valuesIn4)
	{
		return cmzn_sceneviewer_set_background_colour_rgba(id, valuesIn4);
	}

	int getBackgroundColourRGB(double *valuesOut3)
	{
		return cmzn_sceneviewer_get_background_colour_rgb(id, valuesOut3);
	}

	int getBackgroundColourRGBA(double *valuesOut4)
	{
		return cmzn_sceneviewer_get_background_colour_rgba(id, valuesOut4);
	}

	int viewAll()
	{
		return cmzn_sceneviewer_view_all(id);
	}

	TransparencyMode getTransparencyMode()
	{
		return static_cast<TransparencyMode>(cmzn_sceneviewer_get_transparency_mode(id));
	}

	int setTransparencyMode(TransparencyMode transparencyMode)
	{
		return cmzn_sceneviewer_set_transparency_mode(id,
			static_cast<cmzn_sceneviewer_transparency_mode>(transparencyMode));
	}

	int getTransparencyLayers()
	{
		return cmzn_sceneviewer_get_transparency_layers(id);
	}

	int setTransparencyLayers(int layers)
	{
		return cmzn_sceneviewer_set_transparency_layers(id, layers);
	}

	double getViewAngle()
	{
		return cmzn_sceneviewer_get_view_angle(id);
	}

	int setViewAngle(double viewAngle)
	{
		return cmzn_sceneviewer_set_view_angle(id, viewAngle);
	}

	ViewportMode getViewportMode()
	{
		return static_cast<ViewportMode>(cmzn_sceneviewer_get_viewport_mode(id));
	}

	int setViewportMode(ViewportMode viewportMode)
	{
		return cmzn_sceneviewer_set_viewport_mode(id,
			static_cast<cmzn_sceneviewer_viewport_mode>(viewportMode));
	}

	double getFarClippingPlane()
	{
		return cmzn_sceneviewer_get_far_clipping_plane(id);
	}

	double getNearClippingPlane()
	{
		return cmzn_sceneviewer_get_near_clipping_plane(id);
	}

	int setFarClippingPlane(double farClippingPlane)
	{
		return cmzn_sceneviewer_set_far_clipping_plane(id, farClippingPlane);
	}

	int setNearClippingPlane(double nearClippingPlane)
	{
		return cmzn_sceneviewer_set_near_clipping_plane(id, nearClippingPlane);
	}

	int writeImageToFile(const char *file_name, int force_onscreen, int preferred_width,
		int preferred_height, int preferred_antialias, int preferred_transparency_layers)
	{
		return cmzn_sceneviewer_write_image_to_file(id, file_name, force_onscreen, preferred_width,
			preferred_height, preferred_antialias, preferred_transparency_layers);
	}

	int addLight(const Light& light)
	{
		return cmzn_sceneviewer_add_light(id, light.getId());
	}

	bool hasLight(const Light& light)
	{
		return cmzn_sceneviewer_has_light(id, light.getId());
	}

	int removeLight(const Light& light)
	{
		return cmzn_sceneviewer_remove_light(id, light.getId());
	}

	bool isLightingLocalViewer()
	{
		return cmzn_sceneviewer_is_lighting_local_viewer(this->id);
	}

	int setLightingLocalViewer(bool value)
	{
		return cmzn_sceneviewer_set_lighting_local_viewer(this->id, value);
	}

	bool isLightingTwoSided()
	{
		return cmzn_sceneviewer_is_lighting_two_sided(this->id);
	}

	int setLightingTwoSided(bool value)
	{
		return cmzn_sceneviewer_set_lighting_two_sided(this->id, value);
	}

	int transformCoordinates(Scenecoordinatesystem inCoordinateSystem,
		Scenecoordinatesystem outCoordinateSystem, const Scene& localScene,
		const double *valuesIn3, double *valuesOut3)
	{
		return cmzn_sceneviewer_transform_coordinates(id,
			static_cast<cmzn_scenecoordinatesystem>(inCoordinateSystem),
			static_cast<cmzn_scenecoordinatesystem>(outCoordinateSystem),
			localScene.getId(), valuesIn3, valuesOut3);
	}

	int readDescription(const char *description)
	{
		return cmzn_sceneviewer_read_description(this->id, description);
	}

	char *writeDescription()
	{
		return cmzn_sceneviewer_write_description(this->id);
	}

	Sceneviewernotifier createSceneviewernotifier()
	{
		return Sceneviewernotifier(cmzn_sceneviewer_create_sceneviewernotifier(id));
	}

};

class Sceneviewermodule
{
protected:
	cmzn_sceneviewermodule_id id;

public:

	Sceneviewermodule() : id(0)
	{  }

	// takes ownership of C-style region reference
	Sceneviewermodule(cmzn_sceneviewermodule_id in_sceneviewermodule_id) :
		id(in_sceneviewermodule_id)
	{  }

	Sceneviewermodule(const Sceneviewermodule& sceneviewermodule) :
		id(cmzn_sceneviewermodule_access(sceneviewermodule.id))
	{  }

	Sceneviewermodule& operator=(const Sceneviewermodule& sceneviewermodule)
	{
		cmzn_sceneviewermodule_id temp_id = cmzn_sceneviewermodule_access(sceneviewermodule.id);
		if (0 != id)
		{
			cmzn_sceneviewermodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Sceneviewermodule()
	{
		if (0 != id)
		{
			cmzn_sceneviewermodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_sceneviewermodule_id getId() const
	{
		return id;
	}

	Sceneviewer createSceneviewer(Sceneviewer::BufferingMode buffering_mode, Sceneviewer::StereoMode stereo_mode)
	{
		return Sceneviewer(cmzn_sceneviewermodule_create_sceneviewer(id,
			static_cast<cmzn_sceneviewer_buffering_mode>(buffering_mode),
			static_cast<cmzn_sceneviewer_stereo_mode>(stereo_mode)));
	}

	int getDefaultBackgroundColourRGB(double *valuesOut3)
	{
		return cmzn_sceneviewermodule_get_default_background_colour_rgb(id, valuesOut3);
	}

	int getDefaultBackgroundColourRGBA(double *valuesOut4)
	{
		return cmzn_sceneviewermodule_get_default_background_colour_rgba(id, valuesOut4);
	}

	int setDefaultBackgroundColourRGB(const double *valuesIn3)
	{
		return cmzn_sceneviewermodule_set_default_background_colour_rgb(id, valuesIn3);
	}

	int setDefaultBackgroundColourRGBA(const double *valuesIn4)
	{
		return cmzn_sceneviewermodule_set_default_background_colour_rgba(id, valuesIn4);
	}

};

inline Sceneviewermodule Context::getSceneviewermodule()
{
	return Sceneviewermodule(cmzn_context_get_sceneviewermodule(id));
}

inline Sceneviewermodule Scene::getSceneviewermodule()
{
	return Sceneviewermodule(cmzn_scene_get_sceneviewermodule(id));
}

}  // namespace Zinc
}

#endif
