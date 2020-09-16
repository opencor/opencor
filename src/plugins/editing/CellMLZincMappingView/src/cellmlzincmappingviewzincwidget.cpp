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
// CellML-Zinc Mapping view Zinc widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlzincmappingvieweditingwidget.h"
#include "cellmlzincmappingviewzincwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"

//==============================================================================

#include <QDir>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTextStream>
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
    #include "opencmiss/zinc/streamregion.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

CellMLZincMappingViewZincWidget::CellMLZincMappingViewZincWidget(QWidget *pParent, const QStringList &pZincMeshFileNames,
                                             CellMLZincMappingViewEditingWidget *pEditingWidget) :
    ZincWidget::ZincWidget(pParent),
    mZincMeshFileNames(pZincMeshFileNames),
    mEditingWidget(pEditingWidget),
    mNodeSize(pow(nodeSixeExp,nodeSizeOrigin)),
    mLookAtPositionOriginal()
{

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Keep track of our current scene viewer's description
    //TODO usefull ?
    mZincSceneViewerDescription = sceneViewer().writeDescription();


    // Create and connect our menu actions

    mActionAxes = Core::newAction(true,this);
    mActionLines = Core::newAction(true,this);
    mActionSurfaces = Core::newAction(true,this);
    mActionIsosurfaces = Core::newAction(true,this);
    mActionLabel = Core::newAction(true,this);

    //TODO put this in settings
    mActionAxes->setChecked(true);
    mActionLines->setChecked(true);
    mActionSurfaces->setChecked(true);
    mActionIsosurfaces->setChecked(true);
    mActionLabel->setChecked(true);

    mActionAxes->setText("Axes");
    mActionLines->setText("Lines");
    mActionSurfaces->setText("Surfaces");
    mActionIsosurfaces->setText("Isosurfaces");
    mActionLabel->setText("Labels");

    connect(mActionAxes, &QAction::triggered,
            this, &CellMLZincMappingViewZincWidget::actionAxesTriggered);
    connect(mActionLines, &QAction::triggered,
            this, &CellMLZincMappingViewZincWidget::actionLinesTriggered);
    connect(mActionSurfaces, &QAction::triggered,
            this, &CellMLZincMappingViewZincWidget::actionSurfacesTriggered);
    connect(mActionIsosurfaces, &QAction::triggered,
            this, &CellMLZincMappingViewZincWidget::actionIsosurfacesTriggered);
    connect(mActionLabel, &QAction::triggered,
            this, &CellMLZincMappingViewZincWidget::actionLabelTriggered);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create and populate our context menu

    QMenu *contextMenu = new QMenu(this);

    contextMenu->addAction(mActionAxes);
    contextMenu->addAction(mActionLines);
    contextMenu->addAction(mActionSurfaces);
    contextMenu->addAction(mActionIsosurfaces);
    contextMenu->addSeparator();
    contextMenu->addAction(mActionLabel);

    setContextMenu(contextMenu);

    // initialize zinc

    setup();

    initAuxFile();

    setupRegion();

    draw();

    // Display all our objects

    showHideGraphics(GraphicsType::All);
}

//==============================================================================

CellMLZincMappingViewZincWidget::~CellMLZincMappingViewZincWidget()
{
}

//==============================================================================

void CellMLZincMappingViewZincWidget::changeSource(const QStringList &pZincMeshFileNames)
{
    mZincMeshFileNames = pZincMeshFileNames;
    initAuxFile();

    //TODO the emit launched by reset make opencor crashing
    //reset();

    setup();

    setupRegion();

    initializeGL();
    draw();
}

//==============================================================================

bool CellMLZincMappingViewZincWidget::hasNode(int pId)
{
    auto fieldModule = mZincContext.getDefaultRegion().getFieldmodule();
    OpenCMISS::Zinc::Node node = fieldModule
            .findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES)
            .findNodeByIdentifier(pId);
    return node.isValid();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::setNodeMapped(int pId, QString pComponent, QString pVariable)
{
    auto fieldModule = mZincContext.getDefaultRegion().getFieldmodule();
    OpenCMISS::Zinc::Node node = fieldModule
            .findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES)
            .findNodeByIdentifier(pId);

    fieldModule.beginChange();

        if (node.isValid()){
            OpenCMISS::Zinc::Nodeset nodes = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::FieldNodeGroup nodeGroupField = mMappedSelectionGroup.getFieldNodeGroup(nodes);

            if (!nodeGroupField.isValid()) {
                nodeGroupField = mMappedSelectionGroup.createFieldNodeGroup(nodes);
            }
            OpenCMISS::Zinc::NodesetGroup nodesetGroup = nodeGroupField.getNodesetGroup();
            nodesetGroup.addNode(node);

            OpenCMISS::Zinc::Nodeset nodeSet = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::Nodetemplate nodeTemplate = nodeSet.createNodetemplate();
            nodeTemplate.defineField(mDisplayField);

            OpenCMISS::Zinc::Fieldcache fieldCache = fieldModule.createFieldcache();

            fieldCache.setNode(node);
            node.merge(nodeTemplate);

            mDisplayField.assignString(fieldCache,qPrintable(pComponent + " " + pVariable));

        } else {
            if (mMappedSelectionGroup.isValid()) {
                mZincContext.getDefaultRegion().getScene().setSelectionField(OpenCMISS::Zinc::Field());
            }
        }

    fieldModule.endChange();
}

//==============================================================================

QList<bool> CellMLZincMappingViewZincWidget::getCheckedAction()
{
    return {mActionAxes->isChecked(),
            mActionLines->isChecked(),
            mActionSurfaces->isChecked(),
            mActionIsosurfaces->isChecked(),
            mActionLabel->isChecked()};
}

//==============================================================================

void CellMLZincMappingViewZincWidget::setCheckedAction(QList<bool> values)
{
    mActionAxes->setChecked(values.isEmpty() ? true : values.takeFirst());
    mActionLines->setChecked(values.isEmpty() ? true : values.takeFirst());
    mActionSurfaces->setChecked(values.isEmpty() ? true : values.takeFirst());
    mActionIsosurfaces->setChecked(values.isEmpty() ? true : values.takeFirst());
    mActionLabel->setChecked(values.isEmpty() ? true : values.takeFirst());

    showHideGraphics(GraphicsType::All);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::initializeGL()
{
    ZincWidget::initializeGL();

    //TODO usefull ?
    sceneViewer().readDescription(mZincSceneViewerDescription);

    // background colour

    std::array<double, 4> backgroundColor = { 0.85, 0.85, 0.85, 1.0 };

    sceneViewer().setBackgroundColourRGBA(backgroundColor.data());

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();
    sceneViewer().setScene(region.getScene());

    sceneViewer().getLookatPosition(mLookAtPositionOriginal);
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
    if (pEvent->mimeData()->hasFormat(CellMLZincMappingViewEditingMimeType)) {
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
    QString data = pEvent->mimeData()->data(CellMLZincMappingViewEditingMimeType);
    const QStringList splitText = data.split("|");

    pEvent->acceptProposedAction();
    QString component = splitText[1];
    QString variable = splitText.first();

    mEditingWidget->setNodeValue(mIdSelectedNode,component,variable);

    setNodeMapped(mIdSelectedNode,component,variable);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::setup()
{
    // Create and set our Zinc context

    mZincContext = OpenCMISS::Zinc::Context("MappingViewZincContext");

    mZincContext.getMaterialmodule().defineStandardMaterials();
    mZincContext.getGlyphmodule().defineStandardGlyphs();

    setContext(mZincContext);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::setupRegion()
{
    // Create and initialize region and scene

    OpenCMISS::Zinc::Scene scene = mZincContext.getDefaultRegion().getScene();

    // Create and initialize scenePicker with a filter (improvable)

    mScenePicker = OpenCMISS::Zinc::Scenepicker(scene.createScenepicker());

    OpenCMISS::Zinc::Scenefiltermodule sceneFilterModule = scene.getScenefiltermodule();
    OpenCMISS::Zinc::Scenefilter nodeFilter =
            sceneFilterModule.createScenefilterFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
    mScenePicker.setScenefilter(nodeFilter);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::initAuxFile()
{
    // For each exnode file, seek for the exelem

    for (auto file : mZincMeshFileNames) {
        if (file.endsWith(".exnode")) {
            QString newFile = file;
            newFile.remove(".exnode");
            newFile.append(".exelem");
            QFileInfo check_file;
            check_file.setFile(newFile);
            if (check_file.exists() && !mZincMeshFileNames.contains(newFile)) {
                mZincMeshFileNames.append(newFile);
            }
        }
    }
}

//==============================================================================

void CellMLZincMappingViewZincWidget::draw()
{

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();

    //read files
    OpenCMISS::Zinc::StreaminformationRegion sir = region.createStreaminformationRegion();

    if (!mZincMeshFileNames.empty()) {
        for (const auto &zincMeshFileName : mZincMeshFileNames) {
            sir.createStreamresourceFile(zincMeshFileName.toUtf8().constData());
        }
    }

    region.read(sir);

    OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();
    OpenCMISS::Zinc::Scene scene = region.getScene();

    mCoordinatesName ="";

    fieldModule.beginChange();
        OpenCMISS::Zinc::Fielditerator fielditer = fieldModule.createFielditerator();
        OpenCMISS::Zinc::Field field = fielditer.next();
        while (field.isValid()) {
            if (field.isTypeCoordinate() && (field.getValueType() == OpenCMISS::Zinc::Field::VALUE_TYPE_REAL)
                    && (field.getNumberOfComponents() <= 3) && field.castFiniteElement().isValid()) {
                mCoordinates = field.castFiniteElement();
                mCoordinatesName=field.getName();
                break;
            }
            field = fielditer.next();
        }

        // create the mapped group, for map nodes

        mMappedSelectionGroup = fieldModule.createFieldGroup();
        mMappedSelectionGroup.setName("Mapped");

        // Create and initialize display field and initialize it with empty strings

        mDisplayField = fieldModule.createFieldStoredString();

        OpenCMISS::Zinc::Nodeset nodeSet = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        OpenCMISS::Zinc::Nodetemplate nodeTemplate = nodeSet.createNodetemplate();
        nodeTemplate.defineField(mDisplayField);

        OpenCMISS::Zinc::Fieldcache fieldCache = fieldModule.createFieldcache();

        OpenCMISS::Zinc::Nodeiterator nodeIter = nodeSet.createNodeiterator();
        OpenCMISS::Zinc::Node node;

        while ((node = nodeIter.next()).isValid()) {
            fieldCache.setNode(node);
            node.merge(nodeTemplate);

            mDisplayField.assignString(fieldCache,"");
        }

        // create magnitude field for isosurfaces

        mMagnitude = fieldModule.createFieldMagnitude(mCoordinates);

        mMagnitude.setManaged(true);

    fieldModule.endChange();

    scene.beginChange();
        OpenCMISS::Zinc::Materialmodule materialModule = scene.getMaterialmodule();

        // Black lines

        mLines = scene.createGraphicsLines();
        mLines.setCoordinateField(mCoordinates);
        mLines.setMaterial(materialModule.findMaterialByName("black"));

        // Green spheres limiting our scene

        mNodePoints = scene.createGraphicsPoints();

        mNodePoints.setCoordinateField(mCoordinates);
        mNodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        mNodePoints.setMaterial(materialModule.findMaterialByName("grey"));

        // Size of our green spheres

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = mNodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

        // create "mapped" group

        mMappedPoints = scene.createGraphicsPoints();
        mMappedPoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        mMappedPoints.setCoordinateField(mCoordinates);
        mMappedPoints.setSubgroupField(mMappedSelectionGroup);

        mMappedPoints.setMaterial(materialModule.findMaterialByName("green"));
        mMappedPoints.setSelectedMaterial(materialModule.findMaterialByName("orange"));

        pointAttr = mMappedPoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

        scene.moveGraphicsBefore(mMappedPoints, mNodePoints);

        // create label points

        mLabelPoints = scene.createGraphicsPoints();
        mLabelPoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        mLabelPoints.setCoordinateField(mCoordinates);

        mLabelPoints.setMaterial(materialModule.findMaterialByName("black"));
        mLabelPoints.setSelectedMaterial(materialModule.findMaterialByName("black"));

        pointAttr = mLabelPoints.getGraphicspointattributes();

        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_NONE);

        pointAttr.setLabelField(mDisplayField);
        double dist = 0.75;
        pointAttr.setLabelOffset(3,&dist);

        // Axes

        mAxes = scene.createGraphicsPoints();

        mAxes.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_POINT);
        mAxes.setMaterial(materialModule.findMaterialByName("blue"));

        // usefull with devicePixelRatioChanged
        //mAxesFontPointSize = axes.getGraphicspointattributes().getFont().getPointSize();

        // Surfaces

        mSurfaces = scene.createGraphicsSurfaces();
        mSurfaces.setCoordinateField(mCoordinates);

        mSurfaces.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("white"));

        // Isosurfaces

        OpenCMISS::Zinc::Tessellation tessellation = mZincContext.getTessellationmodule().createTessellation();
        int intValue = 8;

        tessellation.setManaged(true);
        tessellation.setMinimumDivisions(1, &intValue);

        mIsosurfaces = scene.createGraphicsContours();
        mIsosurfaces.setCoordinateField(mCoordinates);

        double doubleValue = 1.0;

        mIsosurfaces.setIsoscalarField(mMagnitude);
        mIsosurfaces.setListIsovalues(1, &doubleValue);
        mIsosurfaces.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("gold"));
        mIsosurfaces.setTessellation(tessellation);

    scene.endChange();

    // adding view all command

    sceneViewer().viewAll();
    sceneViewer().getLookatPosition(mLookAtPositionOriginal);

    // Customise the size of our points

    double left, right, bottom, top, nearPlane, farPlane;

    sceneViewer().getViewingVolume(&left, &right, &bottom, &top, &nearPlane, &farPlane);

    //TODO is it the best way to guess ? Maybe min is more accurate
    //TODO is the cast in function below ok ??
    int nodeSize= qCeil(qLn(0.0017*qMax(right-left,qMax(top-bottom,farPlane-nearPlane)))*invLnNodeSizeExp);

    mEditingWidget->setWheelPosition(nodeSize);
    setNodeSizes(nodeSize);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::showHideGraphics(GraphicsType pGraphicsType)
{
    // Show/hide graphics on the scene of our default region

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();
    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        // Axes

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Axes)) {
            mAxes.setVisibilityFlag(mActionAxes->isChecked());
        }

        // Lines

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Lines)) {
            mLines.setVisibilityFlag(mActionLines->isChecked());
        }

        // Surfaces

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Surfaces)) {
            mSurfaces.setVisibilityFlag(mActionSurfaces->isChecked());
        }

        // Isosurfaces

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Isosurfaces)) {
            mIsosurfaces.setVisibilityFlag(mActionIsosurfaces->isChecked());
        }

        // Labels

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Label)) {
            mLabelPoints.setVisibilityFlag(mActionLabel->isChecked());
        }

    scene.endChange();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::click(int pX, int pY, bool pCanDiscard)
{
    mScenePicker.setSceneviewerRectangle(sceneViewer(), OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT,
                                          pX - mSizeSelection,
                                          pY - mSizeSelection,
                                          pX + mSizeSelection,
                                          pY + mSizeSelection);
    OpenCMISS::Zinc::Node node = mScenePicker.getNearestNode();

    int buff = node.getIdentifier();

    if(pCanDiscard||buff!=-1) {

        mIdSelectedNode = buff;
        mEditingWidget->selectNode(mIdSelectedNode);

        // select the node to highlight graphics

        OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();

        OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();
        OpenCMISS::Zinc::Scene scene = region.getScene();

        if (pCanDiscard) {
            if (buff==-1) {
                sceneViewer().setLookatPosition(mLookAtPositionOriginal);
            } else {
                double focusPoint[] = {0,0,0};
                OpenCMISS::Zinc::Field field = fieldModule.findFieldByName(qPrintable(mCoordinatesName));
                OpenCMISS::Zinc::Fieldcache fieldCache = fieldModule.createFieldcache();
                fieldCache.setNode(node);

                field.evaluateReal(fieldCache,3,focusPoint);
                sceneViewer().setLookatPosition(focusPoint);
            }
        }

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

void CellMLZincMappingViewZincWidget:: setNodeSizes(int pSize) {
    mNodeSize = pow(nodeSixeExp,pSize);
    //TODO change size of mapped nodes

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();

    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        mNodePoints.getGraphicspointattributes().setBaseSize(1, &mNodeSize);
        mMappedPoints.getGraphicspointattributes().setBaseSize(1, &mNodeSize);
        mLabelPoints.getGraphicspointattributes().setBaseSize(1, &mNodeSize);
    scene.endChange();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::eraseNodeSlot()
{
    eraseNode(mIdSelectedNode);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::eraseNode(int pId)
{
    mEditingWidget->eraseNodeValue(pId);

    // select and highlight the current node

    auto fieldModule = mZincContext.getDefaultRegion().getFieldmodule();
    OpenCMISS::Zinc::Node node = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES).findNodeByIdentifier(pId);

    fieldModule.beginChange();

        if (node.isValid()){
            OpenCMISS::Zinc::Nodeset nodes = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::FieldNodeGroup nodeGroupField = mMappedSelectionGroup.getFieldNodeGroup(nodes);

            if (!nodeGroupField.isValid()) {
                nodeGroupField = mMappedSelectionGroup.createFieldNodeGroup(nodes);
            }
            OpenCMISS::Zinc::NodesetGroup nodesetGroup = nodeGroupField.getNodesetGroup();
            nodesetGroup.removeNode(node);

            OpenCMISS::Zinc::Nodeset nodeSet = fieldModule.findNodesetByFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
            OpenCMISS::Zinc::Nodetemplate nodeTemplate = nodeSet.createNodetemplate();
            nodeTemplate.defineField(mDisplayField);

            OpenCMISS::Zinc::Fieldcache fieldCache = fieldModule.createFieldcache();

            fieldCache.setNode(node);
            node.merge(nodeTemplate);

            mDisplayField.assignString(fieldCache,"");

        } else {
            if (mMappedSelectionGroup.isValid()) {
                mZincContext.getDefaultRegion().getScene().setSelectionField(OpenCMISS::Zinc::Field());
            }
        }
    fieldModule.endChange();
}

//==============================================================================

void CellMLZincMappingViewZincWidget::actionAxesTriggered()
{
    // Show/hide our axes

    showHideGraphics(GraphicsType::Axes);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::actionLinesTriggered()
{
    // Show/hide our lines

    showHideGraphics(GraphicsType::Lines);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::actionSurfacesTriggered()
{
    // Show/hide our surfaces

    showHideGraphics(GraphicsType::Surfaces);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::actionIsosurfacesTriggered()
{
    // Show/hide our isosurfaces

    showHideGraphics(GraphicsType::Isosurfaces);
}

//==============================================================================

void CellMLZincMappingViewZincWidget::actionLabelTriggered()
{
    // Show/hide our isosurfaces

    showHideGraphics(GraphicsType::Label);
}

//==============================================================================

/*
void CellMLZincMappingViewZincWidget::devicePixelRatioChanged(int pDevicePixelRatio)
{
    // Update our scene using the given devide pixel ratio

    OpenCMISS::Zinc::Scene scene = mZincContext->getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
}
*/

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
