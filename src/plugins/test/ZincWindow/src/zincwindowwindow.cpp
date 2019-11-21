/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Zinc window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "zincwidget.h"
#include "zincwindowwindow.h"

//==============================================================================

#include "ui_zincwindowwindow.h"

//==============================================================================

#include <QDir>

//==============================================================================

#include <array>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/fieldmodule.hpp"
    #include "opencmiss/zinc/fieldvectoroperators.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace ZincWindow {

//==============================================================================

ZincWindowWindow::ZincWindowWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::ZincWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and add a Zinc widget

    mZincWidget = new ZincWidget::ZincWidget(this);

    connect(mZincWidget, &ZincWidget::ZincWidget::contextAboutToBeDestroyed,
            this, &ZincWindowWindow::createAndSetZincContext);
    connect(mZincWidget, &ZincWidget::ZincWidget::graphicsInitialized,
            this, &ZincWindowWindow::graphicsInitialized);
    connect(mZincWidget, &ZincWidget::ZincWidget::devicePixelRatioChanged,
            this, &ZincWindowWindow::devicePixelRatioChanged);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mZincWidget,
                                                     false, true, true, true));
#else
    mGui->layout->addWidget(mZincWidget);
#endif

    // Create and set our Zinc context

    createAndSetZincContext();
}

//==============================================================================

ZincWindowWindow::~ZincWindowWindow()
{
    // Delete some internal objects

    delete mZincContext;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void ZincWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);
}

//==============================================================================

void ZincWindowWindow::createAndSetZincContext()
{
    // Keep track of our current scene viewer's description

    mZincSceneViewerDescription = mZincWidget->sceneViewer().writeDescription();

    // Create and set our Zinc context

    mZincContext = new OpenCMISS::Zinc::Context("ZincWindowWindow");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    mZincWidget->setContext(mZincContext);

    // Add a tri-linear cube to our Zinc context

    // Load our .exfile to our default region using a temporary copy of our
    // .exfile file

    QString exFile = Core::canonicalFileName(QDir::tempPath()+"/ZincWindow/trilinearCube.exfile");

    Core::writeResourceToFile(exFile, ":/ZincWindow/trilinearCube.exfile");

    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

    region.readFile(exFile.toUtf8().constData());

    QFile::remove(exFile);

    // Create a field magnitude for our default region

    OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();

    fieldModule.beginChange();
        OpenCMISS::Zinc::Field coordinates = fieldModule.findFieldByName("coordinates");
        OpenCMISS::Zinc::FieldMagnitude magnitude = fieldModule.createFieldMagnitude(coordinates);

        magnitude.setManaged(true);
    fieldModule.endChange();

    // Customise the scene of our default region

    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        // Black lines limiting our scene

        OpenCMISS::Zinc::Materialmodule materialModule = mZincContext->getMaterialmodule();
        OpenCMISS::Zinc::GraphicsLines lines = scene.createGraphicsLines();

        lines.setCoordinateField(coordinates);
        lines.setMaterial(materialModule.findMaterialByName("black"));

        // Green spheres limiting our scene

        OpenCMISS::Zinc::GraphicsPoints nodePoints = scene.createGraphicsPoints();

        nodePoints.setCoordinateField(coordinates);
        nodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        nodePoints.setMaterial(materialModule.findMaterialByName("green"));

        // Size of our green spheres

        double doubleValue = 0.05;

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = nodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &doubleValue);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

        // Axes for our scene

        OpenCMISS::Zinc::GraphicsPoints axes = scene.createGraphicsPoints();

        axes.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_POINT);
        axes.setMaterial(materialModule.findMaterialByName("blue"));

        mAxesFontPointSize = axes.getGraphicspointattributes().getFont().getPointSize();

        // Length of our axes

        pointAttr = axes.getGraphicspointattributes();

        doubleValue = 1.2;

        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_AXES_XYZ);
        pointAttr.setBaseSize(1, &doubleValue);

        // Tesselation for our scene

        OpenCMISS::Zinc::Tessellation fineTessellation = mZincContext->getTessellationmodule().createTessellation();
        int intValue = 8;

        fineTessellation.setManaged(true);
        fineTessellation.setMinimumDivisions(1, &intValue);

        // Isosurfaces for our scene

        OpenCMISS::Zinc::GraphicsContours isosurfaces = scene.createGraphicsContours();

        doubleValue = 1.0;

        isosurfaces.setCoordinateField(coordinates);
        isosurfaces.setIsoscalarField(magnitude);
        isosurfaces.setListIsovalues(1, &doubleValue);
        isosurfaces.setMaterial(materialModule.findMaterialByName("gold"));
        isosurfaces.setTessellation(fineTessellation);
    scene.endChange();

    // Update our scene using our initial devide pixel ratio

    devicePixelRatioChanged(mZincWidget->devicePixelRatio());
}

//==============================================================================

void ZincWindowWindow::graphicsInitialized()
{
    // Set our 'new' scene viewer's description

    OpenCMISS::Zinc::Sceneviewer sceneViewer = mZincWidget->sceneViewer();

    sceneViewer.readDescription(mZincSceneViewerDescription);

    // Our Zinc widget has had its graphics initialised, so now we can set its
    // background colour

    std::array<double, 4> backgroundColor = { 1.0, 1.0, 1.0, 1.0 };

    sceneViewer.setBackgroundColourRGBA(backgroundColor.data());
}

//==============================================================================

void ZincWindowWindow::devicePixelRatioChanged(int pDevicePixelRatio)
{
    // Update our scene using the given devide pixel ratio

    OpenCMISS::Zinc::Scene scene = mZincContext->getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
}

//==============================================================================

} // namespace ZincWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
