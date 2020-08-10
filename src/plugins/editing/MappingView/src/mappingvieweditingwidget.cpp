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

#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "filemanager.h"
#include "mappingvieweditingwidget.h"
#include "mappingviewmeshreader.h"
#include "toolbarwidget.h"
#include "zincwidget.h"


//==============================================================================

#include <QFile>
#include <QLayout>
#include <QtGui>

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

MappingViewEditingModel::MappingViewEditingModel(QObject *pParent) :
    QStandardItemModel(pParent)
{
}

//==============================================================================

const char *MappingViewEditingModel::MappingViewEdittingMimeType = "opencor/mapping-view-editting";

//==============================================================================

QStringList MappingViewEditingModel::mimeTypes() const
{
    return {MappingViewEdittingMimeType};
}

//==============================================================================

QMimeData * MappingViewEditingModel::mimeData(const QModelIndexList &pIndexes) const
{
    auto res = new QMimeData();
    QString names;

    for (const auto &index : pIndexes) {
        names.append(itemFromIndex(index)->text()+"|"+itemFromIndex(index)->accessibleDescription());
    }

    res->setData(MappingViewEdittingMimeType,names.toUtf8());

    return res;
}

//==============================================================================

MappingViewEditingWidget::MappingViewEditingWidget(const QString &pFileName,
                                                   const QString &pMeshFileName,
                                                   QWidget *pParent) :
    Core::Widget(pParent),
    mMeshFileName(pMeshFileName),
    mMapMatch()
{
    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    //create and fill layout

    QLayout *layout = createLayout();

        //create and add toolbar

        mToolBarWidget = new Core::ToolBarWidget();

            QRect availableGeometry = qApp->primaryScreen()->availableGeometry();

            mDelayWidget = new QwtWheel(mToolBarWidget);

            mDelayWidget->setBorderWidth(0);
            mDelayWidget->setFixedSize(int(0.07*availableGeometry.width()),
                                       mDelayWidget->height()/2);
            mDelayWidget->setFocusPolicy(Qt::NoFocus);
            mDelayWidget->setRange(0.0, 100.0);
            mDelayWidget->setWheelBorderWidth(0);

            mDelayWidget->setValue(MappingViewZincWidget::nodeSizeOrigin);

        mToolBarWidget->addWidget(mDelayWidget);

    layout->addWidget(mToolBarWidget);

        //create horizontal splitterwidget

        mHorizontalSplitterWidget = new Core::SplitterWidget(Qt::Horizontal, this);

        connect(mHorizontalSplitterWidget, &Core::SplitterWidget::splitterMoved,
                this, &MappingViewEditingWidget::emitHorizontalSplitterMoved);

        //create vertical splitterwidget

        mVerticalSplitterWidget = new Core::SplitterWidget(Qt::Vertical, this);

        connect(mVerticalSplitterWidget, &Core::SplitterWidget::splitterMoved,
                this, &MappingViewEditingWidget::emitVerticalSplitterMoved);

        //create and add the variable tree:

        mVariableTree = new QTreeView(this);
        mVariableTree->setDragEnabled(true);

        mVariableTreeModel = new MappingViewEditingModel();
        mVariableTree->setModel(mVariableTreeModel);

        mHorizontalSplitterWidget->addWidget(mVariableTree);

        // Keep track of our movement
        /*
        connect(this, &Core::SplitterWidget::splitterMoved,
                this, &MappingViewEditingWidget::splitterMoved);
        */

        //addWidget(mListWidgetVariables);

        // add a Zinc widget

        mZincWidget = new MappingViewZincWidget(this, mMeshFileName, this);

        connect(mDelayWidget, &QwtWheel::valueChanged,
                mZincWidget, &MappingViewZincWidget::setNodeSizes );

        mHorizontalSplitterWidget->addWidget(mZincWidget);

        //create and add informative labels

        Core::Widget *labelWidget = new Core::Widget(this);
        QGridLayout *labelLayout = new QGridLayout(labelWidget);

        QLabel *nodeLabel = new QLabel("Node:",this);
        labelLayout->addWidget(nodeLabel,0,0);

        mNodeValue = new QLabel(this);
        labelLayout->addWidget(mNodeValue,0,1);

        QLabel *variableLabel = new QLabel("Variable:",this);
        labelLayout->addWidget(variableLabel,1,0);

        mVariableValue = new QLabel(this);
        labelLayout->addWidget(mVariableValue,1,1);

        //fill vertical Splitter

        mVerticalSplitterWidget->addWidget(mHorizontalSplitterWidget);
        mVerticalSplitterWidget->addWidget(labelWidget);

    layout->addWidget(mVerticalSplitterWidget);

    populateTree();
}

//==============================================================================

void MappingViewEditingWidget::retranslateUi()
{
    //TODO ?
}

//==============================================================================

void MappingViewEditingWidget::setNodeValue(const int pId, const QString &pVariable)
{
    if (pId!=-1) {
        mMapMatch.insert(pId,pVariable);
        selectNode(pId);
    }
}

//==============================================================================

void MappingViewEditingWidget::filePermissionsChanged()
{
}

//==============================================================================

bool MappingViewEditingWidget::setMeshFile(const QString &pFileName, bool pShowWarning)
{
    //TODO warnings ?
Q_UNUSED(pShowWarning)

    mMeshFileName = pFileName;
    mZincWidget->changeSource(pFileName);

    return false;
}

//==============================================================================

void MappingViewEditingWidget::populateTree()
{

    // Make sure that we have a model before actually populating ourselves

    iface::cellml_api::Model *cellmlModel = mCellmlFile->model();

    if (cellmlModel == nullptr) {
        return;
    }

    mVariableTreeModel->clear();
    mVariableTree->setSelectionMode(QAbstractItemView::SingleSelection);

    // Retrieve the model's components

    ObjRef<iface::cellml_api::CellMLComponentSet> components = cellmlModel->localComponents();

    if (components->length() != 0) {

        // Retrieve the model's components themselves

        ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIter = components->iterateComponents();

        for (ObjRef<iface::cellml_api::CellMLComponent> component = componentsIter->nextComponent();
            component != nullptr; component = componentsIter->nextComponent()) {

            // Retrieve the model's component's variables

            ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = component->variables();

            if (componentVariables->length() != 0) {

                QStandardItem *componentItem = new QStandardItem(QString::fromStdWString(component->name()));
                componentItem->setDragEnabled(false);

                mVariableTreeModel->invisibleRootItem()->appendRow(componentItem);

                // Retrieve the model's component's variables themselves

                ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIter = componentVariables->iterateVariables();

                for (ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIter->nextVariable();
                    componentVariable != nullptr; componentVariable = componentVariablesIter->nextVariable()) {

                    QStandardItem *variableItem = new QStandardItem(QString::fromStdWString(componentVariable->name()));
                    variableItem->setAccessibleDescription(QString::fromStdWString(component->name()));

                    componentItem->appendRow(variableItem);
                }
            }
        }
    }
}

//==============================================================================

void MappingViewEditingWidget::emitHorizontalSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitterWidget->sizes());
}

//==============================================================================

void MappingViewEditingWidget::emitVerticalSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit verticalSplitterMoved(mVerticalSplitterWidget->sizes());
}

//==============================================================================

void MappingViewEditingWidget::selectNode(int pId)
{
    if (pId==-1) {
        mNodeValue->setText("");
        mVariableValue->setText("");
    } else {
        mNodeValue->setNum(pId);
        mVariableValue->setText(mMapMatch[pId]);
    }
}

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
