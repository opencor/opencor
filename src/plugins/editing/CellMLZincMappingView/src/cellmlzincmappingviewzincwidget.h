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

#pragma once

//==============================================================================

#include <QFile>
#include <QStandardItemModel>
#include <QtMath>

//==============================================================================

#include "zincwidget.h"

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/fieldfiniteelement.hpp"
    #include "opencmiss/zinc/timekeeper.hpp"
    #include "opencmiss/zinc/scenepicker.hpp"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

class CellMLZincMappingViewEditingWidget;

//==============================================================================

class CellMLZincMappingViewZincWidget : public ZincWidget::ZincWidget
{
    Q_OBJECT

public:
    explicit CellMLZincMappingViewZincWidget(QWidget *pParent, const QStringList &pZincMeshFileNames,
                                   CellMLZincMappingViewEditingWidget *pEditingWidget);

    ~CellMLZincMappingViewZincWidget() override;

    void changeSource(const QStringList &pZincMeshFileNames);

    bool hasNode(int pId);
    void setNodeMapped(int pId);

    static constexpr double nodeSizeOrigin = 0;
    static constexpr double nodeSixeExp = 1.1;

public slots:
    void setNodeSizes(int pSize);
    void eraseNode(int pId = -1);

protected:
    void initializeGL() override;

    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void wheelEvent(QWheelEvent *pEvent) override;

//  void changeEvent(QEvent *pEvent) override;
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dragMoveEvent(QDragMoveEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;

private:

    double invLnNodeSizeExp = 1/qLn(nodeSixeExp);

    QStringList mZincMeshFileNames;
    QString mCoordinatesName;

    CellMLZincMappingViewEditingWidget *mEditingWidget;

    QPoint mMousePosClick;

    double mNodeSize;
    double mMappedNodeSize;
    //static constexpr double nodeSixeDiff = 1;

    char *mZincSceneViewerDescription;
    double mLookAtPositionOriginal[3];

    //size of the square drawn around the mouse for selections
    //TODO improve this, should depend on the number of pixel at screen
    int mSizeSelection = 2;
    int mIdSelectedNode = -1;

    OpenCMISS::Zinc::Context mZincContext;
    OpenCMISS::Zinc::Scenepicker mScenePicker;

    OpenCMISS::Zinc::FieldGroup mMappedSelectionGroup;
    OpenCMISS::Zinc::GraphicsPoints mNodePoints;
    OpenCMISS::Zinc::GraphicsPoints mMappedPoints;

    void setup();
    void setupRegion();
    void draw();

    void click(int pX, int pY, bool pCanDiscard = true);

    //TODO ?
//private slots:
    //void devicePixelRatioChanged(int pDevicePixelRatio);

};

//==============================================================================

} // namespace ZincWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
