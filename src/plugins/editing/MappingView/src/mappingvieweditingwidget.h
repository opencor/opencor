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

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "mappingviewzincwidget.h"
#include "splitterwidget.h"
#include "viewwidget.h"

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_wheel.h"
#include "qwtend.h"

//==============================================================================

#include <QLabel>
#include <QStandardItemModel>
#include <QTreeView>

//==============================================================================

namespace Ui {
    class MappingViewWidget;
} // namespace Ui

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class ToolBarWidget;
} // namespace Core

//==============================================================================

namespace MappingView {

//==============================================================================


class MappingViewEditingModel : public QStandardItemModel
{
    Q_OBJECT

public:
    static const char *MappingViewEdittingMimeType;

    explicit MappingViewEditingModel(QObject *pParent = nullptr);

    QStringList mimeTypes() const override;
    QMimeData * mimeData(const QModelIndexList &pIndexes) const override;

};

class MappingViewEditingWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit MappingViewEditingWidget(const QString &pCellmlFileName,
                                      const QString &pMeshFileName,
                                               QWidget *pParent);

    void retranslateUi() override;

    void selectNode(int pId);
    void setNodeValue(const int pId, const QString &pVariable);

    void filePermissionsChanged();

    bool setMeshFile(const QString &pFileName, bool pShowWarning = true);

private:

    QwtWheel *mDelayWidget;

    Core::ToolBarWidget *mToolBarWidget;

    Core::SplitterWidget *mVerticalSplitterWidget;
    Core::SplitterWidget *mHorizontalSplitterWidget;
    MappingViewZincWidget *mZincWidget;

    QLabel *mNodeValue;
    QLabel *mVariableValue;
    QTreeView *mVariableTree;

    MappingViewEditingModel *mVariableTreeModel;

    QString mMeshFileName;

    CellMLSupport::CellmlFile *mCellmlFile;

    QMap<int, QString> mMapMatch;

    void populateTree();
    void populateOutput(const QString &pMeshFileName);

signals:
    void horizontalSplitterMoved(const QIntList &pSizes);
    void verticalSplitterMoved(const QIntList &pSizes);

private slots:
    void emitHorizontalSplitterMoved();
    void emitVerticalSplitterMoved();
};

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
