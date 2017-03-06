/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

#include "opencmiss/zinc/fieldmodule.hpp"
#include "opencmiss/zinc/fieldvectoroperators.hpp"
#include "opencmiss/zinc/status.hpp"

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
    mZincContext = new OpenCMISS::Zinc::Context("ZincWindowWindow");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    connect(mZincWidget, SIGNAL(graphicsInitialized()),
            this, SLOT(graphicsInitialized()));

    mZincWidget->setContext(mZincContext);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mZincWidget,
                                                     false, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(mZincWidget);
#else
    #error Unsupported platform
#endif

    // Add a tri-linear cube to our Zinc widget

    // Make a temporary copy of our .exfile file

    QString exFile = QDir::tempPath()+QDir::separator()+"ZincWindow/trilinearCube.exfile";

    Core::writeResourceToFile(exFile, ":/ZincWindow/trilinearCube.exfile");

    // Load the .exfile to our default region

    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

    region.readFile(exFile.toUtf8().constData());

    QFile::remove(exFile);

    // Create a field magnitude for our default region

    OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();

    fieldModule.beginChange();
        OpenCMISS::Zinc::Field coordinates = fieldModule.findFieldByName("coordinates");
        OpenCMISS::Zinc::FieldMagnitude magnitude = fieldModule.createFieldMagnitude(coordinates);

        magnitude.setManaged(true);
        magnitude.setName("magnitude");
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
        fineTessellation.setName("fine");

        // Isosurfaces for our scene

        OpenCMISS::Zinc::GraphicsContours isosurfaces = scene.createGraphicsContours();

        doubleValue = 1.0;

        isosurfaces.setCoordinateField(coordinates);
        isosurfaces.setIsoscalarField(magnitude);
        isosurfaces.setListIsovalues(1, &doubleValue);
        isosurfaces.setMaterial(materialModule.findMaterialByName("gold"));
        isosurfaces.setTessellation(fineTessellation);
    scene.endChange();
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

void ZincWindowWindow::graphicsInitialized()
{
    // Our Zinc widget has had its graphics initialised, so we can now set its
    // background colour

    double backgroundColor[3] = { 0.0, 0.0, 0.0 };

    mZincWidget->sceneViewer().setBackgroundColourRGB(backgroundColor);
}

//==============================================================================

}   // namespace ZincWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
