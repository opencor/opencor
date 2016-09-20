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
// Preferences dialog
//==============================================================================

#include "cliutils.h"
#include "pluginmanager.h"
#include "preferencesdialog.h"
#include "preferencesinterface.h"

//==============================================================================

#include "ui_preferencesdialog.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void PreferencesItemDelegate::paint(QPainter *pPainter,
                                    const QStyleOptionViewItem &pOption,
                                    const QModelIndex &pIndex) const
{
    // Paint the item as normal, if it is selectable, or bold, if it is a
    // category

    QStandardItem *pluginItem = qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex);

    QStyleOptionViewItem option(pOption);

    initStyleOption(&option, pIndex);

    if (pluginItem->hasChildren())
        option.font.setBold(true);

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

PreferencesDialog::PreferencesDialog(PluginManager *pPluginManager,
                                     QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PreferencesDialog),
    mPluginManager(pPluginManager),
    mCategoryItems(QMap<PluginInfo::Category, QStandardItem *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our general preferences and empty widgets

    mGeneralPreferencesWidget = new GeneralPreferencesWidget(this);
    mEmptyWidget = new QWidget(this);

    mGui->stackedWidget->addWidget(mGeneralPreferencesWidget);
    mGui->stackedWidget->addWidget(mEmptyWidget);

    // Set up the tree view widget

    mModel = new QStandardItemModel(mGui->treeView);

#ifdef Q_OS_MAC
    mGui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our plugins tree view widget...
#endif
    mGui->treeView->setModel(mModel);
    mGui->treeView->setItemDelegate(new PreferencesItemDelegate());

    // Populate the data model with our general category and our plugins that
    // support the Preferences interface

    mGeneralItem = new QStandardItem(tr("General"));

    mModel->invisibleRootItem()->appendRow(mGeneralItem);

    foreach (Plugin *plugin, mPluginManager->sortedPlugins()) {
        PreferencesInterface *preferencesInterface = qobject_cast<PreferencesInterface *>(plugin->instance());

        if (preferencesInterface) {
            // Create the item corresponding to the current plugin

            QStandardItem *pluginItem = new QStandardItem(plugin->name());

            // Only selectable plugins and plugins that are of the right type are
            // checkable

            PluginInfo *pluginInfo = plugin->info();

            if (pluginInfo) {
                // Add the plugin to its corresponding category

                pluginCategoryItem(pluginInfo->category())->appendRow(pluginItem);
            }
        }
    }

    // Expand the whole tree view widget and make sure that it only takes as
    // much width as necessary
    // Note: for some reasons, the retrieved column size gives us a width that
    //       is slightly too small and therefore requires a horizontal scroll
    //       bar, hence we add 15% to it (those extra 15% seems to be enough
    //       to account even for a big number of plugins which would then
    //       require a vertical scroll bar)

    mGui->treeView->expandAll();
    mGui->treeView->resizeColumnToContents(0);

    mGui->treeView->setMinimumWidth(1.15*mGui->treeView->columnWidth(0));
    mGui->treeView->setMaximumWidth(mGui->treeView->minimumWidth());

    // Make sure that the dialog has a reasonable starting size

    mGui->layout->setSizeConstraint(QLayout::SetMinimumSize);
//---ISSUE193--- DO WE ACTUALLY NEED IT?...

    // Connection to handle the change of preferences widget

    connect(mGui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updatePreferencesWidget(const QModelIndex &, const QModelIndex &)));

    // Select our general item

    mGui->treeView->setCurrentIndex(mGeneralItem->index());
}

//==============================================================================

PreferencesDialog::~PreferencesDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

QStandardItem * PreferencesDialog::pluginCategoryItem(const PluginInfo::Category &pCategory)
{
    // Return the given category item, after having created it, if it didn't
    // already exist

    QStandardItem *res = mCategoryItems.value(pCategory);

    if (!res) {
        // No category item exists for the given category, so create one and add
        // it to our data model (and this in the right place)

        bool inserted = false;
        QStandardItem *rootItem = mModel->invisibleRootItem();
        QString categoryName = pluginCategoryName(pCategory);
        QString nonDiacriticCategoryName = nonDiacriticString(categoryName);

        res = new QStandardItem(categoryName);

        for (int i = 0, iMax = rootItem->rowCount(); i < iMax; ++i) {
            QStandardItem *categoryItem = rootItem->child(i);
            int comparison = nonDiacriticCategoryName.compare(nonDiacriticString(categoryItem->text()));

            if (comparison < 0) {
                inserted = true;

                mModel->invisibleRootItem()->insertRow(i, res);

                break;
            }
        }

        if (!inserted)
            mModel->invisibleRootItem()->appendRow(res);

        // Keep track of the relationship between our new item and its category

        mCategoryItems.insert(pCategory, res);
    }

    return res;
}

//==============================================================================

void PreferencesDialog::on_buttonBox_accepted()
{
//---ISSUE193---
    // Confirm that we accepted the changes

    accept();
}

//==============================================================================

void PreferencesDialog::on_buttonBox_rejected()
{
    // Simply cancel whatever was done here

    reject();
}

//==============================================================================

void PreferencesDialog::updatePreferencesWidget(const QModelIndex &pNewIndex,
                                                const QModelIndex &pOldIndex)
{
//---ISSUE193---
Q_UNUSED(pOldIndex);
    // Switch to the widget corresponding to the selected item

    if (pNewIndex == mGeneralItem->index())
        mGui->stackedWidget->setCurrentWidget(mGeneralPreferencesWidget);
//    else if (mCategoryItems.values().contains(mModel->itemFromIndex(pNewIndex)))
//        mGui->stackedWidget->setCurrentWidget(mGui->sectionPage);
    else
        mGui->stackedWidget->setCurrentWidget(mEmptyWidget);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
