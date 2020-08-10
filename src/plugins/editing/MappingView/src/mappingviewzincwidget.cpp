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

#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "filemanager.h"
#include "mappingvieweditingwidget.h"
#include "mappingviewzincwidget.h"

//==============================================================================

#include <QDir>
#include <QMimeData>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTimer>
#include <QtMath>

//==============================================================================

#include <array>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/context.hpp"
    #include "opencmiss/zinc/result.hpp"
    #include "opencmiss/zinc/scenefilter.hpp"
    #include "opencmiss/zinc/field.hpp"
    #include "opencmiss/zinc/graphics.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

MappingViewZincWidget::MappingViewZincWidget(QWidget *pParent, const QString &pMainFileName,
                                             MappingViewEditingWidget *pEditingWidget) :
    ZincWidget::ZincWidget(pParent),
    mMainFileName(pMainFileName),
    mEditingWidget(pEditingWidget),
    mNodeSize(pow(nodeSixeExp,nodeSizeOrigin))
{
    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Keep track of our current scene viewer's description
    //TODO usefull ?
    mZincSceneViewerDescription = sceneViewer().writeDescription();

    initAuxFile();

    setup();
}

//==============================================================================

MappingViewZincWidget::~MappingViewZincWidget()
{
    delete mZincContext;
    delete mScene;
    delete mRegion;
}

//==============================================================================

void MappingViewZincWidget::changeSource(const QString &pMainFileName)
{
    mMainFileName = pMainFileName;
    initAuxFile();
    setup();
    initializeGL();

qDebug(">>>zincwidget charging %s",qPrintable(mMainFileName));
}

//==============================================================================

void MappingViewZincWidget::initializeGL()
{
    ZincWidget::initializeGL();

    //mSceneViewer.readDescription(mZincSceneViewerDescription);

    // background colour

    std::array<double, 4> backgroundColor = { 0.85, 0.85, 0.85, 1.0 };

    mSceneViewer.setBackgroundColourRGBA(backgroundColor.data());

    mSceneViewer.setScene(mRegion->getScene());

    draw();
}

//==============================================================================

void MappingViewZincWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    ZincWidget::mouseMoveEvent(pEvent);
}

//==============================================================================

void MappingViewZincWidget::mousePressEvent(QMouseEvent *pEvent)
{
    ZincWidget::mousePressEvent(pEvent);
    mMousePosClick = pEvent->pos();
}

//==============================================================================

void MappingViewZincWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    ZincWidget::mouseReleaseEvent(pEvent);

    if (mMousePosClick==pEvent->pos()) {
        click(pEvent->x(),pEvent->y());
    }
}

//==============================================================================

void MappingViewZincWidget::wheelEvent(QWheelEvent *pEvent)
{
    ZincWidget::wheelEvent(pEvent);
}

//==============================================================================

void MappingViewZincWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    if (pEvent->mimeData()->hasFormat(MappingViewEditingModel::MappingViewEdittingMimeType)) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void MappingViewZincWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{

    click(pEvent->pos().x(),pEvent->pos().y());

    if (mIdSelectedNode!=-1) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void MappingViewZincWidget::dropEvent(QDropEvent *pEvent)
{
    QString data = pEvent->mimeData()->data(MappingViewEditingModel::MappingViewEdittingMimeType);
    const QStringList splitText = data.split("|");

    pEvent->acceptProposedAction();

    mEditingWidget->setNodeValue(mIdSelectedNode,"component: "+splitText[1]+", variable: "+splitText.first());
}

//==============================================================================

void MappingViewZincWidget::setup()
{
    // Create and set our Zinc context

    mZincContext = new OpenCMISS::Zinc::Context("MappingViewZincContext");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    setContext(mZincContext);

    //Create and initialize region and scene

    mRegion = new OpenCMISS::Zinc::Region(mZincContext->getDefaultRegion());
    mScene = new OpenCMISS::Zinc::Scene(mRegion->getScene());

    // Create and initialize scenePicker with a filter (improvable)

    mScenePicker = new OpenCMISS::Zinc::Scenepicker(mScene->createScenepicker());

    OpenCMISS::Zinc::Scenefiltermodule sceneFilterModule = mScene->getScenefiltermodule();
    OpenCMISS::Zinc::Scenefilter nodeFilter =
            sceneFilterModule.createScenefilterFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
    mScenePicker->setScenefilter(nodeFilter);
}

//==============================================================================

void MappingViewZincWidget::initAuxFile()
{
    mAuxFileName = mMainFileName;
    mAuxFileName.remove(".exnode");
    mAuxFileName.append(".exelem");

    QFileInfo check_file;
    check_file.setFile(mAuxFileName);

    if (!check_file.exists()) {
        mAuxFileName = "";
    }
}

//==============================================================================

void MappingViewZincWidget::draw()
{

    //read files
    mRegion->readFile(qPrintable(mMainFileName));

    if (mAuxFileName!="") {
        mRegion->readFile(qPrintable(mAuxFileName));
    }

    mFieldModule = mRegion->getFieldmodule();

    mFieldModule.beginChange();
        OpenCMISS::Zinc::Field coordinates = mFieldModule.findFieldByName("Coordinates");
    mFieldModule.endChange();

    mScene->beginChange();
        OpenCMISS::Zinc::Materialmodule materialModule = mScene->getMaterialmodule();

        //Black lines

        OpenCMISS::Zinc::GraphicsLines lines = mScene->createGraphicsLines();

        lines.setCoordinateField(coordinates);
        lines.setMaterial(materialModule.findMaterialByName("red"));

        // Green spheres limiting our scene

        mNodePoints = mScene->createGraphicsPoints();

        mNodePoints.setCoordinateField(coordinates);
        mNodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        mNodePoints.setMaterial(materialModule.findMaterialByName("green"));

        // Size of our green spheres

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = mNodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

    mScene->endChange();

    // adding view all command

    mSceneViewer.viewAll();
}

//==============================================================================

void MappingViewZincWidget::click(int pX, int pY)
{
    mScenePicker->setSceneviewerRectangle(mSceneViewer, OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT,
                                          pX - mSizeSelection,
                                          pY - mSizeSelection,
                                          pX + mSizeSelection,
                                          pY + mSizeSelection);
    OpenCMISS::Zinc::Node node = mScenePicker->getNearestNode();

    mIdSelectedNode = node.getIdentifier();
    mEditingWidget->selectNode(mIdSelectedNode);

    // select the node to highlight graphics

    mFieldModule.beginChange();

        OpenCMISS::Zinc::FieldGroup selectionGroup = mScene->getSelectionField().castGroup();

        if (node.isValid()){
            if (!selectionGroup.isValid()) {
                selectionGroup = mFieldModule.createFieldGroup();
                selectionGroup.setName("Selection");
                mScene->setSelectionField(selectionGroup);
            }

            OpenCMISS::Zinc::Nodeset nodes = mFieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::FieldNodeGroup nodeGroupField = selectionGroup.getFieldNodeGroup(nodes);

            if (!nodeGroupField.isValid()) {
                nodeGroupField = selectionGroup.createFieldNodeGroup(nodes);
            }

            OpenCMISS::Zinc::NodesetGroup nodesetGroup = nodeGroupField.getNodesetGroup();
            nodesetGroup.removeAllNodes();
            nodesetGroup.addNode(node);

        } else {

            if (selectionGroup.isValid()) {
                mScene->setSelectionField(OpenCMISS::Zinc::Field());
            }
        }
    mFieldModule.endChange();
}

//==============================================================================

void MappingViewZincWidget::setNodeSizes(int pSize) {
    mNodeSize = pow(nodeSixeExp,pSize);
    //TODO
    mScene->beginChange();
        mNodePoints.getGraphicspointattributes().setBaseSize(1, &mNodeSize);
    mScene->endChange();

}

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
