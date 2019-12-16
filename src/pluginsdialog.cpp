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
// Plugins dialog
//==============================================================================

#include "cliutils.h"
#include "guiutils.h"
#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginsdialog.h"

//==============================================================================

#include "ui_pluginsdialog.h"

//==============================================================================

#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QModelIndex>
#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>
#include <QUrl>

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginItemDelegate::PluginItemDelegate(QObject *pParent) :
    StyledItemDelegate(pParent)
{
}

//==============================================================================

void PluginItemDelegate::paint(QPainter *pPainter,
                               const QStyleOptionViewItem &pOption,
                               const QModelIndex &pIndex) const
{
    // Paint the item as normal, if it is selectable, as disabled, if it isn't
    // selectable, or bold, if it is a category

    QStandardItem *pluginItem = qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex);

    QStyleOptionViewItem option(pOption);

    initStyleOption(&option, pIndex);

    if (pluginItem->parent() != nullptr) {
        if (!pluginItem->isCheckable()) {
            option.state &= ~QStyle::State_Enabled;
        }
    } else {
        option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

static const char *SettingsShowOnlySelectablePlugins = "ShowOnlySelectablePlugins";

//==============================================================================

PluginsDialog::PluginsDialog(PluginManager *pPluginManager,
                             QWidget *pParent) :
    Dialog(pParent),
    mGui(new Ui::PluginsDialog),
    mPluginManager(pPluginManager)
{
    // Customise our settings

    mSettings.beginGroup("PluginsDialog");

    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->treeView, &QTreeView::collapsed,
            this, &PluginsDialog::treeViewCollapsed);

    connect(mGui->selectablePluginsCheckBox, &QCheckBox::toggled,
            this, &PluginsDialog::selectablePluginsCheckBoxToggled);

    connect(mGui->buttonBox, &QDialogButtonBox::accepted,
            this, &PluginsDialog::buttonBoxAccepted);
    connect(mGui->buttonBox, &QDialogButtonBox::rejected,
            this, &PluginsDialog::reject);

    // Make sure that all the widgets in our form layout can be resized, if
    // necessary and if possible
    // Note: indeed, it's not the case on macOS since the field growth policy is
    //       set to FieldsStayAtSizeHint on that platform and also on Windows
    //       and Linux to make sure that, if anything, we get the same behaviour
    //       on all the platforms we support...

    mGui->formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    // Update the note label

    mGui->noteLabel->setText(mGui->noteLabel->text().arg(qAppName()));

    // Make the value of all our fields selectable

    mGui->fieldOneValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::LinksAccessibleByMouse);
    mGui->fieldTwoValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::LinksAccessibleByMouse);
    mGui->fieldThreeValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::LinksAccessibleByMouse);
    mGui->fieldFourValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::LinksAccessibleByMouse);

    // Set up the tree view widget with a delegate, so that we can select
    // plugins that are shown as 'disabled' (to reflect the fact that users
    // cannot decide whether they should be loaded)

    mModel = new QStandardItemModel(this);

#ifdef Q_OS_MAC
    mGui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    mGui->treeView->setModel(mModel);
    mGui->treeView->setItemDelegate(new PluginItemDelegate(this));

    // Populate the data model with our different plugins

    static const QIcon LoadedIcon    = QIcon(":/oxygen/actions/dialog-ok-apply.png");
    static const QIcon NotLoadedIcon = QIcon(":/oxygen/actions/edit-delete.png");

    for (auto plugin : mPluginManager->sortedPlugins()) {
        // Create the item corresponding to the current plugin

        auto pluginItem = new QStandardItem((plugin->status() == Plugin::Status::Loaded)?LoadedIcon:NotLoadedIcon,
                                            plugin->name());

        // Only selectable plugins and plugins that are of the right type are
        // checkable

        PluginInfo *pluginInfo = plugin->info();

        if (pluginInfo != nullptr) {
            pluginItem->setCheckable(pluginInfo->isSelectable());

            if (pluginItem->isCheckable()) {
                // Retrieve the loading state of the plugin

                pluginItem->setCheckState((Plugin::load(plugin->name()))?
                                              Qt::Checked:
                                              Qt::Unchecked);

                // We are dealing with a selectable plugin, so add it to our
                // list of selectable plugins

                mSelectablePluginItems << pluginItem;
            } else {
                // We are dealing with an unselectable plugin, so add it to our
                // list of unselectable plugins

                mUnselectablePluginItems << pluginItem;
            }

            // Add the plugin to its corresponding category

            pluginCategoryItem(pluginInfo->category())->appendRow(pluginItem);
        } else {
            // We are either dealing with a library that is not a plugin or with
            // a plugin that is too old, so add it to the Invalid category

            pluginCategoryItem(PluginInfo::Category::Invalid)->appendRow(pluginItem);
        }
    }

    // Make a category checkable if it contains selectable plugins

    for (auto categoryItem : mCategoryItems) {
        for (int i = 0, iMax = categoryItem->rowCount(); i < iMax; ++i) {
            if (categoryItem->child(i)->isCheckable()) {
                categoryItem->setCheckable(true);

                break;
            }
        }
    }

    // Make sure that the loading state of all the plugins is right, including
    // that of the plugins which the user cannot manage

    updatePluginsSelectedState(nullptr, true);

    // Expand the whole tree view widget and make sure that it only takes as
    // much width as necessary
    // Note: for some reasons, the retrieved column size gives us a width that
    //       is slightly too small and therefore requires a horizontal scroll
    //       bar, hence we add 15% to it (those extra 15% seems to be enough
    //       to account even for a big number of plugins which would then
    //       require a vertical scroll bar)

    mGui->treeView->expandAll();
    mGui->treeView->resizeColumnToContents(0);

    mGui->treeView->setMinimumWidth(int(1.15*mGui->treeView->columnWidth(0)));
    mGui->treeView->setMaximumWidth(mGui->treeView->minimumWidth());

    // Make, through the note label, sure that the dialog has a minimum width

    mGui->noteLabel->setMinimumWidth(int(2.5*mGui->treeView->minimumWidth()));

    // Make sure that the dialog has a reasonable starting size

    mGui->layout->setSizeConstraint(QLayout::SetMinimumSize);

    // Connection to handle a plugin's information

    connect(mGui->treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PluginsDialog::updateInformation);

    // Connection to handle the activation of a link in the description

    connect(mGui->fieldThreeValue, &QLabel::linkActivated,
            this, &PluginsDialog::openLink);

    // Connection to handle the dialog's buttons

    connect(mGui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            this, &PluginsDialog::apply);

    // Retrieve whether to show selectable plugins

    mGui->selectablePluginsCheckBox->setChecked(mSettings.value(SettingsShowOnlySelectablePlugins, true).toBool());

    // Show/hide our unselectable plugins

    selectablePluginsCheckBoxToggled(mGui->selectablePluginsCheckBox->isChecked());
}

//==============================================================================

PluginsDialog::~PluginsDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

QString PluginsDialog::statusDescription(Plugin *pPlugin) const
{
    // Return the plugin's status' description, if any

    switch (pPlugin->status()) {
    case Plugin::Status::NotWanted:
        return tr("the plugin is not wanted.");
    case Plugin::Status::NotNeeded:
        return tr("the plugin is not needed.");
    case Plugin::Status::Loaded:
        return tr("the plugin is loaded and fully functional.");
    case Plugin::Status::NotLoaded:
        return tr("the plugin could not be loaded (%1).").arg(formatMessage(pPlugin->statusErrors()));
    case Plugin::Status::NotPlugin:
        return tr("this is not a plugin (%1).").arg(formatMessage(pPlugin->statusErrors()));
    case Plugin::Status::OldPlugin:
        return tr("the plugin could not be loaded (one or several of the interfaces it supports are too old).");
    case Plugin::Status::NotCorePlugin:
        return tr("the plugin claims to be the core plugin, but it is not.");
    case Plugin::Status::InvalidCorePlugin:
        return tr("the plugin claims to be the core plugin, but it does not support the core interface.");
    case Plugin::Status::NeededSelectablePlugin:
        return tr("the plugin claims to be selectable, but it is needed.");
    case Plugin::Status::NotCliPluginNoCliSupport:
        return tr("the plugin supports the CLI interface, but it does not claim to be CLI-capable.");
    case Plugin::Status::NotCliPluginNoCliInterface:
        return tr("the plugin claims to be CLI-capable, but it does not support the CLI interface.");
    case Plugin::Status::MissingOrInvalidDependencies:
        if (pPlugin->statusErrorsCount() == 1) {
            return tr("the plugin could not be loaded due to the %1 plugin being missing or invalid.").arg(pPlugin->statusErrors());
        } else {
            return tr("the plugin could not be loaded due to missing or invalid plugins:\n%1").arg(pPlugin->statusErrors());
        }
    }

    return "???";
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

void PluginsDialog::updateInformation(const QModelIndex &pNewIndex,
                                      const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex)

    // Make sure that we have a valid index
    // Note: it may happen (e.g. there are only non-selectable plugins and we
    //       only want to see selectable plugins) that no categories/plugins are
    //       shown...

    bool pluginItem = false;
    bool libraryItem = false;
    bool oldPluginItem = false;
    bool categoryItem = false;

    // Update the information view with the category's or plugin's information

    QStandardItem *item = pNewIndex.isValid()?mModel->itemFromIndex(pNewIndex):nullptr;
    QString itemText = (item != nullptr)?item->text():QString();
    Plugin *plugin = ((item != nullptr) && (item->parent() != nullptr))?mPluginManager->plugin(itemText):nullptr;

    if (plugin != nullptr) {
        // We are supposedly dealing with a plugin, so retrieve its information,
        // if possible

        PluginInfo *pluginInfo = plugin->info();

        if (pluginInfo != nullptr) {
            // We are indeed dealing with a plugin

            pluginItem = true;

            // The plugin's name

            mGui->fieldOneLabel->setText(tr("Plugin:"));
            mGui->fieldOneValue->setText(plugin->name());

            // The plugin's dependencies

            QStringList dependencies = pluginInfo->dependencies();

            mGui->fieldTwoLabel->setText(tr("Dependencies:"));

            if (dependencies.isEmpty()) {
                mGui->fieldTwoValue->setText(tr("none"));
            } else {
                dependencies.sort(Qt::CaseInsensitive);

                mGui->fieldTwoValue->setText("- "+dependencies.join("\n- "));
            }

            // The plugin's description

            QString description = pluginInfo->description(OpenCOR::locale());

            mGui->fieldThreeLabel->setText(tr("Description:"));
            mGui->fieldThreeValue->setText(description.isEmpty()?tr("none"):description);

            // The plugin's status

            mGui->fieldFourLabel->setText(tr("Status:"));
            mGui->fieldFourValue->setText(statusDescription(plugin));
        } else if (plugin->status() == Plugin::Status::NotPlugin) {
            // We are not dealing with a plugin, but a simple library

            libraryItem = true;

            // The plugin's status

            mGui->fieldOneLabel->setText(tr("Status:"));
            mGui->fieldOneValue->setText(statusDescription(plugin));
        } else {
            // We are dealing with a plugin that is too old

            oldPluginItem = true;

            // The plugin's name

            mGui->fieldOneLabel->setText(tr("Plugin:"));
            mGui->fieldOneValue->setText(plugin->name());

            // The plugin's status

            mGui->fieldTwoLabel->setText(tr("Status:"));
            mGui->fieldTwoValue->setText(statusDescription(plugin));
        }
    } else if (pNewIndex.isValid()) {
        // We are dealing with a category

        categoryItem = true;

        // The category's name

        mGui->fieldOneLabel->setText(tr("Category:"));
        mGui->fieldOneValue->setText(itemText);

        // The category's description

        mGui->fieldTwoLabel->setText(tr("Description:"));
        mGui->fieldTwoValue->setText(tr("%1.").arg(formatMessage(pluginCategoryDescription(mItemCategories.value(item)))));
    }

    // Show/hide the different fields

    mGui->fieldOneLabel->setVisible(pluginItem || libraryItem || oldPluginItem || categoryItem);
    mGui->fieldOneValue->setVisible(pluginItem || libraryItem || oldPluginItem || categoryItem);

    mGui->fieldTwoLabel->setVisible(pluginItem || oldPluginItem || categoryItem);
    mGui->fieldTwoValue->setVisible(pluginItem || oldPluginItem || categoryItem);

    mGui->fieldThreeLabel->setVisible(pluginItem);
    mGui->fieldThreeValue->setVisible(pluginItem);

    mGui->fieldFourLabel->setVisible(pluginItem);
    mGui->fieldFourValue->setVisible(pluginItem);

    // Make sure that we are big enough to show our contents

    adjustWidgetSize(this);
}

//==============================================================================

void PluginsDialog::updatePluginsSelectedState(QStandardItem *pItem,
                                               bool pInitializing)
{
    // Disable the connection that handles a change in a plugin's loading state
    // (otherwise what we are doing here is going to be completely uneffective)

    disconnect(mModel, &QStandardItemModel::itemChanged,
               this, QOverload<QStandardItem *>::of(&PluginsDialog::updatePluginsSelectedState));

    // In case we un/select a category, then go through its selectable plugins
    // and un/select them accordingly

    if ((pItem != nullptr) && (pItem->parent() == nullptr)) {
        Qt::CheckState newCheckState = (pItem->checkState() == Qt::Unchecked)?Qt::Unchecked:Qt::Checked;

        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            QStandardItem *pluginItem = pItem->child(i);

            if (mSelectablePluginItems.contains(pluginItem)) {
                pluginItem->setCheckState(newCheckState);
            }
        }
    }

    // Update the selected state of all our unselectable plugins

    for (auto unselectablePluginItem : mUnselectablePluginItems) {
        // First, reset the selected state of our unselectable plugin

        unselectablePluginItem->setCheckState(Qt::Unchecked);

        // Next, go through our selectable plugins and check whether one of them
        // needs our unselectable plugin

        for (auto selectablePluginItem : mSelectablePluginItems) {
            if (   (selectablePluginItem->checkState() == Qt::Checked)
                && (mPluginManager->plugin(selectablePluginItem->text())->info()->fullDependencies().contains(unselectablePluginItem->text()))) {
                unselectablePluginItem->setCheckState(Qt::Checked);

                break;
            }
        }
    }

    // Update the selected state of all our categories which have at least one
    // selectable plugin

    for (auto categoryItem : mCategoryItems) {
        int nbOfPlugins = categoryItem->rowCount();

        if (nbOfPlugins != 0) {
            int nbOfSelectablePlugins = 0;
            int nbOfUnselectablePlugins = 0;
            int nbOfSelectedSelectablePlugins = 0;

            for (int i = 0; i < nbOfPlugins; ++i) {
                QStandardItem *pluginItem = categoryItem->child(i);

                if (pluginItem->isCheckable()) {
                    ++nbOfSelectablePlugins;

                    if (pluginItem->checkState() == Qt::Checked) {
                        ++nbOfSelectedSelectablePlugins;
                    }
                } else {
                    ++nbOfUnselectablePlugins;
                }
            }

            if (nbOfPlugins != nbOfUnselectablePlugins) {
                categoryItem->setCheckState((nbOfSelectedSelectablePlugins != 0)?
                                                (nbOfSelectedSelectablePlugins == nbOfSelectablePlugins)?
                                                    Qt::Checked:
                                                    Qt::PartiallyChecked:
                                                Qt::Unchecked);
            }
        }
    }

    // Check whether the OK and apply buttons should be enabled

    if (pInitializing) {
        // We are initialising the plugins dialog, so retrieve the initial
        // loading state of the plugins

        for (auto plugin : mSelectablePluginItems+mUnselectablePluginItems) {
            mInitialLoadingStates.insert(plugin->text(),
                                         plugin->checkState() == Qt::Checked);
        }
    }

    bool buttonsEnabled = false;

    for (auto plugin : mSelectablePluginItems+mUnselectablePluginItems) {
        if (mInitialLoadingStates.value(plugin->text()) != (plugin->checkState() == Qt::Checked)) {
            buttonsEnabled = true;

            break;
        }
    }

    mGui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(buttonsEnabled);
    mGui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(buttonsEnabled);

    // Re-enable the connection that handles a change in a plugin's loading
    // state

    connect(mModel, &QStandardItemModel::itemChanged,
            this, QOverload<QStandardItem *>::of(&PluginsDialog::updatePluginsSelectedState));
}

//==============================================================================

void PluginsDialog::updatePluginsSelectedState(QStandardItem *pItem)
{
    // Update our plugins selected state

    updatePluginsSelectedState(pItem, false);
}

//==============================================================================

void PluginsDialog::openLink(const QString &pLink) const
{
    // Open the link in the user's browser

    QDesktopServices::openUrl(pLink);
}

//==============================================================================

void PluginsDialog::buttonBoxAccepted()
{
    // Keep track of the loading state of the various selectable plugins

    for (auto selectablePluginItem : mSelectablePluginItems) {
        Plugin::setLoad(selectablePluginItem->text(),
                        selectablePluginItem->checkState() == Qt::Checked);
    }

    // Confirm that we accepted the changes

    accept();
}

//==============================================================================

void PluginsDialog::apply()
{
    if (questionMessageBox(qAppName(),
                           tr("<strong>%1</strong> must be restarted for your changes to take effect. Do you want to proceed?").arg(qAppName())) == QMessageBox::Yes ) {
        // Do what is done when clicking on the OK button

        buttonBoxAccepted();

        // Let people know that we are done and that we want the settings to be
        // applied

        done(QMessageBox::Apply);
    }
}

//==============================================================================

QStandardItem * PluginsDialog::pluginCategoryItem(PluginInfo::Category pCategory)
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
            QStandardItem *categoryItem = rootItem->child(i);

            if (nonDiacriticCategoryName.compare(nonDiacriticString(categoryItem->text())) < 0) {
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

void PluginsDialog::treeViewCollapsed(const QModelIndex &pIndex)
{
    // We don't want plugin categories to be collapse, so cancel all collapsings

    mGui->treeView->expand(pIndex);
}

//==============================================================================

void PluginsDialog::selectablePluginsCheckBoxToggled(bool pChecked)
{
    // Keep track of whether to show selectable plugins

    mSettings.setValue(SettingsShowOnlySelectablePlugins,
                       mGui->selectablePluginsCheckBox->isChecked());

    // Show/hide our unselectable plugins

    for (auto unselectablePluginItem : mUnselectablePluginItems) {
        mGui->treeView->setRowHidden(unselectablePluginItem->row(),
                                     unselectablePluginItem->parent()->index(), pChecked);
    }

    // Show/hide our categories, based on whether they contain visible plugins

    for (auto categoryItem : mCategoryItems) {
        if (categoryItem->hasChildren()) {
            // The category contains plugins, but the question is whether they
            // are visible

            bool hideCategory = true;

            for (int i = 0, iMax = categoryItem->rowCount(); i < iMax; ++i) {
                if (!mGui->treeView->isRowHidden(categoryItem->child(i)->row(),
                                                 categoryItem->index())) {
                    hideCategory = false;

                    break;
                }
            }

            mGui->treeView->setRowHidden(categoryItem->row(),
                                         mModel->invisibleRootItem()->index(),
                                         hideCategory);
        }
    }

    // Select the first visible category

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        if (!mGui->treeView->isRowHidden(i, mModel->invisibleRootItem()->index())) {
            mGui->treeView->setCurrentIndex(mModel->invisibleRootItem()->child(i)->index());

            return;
        }
    }

    mGui->treeView->setCurrentIndex(QModelIndex());
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
