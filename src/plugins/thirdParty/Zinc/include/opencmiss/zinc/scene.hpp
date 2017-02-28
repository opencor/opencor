/**
 * @file scene.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SCENE_HPP__
#define CMZN_SCENE_HPP__

#include "opencmiss/zinc/scene.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/graphics.hpp"
#include "opencmiss/zinc/light.hpp"
#include "opencmiss/zinc/region.hpp"
#include "opencmiss/zinc/scenefilter.hpp"
#include "opencmiss/zinc/selection.hpp"
#include "opencmiss/zinc/spectrum.hpp"
#include "opencmiss/zinc/timekeeper.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class StreaminformationScene;
class Sceneviewermodule;
class Scenepicker;

class Scene
{

protected:
	cmzn_scene_id id;

public:

	Scene() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Scene(cmzn_scene_id scene_id) : id(scene_id)
	{  }

	Scene(const Scene& scene) : id(cmzn_scene_access(scene.id))
	{  }

	Scene& operator=(const Scene& scene)
	{
		cmzn_scene_id temp_id = cmzn_scene_access(scene.id);
		if (0 != id)
		{
			cmzn_scene_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Scene()
	{
		if (0 != id)
		{
			cmzn_scene_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_scene_id getId() const
	{
		return id;
	}

	int beginChange()
	{
		return cmzn_scene_begin_change(id);
	}

	int endChange()
	{
		return cmzn_scene_end_change(id);
	}

	int convertToPointCloud(const Scenefilter& filter, const Nodeset& nodeset,
		const Field& coordinateField, double lineDensity, double lineDensityScaleFactor,
		double surfaceDensity, double surfaceDensityScaleFactor)
	{
		return cmzn_scene_convert_to_point_cloud(id, filter.getId(),
			nodeset.getId(), coordinateField.getId(),
			lineDensity, lineDensityScaleFactor,
			surfaceDensity, surfaceDensityScaleFactor);
	}

	int convertPointsToNodes(const Scenefilter& filter, const Nodeset& nodeset,
		const Field& coordinateField)
	{
		return cmzn_scene_convert_points_to_nodes(id, filter.getId(),
			nodeset.getId(), coordinateField.getId());
	}

	Graphics createGraphics(Graphics::Type graphicsType)
	{
		return Graphics(cmzn_scene_create_graphics(id,
			static_cast<cmzn_graphics_type>(graphicsType)));
	}

	GraphicsContours createGraphicsContours()
	{
		return GraphicsContours(cmzn_scene_create_graphics_contours(id));
	}

	GraphicsLines createGraphicsLines()
	{
		return GraphicsLines(cmzn_scene_create_graphics_lines(id));
	}

	GraphicsPoints createGraphicsPoints()
	{
		return GraphicsPoints(cmzn_scene_create_graphics_points(id));
	}

	GraphicsStreamlines createGraphicsStreamlines()
	{
		return GraphicsStreamlines(cmzn_scene_create_graphics_streamlines(id));
	}

	GraphicsSurfaces createGraphicsSurfaces()
	{
		return GraphicsSurfaces(cmzn_scene_create_graphics_surfaces(id));
	}

	Selectionnotifier createSelectionnotifier()
	{
		return Selectionnotifier(cmzn_scene_create_selectionnotifier(id));
	}

	/**
	 * Returns the graphics of the specified name from the scene. Beware that
	 * graphics in the same scene may have the same name and this function will
	 * only return the first graphics found with the specified name;
	 *
	 * @param scene  Scene in which to find the graphics.
	 * @param name  The name of the graphics to find.
	 * @return  New reference to graphics of specified name, or 0 if not found.
	 */
	Graphics findGraphicsByName(const char *name)
	{
		return Graphics(cmzn_scene_find_graphics_by_name(id, name));
	}

	Graphics getFirstGraphics()
	{
		return Graphics(cmzn_scene_get_first_graphics(id));
	}

	Graphics getNextGraphics(const Graphics& refGraphics)
	{
		return Graphics(cmzn_scene_get_next_graphics(id, refGraphics.getId()));
	}

	Graphics getPreviousGraphics(const Graphics& refGraphics)
	{
		return Graphics(cmzn_scene_get_previous_graphics(id, refGraphics.getId()));
	}

	int getNumberOfGraphics()
	{
		return cmzn_scene_get_number_of_graphics(id);
	}

	inline Region getRegion() const
	{
		return Region(cmzn_scene_get_region(id));
	}

	inline Fontmodule getFontmodule()
	{
		return Fontmodule(cmzn_scene_get_fontmodule(id));
	}

	inline Glyphmodule getGlyphmodule()
	{
		return Glyphmodule(cmzn_scene_get_glyphmodule(id));
	}

	inline Lightmodule getLightmodule()
	{
		return Lightmodule(cmzn_scene_get_lightmodule(id));
	}

	inline Materialmodule getMaterialmodule()
	{
		return Materialmodule(cmzn_scene_get_materialmodule(id));
	}

	inline Scenefiltermodule getScenefiltermodule()
	{
		return Scenefiltermodule(cmzn_scene_get_scenefiltermodule(id));
	}

	inline Sceneviewermodule getSceneviewermodule();

	inline Spectrummodule getSpectrummodule()
	{
		return Spectrummodule(cmzn_scene_get_spectrummodule(id));
	}

	inline Tessellationmodule getTessellationmodule()
	{
		return Tessellationmodule(cmzn_scene_get_tessellationmodule(id));
	}

	inline Timekeepermodule getTimekeepermodule()
	{
		return Timekeepermodule(cmzn_scene_get_timekeepermodule(id));
	}

	Field getSelectionField()
	{
		return Field(cmzn_scene_get_selection_field(id));
	}

	int setSelectionField(const Field& selectionField)
	{
		return cmzn_scene_set_selection_field(id, selectionField.getId());
	}

	int getSpectrumDataRange(const Scenefilter& filter, const Spectrum& spectrum,
		int valuesCount, double *minimumValuesOut, double *maximumValuesOut)
	{
		return cmzn_scene_get_spectrum_data_range(id, filter.getId(),
			spectrum.getId(), valuesCount, minimumValuesOut, maximumValuesOut);
	}

	bool getVisibilityFlag()
	{
		return cmzn_scene_get_visibility_flag(id);
	}

	int setVisibilityFlag(bool visibilityFlag)
	{
		return cmzn_scene_set_visibility_flag(id, visibilityFlag);
	}

	int moveGraphicsBefore(const Graphics& graphics, const Graphics& refGraphics)
	{
		return cmzn_scene_move_graphics_before(id, graphics.getId(), refGraphics.getId());
	}

	int removeAllGraphics()
	{
		return cmzn_scene_remove_all_graphics(id);
	}

	int removeGraphics(const Graphics& graphics)
	{
		return cmzn_scene_remove_graphics(id, graphics.getId());
	}

	char *writeDescription()
	{
		return cmzn_scene_write_description(id);
	}

	int readDescription(const char *description, bool overwrite)
	{
		return cmzn_scene_read_description(id, description, overwrite);
	}

	inline Scenepicker createScenepicker();

	inline int write(const StreaminformationScene& streaminformationScene);

	inline int read(const StreaminformationScene& streaminformationScene);

	inline StreaminformationScene createStreaminformationScene();

};

inline bool operator==(const Scene& a, const Scene& b)
{
	return a.getId() == b.getId();
}

inline Scene Region::getScene()
{
	return Scene(cmzn_region_get_scene(id));
}

inline Scene Graphics::getScene()
{
	return Scene(cmzn_graphics_get_scene(id));
}

inline int Spectrum::autorange(const Scene &scene, const Scenefilter &scenefilter)
{
	return cmzn_spectrum_autorange(id, scene.getId(), scenefilter.getId());
}

} // namespace Zinc
}

#endif
