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
// Preferences dialog
//==============================================================================

#include "cliutils.h"
#include "generalpreferenceswidget.h"
#include "guiutils.h"
#include "pluginmanager.h"
#include "preferencesdialog.h"
#include "preferencesinterface.h"

//==============================================================================

#include "ui_preferencesdialog.h"

//==============================================================================

#include <QFormLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {

//==============================================================================

PreferencesPluginCategoryWidget::PreferencesPluginCategoryWidget(QWidget *pParent) :
    QWidget(pParent)
{
    // Create a form widget layout

    auto formWidgetLayout = new QFormLayout(this);

    formWidgetLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    setLayout(formWidgetLayout);

    // Add both our category and description fields to our form widget layout

    mCategoryValue = new QLabel(this);
    mDescriptionValue = new QLabel(this);

    formWidgetLayout->addRow(label(tr("Category:")), mCategoryValue);
    formWidgetLayout->addRow(label(tr("Description:")), mDescriptionValue);
}

//==============================================================================

void PreferencesPluginCategoryWidget::setCategory(const QString &pCategory)
{
    // Set our category

    mCategoryValue->setText(pCategory);
}

//==============================================================================

void PreferencesPluginCategoryWidget::setDescription(const QString &pDescription)
{
    // Set our description

    mDescriptionValue->setText(pDescription);
}

//==============================================================================

QLabel * PreferencesPluginCategoryWidget::label(const QString &pLabel)
{
    // Create and return a label

    auto res = new QLabel(pLabel, this);

    res->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    QFont newFont = res->font();

    newFont.setBold(true);

    res->setFont(newFont);

    return res;
}

//==============================================================================

PreferencesItemDelegate::PreferencesItemDelegate(QObject *pParent) :
    StyledItemDelegate(pParent)
{
}

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

    if (pluginItem->hasChildren()) {
        option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

PreferencesDialog::PreferencesDialog(PluginManager *pPluginManager,
                                     const QString &pPluginName,
                                     QWidget *pParent) :
    Dialog(pParent),
    mGui(new Ui::PreferencesDialog()),
    mPluginManager(pPluginManager)
{
    // Customise our settings

    mSettings.beginGroup("PreferencesDialog");

    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->treeView, &QTreeView::collapsed,
            this, &PreferencesDialog::treeViewCollapsed);

    connect(mGui->buttonBox, &QDialogButtonBox::accepted,
            this, &PreferencesDialog::buttonBoxAccepted);
    connect(mGui->buttonBox, &QDialogButtonBox::rejected,
            this, &PreferencesDialog::reject);

    // Customise our GUI's button box by having both a reset and a reset all
    // (settings) button

    mResetButton = mGui->buttonBox->addButton(tr("Reset"), QDialogButtonBox::ActionRole);
    mResetAllButton = mGui->buttonBox->addButton(tr("Reset All"), QDialogButtonBox::ActionRole);

    connect(mResetButton, &QPushButton::clicked,
            this, &PreferencesDialog::reset);
    connect(mResetAllButton, &QPushButton::clicked,
            this, &PreferencesDialog::resetAll);

    // Create and add our plugin category widget

    mPluginCategoryWidget = new PreferencesPluginCategoryWidget(this);

    mGui->stackedWidget->addWidget(mPluginCategoryWidget);

    // Set up the tree view widget

    mModel = new QStandardItemModel(this);

#ifdef Q_OS_MAC
    mGui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    mGui->treeView->setModel(mModel);
    mGui->treeView->setItemDelegate(new PreferencesItemDelegate(this));

    // Populate the data model with our General section

    auto generalPluginItem = new QStandardItem(tr("General"));
    auto selectedPluginItem = generalPluginItem;

    mModel->invisibleRootItem()->appendRow(generalPluginItem);

    auto generalPreferencesWidget = new GeneralPreferencesWidget(mainWindow());

    mGui->stackedWidget->addWidget(generalPreferencesWidget);

    mItemPreferencesWidgets.insert(generalPluginItem, generalPreferencesWidget);

    // Populate the data model with our plugins that support the Preferences
    // interface

    for (auto plugin : mPluginManager->sortedPlugins()) {
        PreferencesInterface *preferencesInterface = qobject_cast<PreferencesInterface *>(plugin->instance());

        if (   (preferencesInterface != nullptr)
            && (preferencesInterface->preferencesWidget() != nullptr)) {
            // Create the item corresponding to the current plugin and add it to
            // its corresponding category

            auto pluginItem = new QStandardItem(plugin->name());

            pluginCategoryItem(plugin->info()->category())->appendRow(pluginItem);

            if (plugin->name() == pPluginName) {
                selectedPluginItem = pluginItem;
            }

            // Retrieve the corresponding preferences widget and add it to our
            // stacked widget, as well as keep track of it

            Preferences::PreferencesWidget *preferencesWidget = preferencesInterface->preferencesWidget();

            mGui->stackedWidget->addWidget(preferencesWidget);

            mItemPreferencesWidgets.insert(pluginItem, preferencesWidget);
            mPreferencesWidgetPluginNames.insert(preferencesWidget, plugin->name());
        }
    }

    // Expand the whole tree view widget and make sure that it only takes as
    // much width as necessary
    // Note: for some reasons, the retrieved column size gives us a width that
    //       is slightly too small and therefore requires a horizontal scroll
    //       bar, hence we add 15% to it (those extra 15% seem to be enough to
    //       account even for a big number of plugins which would then require a
    //       vertical scroll bar)

    mGui->treeView->expandAll();
    mGui->treeView->resizeColumnToContents(0);

    mGui->treeView->setMinimumWidth(int(1.15*mGui->treeView->columnWidth(0)));
    mGui->treeView->setMaximumWidth(mGui->treeView->minimumWidth());

    // Make, through the stacked widget, sure that the dialog has a minimum
    // width

    mGui->stackedWidget->setMinimumWidth(int(2.5*mGui->treeView->minimumWidth()));

    // Make sure that the dialog has a reasonable starting size

    mGui->layout->setSizeConstraint(QLayout::SetMinimumSize);

    // Connection to handle the change of preferences widget

    connect(mGui->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PreferencesDialog::updatePreferencesWidget);

    // Select our selected item (!!)

    mGui->treeView->setCurrentIndex(selectedPluginItem->index());

    mGui->stackedWidget->currentWidget()->setFocus();
}

//==============================================================================

PreferencesDialog::~PreferencesDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

QStringList PreferencesDialog::pluginNames() const
{
    // Return our plugin names

    return mPluginNames;
}

//==============================================================================

QStandardItem * PreferencesDialog::pluginCategoryItem(PluginInfo::Category pCategory)
{
    // Return the given category item, after having created it, if it didn't
    // already exist

    QStandardItem *res = mCategoryItems.value(pCategory);

    if (res == nullptr) {
        // No category item exists for the given category, so create one and add
        // it to our data model (and this in the right place)

        bool inserted = false;
        QStandardItem *rootItem = mModel->invisibleRootItem();
        QString categoryName = pluginCategoryName(pCategory);
        QString nonDiacriticCategoryName = nonDiacriticString(categoryName);

        res = new QStandardItem(categoryName);

        for (int i = 0, iMax = rootItem->rowCount(); i < iMax; ++i) {
            if (nonDiacriticCategoryName.compare(nonDiacriticString(rootItem->child(i)->text())) < 0) {
                inserted = true;

                mModel->invisibleRootItem()->insertRow(i, res);

                break;
            }
        }

        if (!inserted) {
            mModel->invisibleRootItem()->appendRow(res);
        }

        // Keep track of the relationship between our new item and its category

        mCategoryItems.insert(pCategory, res);
        mItemCategories.insert(res, pCategory);
    }

    return res;
}

//==============================================================================

void PreferencesDialog::treeViewCollapsed(const QModelIndex &pIndex)
{
    // We don't want plugin categories to be collapsed, so cancel all
    // collapsings

    mGui->treeView->expand(pIndex);
}

//==============================================================================

void PreferencesDialog::buttonBoxAccepted()
{
    // Save all of our general and plugins' preferences, if they have changed,
    // and keep track of the plugins' name

    mPluginNames = QStringList();

    for (auto preferencesWidget : mItemPreferencesWidgets) {
        if (preferencesWidget->preferencesChanged()) {
            preferencesWidget->savePreferences();

            if (mPreferencesWidgetPluginNames.contains(preferencesWidget)) {
                mPluginNames << mPreferencesWidgetPluginNames.value(preferencesWidget);
            }
        }
    }

    // Confirm that we accepted the changes

    done(QMessageBox::Ok);
}

//==============================================================================

void PreferencesDialog::updatePreferencesWidget(const QModelIndex &pNewIndex,
                                                const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex)

    // Check whether we are dealing with a plugin category

    QStandardItem *item = mModel->itemFromIndex(pNewIndex);
    bool isPluginCategory = std::find(mCategoryItems.begin(), mCategoryItems.end(), item) != mCategoryItems.end();

    mResetButton->setEnabled(!isPluginCategory);

    if (isPluginCategory) {
        // We are dealing with a plugin category, so retrieve and set its name
        // and description

        mPluginCategoryWidget->setCategory(item->text());
        mPluginCategoryWidget->setDescription(tr("%1.").arg(formatMessage(pluginCategoryDescription(mItemCategories.value(item)))));

        mGui->stackedWidget->setCurrentWidget(mPluginCategoryWidget);
    } else {
        // We are dealing with our general or a plugin's preferences, so show
        // them

        mGui->stackedWidget->setCurrentWidget(mItemPreferencesWidgets.value(item));
    }

    // Make sure that the current widget has no layout margin (so that not only
    // we don't waste space, but also developers don't have to worry about the
    // layout margin of their preferences widget, assuming it has a layout)

    QLayout *widgetLayout = mGui->stackedWidget->currentWidget()->layout();

    if (widgetLayout != nullptr) {
        widgetLayout->setContentsMargins(QMargins());
    }

    // Make sure that we are big enough to show our contents

    adjustWidgetSize(this);
}

//==============================================================================

void PreferencesDialog::reset()
{
    // Reset all of our general / current plugin's preferences

    static_cast<Preferences::PreferencesWidget *>(mGui->stackedWidget->currentWidget())->resetPreferences();
}

//==============================================================================

void PreferencesDialog::resetAll()
{
    // Reset all of our general and plugins' preferences

    for (auto preferencesWidget : mItemPreferencesWidgets) {
        preferencesWidget->resetPreferences();
    }
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
