/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Zinc widget
//==============================================================================

#include "mappingviewzincwidget.h"

//==============================================================================

#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTimer>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/context.hpp"
    #include "opencmiss/zinc/result.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

MappingViewZincWidget::MappingViewZincWidget(QWidget *pParent) :
    ZincWidget::ZincWidget(pParent)
{

    //TODO usefull ?
    // Keep track of our current scene viewer's description

    mZincSceneViewerDescription = sceneViewer().writeDescription();

    // Create and set our Zinc context

    mZincContext = new OpenCMISS::Zinc::Context("MappingViewZincContext");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    setContext(mZincContext);
}

//==============================================================================

MappingViewZincWidget::~MappingViewZincWidget()
{
    delete mZincContext;
}

//==============================================================================

void MappingViewZincWidget::initializeGL()
{
    ZincWidget::initializeGL();

    //mSceneViewer.readDescription(mZincSceneViewerDescription);

    // Our Zinc widget has had its graphics initialised, so now we can set its
    // background colour

    std::array<double, 4> backgroundColor = { 0.85, 0.85, 0.85, 1.0 };

    mSceneViewer.setBackgroundColourRGBA(backgroundColor.data());

    // Our initial look at and eye positions, and up vector
/* TODO free when it works
    mSceneViewer.setViewingVolume(-1.922499, 1.922499, -1.922499, 1.922499, 0.632076, 22.557219);

    //TODO adapt to the model
    std::array<const double, 3> lookAtPosition = { -63.005, -64.247, 189.21 };
    std::array<const double, 3> eyePosition = { -50, -64, 180 };
    std::array<const double, 3> upVector = { -1.000000, 0.000000, 0.000000 };

    mSceneViewer.setLookatPosition(lookAtPosition.data());
    mSceneViewer.setEyePosition(eyePosition.data());
    mSceneViewer.setUpVector(upVector.data());
*/
    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

    mSceneViewer.setScene(region.getScene());

    //TODO copy
    region.readFile(qPrintable("../opencor/meshes/circulation.exnode"));
    region.readFile(qPrintable("../opencor/meshes/circulation.exelem"));

    mFieldModule = region.getFieldmodule();

    mFieldModule.beginChange();
        OpenCMISS::Zinc::Field coordinates = mFieldModule.findFieldByName("Coordinates");
    mFieldModule.endChange();

    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        OpenCMISS::Zinc::Materialmodule materialModule = scene.getMaterialmodule();

        //Black lines

        OpenCMISS::Zinc::GraphicsLines lines = scene.createGraphicsLines();

        lines.setCoordinateField(coordinates);
        lines.setMaterial(materialModule.findMaterialByName("red"));

        // Green spheres limiting our scene

        OpenCMISS::Zinc::GraphicsPoints nodePoints = scene.createGraphicsPoints();

        nodePoints.setCoordinateField(coordinates);
        nodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        nodePoints.setMaterial(materialModule.findMaterialByName("green"));

        // Size of our green spheres

        mNodeSize = 20.;

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = nodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

    scene.endChange();

    // adding view all command

    mSceneViewer.viewAll();
}

//==============================================================================

void MappingViewZincWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    ZincWidget::mouseMoveEvent(pEvent);
    mouse_fix_click = false;
}

//==============================================================================

void MappingViewZincWidget::mousePressEvent(QMouseEvent *pEvent)
{
    ZincWidget::mousePressEvent(pEvent);
    mouse_fix_click = true;
}

//==============================================================================

void MappingViewZincWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    ZincWidget::mouseReleaseEvent(pEvent);

    if (mouse_fix_click) {
        //TODO
    }
}

//==============================================================================

void MappingViewZincWidget::wheelEvent(QWheelEvent *pEvent)
{
    ZincWidget::wheelEvent(pEvent);
}


//==============================================================================

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
