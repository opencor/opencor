/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Data store window
//==============================================================================

#include "datastoreinterface.h"
#include "datastorewindow.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

#include "ui_datastorewindow.h"

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

DataStoreWindow::DataStoreWindow(DataStore *pDataStore, QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::DataStoreWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our tree view widget...
#endif

    // Populate our tree view

    mModel = new QStandardItemModel(mGui->treeView);

    mGui->treeView->setModel(mModel);

    QString variableHierarchy = QString();
    QStandardItem *hierarchyItem = 0;

    foreach (DataStoreVariable *variable, pDataStore->voiAndVariables()) {
        if (variable->isVisible()) {
            // Check whether the variable is in the same hierarchy as the
            // previous one

            static const QRegularExpression VariableNameRegEx = QRegularExpression("/[^/]+(/prime)*$");

            QString crtVariableHierarchy = variable->uri().remove(VariableNameRegEx);

            if (crtVariableHierarchy.compare(variableHierarchy)) {
                // The variable is in a different component hierarchy, so create
                // a new section hierarchy for our 'new' component, reusing
                // existing sections, whenever possible

                QStandardItem *parentHierarchyItem = mModel->invisibleRootItem();

                foreach (const QString &hierarchyPart, crtVariableHierarchy.split("/")) {
                    hierarchyItem = 0;

                    for (int i = 0, iMax = parentHierarchyItem->rowCount(); i < iMax; ++i) {
                        QStandardItem *childHierarchyItem = parentHierarchyItem->child(i);

                        if (!childHierarchyItem->text().compare(hierarchyPart)) {
                            hierarchyItem = childHierarchyItem;

                            break;
                        }
                    }

                    if (!hierarchyItem) {
                        hierarchyItem = new QStandardItem(hierarchyPart);

                        hierarchyItem->setAutoTristate(true);
                        hierarchyItem->setCheckable(true);
                        hierarchyItem->setEditable(false);

                        parentHierarchyItem->appendRow(hierarchyItem);
                    }

                    parentHierarchyItem = hierarchyItem;
                }

                variableHierarchy = crtVariableHierarchy;
            }

            QStandardItem *variableItem = new QStandardItem(variable->icon(),
                                                            variable->label());

            variableItem->setCheckable(true);
            variableItem->setCheckState(Qt::Checked);
            variableItem->setEditable(false);

            hierarchyItem->appendRow(variableItem);
        }
    }

    mGui->treeView->expandAll();

    updateVariablesSelectedState();
}

//==============================================================================

DataStoreWindow::~DataStoreWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void DataStoreWindow::on_allVariablesCheckBox_clicked()
{
    // Un/select all the variables

    updateVariablesSelectedState(mModel->invisibleRootItem(),
                                 mGui->allVariablesCheckBox->isChecked()?Qt::Checked:Qt::Unchecked);
}

//==============================================================================

void DataStoreWindow::on_buttonBox_accepted()
{
    // Confirm that we accepted the data selection

    accept();
}

//==============================================================================

void DataStoreWindow::on_buttonBox_rejected()
{
    // Simply cancel whatever was done here

    reject();
}

//==============================================================================

void DataStoreWindow::updateVariablesSelectedState(QStandardItem *pItem,
                                                   const Qt::CheckState &pCheckState)
{
    // Update the selected state of the given item's children

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        QStandardItem *childItem = pItem->child(i);

        childItem->setCheckState(pCheckState);

        if (childItem->rowCount())
            updateVariablesSelectedState(childItem, pCheckState);
    }
}

//==============================================================================

void DataStoreWindow::checkVariablesSelectedState(QStandardItem *pItem)
{
    // Update the selected state of the given item's children

    int nbOfSelectedChildItems = 0;
    bool partiallySelectedChildItems = false;

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        QStandardItem *childItem = pItem->child(i);

        if (childItem->rowCount())
            checkVariablesSelectedState(childItem);

        nbOfSelectedChildItems += childItem->checkState() == Qt::Checked;

        if (childItem->checkState() == Qt::PartiallyChecked)
            partiallySelectedChildItems = true;
    }

    pItem->setCheckState(nbOfSelectedChildItems?
                             (nbOfSelectedChildItems == pItem->rowCount())?
                                 Qt::Checked:
                                 Qt::PartiallyChecked:
                             partiallySelectedChildItems?
                                 Qt::PartiallyChecked:
                                 Qt::Unchecked);
}

//==============================================================================

void DataStoreWindow::updateVariablesSelectedState(QStandardItem *pItem)
{
    // Disable the connection that handles a change in a variable's selected
    // state (otherwise what we are doing here is going to be completely
    // uneffective)

    disconnect(mModel, SIGNAL(itemChanged(QStandardItem *)),
               this, SLOT(updateVariablesSelectedState(QStandardItem *)));

    // In case we un/select a hierarchy, then go through its variables and
    // un/select them accordingly

    if (pItem && pItem->isAutoTristate())
        updateVariablesSelectedState(pItem, (pItem->checkState() == Qt::Unchecked)?Qt::Unchecked:Qt::Checked);

    // Update the selected state of all our hierarchies

    int nbOfTopLevelHierarchiesChecked = 0;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *childItem = mModel->invisibleRootItem()->child(i);

        checkVariablesSelectedState(childItem);

        nbOfTopLevelHierarchiesChecked += childItem->checkState() == Qt::Checked;
    }

    mGui->allVariablesCheckBox->setCheckState(nbOfTopLevelHierarchiesChecked?
                                                  (nbOfTopLevelHierarchiesChecked == mModel->invisibleRootItem()->rowCount())?
                                                      Qt::Checked:
                                                      Qt::PartiallyChecked:
                                                  Qt::Unchecked);

    // Re-enable the connection that handles a change in a variable's selected
    // state

    connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(updateVariablesSelectedState(QStandardItem *)));
}

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
