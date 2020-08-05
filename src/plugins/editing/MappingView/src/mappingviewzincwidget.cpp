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
#include "corecliutils.h"
#include "filemanager.h"
#include "cellmlfilemanager.h"

//==============================================================================

#include <QMimeData>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTimer>
#include <QDir>

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

MappingViewZincWidget::MappingViewZincWidget(QWidget *pParent, const QString &pMainFileName) :
    ZincWidget::ZincWidget(pParent),
    mMainFileName(pMainFileName),
    mNodeSize(nodeSizeOrigin)
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
        click(pEvent);
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
    // Accept the proposed action for the event, but only if we are dropping
    // URIs
qDebug(">>> dragEnterZincWidget text:%s html:%s",
       qPrintable(pEvent->mimeData()->text()),
       qPrintable(pEvent->mimeData()->html()));
for (auto format : pEvent->mimeData()->formats())
    qDebug("%s",qPrintable(format));
    if (   pEvent->mimeData()->hasFormat(Core::FileSystemMimeType)) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void MappingViewZincWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    /*
    // Set the state to dragging, since we can only come here when dragging
    // Note: the state is properly set for file organiser objects being dragged,
    //       but should we be dragging external objects over our file organiser
    //       widget, then the state will (obviously) not be set. This wouldn't
    //       be a problem in itself if it was for the fact that this prevents
    //       the drop indicator from being painted...

    setState(QAbstractItemView::DraggingState);

    // Default handling of the event
    // Note: this gives us the drop indicator...

    TreeViewWidget::dragMoveEvent(pEvent);

    // Accept the proposed action for the event, but only if there are objects
    // to drop and if we are not trying to drop the objects above/on/below one
    // of them (should the objects come from the file organiser widget) or on a
    // file item
    // Note #1: for the number of objects being dropped, we have to check the
    //          number of URLs information (i.e. external objects), as well as
    //          the MIME data associated with FileOrganiserWindowMimeType (i.e.
    //          objects from the file organiser widget, after we have )
    // Note #2: for the dropping location, it can be either a folder or a file
    //          (as long as the indicator position isn't on the item itself),
    //          but not above/on/below any of the objects (or any of their
    //          children) being dragged (only relevant when dragging items from
    //          the file organiser widget)

    QByteArray data = pEvent->mimeData()->data(FileOrganiserWindowMimeType);
    QModelIndexList indexes = mModel->decodeData(data);
    FileOrganiserWindowItem *dropItem = static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(indexAt(pEvent->pos())));
    bool draggingOnSelfOrChild = false;

    if (dropItem != nullptr) {
        for (int i = 0; (i < indexes.count()) && !draggingOnSelfOrChild; ++i) {
            draggingOnSelfOrChild = itemIsOrIsChildOf(dropItem,
                                                      mModel->itemFromIndex(indexes[i]));
        }
    }

    if (   (!pEvent->mimeData()->urls().isEmpty() || !indexes.isEmpty())
        && (   ((dropItem != nullptr) && dropItem->isFolder())
            || (dropIndicatorPosition() != QAbstractItemView::OnItem))
        && !draggingOnSelfOrChild) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
    */
    pEvent->accept();
}

//==============================================================================

void MappingViewZincWidget::dropEvent(QDropEvent *pEvent)
{
    /*
    // Note: the MIME data definitely contains the FileSystemMimeType MIME type
    //       (for objects originating from outside this widget), but it may also
    //       contain the FileOrganiserWindowMimeType MIME type (for objects
    //       originating from within this widget). FileOrganiserWindowMimeType
    //       is used by this widget while FileSystemMimeType by external
    //       widgets. So, this means that we must check for
    //       FileOrganiserWindowMimeType first

    // Files have been dropped, so add them to the widget and this at the right
    // place (i.e. above/on/below a folder, above/below a file or on the
    // invisible root folder)

    // First, determine the item above/on/below which objects are to be dropped,
    // as well as the drop position (i.e. above, on or below)

    QStandardItem *dropItem;
    DropIndicatorPosition dropPosition = dropIndicatorPosition();

    if (dropPosition == QAbstractItemView::OnViewport) {
        // We dropped the files on the viewport

        dropItem = mModel->invisibleRootItem();

        // Change the drop position since we know that we want want the objects
        // to be dropped on the root folder

        dropPosition = QAbstractItemView::OnItem;
    } else {
        // We dropped the files above/on/below a folder or above/below a file

        dropItem = mModel->itemFromIndex(indexAt(pEvent->pos()));
    }

    // Check the type of MIME data to be dropped

    if (pEvent->mimeData()->hasFormat(FileOrganiserWindowMimeType)) {
        // The user is dropping folders/files from ourselves, i.e. s/he wants
        // some folders/files to be moved around

        // Retrieve the list of indexes to move around and clean it

        QByteArray data = pEvent->mimeData()->data(FileOrganiserWindowMimeType);
        QModelIndexList indexes = cleanIndexList(mModel->decodeData(data));

        // Convert our list of indexes to a list of items
        // Note: indeed, by moving the item corresponding to a particular index,
        //       we may mess up the other indexes, meaning that we may not be
        //       able to retrieve their corresponding item...

        QList<QStandardItem *> items;

        for (int i = 0, iMax = indexes.count(); i < iMax; ++i) {
            items << mModel->itemFromIndex(indexes[i]);
        }

        // Move the contents of the list to its final destination

        if (dropPosition != QAbstractItemView::BelowItem) {
            // Move the items in the order they were dropped

            for (int i = 0, iMax = items.count(); i < iMax; ++i) {
                moveItem(items[i], dropItem, dropPosition);
            }
        } else {
            // Move the items in a reverse order to that they were dropped since
            // we want them moved below the current item

            for (int i = items.count()-1; i >= 0; --i) {
                moveItem(items[i], dropItem, dropPosition);
            }
        }
    } else {
        // The user wants to drop files, so add them to the widget and this at
        // the right place

        QList<QUrl> urls = pEvent->mimeData()->urls();

        if (dropPosition != QAbstractItemView::BelowItem) {
            // Add the files in the order they were dropped

            for (int i = 0, iMax = urls.count(); i < iMax; ++i) {
                addFile(urls[i].toLocalFile(), dropItem, dropPosition);
            }
        } else {
            // Add the files in a reverse order to that they were dropped since
            // we want them added below the current item

            for (int i = urls.count()-1; i >= 0; --i) {
                addFile(urls[i].toLocalFile(), dropItem, dropPosition);
            }
        }
    }

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();

    // Reset the state of the widget
    // Note: there doesn't seem to be and there shouldn't be a need to reset the
    //       state after the widget (the resetting seems to be done elsewhere),
    //       but then if we don't reset the state of the widget, then the drop
    //       indicator may, in some cases, remain visible (a bug in Qt?)...

    setState(QAbstractItemView::NoState);
    */
    Q_UNUSED(pEvent);
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

        OpenCMISS::Zinc::GraphicsPoints nodePoints = mScene->createGraphicsPoints();

        nodePoints.setCoordinateField(coordinates);
        nodePoints.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        nodePoints.setMaterial(materialModule.findMaterialByName("green"));

        // Size of our green spheres

        OpenCMISS::Zinc::Graphicspointattributes pointAttr = nodePoints.getGraphicspointattributes();

        pointAttr.setBaseSize(1, &mNodeSize);
        pointAttr.setGlyphShapeType(OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE);

    mScene->endChange();

    // adding view all command

    mSceneViewer.viewAll();
}

//==============================================================================

void MappingViewZincWidget::click(QMouseEvent *pEvent)
{
    mScenePicker->setSceneviewerRectangle(mSceneViewer, OpenCMISS::Zinc::SCENECOORDINATESYSTEM_WINDOW_PIXEL_TOP_LEFT,
                                          pEvent->x() - mSizeSelection,
                                          pEvent->y() - mSizeSelection,
                                          pEvent->x() + mSizeSelection,
                                          pEvent->y() + mSizeSelection);
    OpenCMISS::Zinc::Node node = mScenePicker->getNearestNode();

    int id = node.getIdentifier();
    emit nodeSelection(id);

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
     mNodeSize = pSize;
    //TODO
    setup();
    initializeGL();
}

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
