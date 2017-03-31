/**
 * @file graphics.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_GRAPHICS_HPP__
#define CMZN_GRAPHICS_HPP__

#include "opencmiss/zinc/types/scenecoordinatesystem.hpp"
#include "opencmiss/zinc/graphics.h"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/glyph.hpp"
#include "opencmiss/zinc/font.hpp"
#include "opencmiss/zinc/material.hpp"
#include "opencmiss/zinc/spectrum.hpp"
#include "opencmiss/zinc/tessellation.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class GraphicsContours;
class GraphicsLines;
class GraphicsPoints;
class GraphicsStreamlines;
class GraphicsSurfaces;
class Graphicslineattributes;
class Graphicspointattributes;
class Graphicssamplingattributes;
class Scene;

class Graphics
{

protected:
	cmzn_graphics_id id;

public:

	Graphics() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Graphics(cmzn_graphics_id graphics_id) : id(graphics_id)
	{  }

	Graphics(const Graphics& graphics) : id(cmzn_graphics_access(graphics.id))
	{  }

	Graphics& operator=(const Graphics& graphics)
	{
		cmzn_graphics_id temp_id = cmzn_graphics_access(graphics.id);
		if (0 != id)
		{
			cmzn_graphics_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Graphics()
	{
		if (0 != id)
		{
			cmzn_graphics_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	enum RenderPolygonMode
	{
		RENDER_POLYGON_MODE_INVALID = CMZN_GRAPHICS_RENDER_POLYGON_MODE_INVALID,
		RENDER_POLYGON_MODE_SHADED = CMZN_GRAPHICS_RENDER_POLYGON_MODE_SHADED,
		RENDER_POLYGON_MODE_WIREFRAME = CMZN_GRAPHICS_RENDER_POLYGON_MODE_WIREFRAME
	};

	enum SelectMode
	{
		SELECT_MODE_INVALID = CMZN_GRAPHICS_SELECT_MODE_INVALID,
		SELECT_MODE_ON = CMZN_GRAPHICS_SELECT_MODE_ON,
		SELECT_MODE_OFF = CMZN_GRAPHICS_SELECT_MODE_OFF,
		SELECT_MODE_DRAW_SELECTED = CMZN_GRAPHICS_SELECT_MODE_DRAW_SELECTED,
		SELECT_MODE_DRAW_UNSELECTED = CMZN_GRAPHICS_SELECT_MODE_DRAW_UNSELECTED
	};

	enum Type
	{
		TYPE_INVALID = CMZN_GRAPHICS_TYPE_INVALID,
		TYPE_POINTS = CMZN_GRAPHICS_TYPE_POINTS,
		TYPE_LINES = CMZN_GRAPHICS_TYPE_LINES,
		TYPE_SURFACES = CMZN_GRAPHICS_TYPE_SURFACES,
		TYPE_CONTOURS = CMZN_GRAPHICS_TYPE_CONTOURS,
		TYPE_STREAMLINES = CMZN_GRAPHICS_TYPE_STREAMLINES
	};

	cmzn_graphics_id getId() const
	{
		return id;
	}

	Field getCoordinateField()
	{
		return Field(cmzn_graphics_get_coordinate_field(id));
	}

	int setCoordinateField(const Field& coordinateField)
	{
		return cmzn_graphics_set_coordinate_field(id, coordinateField.getId());
	}

	Field getDataField()
	{
		return Field(cmzn_graphics_get_data_field(id));
	}

	int setDataField(const Field& dataField)
	{
		return cmzn_graphics_set_data_field(id, dataField.getId());
	}

	double getRenderLineWidth()
	{
		return cmzn_graphics_get_render_line_width(id);
	}

	int setRenderLineWidth(double width)
	{
		return cmzn_graphics_set_render_line_width(id, width);
	}

	double getRenderPointSize()
	{
		return cmzn_graphics_get_render_point_size(id);
	}

	int setRenderPointSize(double size)
	{
		return cmzn_graphics_set_render_point_size(id, size);
	}

	enum RenderPolygonMode getRenderPolygonMode()
	{
		return static_cast<RenderPolygonMode>(cmzn_graphics_get_render_polygon_mode(id));
	}

	int setRenderPolygonMode(RenderPolygonMode renderPolygonMode)
	{
		return cmzn_graphics_set_render_polygon_mode(id,
			static_cast<cmzn_graphics_render_polygon_mode>(renderPolygonMode));
	}

	inline Scene getScene();

	enum SelectMode getSelectMode()
	{
		return static_cast<SelectMode>(cmzn_graphics_get_select_mode(id));
	}

	int setSelectMode(SelectMode selectMode)
	{
		return cmzn_graphics_set_select_mode(id, static_cast<cmzn_graphics_select_mode>(selectMode));
	}

	Field getSubgroupField()
	{
		return Field(cmzn_graphics_get_subgroup_field(id));
	}

	int setSubgroupField(const Field& subgroupField)
	{
		return cmzn_graphics_set_subgroup_field(id, subgroupField.getId());
	}

	Field getTextureCoordinateField()
	{
		return Field(cmzn_graphics_get_texture_coordinate_field(id));
	}

	int setTextureCoordinateField(const Field& textureCoordinateField)
	{
		return cmzn_graphics_set_texture_coordinate_field(id, textureCoordinateField.getId());
	}

	Material getMaterial()
	{
		return Material(cmzn_graphics_get_material(id));
	}

	int setMaterial(const Material& material)
	{
		return cmzn_graphics_set_material(id, material.getId());
	}

	Graphicslineattributes getGraphicslineattributes();

	Graphicspointattributes getGraphicspointattributes();

	Graphicssamplingattributes getGraphicssamplingattributes();

	Material getSelectedMaterial()
	{
		return Material(cmzn_graphics_get_selected_material(id));
	}

	int setSelectedMaterial(const Material& material)
	{
		return cmzn_graphics_set_selected_material(id, material.getId());
	}

	Spectrum getSpectrum()
	{
		return Spectrum(cmzn_graphics_get_spectrum(id));
	}

	int setSpectrum(const Spectrum& spectrum)
	{
		return cmzn_graphics_set_spectrum(id, spectrum.getId());
	}

	enum Type getType()
	{
		return static_cast<Type>(cmzn_graphics_get_type(id));
	}

	Tessellation getTessellation()
	{
		return Tessellation(cmzn_graphics_get_tessellation(id));
	}

	int setTessellation(const Tessellation& tessellation)
	{
		return cmzn_graphics_set_tessellation(id, tessellation.getId());
	}

	Field getTessellationField()
	{
		return Field(cmzn_graphics_get_tessellation_field(id));
	}

	int setTessellationField(const Field& tessellationField)
	{
		return cmzn_graphics_set_tessellation_field(id, tessellationField.getId());
	}

	bool getVisibilityFlag()
	{
		return cmzn_graphics_get_visibility_flag(id);
	}

	int setVisibilityFlag(bool visibilityFlag)
	{
		return cmzn_graphics_set_visibility_flag(id, visibilityFlag);
	}

	enum Scenecoordinatesystem getScenecoordinatesystem()
	{
		return static_cast<Scenecoordinatesystem>(cmzn_graphics_get_scenecoordinatesystem(id));
	}

	int setScenecoordinatesystem(Scenecoordinatesystem coordinateSystem)
	{
		return cmzn_graphics_set_scenecoordinatesystem(id,
			static_cast<cmzn_scenecoordinatesystem>(coordinateSystem));
	}

	Field::DomainType getFieldDomainType()
	{
		return static_cast<Field::DomainType>(cmzn_graphics_get_field_domain_type(id));
	}

	int setFieldDomainType(Field::DomainType domainType)
	{
		return cmzn_graphics_set_field_domain_type(id, static_cast<cmzn_field_domain_type>(domainType));
	}

	char *getName()
	{
		return cmzn_graphics_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_graphics_set_name(id, name);
	}

	int setElementFaceType(Element::FaceType faceType)
	{
		return cmzn_graphics_set_element_face_type(id, static_cast<cmzn_element_face_type>(faceType));
	}

	Element::FaceType getElementFaceType()
	{
		return static_cast<Element::FaceType>(cmzn_graphics_get_element_face_type(id));
	}

	bool isExterior()
	{
		return cmzn_graphics_is_exterior(id);
	}

	int setExterior(bool exterior)
	{
		return cmzn_graphics_set_exterior(id, exterior);
	}

	inline GraphicsContours castContours();
	inline GraphicsLines castLines();
	inline GraphicsPoints castPoints();
	inline GraphicsStreamlines castStreamlines();
	inline GraphicsSurfaces castSurfaces();
};

inline bool operator==(const Graphics& a, const Graphics& b)
{
	return a.getId() == b.getId();
}

class GraphicsContours : public Graphics
{
friend class Scene;
private:
	explicit GraphicsContours(cmzn_graphics_id graphics_id) : Graphics(graphics_id) {}

	inline cmzn_graphics_contours_id getDerivedId()
	{
		return reinterpret_cast<cmzn_graphics_contours_id>(this->id);
	}

public:
	GraphicsContours() : Graphics(0) {}

	explicit GraphicsContours(cmzn_graphics_contours_id contours_id)
		: Graphics(reinterpret_cast<cmzn_graphics_id>(contours_id))
	{}

	Field getIsoscalarField()
	{
		return Field(cmzn_graphics_contours_get_isoscalar_field(this->getDerivedId()));
	}

	int setIsoscalarField(const Field& field)
	{
		return cmzn_graphics_contours_set_isoscalar_field(this->getDerivedId(), field.getId());
	}

	int getListIsovalues(int valuesCount, double *valuesOut)
	{
		return cmzn_graphics_contours_get_list_isovalues(this->getDerivedId(),
			valuesCount, valuesOut);
	}

	int setListIsovalues(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphics_contours_set_list_isovalues(this->getDerivedId(),
			valuesCount, valuesIn);
	}

	double getRangeFirstIsovalue()
	{
		return cmzn_graphics_contours_get_range_first_isovalue(this->getDerivedId());
	}

	double getRangeLastIsovalue()
	{
		return cmzn_graphics_contours_get_range_last_isovalue(this->getDerivedId());
	}

	int getRangeNumberOfIsovalues()
	{
		return cmzn_graphics_contours_get_range_number_of_isovalues(this->getDerivedId());
	}

	int setRangeIsovalues(int numberOfValues, double firstIsovalue, double lastIsovalue)
	{
		return cmzn_graphics_contours_set_range_isovalues(this->getDerivedId(),
			numberOfValues, firstIsovalue, lastIsovalue);
	}

};

class GraphicsLines : public Graphics
{
friend class Scene;
private:
	explicit GraphicsLines(cmzn_graphics_id graphics_id) : Graphics(graphics_id) {}

public:
	GraphicsLines() : Graphics(0) {}

	explicit GraphicsLines(cmzn_graphics_lines_id lines_id)
		: Graphics(reinterpret_cast<cmzn_graphics_id>(lines_id))
	{}
};

class GraphicsPoints : public Graphics
{
friend class Scene;
private:
	explicit GraphicsPoints(cmzn_graphics_id graphics_id) : Graphics(graphics_id) {}

public:
	GraphicsPoints() : Graphics(0) {}

	explicit GraphicsPoints(cmzn_graphics_points_id points_id)
		: Graphics(reinterpret_cast<cmzn_graphics_id>(points_id))
	{}
};

class GraphicsStreamlines : public Graphics
{
friend class Scene;
private:
	explicit GraphicsStreamlines(cmzn_graphics_id graphics_id) : Graphics(graphics_id) {}

	inline cmzn_graphics_streamlines_id getDerivedId()
	{
		return reinterpret_cast<cmzn_graphics_streamlines_id>(this->id);
	}

public:
	GraphicsStreamlines() : Graphics(0) {}

	explicit GraphicsStreamlines(cmzn_graphics_streamlines_id streamlines_id)
		: Graphics(reinterpret_cast<cmzn_graphics_id>(streamlines_id))
	{}

	enum ColourDataType
	{
		COLOUR_DATA_TYPE_INVALID = CMZN_GRAPHICS_STREAMLINES_COLOUR_DATA_TYPE_INVALID,
		COLOUR_DATA_TYPE_FIELD = CMZN_GRAPHICS_STREAMLINES_COLOUR_DATA_TYPE_FIELD,
		COLOUR_DATA_TYPE_MAGNITUDE = CMZN_GRAPHICS_STREAMLINES_COLOUR_DATA_TYPE_MAGNITUDE,
		COLOUR_DATA_TYPE_TRAVEL_TIME = CMZN_GRAPHICS_STREAMLINES_COLOUR_DATA_TYPE_TRAVEL_TIME
	};

	enum TrackDirection
	{
		TRACK_DIRECTION_INVALID = CMZN_GRAPHICS_STREAMLINES_TRACK_DIRECTION_INVALID,
		TRACK_DIRECTION_FORWARD = CMZN_GRAPHICS_STREAMLINES_TRACK_DIRECTION_FORWARD,
		TRACK_DIRECTION_REVERSE = CMZN_GRAPHICS_STREAMLINES_TRACK_DIRECTION_REVERSE
	};

	ColourDataType getColourDataType()
	{
		return static_cast<ColourDataType>(cmzn_graphics_streamlines_get_colour_data_type(this->getDerivedId()));
	}

	int setColourDataType(ColourDataType dataType)
	{
		return cmzn_graphics_streamlines_set_colour_data_type(this->getDerivedId(),
			static_cast<cmzn_graphics_streamlines_colour_data_type>(dataType));
	}

	Field getStreamVectorField()
	{
		return Field(cmzn_graphics_streamlines_get_stream_vector_field(this->getDerivedId()));
	}

	int setStreamVectorField(const Field& field)
	{
		return cmzn_graphics_streamlines_set_stream_vector_field(this->getDerivedId(), field.getId());
	}

	TrackDirection getTrackDirection()
	{
		return static_cast<TrackDirection>(
			cmzn_graphics_streamlines_get_track_direction(this->getDerivedId()));
	}

	int setTrackDirection(TrackDirection trackDirection)
	{
		return cmzn_graphics_streamlines_set_track_direction(this->getDerivedId(),
			static_cast<cmzn_graphics_streamlines_track_direction>(trackDirection));
	}

	double getTrackLength()
	{
		return cmzn_graphics_streamlines_get_track_length(this->getDerivedId());
	}

	int setTrackLength(double length)
	{
		return cmzn_graphics_streamlines_set_track_length(this->getDerivedId(), length);
	}

};

class GraphicsSurfaces : public Graphics
{
friend class Scene;
private:
	explicit GraphicsSurfaces(cmzn_graphics_id graphics_id) : Graphics(graphics_id) {}

public:
	GraphicsSurfaces() : Graphics(0) {}

	explicit GraphicsSurfaces(cmzn_graphics_surfaces_id surfaces_id)
		: Graphics(reinterpret_cast<cmzn_graphics_id>(surfaces_id))
	{}
};

inline GraphicsContours Graphics::castContours()
{
	return GraphicsContours(cmzn_graphics_cast_contours(id));
}

inline GraphicsLines Graphics::castLines()
{
	return GraphicsLines(cmzn_graphics_cast_lines(id));
}

inline GraphicsPoints Graphics::castPoints()
{
	return GraphicsPoints(cmzn_graphics_cast_points(id));
}

inline GraphicsStreamlines Graphics::castStreamlines()
{
	return GraphicsStreamlines(cmzn_graphics_cast_streamlines(id));
}

inline GraphicsSurfaces Graphics::castSurfaces()
{
	return GraphicsSurfaces(cmzn_graphics_cast_surfaces(id));
}

class Graphicslineattributes
{
protected:
	cmzn_graphicslineattributes_id id;

public:
	Graphicslineattributes() : id(0)
	{}

	// takes ownership of C handle, responsibility for destroying it
	explicit Graphicslineattributes(cmzn_graphicslineattributes_id line_attributes_id) :
		id(line_attributes_id)
	{}

	Graphicslineattributes(const Graphicslineattributes& lineAttributes) :
		id(cmzn_graphicslineattributes_access(lineAttributes.id))
	{}

	Graphicslineattributes& operator=(const Graphicslineattributes& graphicslineattributes)
	{
		cmzn_graphicslineattributes_id temp_id = cmzn_graphicslineattributes_access(graphicslineattributes.id);
		if (0 != id)
			cmzn_graphicslineattributes_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Graphicslineattributes()
	{
		cmzn_graphicslineattributes_destroy(&id);
	}

	bool isValid() const
	{
		return (0 != id);
	}

	enum ShapeType
	{
		SHAPE_TYPE_INVALID = CMZN_GRAPHICSLINEATTRIBUTES_SHAPE_TYPE_INVALID,
		SHAPE_TYPE_LINE = CMZN_GRAPHICSLINEATTRIBUTES_SHAPE_TYPE_LINE,
		SHAPE_TYPE_RIBBON = CMZN_GRAPHICSLINEATTRIBUTES_SHAPE_TYPE_RIBBON,
		SHAPE_TYPE_CIRCLE_EXTRUSION = CMZN_GRAPHICSLINEATTRIBUTES_SHAPE_TYPE_CIRCLE_EXTRUSION,
		SHAPE_TYPE_SQUARE_EXTRUSION = CMZN_GRAPHICSLINEATTRIBUTES_SHAPE_TYPE_SQUARE_EXTRUSION
	};

	int getBaseSize(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicslineattributes_get_base_size(id, valuesCount, valuesOut);
	}

	int setBaseSize(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicslineattributes_set_base_size(id, valuesCount, valuesIn);
	}

	Field getOrientationScaleField()
	{
		return Field(cmzn_graphicslineattributes_get_orientation_scale_field(id));
	}

	int setOrientationScaleField(const Field& orientationScaleField)
	{
		return cmzn_graphicslineattributes_set_orientation_scale_field(id, orientationScaleField.getId());
	}

	int getScaleFactors(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicslineattributes_get_scale_factors(id, valuesCount, valuesOut);
	}

	int setScaleFactors(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicslineattributes_set_scale_factors(id, valuesCount, valuesIn);
	}

	ShapeType getShapeType()
	{
		return static_cast<ShapeType>(cmzn_graphicslineattributes_get_shape_type(id));
	}

	int setShapeType(ShapeType shapeType)
	{
		return cmzn_graphicslineattributes_set_shape_type(id, static_cast<cmzn_graphicslineattributes_shape_type>(shapeType));
	}

};

inline Graphicslineattributes Graphics::getGraphicslineattributes()
{
	return Graphicslineattributes(cmzn_graphics_get_graphicslineattributes(id));
}

class Graphicspointattributes
{
protected:
	cmzn_graphicspointattributes_id id;

public:
	Graphicspointattributes() : id(0)
	{}

	// takes ownership of C handle, responsibility for destroying it
	explicit Graphicspointattributes(cmzn_graphicspointattributes_id point_attributes_id) :
		id(point_attributes_id)
	{}

	Graphicspointattributes(const Graphicspointattributes& pointAttributes) :
		id(cmzn_graphicspointattributes_access(pointAttributes.id))
	{}

	Graphicspointattributes& operator=(const Graphicspointattributes& graphicspointattributes)
	{
		cmzn_graphicspointattributes_id temp_id = cmzn_graphicspointattributes_access(graphicspointattributes.id);
		if (0 != id)
			cmzn_graphicspointattributes_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Graphicspointattributes()
	{
		cmzn_graphicspointattributes_destroy(&id);
	}

	bool isValid() const
	{
		return (0 != id);
	}

	int getBaseSize(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicspointattributes_get_base_size(id, valuesCount, valuesOut);
	}

	int setBaseSize(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicspointattributes_set_base_size(id, valuesCount, valuesIn);
	}

	Font getFont()
	{
		return Font(cmzn_graphicspointattributes_get_font(id));
	}

	int setFont(const Font& font)
	{
		return cmzn_graphicspointattributes_set_font(id, font.getId());
	}

	Glyph getGlyph()
	{
		return Glyph(cmzn_graphicspointattributes_get_glyph(id));
	}

	int setGlyph(const Glyph& glyph)
	{
		return cmzn_graphicspointattributes_set_glyph(id, glyph.getId());
	}

	int getGlyphOffset(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicspointattributes_get_glyph_offset(id, valuesCount, valuesOut);
	}

	int setGlyphOffset(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicspointattributes_set_glyph_offset(id, valuesCount, valuesIn);
	}

	Glyph::RepeatMode getGlyphRepeatMode()
	{
		return static_cast<Glyph::RepeatMode>(cmzn_graphicspointattributes_get_glyph_repeat_mode(id));
	}

	int setGlyphRepeatMode(Glyph::RepeatMode glyphRepeatMode)
	{
		return cmzn_graphicspointattributes_set_glyph_repeat_mode(id,
			static_cast<enum cmzn_glyph_repeat_mode>(glyphRepeatMode));
	}

	Glyph::ShapeType getGlyphShapeType()
	{
		return static_cast<Glyph::ShapeType>(cmzn_graphicspointattributes_get_glyph_shape_type(id));
	}

	int setGlyphShapeType(Glyph::ShapeType shapeType)
	{
		return cmzn_graphicspointattributes_set_glyph_shape_type(id,
			static_cast<cmzn_glyph_shape_type>(shapeType));
	}

	Field getLabelField()
	{
		return Field(cmzn_graphicspointattributes_get_label_field(id));
	}

	int setLabelField(const Field& labelField)
	{
		return cmzn_graphicspointattributes_set_label_field(id, labelField.getId());
	}

	int getLabelOffset(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicspointattributes_get_label_offset(id, valuesCount, valuesOut);
	}

	int setLabelOffset(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicspointattributes_set_label_offset(id, valuesCount, valuesIn);
	}

	char *getLabelText(int labelNumber)
	{
		return cmzn_graphicspointattributes_get_label_text(id, labelNumber);
	}

	int setLabelText(int labelNumber, const char *labelText)
	{
		return cmzn_graphicspointattributes_set_label_text(id, labelNumber, labelText);
	}

	Field getOrientationScaleField()
	{
		return Field(cmzn_graphicspointattributes_get_orientation_scale_field(id));
	}

	int setOrientationScaleField(const Field& orientationScaleField)
	{
		return cmzn_graphicspointattributes_set_orientation_scale_field(id, orientationScaleField.getId());
	}

	int getScaleFactors(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicspointattributes_get_scale_factors(id, valuesCount, valuesOut);
	}

	int setScaleFactors(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicspointattributes_set_scale_factors(id, valuesCount, valuesIn);
	}

	Field getSignedScaleField()
	{
		return Field(cmzn_graphicspointattributes_get_signed_scale_field(id));
	}

	int setSignedScaleField(const Field& signedScaleField)
	{
		return cmzn_graphicspointattributes_set_signed_scale_field(id, signedScaleField.getId());
	}

};

inline Graphicspointattributes Graphics::getGraphicspointattributes()
{
	return Graphicspointattributes(cmzn_graphics_get_graphicspointattributes(id));
}

class Graphicssamplingattributes
{
protected:
	cmzn_graphicssamplingattributes_id id;

public:
	Graphicssamplingattributes() : id(0)
	{}

	// takes ownership of C handle, responsibility for destroying it
	explicit Graphicssamplingattributes(cmzn_graphicssamplingattributes_id sampling_attributes_id) :
		id(sampling_attributes_id)
	{}

	Graphicssamplingattributes(const Graphicssamplingattributes& samplingAttributes) :
		id(cmzn_graphicssamplingattributes_access(samplingAttributes.id))
	{}

	Graphicssamplingattributes& operator=(const Graphicssamplingattributes& graphicssamplingattributes)
	{
		cmzn_graphicssamplingattributes_id temp_id = cmzn_graphicssamplingattributes_access(graphicssamplingattributes.id);
		if (0 != id)
			cmzn_graphicssamplingattributes_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Graphicssamplingattributes()
	{
		cmzn_graphicssamplingattributes_destroy(&id);
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Field getDensityField()
	{
		return Field(cmzn_graphicssamplingattributes_get_density_field(id));
	}

	int setDensityField(const Field& densityField)
	{
		return cmzn_graphicssamplingattributes_set_density_field(id, densityField.getId());
	}

	int getLocation(int valuesCount, double *valuesOut)
	{
		return cmzn_graphicssamplingattributes_get_location(id, valuesCount, valuesOut);
	}

	int setLocation(int valuesCount, const double *valuesIn)
	{
		return cmzn_graphicssamplingattributes_set_location(id, valuesCount, valuesIn);
	}

	Element::PointSamplingMode getElementPointSamplingMode()
	{
		return static_cast<Element::PointSamplingMode>(cmzn_graphicssamplingattributes_get_element_point_sampling_mode(id));
	}

	int setElementPointSamplingMode(Element::PointSamplingMode samplingMode)
	{
		return cmzn_graphicssamplingattributes_set_element_point_sampling_mode(id,
			static_cast<cmzn_element_point_sampling_mode>(samplingMode));
	}

};

inline Glyph Glyphmodule::createStaticGlyphFromGraphics(const Graphics& graphics)
{
	return Glyph(cmzn_glyphmodule_create_static_glyph_from_graphics(id, graphics.getId()));
}

inline Graphicssamplingattributes Graphics::getGraphicssamplingattributes()
{
	return Graphicssamplingattributes(cmzn_graphics_get_graphicssamplingattributes(id));
}

} // namespace Zinc
}

#endif
