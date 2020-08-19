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

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/context.hpp"
    #include "opencmiss/zinc/field.hpp"
    #include "opencmiss/zinc/fieldvectoroperators.hpp"
    #include "opencmiss/zinc/graphics.hpp"
#include "zincend.h"

//==============================================================================

class QMenu;

//==============================================================================

namespace Ui {
    class ZincWindowWindow;
} // namespace Ui

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace ZincWidget {
    class ZincWidget;
} // namespace ZincWidget

//==============================================================================

namespace ZincWindow {

//==============================================================================

class ZincWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit ZincWindowWindow(QWidget *pParent);
    ~ZincWindowWindow() override;

    void retranslateUi() override;

protected:
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dragMoveEvent(QDragMoveEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;

private:
    enum class GraphicsType {
        All,
        Axes,
        Points,
        Lines,
        Surfaces,
        Isosurfaces
    };

    Ui::ZincWindowWindow *mGui;

    bool mShuttingDown = false;

    ZincWidget::ZincWidget *mZincWidget;
    OpenCMISS::Zinc::Context mZincContext;

    char *mZincSceneViewerDescription = nullptr;

    OpenCMISS::Zinc::Field mCoordinates;
    OpenCMISS::Zinc::FieldMagnitude mMagnitude;
    OpenCMISS::Zinc::Graphicspointattributes mAxesAttributes;
    OpenCMISS::Zinc::Graphicspointattributes mPointsAttributes;
    OpenCMISS::Zinc::GraphicsLines mLines;
    OpenCMISS::Zinc::GraphicsSurfaces mSurfaces;
    OpenCMISS::Zinc::GraphicsContours mIsosurfaces;

    QStringList mZincMeshFileNames;
    bool mDroppedZincMeshFiles = false;

    int mAxesFontPointSize = 0;

    QString mTrilinearCubeMeshFileName;

    void showHideGraphics(GraphicsType pGraphicsType);

    void loadZincMeshFiles(const QStringList &pZincMeshFiles);

private slots:
    void createAndSetZincContext();
    void graphicsInitialized();
    void devicePixelRatioChanged(int pDevicePixelRatio);

    void actionAxesTriggered();
    void actionPointsTriggered();
    void actionLinesTriggered();
    void actionSurfacesTriggered();
    void actionIsosurfacesTriggered();
    void actionTrilinearCubeTriggered();
};

//==============================================================================

} // namespace ZincWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
