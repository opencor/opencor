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
#include "cellmlzincmappingvieweditingwidget.h"
#include "cellmlzincmappingviewzincwidget.h"
#include "corecliutils.h"
#include "filemanager.h"

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
    #include "opencmiss/zinc/scenefilter.hpp"
    #include "opencmiss/zinc/field.hpp"
    #include "opencmiss/zinc/graphics.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

CellMLZincMappingViewZincWidget::CellMLZincMappingViewZincWidget(QWidget *pParent, const QString &pMainFileName,
                                             CellMLZincMappingViewEditingWidget *pEditingWidget) :
    ZincWidget::ZincWidget(pParent),
    mMainFileName(pMainFileName),
    mEditingWidget(pEditingWidget),
    mNodeSize(pow(nodeSixeExp,nodeSizeOrigin))//,
    //mMappedNodeSize()
{
    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Keep track of our current scene viewer's description
    //TODO usefull ?
    mZincSceneViewerDescription = sceneViewer().writeDescription();

    setup();

    initAuxFile();

    setupRegion();
}

//==============================================================================

CellMLZincMappingViewZincWidget::~CellMLZincMappingViewZincWidget()
{
    delete mZincContext;
}

//==============================================================================

void CellMLZincMappingViewZincWidget::changeSource(const QString &pMainFileName)
{
    mMainFileName = pMainFileName;
    initAuxFile();

    OpenCMISS::Zinc::Region region = mZincContext->createRegion();
    mZincContext->setDefaultRegion(region);

    setupRegion();

    draw();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::initializeGL()
{
    ZincWidget::initializeGL();
    //TODO usefull ?
    //mSceneViewer.readDescription(mZincSceneViewerDescription);

    // background colour

    std::array<double, 4> backgroundColor = { 0.85, 0.85, 0.85, 1.0 };

    mSceneViewer.setBackgroundColourRGBA(backgroundColor.data());

    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();
    mSceneViewer.setScene(region.getScene());

    draw();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    ZincWidget::mouseMoveEvent(pEvent);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::mousePressEvent(QMouseEvent *pEvent)
{
    ZincWidget::mousePressEvent(pEvent);
    mMousePosClick = pEvent->pos();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    ZincWidget::mouseReleaseEvent(pEvent);

    if (mMousePosClick==pEvent->pos()) {
        click(pEvent->x(),pEvent->y());
    }
}

//==============================================================================

void CellMLZincMappingViewZincWidget::wheelEvent(QWheelEvent *pEvent)
{
    ZincWidget::wheelEvent(pEvent);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    if (pEvent->mimeData()->hasFormat(CellMLZincMappingViewEditingModel::MappingViewEdittingMimeType)) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void CellMLZincMappingViewZincWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    click(pEvent->pos().x(),pEvent->pos().y(), false);

    if (mIdSelectedNode!=-1) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void CellMLZincMappingViewZincWidget::dropEvent(QDropEvent *pEvent)
{
    QString data = pEvent->mimeData()->data(CellMLZincMappingViewEditingModel::MappingViewEdittingMimeType);
    const QStringList splitText = data.split("|");

    pEvent->acceptProposedAction();

    mEditingWidget->setNodeValue(mIdSelectedNode,"component: "+splitText[1]+", variable: "+splitText.first());

    // select and highlight the current node

    auto fieldModule = mZincContext->getDefaultRegion().getFieldmodule();
    OpenCMISS::Zinc::Node node = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES).findNodeByIdentifier(mIdSelectedNode);

    fieldModule.beginChange();

        if (node.isValid()){
            OpenCMISS::Zinc::Nodeset nodes = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::FieldNodeGroup nodeGroupField = mMappedSelectionGroup.getFieldNodeGroup(nodes);

            if (!nodeGroupField.isValid()) {
                nodeGroupField = mMappedSelectionGroup.createFieldNodeGroup(nodes);
            }
            OpenCMISS::Zinc::NodesetGroup nodesetGroup = nodeGroupField.getNodesetGroup();
            nodesetGroup.addNode(node);

        } else {
            if (mMappedSelectionGroup.isValid()) {
                mZincContext->getDefaultRegion().getScene().setSelectionField(OpenCMISS::Zinc::Field());
            }
        }
    fieldModule.endChange();

}

//==============================================================================

void CellMLZincMappingViewZincWidget::setup()
{
    // Create and set our Zinc context

    mZincContext = new OpenCMISS::Zinc::Context("MappingViewZincContext");

    mZincContext->getMaterialmodule().defineStandardMaterials();
    mZincContext->getGlyphmodule().defineStandardGlyphs();

    setContext(mZincContext);
}

void CellMLZincMappingViewZincWidget::setupRegion()
{
    //Create and initialize region and scene

    OpenCMISS::Zinc::Scene scene = mZincContext->getDefaultRegion().getScene();

    // Create and initialize scenePicker with a filter (improvable)

    mScenePicker = new OpenCMISS::Zinc::Scenepicker(scene.createScenepicker());

    OpenCMISS::Zinc::Scenefiltermodule sceneFilterModule = scene.getScenefiltermodule();
    OpenCMISS::Zinc::Scenefilter nodeFilter =
            sceneFilterModule.createScenefilterFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
    mScenePicker->setScenefilter(nodeFilter);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::initAuxFile()
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

void CellMLZincMappingViewZincWidget::draw()
{

    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

    //read files
    region.readFile(qPrintable(mMainFileName));

    if (mAuxFileName!="") {
        region.readFile(qPrintable(mAuxFileName));
    }

    OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();
    OpenCMISS::Zinc::Scene scene = region.getScene();


    fieldModule.beginChange();
        OpenCMISS::Zinc::Field coordinates = fieldModule.findFieldByName("Coordinates");

        mMappedSelectionGroup = fieldModule.createFieldGroup();
        mMappedSelectionGroup.setName("Mapped");

    fieldModule.endChange();

    scene.beginChange();
        OpenCMISS::Zinc::Materialmodule materialModule = scene.getMaterialmodule();

        //Black lines

        OpenCMISS::Zinc::GraphicsLines lines = scene.createGraphicsLines();

        lines.setCoordinateField(coordinates);
        lines.setMaterial(materialModule.findMaterialByName("black"));

        // Green spheres limiting our scene

        mNodePoints = scene.createGraphicsPoints();

        mNodePoints.setCoordinateField(coordinates);
        mNodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        mNodePoints.setMaterial(materialModule.findMaterialByName("grey"));


        // Size of our green spheres

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = mNodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

        // create "mapped" group

        mMappedPoints = scene.createGraphicsPoints();
        mMappedPoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        mMappedPoints.setCoordinateField(coordinates);
        mMappedPoints.setSubgroupField(mMappedSelectionGroup);

        mMappedPoints.setMaterial(materialModule.findMaterialByName("green"));
        mMappedPoints.setSelectedMaterial(materialModule.findMaterialByName("orange"));

        pointAttr = mMappedPoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

        scene.moveGraphicsBefore(mMappedPoints, mNodePoints);

    scene.endChange();

    // adding view all command

    mSceneViewer.viewAll();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::click(int pX, int pY, bool pCanDiscard)
{
    mScenePicker->setSceneviewerRectangle(mSceneViewer, OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT,
                                          pX - mSizeSelection,
                                          pY - mSizeSelection,
                                          pX + mSizeSelection,
                                          pY + mSizeSelection);
    OpenCMISS::Zinc::Node node = mScenePicker->getNearestNode();

    int buff = node.getIdentifier();

    if(pCanDiscard||buff!=-1) {

        mIdSelectedNode = buff;
        mEditingWidget->selectNode(mIdSelectedNode);

        // select the node to highlight graphics

        OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

        OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();
        OpenCMISS::Zinc::Scene scene = region.getScene();


        fieldModule.beginChange();

            OpenCMISS::Zinc::FieldGroup selectionGroup = scene.getSelectionField().castGroup();

            if (node.isValid()){
                if (!selectionGroup.isValid()) {
                    selectionGroup = fieldModule.createFieldGroup();
                    selectionGroup.setName("Selection");

                    scene.setSelectionField(selectionGroup);
                }

                OpenCMISS::Zinc::Nodeset nodes = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
                OpenCMISS::Zinc::FieldNodeGroup nodeGroupField = selectionGroup.getFieldNodeGroup(nodes);

                if (!nodeGroupField.isValid()) {
                    nodeGroupField = selectionGroup.createFieldNodeGroup(nodes);
                }

                OpenCMISS::Zinc::NodesetGroup nodesetGroup = nodeGroupField.getNodesetGroup();
                nodesetGroup.removeAllNodes();
                nodesetGroup.addNode(node);

            } else {

                if (selectionGroup.isValid()) {
                    scene.setSelectionField(OpenCMISS::Zinc::Field());
                }
            }
        fieldModule.endChange();
    }
}

//==============================================================================

void CellMLZincMappingViewZincWidget::setNodeSizes(int pSize) {
    mNodeSize = pow(nodeSixeExp,pSize);
    //TODO change size of mapped nodes

    OpenCMISS::Zinc::Region region = mZincContext->getDefaultRegion();

    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        mNodePoints.getGraphicspointattributes().setBaseSize(1, &mNodeSize);
        mNodePoints.getGraphicspointattributes().setBaseSize(1, &mNodeSize);
    scene.endChange();

}

//==============================================================================

void CellMLZincMappingViewZincWidget::eraseNode()
{

    mEditingWidget->eraseNodeValue(mIdSelectedNode);

    // select and highlight the current node

    auto fieldModule = mZincContext->getDefaultRegion().getFieldmodule();
    OpenCMISS::Zinc::Node node = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES).findNodeByIdentifier(mIdSelectedNode);

    fieldModule.beginChange();

        if (node.isValid()){
            OpenCMISS::Zinc::Nodeset nodes = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::FieldNodeGroup nodeGroupField = mMappedSelectionGroup.getFieldNodeGroup(nodes);

            if (!nodeGroupField.isValid()) {
                nodeGroupField = mMappedSelectionGroup.createFieldNodeGroup(nodes);
            }
            OpenCMISS::Zinc::NodesetGroup nodesetGroup = nodeGroupField.getNodesetGroup();
            nodesetGroup.removeNode(node);

        } else {
            if (mMappedSelectionGroup.isValid()) {
                mZincContext->getDefaultRegion().getScene().setSelectionField(OpenCMISS::Zinc::Field());
            }
        }
    fieldModule.endChange();
}

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
