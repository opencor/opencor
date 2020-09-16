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
// CellML-Zinc Mapping view editing widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "corecliutils.h"
#include "cellmlzincmappingviewzincwidget.h"
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

namespace CellMLZincMappingView {

//==============================================================================

static const auto CellMLZincMappingViewEditingMimeType = QStringLiteral("opencor/mapping-view-editing");

//==============================================================================

class CellMLZincMappingViewEditingModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit CellMLZincMappingViewEditingModel(QObject *pParent = nullptr);

    QStringList mimeTypes() const override;
    QMimeData * mimeData(const QModelIndexList &pIndexes) const override;

};

//==============================================================================

class CellMLZincMappingViewWidget;

//==============================================================================

class CellMLZincMappingViewEditingWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellMLZincMappingViewEditingWidget(const QString &pCellmlFileName,
                                                const QStringList &pMeshFileNames,
                                                QWidget *pParent,
                                                CellMLZincMappingViewWidget *pViewWidget);

    void retranslateUi() override;

    void selectNode(int pId);
    void setNodeValue(const int pId, const QString &pComponent, const QString &pVariable);
    void eraseNodeValue(const int pId);

    void setWheelPosition(int pValue);

    void filePermissionsChanged();

    bool setMeshFiles(const QStringList &pFileNames, bool pShowWarning = true);

    void setSizes(const QIntList &pSizesHorizontal,
                  const QIntList &pSizesVertical);

    CellMLZincMappingViewZincWidget * getZincWidget();

signals:
    void horizontalSplitterMoved(const QIntList &pSizes);
    void verticalSplitterMoved(const QIntList &pSizes);

protected:
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    void dragMoveEvent(QDragMoveEvent *pEvent) override;
    void dropEvent(QDropEvent *pEvent) override;

private:
    struct _variable {
        QString component;
        QString variable;
    };

    CellMLZincMappingViewWidget *mViewWidget;

    QMap<int, _variable> mMapMatch;

    QAction *mClearNode;
    QAction *mSaveMapping;
    QAction *mOpenMeshFile;
    QAction *mOpenMappingFile;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QwtWheel *mNodeSizeWidget;

    Core::ToolBarWidget *mToolBarWidget;

    Core::SplitterWidget *mVerticalSplitterWidget;
    Core::SplitterWidget *mHorizontalSplitterWidget;
    CellMLZincMappingViewZincWidget *mZincWidget;

    QLineEdit *mFilterLineEdit;

    QLabel *mNodeValue;
    QLabel *mComponentValue;
    QLabel *mVariableValue;
    QTreeView *mVariableTree;

    CellMLZincMappingViewEditingModel *mVariableTreeModel;

    QStringList mZincMeshFileNames;

    CellMLSupport::CellmlFile *mCellmlFile;

    void populateTree();
    void saveMapping(const QString &pFileName);
    void openMapping(const QString &pFileName);

    QString fileName(const QString &pFileName, const QString &pBaseFileName,
                     const QString &pFileExtension, const QString &pCaption,
                     const QStringList &pFileFilters);

    QMap<QString, FileTypeInterface *> mFileTypeInterfaces;

private slots:
    void filterChanged(const QString &text);
    void emitHorizontalSplitterMoved();
    void emitVerticalSplitterMoved();
    void saveMappingSlot();
    void openMeshFile();
    void openMappingFile();
};

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
