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
// Data store dialog
//==============================================================================

#include "coreguiutils.h"
#include "datastoredialog.h"
#include "datastoreinterface.h"
#include "plugin.h"

//==============================================================================

#include "ui_datastoredialog.h"

//==============================================================================

#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

DataItemDelegate::DataItemDelegate(QObject *pParent) :
    Core::StyledItemDelegate(pParent)
{
}

//==============================================================================

void DataItemDelegate::paint(QPainter *pPainter,
                             const QStyleOptionViewItem &pOption,
                             const QModelIndex &pIndex) const
{
    // Paint the item as normal unless it's a hierarchy item, in which case we
    // make it bold

    QStandardItem *dataItem = qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex);

    QStyleOptionViewItem option(pOption);

    initStyleOption(&option, pIndex);

    if (dataItem->hasChildren()) {
        option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

DataStoreDialog::DataStoreDialog(const QString &pDataStoreName,
                                 DataStore *pDataStore, bool pIncludeVoi,
                                 const QMap<int, QIcon> &pIcons,
                                 QWidget *pParent) :
    Core::Dialog(pParent),
    mGui(new Ui::DataStoreDialog)
{
    // Customise our settings

    mSettings.beginGroup(SettingsPlugins);
    mSettings.beginGroup(pDataStoreName);
    mSettings.beginGroup("DataStoreDialog");

    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    mGui->dataLabel->hide();

    connect(mGui->allDataCheckBox, &QCheckBox::clicked,
            this, &DataStoreDialog::allDataCheckBoxClicked);
    connect(mGui->allDataCheckBox, &QCheckBox::toggled,
            mGui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::setEnabled);

    connect(mGui->buttonBox, &QDialogButtonBox::accepted,
            this, &DataStoreDialog::accept);
    connect(mGui->buttonBox, &QDialogButtonBox::rejected,
            this, &DataStoreDialog::reject);

    // Populate our tree view with the data store's variables and, or not, the
    // VOI
    // Note: indeed, in some cases (e.g. CSV export), we want to list all the
    //       variables including the VOI while in some other cases (e.g.
    //       BioSignalML export), we don't want to list the VOI (since, to
    //       respect the BioSignalML format, the VOI must absolutely be
    //       exported)...

    mModel = new QStandardItemModel(this);

    mGui->treeView->setModel(mModel);
    mGui->treeView->setItemDelegate(new DataItemDelegate(this));

    QString dataHierarchy;
    QStandardItem *hierarchyItem = nullptr;

    for (auto variable : pIncludeVoi?pDataStore->voiAndVariables():pDataStore->variables()) {
        if (variable->isVisible()) {
            // Check whether the variable is in the same hierarchy as the
            // previous one

            static const QRegularExpression VariableNameRegEx = QRegularExpression("/[^/]+(/prime)*$");

            QString crtDataHierarchy = variable->uri().remove(VariableNameRegEx);

            if (crtDataHierarchy != dataHierarchy) {
                // The variable is in a different component hierarchy, so create
                // a new section hierarchy for our 'new' component, reusing
                // existing sections, whenever possible

                QStandardItem *parentHierarchyItem = mModel->invisibleRootItem();

                for (const auto &hierarchyPart : crtDataHierarchy.split('/')) {
                    hierarchyItem = nullptr;

                    for (int i = 0, iMax = parentHierarchyItem->rowCount(); i < iMax; ++i) {
                        QStandardItem *childHierarchyItem = parentHierarchyItem->child(i);

                        if (childHierarchyItem->text() == hierarchyPart) {
                            hierarchyItem = childHierarchyItem;

                            break;
                        }
                    }

                    if (hierarchyItem == nullptr) {
                        hierarchyItem = new QStandardItem(hierarchyPart);

                        hierarchyItem->setAutoTristate(true);
                        hierarchyItem->setCheckable(true);
                        hierarchyItem->setEditable(false);

                        parentHierarchyItem->appendRow(hierarchyItem);
                    }

                    parentHierarchyItem = hierarchyItem;
                }

                dataHierarchy = crtDataHierarchy;
            }

            // Create a new item and add it to our hierarchy, but only if it
            // really exists

            if (hierarchyItem != nullptr) {
                static const QIcon ErrorNodeIcon = QIcon(":/oxygen/emblems/emblem-important.png");

                QIcon variableIcon = pIcons.value(variable->type());
                auto dataItem = new QStandardItem(variableIcon.isNull()?
                                                      ErrorNodeIcon:
                                                      variableIcon,
                                                  variable->name());

                dataItem->setCheckable(true);
                dataItem->setCheckState(Qt::Checked);
                dataItem->setEditable(false);

                hierarchyItem->appendRow(dataItem);

                mData.insert(dataItem, variable);

                ++mNbOfData;
            }
        }
    }

    mGui->treeView->expandAll();

    updateDataSelectedState(nullptr);

    // Set our minimum size

    setMinimumSize(Core::minimumWidgetSize(this));
}

//==============================================================================

DataStoreDialog::~DataStoreDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void DataStoreDialog::addWidget(QWidget *pWidget)
{
    // Insert the given widget before our tree view, set our focus to it and
    // show our data selector label

    mGui->layout->insertWidget(0, pWidget);

    setFocusProxy(pWidget);
    setFocus();

    mGui->dataLabel->show();

    // Resize ourselves to make sure that the new widget doesn't squash our
    // original contents

    resize(QSize(qMax(qMax(width(), pWidget->sizeHint().width()), mGui->dataLabel->sizeHint().width()),
                 pWidget->sizeHint().height()+mGui->dataLabel->sizeHint().height()+height()));
}

//==============================================================================

DataStoreVariables DataStoreDialog::selectedData(QStandardItem *pItem) const
{
    // Return the selected data for the given item

    DataStoreVariables res;

    if (pItem->hasChildren()) {
        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            res << selectedData(pItem->child(i));
        }
    } else if (pItem->checkState() == Qt::Checked) {
        res << mData.value(pItem);
    }

    return res;
}

//==============================================================================

DataStoreVariables DataStoreDialog::selectedData() const
{
    // Return our selected data

    DataStoreVariables res;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        res << selectedData(mModel->invisibleRootItem()->child(i));
    }

    return res;
}

//==============================================================================

void DataStoreDialog::checkDataSelectedState(QStandardItem *pItem,
                                             int &pNbOfselectedData)
{
    // Update the selected state of the given item's children

    int nbOfSelectedChildItems = 0;
    bool partiallySelectedChildItems = false;

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        QStandardItem *childItem = pItem->child(i);

        if (childItem->hasChildren()) {
            checkDataSelectedState(childItem, pNbOfselectedData);
        }

        if (childItem->checkState() == Qt::Checked) {
            ++nbOfSelectedChildItems;

            if (!childItem->hasChildren()) {
                ++pNbOfselectedData;
            }
        }

        if (childItem->checkState() == Qt::PartiallyChecked) {
            partiallySelectedChildItems = true;
        }
    }

    pItem->setCheckState((nbOfSelectedChildItems != 0)?
                             (nbOfSelectedChildItems == pItem->rowCount())?
                                 Qt::Checked:
                                 Qt::PartiallyChecked:
                             partiallySelectedChildItems?
                                 Qt::PartiallyChecked:
                                 Qt::Unchecked);
}

//==============================================================================

void DataStoreDialog::updateDataSelectedState(QStandardItem *pItem,
                                              const Qt::CheckState &pCheckState)
{
    // Update the selected state of the given item's children

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        QStandardItem *childItem = pItem->child(i);

        childItem->setCheckState(pCheckState);

        if (childItem->hasChildren()) {
            updateDataSelectedState(childItem, pCheckState);
        }
    }
}

//==============================================================================

void DataStoreDialog::updateDataSelectedState(QStandardItem *pItem)
{
    // Disable the handling of the itemChanged() signal (otherwise what we are
    // doing here is going to be completely ineffective)

    disconnect(mModel, &QStandardItemModel::itemChanged,
               this, QOverload<QStandardItem *>::of(&DataStoreDialog::updateDataSelectedState));

    // In case we un/select a hierarchy, then go through its data and un/select
    // it accordingly, or keep track of the number of selected data, if we
    // un/select some data

    if ((pItem != nullptr) && pItem->isAutoTristate()) {
        updateDataSelectedState(pItem, (pItem->checkState() == Qt::Unchecked)?Qt::Unchecked:Qt::Checked);
    }

    // Update the selected state of all our hierarchies

    int nbOfSelectedData = 0;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *childItem = mModel->invisibleRootItem()->child(i);

        checkDataSelectedState(childItem, nbOfSelectedData);
    }

    mGui->allDataCheckBox->setCheckState((nbOfSelectedData != 0)?
                                             (nbOfSelectedData == mNbOfData)?
                                                 Qt::Checked:
                                                 Qt::PartiallyChecked:
                                             Qt::Unchecked);

    // Re-enable the handling of the itemChanged() signal

    connect(mModel, &QStandardItemModel::itemChanged,
            this, QOverload<QStandardItem *>::of(&DataStoreDialog::updateDataSelectedState));
}

//==============================================================================

void DataStoreDialog::allDataCheckBoxClicked()
{
    // If our checked state is partially checked, then we want to make it fully
    // so

    if (mGui->allDataCheckBox->checkState() == Qt::PartiallyChecked) {
        mGui->allDataCheckBox->setCheckState(Qt::Checked);
    }

    // Un/select all the data
    // Note: we temporally disable the handling of the itemChanged() signal
    //       since we 'manually' set everything ourselves...

    disconnect(mModel, &QStandardItemModel::itemChanged,
               this, QOverload<QStandardItem *>::of(&DataStoreDialog::updateDataSelectedState));

    updateDataSelectedState(mModel->invisibleRootItem(),
                            mGui->allDataCheckBox->isChecked()?Qt::Checked:Qt::Unchecked);

    connect(mModel, &QStandardItemModel::itemChanged,
            this, QOverload<QStandardItem *>::of(&DataStoreDialog::updateDataSelectedState));
}

//==============================================================================

} // namespace DataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
