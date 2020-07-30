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

    // Create and initialize scenePicker
/*
    pScenePicker = mScene->createScenepicker();

    sfm = mScene->getScenefiltermodule();

    nodeFilter = sfm.createScenefilterFieldDomainType(Field::DOMAIN_TYPE_NODES);

    pScenePicker.setScenefilter(nodeFilter);
*/

}

//==============================================================================

MappingViewZincWidget::~MappingViewZincWidget()
{
    delete mZincContext;
    delete mScene;
    delete mRegion;
}

//==============================================================================

void MappingViewZincWidget::initializeGL()
{
    ZincWidget::initializeGL();

    //mSceneViewer.readDescription(mZincSceneViewerDescription);

    // background colour

    std::array<double, 4> backgroundColor = { 0.85, 0.85, 0.85, 1.0 };

    mSceneViewer.setBackgroundColourRGBA(backgroundColor.data());

    mRegion = new OpenCMISS::Zinc::Region(mZincContext->getDefaultRegion());

    mSceneViewer.setScene(mRegion->getScene());

    //TODO copy
    mRegion->readFile(qPrintable("../opencor/meshes/circulation.exnode"));
    mRegion->readFile(qPrintable("../opencor/meshes/circulation.exelem"));

    mFieldModule = mRegion->getFieldmodule();

    mFieldModule.beginChange();
        OpenCMISS::Zinc::Field coordinates = mFieldModule.findFieldByName("Coordinates");
    mFieldModule.endChange();

    mScene = new OpenCMISS::Zinc::Scene(mRegion->getScene());

    mScene->beginChange();
        OpenCMISS::Zinc::Materialmodule materialModule = mScene->getMaterialmodule();

        //Black lines

        OpenCMISS::Zinc::GraphicsLines lines = mScene->createGraphicsLines();

        lines.setCoordinateField(coordinates);
        lines.setMaterial(materialModule.findMaterialByName("red"));

        // Green spheres limiting our scene

        OpenCMISS::Zinc::GraphicsPoints nodePoints = mScene->createGraphicsPoints();

        nodePoints.setCoordinateField(coordinates);
        nodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        nodePoints.setMaterial(materialModule.findMaterialByName("green"));

        // Size of our green spheres

        mNodeSize = 20.;

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = nodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

    mScene->endChange();

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
        click(pEvent);
    }
}

//==============================================================================

void MappingViewZincWidget::wheelEvent(QWheelEvent *pEvent)
{
    ZincWidget::wheelEvent(pEvent);
}

//==============================================================================

void click(QMouseEvent *pEvent)
{
    Q_UNUSED(pEvent);
}

//==============================================================================

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
