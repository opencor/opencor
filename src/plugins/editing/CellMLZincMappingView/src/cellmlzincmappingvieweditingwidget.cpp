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
#include "coreguiutils.h"
#include "filemanager.h"
#include "cellmlzincmappingvieweditingwidget.h"
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
namespace CellMLZincMappingView {

//==============================================================================

CellMLZincMappingViewEditingModel::CellMLZincMappingViewEditingModel(QObject *pParent) :
    QStandardItemModel(pParent)
{
}

//==============================================================================

const char *CellMLZincMappingViewEditingModel::MappingViewEdittingMimeType = "opencor/mapping-view-editting";

//==============================================================================

QStringList CellMLZincMappingViewEditingModel::mimeTypes() const
{
    return {MappingViewEdittingMimeType};
}

//==============================================================================

QMimeData * CellMLZincMappingViewEditingModel::mimeData(const QModelIndexList &pIndexes) const
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

CellMLZincMappingViewEditingWidget::CellMLZincMappingViewEditingWidget(const QString &pFileName,
                                                   const QString &pMeshFileName,
                                                   QWidget *pParent) :
    Core::Widget(pParent),
    mMapMatch(),
    mMeshFileName(pMeshFileName)
{
    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    //create and fill layout

    QLayout *layout = createLayout();

        //create and add toolbar

        mToolBarWidget = new Core::ToolBarWidget();

        //create and add widget and actions

        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();

        mSaveMapping = Core::newAction(QIcon(":/oxygen/actions/document-save.png"),
                                                        mToolBarWidget);
        //TODO trash could be hidden when nothing to show
        mClearNode = Core::newAction(QIcon(":/oxygen/actions/edit-clear.png"),
                                                        mToolBarWidget);

        mDelayWidget = new QwtWheel(mToolBarWidget);
            mDelayWidget->setBorderWidth(0);
            mDelayWidget->setFixedSize(int(0.07*availableGeometry.width()),
                                       mDelayWidget->height()/2);
            mDelayWidget->setFocusPolicy(Qt::NoFocus);
            mDelayWidget->setRange(0.0, 100.0);
            mDelayWidget->setWheelBorderWidth(0);
            mDelayWidget->setValue(CellMLZincMappingViewZincWidget::nodeSizeOrigin);

        mToolBarWidget->addAction(mSaveMapping);
        mToolBarWidget->addAction(mClearNode);
        mToolBarWidget->addWidget(mDelayWidget);

    layout->addWidget(mToolBarWidget);

        //create horizontal splitterwidget

        mHorizontalSplitterWidget = new Core::SplitterWidget(Qt::Horizontal, this);

        connect(mHorizontalSplitterWidget, &Core::SplitterWidget::splitterMoved,
                this, &CellMLZincMappingViewEditingWidget::emitHorizontalSplitterMoved);

        //create vertical splitterwidget

        mVerticalSplitterWidget = new Core::SplitterWidget(Qt::Vertical, this);

        connect(mVerticalSplitterWidget, &Core::SplitterWidget::splitterMoved,
                this, &CellMLZincMappingViewEditingWidget::emitVerticalSplitterMoved);

        //create and add the variable tree:

        mVariableTree = new QTreeView(this);
        mVariableTree->setDragEnabled(true);
        mVariableTree->setEditTriggers(QTreeView::NoEditTriggers);

        mVariableTreeModel = new CellMLZincMappingViewEditingModel();
        mVariableTree->setModel(mVariableTreeModel);

        mHorizontalSplitterWidget->addWidget(mVariableTree);

        // Keep track of our movement
        /*
        connect(this, &Core::SplitterWidget::splitterMoved,
                this, &MappingViewEditingWidget::splitterMoved);
        */

        //addWidget(mListWidgetVariables);

        // add a Zinc widget

        mZincWidget = new CellMLZincMappingViewZincWidget(this, mMeshFileName, this);

        connect(mClearNode, &QAction::triggered,
                mZincWidget, &CellMLZincMappingViewZincWidget::eraseNode);
        connect(mDelayWidget, &QwtWheel::valueChanged,
                mZincWidget, &CellMLZincMappingViewZincWidget::setNodeSizes);

        mHorizontalSplitterWidget->addWidget(mZincWidget);

        //add the other connectionss

        connect(mSaveMapping, &QAction::triggered,
                this, &CellMLZincMappingViewEditingWidget::saveMappingSlot);

        //create and add informative labels

        Core::Widget *labelWidget = new Core::Widget(this);
        QGridLayout *labelLayout = new QGridLayout(labelWidget);

        QLabel *nodeLabel = new QLabel("Node:",this);
        labelLayout->addWidget(nodeLabel,0,0);

        mNodeValue = new QLabel(this);
        labelLayout->addWidget(mNodeValue,0,1);

        QLabel *componentLabel = new QLabel("Component:",this);
        labelLayout->addWidget(componentLabel,1,0);

        mComponentValue = new QLabel(this);
        labelLayout->addWidget(mComponentValue,1,1);

        QLabel *variableLabel = new QLabel("Variable:",this);
        labelLayout->addWidget(variableLabel,2,0);

        mVariableValue = new QLabel(this);
        labelLayout->addWidget(mVariableValue,2,1);

        labelLayout->columnMinimumWidth(0);

        //fill vertical Splitter

        mVerticalSplitterWidget->addWidget(mHorizontalSplitterWidget);
        mVerticalSplitterWidget->addWidget(labelWidget);

    layout->addWidget(mVerticalSplitterWidget);

    populateTree();
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::retranslateUi()
{
    //TODO ?
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::setNodeValue(const int pId, const QString &pComponent, const QString &pVariable)
{
    if (pId!=-1) {
        mMapMatch.insert(pId,{pComponent,pVariable});
        selectNode(pId);
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::eraseNodeValue(const int pId)
{
    if (pId!=-1) {
        mMapMatch.remove(pId);
        selectNode(pId);
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::filePermissionsChanged()
{
}

//==============================================================================

bool CellMLZincMappingViewEditingWidget::setMeshFile(const QString &pFileName, bool pShowWarning)
{
    //TODO warnings ?
Q_UNUSED(pShowWarning)

    mMeshFileName = pFileName;
    mZincWidget->changeSource(pFileName);
    mMapMatch.clear();

    return true;
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::populateTree()
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

void CellMLZincMappingViewEditingWidget::saveMapping(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    //find a filename

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
    QString localCellmlFileName = mCellmlFile->fileName();
    bool remoteCellmlFile = fileManagerInstance->isRemote(localCellmlFileName);
    QString cellmlFileName = remoteCellmlFile?
                                 fileManagerInstance->url(localCellmlFileName):
                                 localCellmlFileName;
    //TODO do something more json related ?
    QString mappingFileName = fileName(pFileName, cellmlFileName,
                                     QStringLiteral("json"),
                                     tr("Save CellML Mapping File"),
                                     QStringList());

    //Jsonise the map

    QJsonArray jsonMapArray;

    for (int node : mMapMatch.keys()) {
        QJsonObject item;
        item.insert("node",QJsonValue::fromVariant(node));
        _variable var = mMapMatch[node];
        item.insert("component",QJsonValue::fromVariant(var.component));
        item.insert("variable",QJsonValue::fromVariant(var.variable));
        jsonMapArray.push_back(item);
    }

    QJsonDocument doc(jsonMapArray);

    //saving it

    QFile jsonFile(mappingFileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
}


//==============================================================================

QString CellMLZincMappingViewEditingWidget::fileName(const QString &pFileName,
                                                           const QString &pBaseFileName,
                                                           const QString &pFileExtension,
                                                           const QString &pCaption,
                                                           const QStringList &pFileFilters)
{
    // Return the given file name, if it is not empty, or ask the user to
    // provide one using the additional information that is given

    if (!pFileName.isEmpty()) {
        return pFileName;
    }

    QString fileName = pBaseFileName;
    QString baseFileCompleteSuffix = QFileInfo(pBaseFileName).completeSuffix();

    if (baseFileCompleteSuffix.isEmpty()) {
        fileName += "."+pFileExtension;
    } else {
        fileName.replace(QRegularExpression(QRegularExpression::escape(baseFileCompleteSuffix)+"$"), pFileExtension);
    }

    QString firstFileFilter = pFileFilters.isEmpty()?QString():pFileFilters.first();

    return Core::getSaveFileName(pCaption, fileName, pFileFilters, &firstFileFilter);
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::emitHorizontalSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitterWidget->sizes());
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::emitVerticalSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit verticalSplitterMoved(mVerticalSplitterWidget->sizes());
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::selectNode(int pId)
{
    if (pId==-1) {
        mNodeValue->setText("");
        mComponentValue->setText("");
        mVariableValue->setText("");
    } else {
        mNodeValue->setNum(pId);
        if (mMapMatch.contains(pId)) {
            mComponentValue->setText(mMapMatch[pId].component);
            mVariableValue->setText(mMapMatch[pId].variable);
        } else {
            mComponentValue->setText("");
            mVariableValue->setText("");
        }
    }
}

//==============================================================================

void CellMLZincMappingViewEditingWidget::saveMappingSlot()
{
    saveMapping({});
}

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
