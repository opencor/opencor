/**
 * @file scenepicker.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_SCENEPICKER_HPP__
#define CMZN_SCENEPICKER_HPP__

#include "opencmiss/zinc/scenepicker.h"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/fieldgroup.hpp"
#include "opencmiss/zinc/graphics.hpp"
#include "opencmiss/zinc/node.hpp"
#include "opencmiss/zinc/scene.hpp"
#include "opencmiss/zinc/scenefilter.hpp"
#include "opencmiss/zinc/sceneviewer.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Scenepicker
{
protected:
	cmzn_scenepicker_id id;

public:

	Scenepicker() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Scenepicker(cmzn_scenepicker_id in_scenepicker_id) :
		id(in_scenepicker_id)
	{  }

	Scenepicker(const Scenepicker& scenepicker) :
		id(cmzn_scenepicker_access(scenepicker.id))
	{  }

	Scenepicker& operator=(const Scenepicker& scenepicker)
	{
		cmzn_scenepicker_id temp_id = cmzn_scenepicker_access(scenepicker.id);
		if (0 != id)
		{
			cmzn_scenepicker_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Scenepicker()
	{
		if (0 != id)
		{
			cmzn_scenepicker_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_scenepicker_id getId() const
	{
		return id;
	}

	int setSceneviewerRectangle(const Sceneviewer& sceneviewer, Scenecoordinatesystem coordinateSystem, double x1,
		double y1, double x2, double y2)
	{
		return cmzn_scenepicker_set_sceneviewer_rectangle(
			id , sceneviewer.getId(),
			static_cast<cmzn_scenecoordinatesystem>(coordinateSystem),
			x1, y1, x2, y2);
	}

	Element getNearestElement()
	{
		return Element(cmzn_scenepicker_get_nearest_element(id));
	}

	Node getNearestNode()
	{
		return Node(cmzn_scenepicker_get_nearest_node(id));
	}

	Graphics getNearestElementGraphics()
	{
		return Graphics(cmzn_scenepicker_get_nearest_element_graphics(id));
	}

	Graphics getNearestNodeGraphics()
	{
		return Graphics(cmzn_scenepicker_get_nearest_node_graphics(id));
	}

	Graphics getNearestGraphics()
	{
		return Graphics(cmzn_scenepicker_get_nearest_graphics(id));
	}

	int addPickedElementsToFieldGroup(const FieldGroup& fieldGroup)
	{
		return cmzn_scenepicker_add_picked_elements_to_field_group(id,
			(reinterpret_cast<cmzn_field_group_id>(fieldGroup.getId())));
	}

	int addPickedNodesToFieldGroup(const FieldGroup& fieldGroup)
	{
		return cmzn_scenepicker_add_picked_nodes_to_field_group(id,
			(reinterpret_cast<cmzn_field_group_id>(fieldGroup.getId())));
	}

	Scene getScene()
	{
		return Scene(cmzn_scenepicker_get_scene(id));
	}

	int setScene(const Scene& scene)
	{
		return cmzn_scenepicker_set_scene(id, scene.getId());
	}

	Scenefilter getScenefilter()
	{
		return Scenefilter(cmzn_scenepicker_get_scenefilter(id));
	}

	int setScenefilter(const Scenefilter& filter)
	{
		return cmzn_scenepicker_set_scenefilter(id, filter.getId());
	}

	int getPickingVolumeCentre(double *coordinateValuesOut3)
	{
		return cmzn_scenepicker_get_picking_volume_centre(id, coordinateValuesOut3);
	}

};

inline Scenepicker Scene::createScenepicker()
{
	return Scenepicker(cmzn_scene_create_scenepicker(id));
}

}  // namespace Zinc
}

#endif
