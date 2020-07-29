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
// Mapping view widget
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "filemanager.h"
#include "mappingviewwidget.h"
#include "meshreader.h"
#include "cellmlfilemanager.h"
#include "zincwidget.h"

//==============================================================================

#include "ui_mappingviewwidget.h"

//==============================================================================

#include <QFile>
#include <QtGui>

#include<QDebug> //TODO no keep this one of course

//==============================================================================

#include "zincbegin.h" //TODO takeaway the useless
    #include "opencmiss/zinc/fieldconstant.hpp"
    #include "opencmiss/zinc/fieldmodule.hpp"
    #include "opencmiss/zinc/fieldvectoroperators.hpp"
    #include "opencmiss/zinc/status.h"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

MappingViewWidget::MappingViewWidget(QWidget *pParent) :
    Core::SplitterWidget(pParent),
    mAxesFontPointSize(0),
    mZincContext(nullptr),
    mZincSceneViewerDescription(nullptr)
{
    // Set our orientation

    setOrientation(Qt::Horizontal);

    // Keep track of our movement
    /*
    connect(this, &Core::SplitterWidget::splitterMoved,
            this, &MappingViewEditingWidget::splitterMoved);
    */

    mListWidgetVariables = new QListWidget();
    mListWidgetOutput = new QListWidget();

    addWidget(mListWidgetVariables);

    // Create a local copy of our .exnode and .exelem files

    QString applicationBaseFileName = QDir::tempPath()+"/mesh";

    mExNodeFileName = applicationBaseFileName+".exnode";
    mExElemFileName = applicationBaseFileName+".exelem";

    Core::writeResourceToFile(mExNodeFileName, QDir::currentPath()+"/../opencor/meshes/circulation.exnode");
    Core::writeResourceToFile(mExElemFileName, QDir::currentPath()+"/../opencor/meshes/circulation.exelem");
    //but nothing appear in /tmp/ !!!
    mListWidgetVariables->addItem(QDir::currentPath()+"/../opencor/meshes/circulation.exnode");
    mListWidgetVariables->addItem(mExElemFileName);

    // Create and add a Zinc widget

    mZincWidget = new ZincWidget::ZincWidget(this);

    connect(mZincWidget, SIGNAL(contextAboutToBeDestroyed()),
            this, SLOT(createAndSetZincContext()));
    connect(mZincWidget, SIGNAL(graphicsInitialized()),
            this, SLOT(graphicsInitialized()));
    connect(mZincWidget, SIGNAL(devicePixelRatioChanged(const int &)),
            this, SLOT(devicePixelRatioChanged(const int &)));

    addWidget(mZincWidget);

    // Create and set our Zinc context

    createAndSetZincContext();

}

//==============================================================================

MappingViewWidget::~MappingViewWidget()
{

    delete mZincContext;

    QFile::remove(mExNodeFileName);
    QFile::remove(mExElemFileName);

}

//==============================================================================

void MappingViewWidget::retranslateUi()
{
    // Update ourself too since some widgets will have been reset following the
    // retranslation (e.g. mGui->fileNameValue)

    for(auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

void MappingViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (mEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new MappingViewEditingWidget(pFileName, this);

        mEditingWidgets.insert(pFileName, mEditingWidget);
    }

    //mListWidgetVariables->setModel(mEditingWidget->listViewModelVariables()); //TODO set only when charging the plugin ?
    //mGui->outputList->setModel(mEditingWidget->listViewModelOutput());
}

//==============================================================================

void MappingViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        // There is an editing widget for the given file name, so delete it and
        // remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editingWidget == mEditingWidget) {
            mEditingWidget = nullptr;
        }
    }
}

//==============================================================================

void MappingViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        editingWidget->filePermissionsChanged();
    }
}

//==============================================================================

void MappingViewWidget::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);
}

//==============================================================================

void MappingViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (mEditingWidgets.contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void MappingViewWidget::fileRenamed(const QString &pOldFileName, const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

void MappingViewWidget::createAndSetZincContext()
{
    // Keep track of our current scene viewer's description

    mZincSceneViewerDescription = mZincWidget->sceneViewer().writeDescription();

    // Create and set our Zinc context

    mZincContext = new OpenCMISS::Zinc::Context("TestZinc");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    mZincWidget->setContext(mZincContext);
}

//==============================================================================

void MappingViewWidget::graphicsInitialized()
{
    // Set our 'new' scene viewer's description

    OpenCMISS::Zinc::Sceneviewer sceneViewer = mZincWidget->sceneViewer();

    //sceneViewer.readDescription(mZincSceneViewerDescription);

    // Our Zinc widget has had its graphics initialised, so now we can set its
    // background colour

    std::array<double, 4> backgroundColor = { 0.85, 0.85, 0.85, 1.0 };

    sceneViewer.setBackgroundColourRGBA(backgroundColor.data());

    // Our initial look at and eye positions, and up vector
/* TODO free when it works
    sceneViewer.setViewingVolume(-1.922499, 1.922499, -1.922499, 1.922499, 0.632076, 22.557219);

    //TODO adapt to the model
    std::array<const double, 3> lookAtPosition = { -63.005, -64.247, 189.21 };
    std::array<const double, 3> eyePosition = { -50, -64, 180 };
    std::array<const double, 3> upVector = { -1.000000, 0.000000, 0.000000 };

    sceneViewer.setLookatPosition(lookAtPosition.data());
    sceneViewer.setEyePosition(eyePosition.data());
    sceneViewer.setUpVector(upVector.data());
*/
    OpenCMISS::Zinc::Region defaultRegion = mZincContext->getDefaultRegion();

    sceneViewer.setScene(defaultRegion.getScene());


    //===== Do stuff to fold later

    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

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

    sceneViewer.viewAll();


}

//==============================================================================

void MappingViewWidget::devicePixelRatioChanged(const int &pDevicePixelRatio)
{
    //TODO to confirm
    // Update our scene using the given devide pixel ratio

    OpenCMISS::Zinc::Scene scene = mZincContext->getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
}

//==============================================================================



} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
