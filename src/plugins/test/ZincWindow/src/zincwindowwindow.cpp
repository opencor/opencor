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
#include <QDragEnterEvent>
#include <QMenu>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/fieldfiniteelement.hpp"
    #include "opencmiss/zinc/streamregion.hpp"
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

    connect(mGui->actionAxes, &QAction::triggered,
            this, &ZincWindowWindow::actionAxesTriggered);
    connect(mGui->actionPoints, &QAction::triggered,
            this, &ZincWindowWindow::actionPointsTriggered);
    connect(mGui->actionLines, &QAction::triggered,
            this, &ZincWindowWindow::actionLinesTriggered);
    connect(mGui->actionSurfaces, &QAction::triggered,
            this, &ZincWindowWindow::actionSurfacesTriggered);
    connect(mGui->actionIsosurfaces, &QAction::triggered,
            this, &ZincWindowWindow::actionIsosurfacesTriggered);
    connect(mGui->actionTrilinearCube, &QAction::triggered,
            this, &ZincWindowWindow::actionTrilinearCubeTriggered);

    // Create a temporary copy of our trilinear cube mesh file

    static const QString ExFileName = "/ZincWindow/trilinearCube.exfile";

    mTrilinearCubeMeshFileName = Core::canonicalFileName(QDir::tempPath()+ExFileName);

    Core::writeResourceToFile(mTrilinearCubeMeshFileName, ":"+ExFileName);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create and populate our context menu

    QMenu *contextMenu = new QMenu(this);

    contextMenu->addAction(mGui->actionAxes);
    contextMenu->addAction(mGui->actionPoints);
    contextMenu->addAction(mGui->actionLines);
    contextMenu->addAction(mGui->actionSurfaces);
    contextMenu->addAction(mGui->actionIsosurfaces);
    contextMenu->addSeparator();
    contextMenu->addAction(mGui->actionTrilinearCube);

    // Create and add a Zinc widget

    mZincWidget = new ZincWidget::ZincWidget(this);

    mZincWidget->setContextMenu(contextMenu);

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
    // Keep track of the fact that we are shutting down

    mShuttingDown = true;

    // Delete the temporary copy of our .exfile file

    QFile::remove(mTrilinearCubeMeshFileName);

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

void ZincWindowWindow::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void ZincWindowWindow::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void ZincWindowWindow::dropEvent(QDropEvent *pEvent)
{
    // Load the dropped files as Zinc mesh files

    loadZincMeshFiles(Core::droppedFileNames(pEvent));

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

//==============================================================================

void ZincWindowWindow::loadZincMeshFiles(const QStringList &pZincMeshFiles)
{
    // Keep track of Zinc mesh files and reset our scene viewer description

    mZincMeshFileNames = pZincMeshFiles;
    mDroppedZincMeshFiles = true;

    // Reset our Zinc widget

    mZincWidget->reset();
}

//==============================================================================

void ZincWindowWindow::createAndSetZincContext()
{
    // Make sure that we are not shutting down (i.e. skip the case where we are
    // coming here as a result of closing OpenCOR)

    if (mShuttingDown) {
        return;
    }

    // Keep track of our current scene viewer's description, if needed

    mZincSceneViewerDescription = mDroppedZincMeshFiles?
                                      nullptr:
                                      mZincWidget->sceneViewer().writeDescription();
    mDroppedZincMeshFiles = false;

    // Create and set our Zinc context

    mZincContext = OpenCMISS::Zinc::Context("ZincWindowWindow");

    mZincContext.getMaterialmodule().defineStandardMaterials();
    mZincContext.getGlyphmodule().defineStandardGlyphs();

    mZincWidget->setContext(mZincContext);

    // Add the Zinc mesh files to our default region, or show a tri-linear cube
    // if there are no Zinc mesh files

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();
    OpenCMISS::Zinc::StreaminformationRegion sir = region.createStreaminformationRegion();

    if (mZincMeshFileNames.empty()) {
        sir.createStreamresourceFile(mTrilinearCubeMeshFileName.toUtf8().constData());
    } else {
        for (const auto &zincMeshFileName : mZincMeshFileNames) {
            sir.createStreamresourceFile(zincMeshFileName.toUtf8().constData());
        }
    }

    region.read(sir);

    // Create a field magnitude for our default region

    OpenCMISS::Zinc::Fieldmodule fieldModule = region.getFieldmodule();

    fieldModule.beginChange();
        OpenCMISS::Zinc::Fielditerator fieldIterator = fieldModule.createFielditerator();
        OpenCMISS::Zinc::Field field = fieldIterator.next();

        while (field.isValid()) {
            if (    field.isTypeCoordinate()
                && (field.getValueType() == OpenCMISS::Zinc::Field::VALUE_TYPE_REAL)
                && (field.getNumberOfComponents() <= 3)
                &&  field.castFiniteElement().isValid()) {
                mCoordinates = field.castFiniteElement();

                break;
            }

            field = fieldIterator.next();
        }

        mMagnitude = fieldModule.createFieldMagnitude(mCoordinates);

        mMagnitude.setManaged(true);
    fieldModule.endChange();

    // Show/hide graphics on the scene of our default region

    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        // Axes

        OpenCMISS::Zinc::GraphicsPoints axes = scene.createGraphicsPoints();
        OpenCMISS::Zinc::Materialmodule materialModule = mZincContext.getMaterialmodule();

        axes.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_POINT);
        axes.setMaterial(materialModule.findMaterialByName("blue"));

        mAxesFontPointSize = axes.getGraphicspointattributes().getFont().getPointSize();
        mAxesAttributes = axes.getGraphicspointattributes();

        // Points

        OpenCMISS::Zinc::GraphicsPoints points = scene.createGraphicsPoints();

        points.setCoordinateField(mCoordinates);
        points.setFieldDomainType(OpenCMISS::Zinc::Field::DOMAIN_TYPE_NODES);
        points.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("green"));

        mPointsAttributes = points.getGraphicspointattributes();

        // Lines

        mLines = scene.createGraphicsLines();

        mLines.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("black"));

        // Surfaces

        mSurfaces = scene.createGraphicsSurfaces();

        mSurfaces.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("white"));

        // Isosurfaces

        OpenCMISS::Zinc::Tessellation tessellation = mZincContext.getTessellationmodule().createTessellation();
        int intValue = 8;

        tessellation.setManaged(true);
        tessellation.setMinimumDivisions(1, &intValue);

        mIsosurfaces = scene.createGraphicsContours();

        double doubleValue = 1.0;

        mIsosurfaces.setIsoscalarField(mMagnitude);
        mIsosurfaces.setListIsovalues(1, &doubleValue);
        mIsosurfaces.setMaterial(mZincContext.getMaterialmodule().findMaterialByName("gold"));
        mIsosurfaces.setTessellation(tessellation);
    scene.endChange();

    showHideGraphics(GraphicsType::All);

    // Update our scene using our initial device pixel ratio

    devicePixelRatioChanged(mZincWidget->devicePixelRatio());

    // Make sure that the mesh is visible

    mZincWidget->viewAll();

    // Customise the size of our axes and points

    double left, right, bottom, top, nearPlane, farPlane;

    mZincWidget->sceneViewer().getViewingVolume(&left, &right, &bottom, &top, &nearPlane, &farPlane);

    doubleValue = 0.65*right;

    mAxesAttributes.setBaseSize(1, &doubleValue);

    doubleValue = 0.02*right;

    mPointsAttributes.setBaseSize(1, &doubleValue);
}

//==============================================================================

void ZincWindowWindow::graphicsInitialized()
{
    // Set our 'new' scene viewer's description

    mZincWidget->sceneViewer().readDescription(mZincSceneViewerDescription);
}

//==============================================================================

void ZincWindowWindow::devicePixelRatioChanged(int pDevicePixelRatio)
{
    // Update our scene using the given device pixel ratio

    OpenCMISS::Zinc::Scene scene = mZincContext.getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
}

//==============================================================================

void ZincWindowWindow::showHideGraphics(GraphicsType pGraphicsType)
{
    // Show/hide graphics on the scene of our default region

    OpenCMISS::Zinc::Region region = mZincContext.getDefaultRegion();
    OpenCMISS::Zinc::Scene scene = region.getScene();

    scene.beginChange();
        // Axes

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Axes)) {
            mAxesAttributes.setGlyphShapeType(mGui->actionAxes->isChecked()?
                                                  OpenCMISS::Zinc::Glyph::SHAPE_TYPE_AXES_XYZ:
                                                  OpenCMISS::Zinc::Glyph::SHAPE_TYPE_NONE);
        }

        // Points

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Points)) {
            mPointsAttributes.setGlyphShapeType(mGui->actionPoints->isChecked()?
                                                    OpenCMISS::Zinc::Glyph::SHAPE_TYPE_SPHERE:
                                                    OpenCMISS::Zinc::Glyph::SHAPE_TYPE_NONE);
        }

        // Lines

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Lines)) {
            mLines.setCoordinateField(mGui->actionLines->isChecked()?
                                          mCoordinates:
                                          OpenCMISS::Zinc::Field());
        }

        // Surfaces

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Surfaces)) {
            mSurfaces.setCoordinateField(mGui->actionSurfaces->isChecked()?
                                             mCoordinates:
                                             OpenCMISS::Zinc::Field());
        }

        // Isosurfaces

        if (   (pGraphicsType == GraphicsType::All)
            || (pGraphicsType == GraphicsType::Isosurfaces)) {
            mIsosurfaces.setCoordinateField(mGui->actionIsosurfaces->isChecked()?
                                                mCoordinates:
                                                OpenCMISS::Zinc::Field());
        }
    scene.endChange();
}

//==============================================================================

void ZincWindowWindow::actionAxesTriggered()
{
    // Show/hide our axes

    showHideGraphics(GraphicsType::Axes);
}

//==============================================================================

void ZincWindowWindow::actionPointsTriggered()
{
    // Show/hide our points

    showHideGraphics(GraphicsType::Points);
}

//==============================================================================

void ZincWindowWindow::actionLinesTriggered()
{
    // Show/hide our lines

    showHideGraphics(GraphicsType::Lines);
}

//==============================================================================

void ZincWindowWindow::actionSurfacesTriggered()
{
    // Show/hide our surfaces

    showHideGraphics(GraphicsType::Surfaces);
}

//==============================================================================

void ZincWindowWindow::actionIsosurfacesTriggered()
{
    // Show/hide our isosurfaces

    showHideGraphics(GraphicsType::Isosurfaces);
}

//==============================================================================

void ZincWindowWindow::actionTrilinearCubeTriggered()
{
    // Load the trilinear cube mesh

    loadZincMeshFiles(QStringList() << mTrilinearCubeMeshFileName);
}

//==============================================================================

} // namespace ZincWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
