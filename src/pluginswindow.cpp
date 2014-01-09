/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Plugins window
//==============================================================================

#include "cliutils.h"
#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"

//==============================================================================

#include "ui_pluginswindow.h"

//==============================================================================

#include <QDesktopServices>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardItemModel>
#include <QUrl>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void PluginItemDelegate::paint(QPainter *pPainter,
                               const QStyleOptionViewItem &pOption,
                               const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for the items which are not selectable
    // in which case we paint them as if they were disabled

    QStandardItem *pluginItem = qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex);

    QStyleOptionViewItemV4 option(pOption);

    initStyleOption(&option, pIndex);

    // If the item is a category, then make it bold

    if (!pluginItem->parent())
        option.font.setBold(true);

    // If the item is not checkable, then render it as disabled

    if (!pluginItem->isCheckable())
        option.state &= ~QStyle::State_Enabled;

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

bool sortPlugins(Plugin *pPlugin1, Plugin *pPlugin2)
{
    // Determine which of the two plugins should be first based on their name
    // Note: the two comparisons are case insensitive, so that it's easier for
    //       people to search a plugin...

    return pPlugin1->name().compare(pPlugin2->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

PluginsWindow::PluginsWindow(PluginManager *pPluginManager,
                             MainWindow *pMainWindow) :
    QDialog(pMainWindow),
    CommonWidget(pMainWindow),
    mGui(new Ui::PluginsWindow),
    mMainWindow(pMainWindow),
    mPluginManager(pPluginManager),
    mSelectablePluginItems(QList<QStandardItem *>()),
    mUnselectablePluginItems(QList<QStandardItem *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Make sure that all the widgets in our details form layout can be resized
    // if necessary and if possible
    // Note: indeed, it's not the case on OS X since the field growth policy is
    //       set to FieldsStayAtSizeHint on that platform and also on Windows
    //       and Linux to make sure that, if anything, we get the same behaviour
    //       on all the platforms we support...

    mGui->detailsFormLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    // Update the note label

    mGui->noteLabel->setText(mGui->noteLabel->text().arg(qApp->applicationName()));

    // Set up the tree view widget with a delegate, so that we can select
    // plugins that are shown as 'disabled' (to reflect the fact that users
    // cannot decide whether they should be loaded)

    mModel = new QStandardItemModel(mGui->pluginsTreeView);
    mPluginItemDelegate = new PluginItemDelegate();

    mGui->pluginsTreeView->setModel(mModel);
    mGui->pluginsTreeView->setItemDelegate(mPluginItemDelegate);

    // Populate the data model with our different categories of plugins, making
    // sure that they are in alphabetical order, no matter the locale

    QMap<QString, PluginInfo::Category> mappedCategories = QMap<QString, PluginInfo::Category>();

    mappedCategories.insert(tr("Analysis"), PluginInfo::Analysis);
    mappedCategories.insert(tr("API"), PluginInfo::Api);
    mappedCategories.insert(tr("Editing"), PluginInfo::Editing);
    mappedCategories.insert(tr("Miscellaneous"), PluginInfo::Miscellaneous);
    mappedCategories.insert(tr("Organisation"), PluginInfo::Organisation);
    mappedCategories.insert(tr("Simulation"), PluginInfo::Simulation);
    mappedCategories.insert(tr("Solver"), PluginInfo::Solver);
    mappedCategories.insert(tr("Support"), PluginInfo::Support);
    mappedCategories.insert(tr("Third-party"), PluginInfo::ThirdParty);
    mappedCategories.insert(tr("Widget"), PluginInfo::Widget);

    QMap<QString, QString> diacriticCategories = QMap<QString, QString>();

    foreach (const QString &diacriticCategory, mappedCategories.keys())
        diacriticCategories.insert(Core::nonDiacriticString(diacriticCategory), diacriticCategory);

    QStringList nonDiacriticCategories = diacriticCategories.keys();

    nonDiacriticCategories.sort(Qt::CaseInsensitive);

    foreach (const QString &nonDiacriticCategory, nonDiacriticCategories) {
        QString diacriticCategory = diacriticCategories.value(nonDiacriticCategory);

        newPluginCategory(mappedCategories.value(diacriticCategory), diacriticCategory);
    }

    // Sort our different plugins by their name
    // Note: indeed, they are currently sorted based on their depedencies with
    //       one another while here it makes more sense to have them sorted by
    //       name...

    Plugins plugins = mPluginManager->plugins();

    qSort(plugins.begin(), plugins.end(), sortPlugins);

    // Populate the data model with our different plugins

    foreach (Plugin *plugin, plugins) {
        // Create the item corresponding to the current plugin

        QStandardItem *pluginItem = new QStandardItem((plugin->status() == Plugin::Loaded)?
                                                          QIcon(":/oxygen/actions/dialog-ok-apply.png"):
                                                          QIcon(":/oxygen/actions/edit-delete.png"),
                                                      plugin->name());

        // Only selectable plugins and plugins that are of the right type are
        // checkable

        PluginInfo *pluginInfo = plugin->info();

        if (pluginInfo) {
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

            // Add the plugin to the right category

            mPluginCategories.value(pluginInfo->category())->appendRow(pluginItem);
        } else {
            // We are not actually dealing with a plugin, so add it to the
            // Miscellaneous category

            mUnselectablePluginItems << pluginItem;

            mPluginCategories.value(PluginInfo::Miscellaneous)->appendRow(pluginItem);
        }
    }

    // Make a category checkable if it contains selectable plugins and hide it,
    // if it doesn't contain any plugin

    foreach (QStandardItem *categoryItem, mPluginCategories) {
        for (int i = 0, iMax = categoryItem->rowCount(); i < iMax; ++i)
            if (categoryItem->child(i)->isCheckable()) {
                categoryItem->setCheckable(true);

                break;
            }

        if (!categoryItem->hasChildren())
            mGui->pluginsTreeView->setRowHidden(categoryItem->row(),
                                                mModel->invisibleRootItem()->index(),
                                                true);
    }

    // Make sure that the loading state of all the plugins is right, including
    // that of the plugins which the user cannot manage

    updatePluginsSelectedState(0, true);

    // Expand the whole tree view widget and make sure that it only takes as
    // much width as necessary
    // Note: for some reasons (maybe because we have check boxes?), the
    //       retrieved column size gives us a width that is slightly too small
    //       and therefore requires a horizontal scroll bar, hence we add 15% to
    //       it (the extra 15% seems to be enough even to even account for a big
    //       number of plugins which would then require a vertical scroll bar)

    mGui->pluginsTreeView->expandAll();
    mGui->pluginsTreeView->resizeColumnToContents(0);

    mGui->pluginsTreeView->setMinimumWidth(1.15*mGui->pluginsTreeView->columnWidth(0));
    mGui->pluginsTreeView->setMaximumWidth(mGui->pluginsTreeView->minimumWidth());

    // Make, through the note label, sure that the window has a minimum width

    mGui->noteLabel->setMinimumWidth(2.5*mGui->pluginsTreeView->minimumWidth());

    // Make sure that the window has a reasonable starting size

    mGui->layout->setSizeConstraint(QLayout::SetMinimumSize);

    // Connection to handle a plugin's information

    connect(mGui->pluginsTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateInformation(const QModelIndex &, const QModelIndex &)));

    // Connection to handle the activation of a link in the description

    connect(mGui->fieldThreeValue, SIGNAL(linkActivated(const QString &)),
            this, SLOT(openLink(const QString &)));

    // Connection to handle the window's buttons

    connect(mGui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)),
            this, SLOT(apply()));

    // Select the first category item

    selectFirstVisibleCategory();
}

//==============================================================================

PluginsWindow::~PluginsWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PluginsWindow::selectFirstVisibleCategory()
{
    // Select the first visible category

    foreach (QStandardItem *categoryItem, mPluginCategories)
        if (!mGui->pluginsTreeView->isRowHidden(categoryItem->row(),
                                                mModel->invisibleRootItem()->index())) {
            // We have found the first visible category, so...

            mGui->pluginsTreeView->setCurrentIndex(categoryItem->index());

            // We are done, so...

            return;
        }

    // No visible category could be found, so...

    mGui->pluginsTreeView->setCurrentIndex(QModelIndex());
}

//==============================================================================

static const auto SettingsShowOnlySelectablePlugins = QStringLiteral("ShowOnlySelectablePlugins");

//==============================================================================

void PluginsWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve whether to show selectable plugins

    mGui->selectablePluginsCheckBox->setChecked(pSettings->value(SettingsShowOnlySelectablePlugins,
                                                                 true).toBool());

    // Show/hide our unselectable plugins

    on_selectablePluginsCheckBox_toggled(mGui->selectablePluginsCheckBox->isChecked());
}

//==============================================================================

void PluginsWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of whether to show selectable plugins

    pSettings->setValue(SettingsShowOnlySelectablePlugins,
                        mGui->selectablePluginsCheckBox->isChecked());
}

//==============================================================================

QString PluginsWindow::statusDescription(Plugin *pPlugin) const
{
    // Return the plugin's status' description, if any

    switch (pPlugin->status()) {
    case Plugin::NotWanted:
        return tr("the plugin is not wanted.");
    case Plugin::NotNeeded:
        return tr("the plugin is not needed.");
    case Plugin::Loaded:
        return tr("the plugin is loaded and fully functional.");
    case Plugin::NotLoaded:
        if (pPlugin->statusErrorsCount() == 1)
            return tr("the plugin could not be loaded due to the following problem: %1.").arg(pPlugin->statusErrors());
        else
            return tr("the plugin could not be loaded due to the following problems:\n%1").arg(pPlugin->statusErrors());
    case Plugin::NotPlugin:
        return tr("this is not a plugin.");
    case Plugin::MissingOrInvalidDependencies:
        if (pPlugin->statusErrorsCount() == 1)
            return tr("the plugin could not be loaded due to the %1 plugin being missing or invalid.").arg(pPlugin->statusErrors());
        else
            return tr("the plugin could not be loaded due to missing or invalid plugins:\n%1").arg(pPlugin->statusErrors());
    default:   // Plugin::UndefinedStatus
        return tr("the status of the plugin is undefined.");
    }
}

//==============================================================================

void PluginsWindow::updateInformation(const QModelIndex &pNewIndex,
                                      const QModelIndex &pOldIndex) const
{
    Q_UNUSED(pOldIndex);

    // Make sure that we have a valid index
    // Note: it may happen (e.g. there are only non-selectable plugins and we
    //       only want to see selectable plugins) that no categories/plugins are
    //       shown, so...

    bool atLeastOneItem = pNewIndex.isValid();
    bool pluginItem = false;
    bool validItem = true;

    // Update the information view with the category's or plugin's information

    QString itemText = atLeastOneItem?mModel->itemFromIndex(pNewIndex)->text():QString();
    Plugin *plugin = mPluginManager->plugin(itemText);

    if (plugin) {
        // We are dealing with a plugin, so retrieve its information

        pluginItem = true;

        PluginInfo *pluginInfo = plugin->info();

        if (pluginInfo) {
            // The plugin's name

            mGui->fieldOneLabel->setText(tr("Plugin:"));
            mGui->fieldOneValue->setText(plugin->name());

            // The plugin's dependencies

            QStringList dependencies = pluginInfo->dependencies();

            mGui->fieldTwoLabel->setText(tr("Dependencies:"));

            if (dependencies.isEmpty()) {
                mGui->fieldTwoValue->setText(tr("none"));
            } else {
                dependencies.sort();

                mGui->fieldTwoValue->setText("- "+dependencies.join("\n- "));
            }

            // The plugin's description

            QString description = pluginInfo->description(mMainWindow->locale());

            mGui->fieldThreeLabel->setText(tr("Description:"));
            mGui->fieldThreeValue->setText(description.isEmpty()?tr("none"):description);

            // The plugin's status

            mGui->fieldFourLabel->setText(tr("Status:"));
            mGui->fieldFourValue->setText(statusDescription(plugin));
        } else {
            // We are not dealing with a plugin

            validItem = false;

            // The plugin's status

            mGui->fieldOneLabel->setText(tr("Status:"));
            mGui->fieldOneValue->setText(statusDescription(plugin));

            // The plugin's error

            mGui->fieldTwoLabel->setText(tr("Error:"));
            mGui->fieldTwoValue->setText(plugin->errorMessage());
        }
    } else if (atLeastOneItem) {
        // We are not dealing with a plugin, but a plugin category

        validItem = true;

        // The category's name

        mGui->fieldOneLabel->setText(tr("Category:"));
        mGui->fieldOneValue->setText(itemText);

        // The category's description

        mGui->fieldTwoLabel->setText(tr("Description:"));

        if (!itemText.compare(tr("Analysis")))
            mGui->fieldTwoValue->setText(tr("Plugins to analyse files."));
        else if (!itemText.compare(tr("API")))
            mGui->fieldTwoValue->setText(tr("Plugins to access various APIs."));
        else if (!itemText.compare(tr("Editing")))
            mGui->fieldTwoValue->setText(tr("Plugins to edit files."));
        else if (!itemText.compare(tr("Miscellaneous")))
            mGui->fieldTwoValue->setText(tr("Plugins for various things."));
        else if (!itemText.compare(tr("Organisation")))
            mGui->fieldTwoValue->setText(tr("Plugins to organise files."));
        else if (!itemText.compare(tr("Simulation")))
            mGui->fieldTwoValue->setText(tr("Plugins to simulate files."));
        else if (!itemText.compare(tr("Solver")))
            mGui->fieldTwoValue->setText(tr("Plugins to access various solvers."));
        else if (!itemText.compare(tr("Support")))
            mGui->fieldTwoValue->setText(tr("Plugins to support various third-party libraries and APIs."));
        else if (!itemText.compare(tr("Third-party")))
            mGui->fieldTwoValue->setText(tr("Plugins to access various third-party libraries."));
        else if (!itemText.compare(tr("Widget")))
            mGui->fieldTwoValue->setText(tr("Plugins to access various ad hoc widgets."));
    }

    // Show/hide the different fields

    mGui->fieldOneLabel->setVisible(atLeastOneItem);
    mGui->fieldOneValue->setVisible(atLeastOneItem);

    mGui->fieldTwoLabel->setVisible(atLeastOneItem);
    mGui->fieldTwoValue->setVisible(atLeastOneItem);

    mGui->fieldThreeLabel->setVisible(atLeastOneItem && validItem && pluginItem);
    mGui->fieldThreeValue->setVisible(atLeastOneItem && validItem && pluginItem);

    mGui->fieldFourLabel->setVisible(atLeastOneItem && validItem && pluginItem);
    mGui->fieldFourValue->setVisible(atLeastOneItem && validItem && pluginItem);
}

//==============================================================================

void PluginsWindow::updatePluginsSelectedState(QStandardItem *pItem,
                                               const bool &pInitializing)
{
    // Disable the connection that handles a change in a plugin's loading state
    // (otherwise what we are doing here is going to be completely uneffective)

    disconnect(mModel, SIGNAL(itemChanged(QStandardItem *)),
               this, SLOT(updatePluginsSelectedState(QStandardItem *)));

    // Prevent the list view from being updated, since we may end up changing
    // quite a bit of its visual contents

    mGui->pluginsTreeView->setUpdatesEnabled(false);

    // In case we un/select a category, then go through its selectable plugins
    // and un/select them accordingly

    if (pItem && !pItem->parent())
        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            QStandardItem *pluginItem = pItem->child(i);

            if (pluginItem->isCheckable())
                pluginItem->setCheckState(pItem->checkState());
        }

    // Update the selected state of all our unselectable plugins

    foreach (QStandardItem *unselectablePluginItem, mUnselectablePluginItems) {
        // First, reset the selected state of our unaselectable plugin

        unselectablePluginItem->setCheckState(Qt::Unchecked);

        // Next, go through our selectable plugins and check whether one of them
        // needs our unselectable plugin

        foreach (QStandardItem *selectablePluginItem, mSelectablePluginItems)
            if (selectablePluginItem->checkState() == Qt::Checked) {
                // The selectable plugin is selected, so go through its plugin
                // dependencies and check whether one of them is our
                // unselectable plugin

                foreach (const QString &requiredPlugin,
                         mPluginManager->plugin(selectablePluginItem->text())->info()->fullDependencies())
                    if (!requiredPlugin.compare(unselectablePluginItem->text())) {
                        // The selectable plugin requires our unselectable
                        // plugin, so...

                        unselectablePluginItem->setCheckState(Qt::Checked);

                        break;
                    }

                if (unselectablePluginItem->checkState() == Qt::Checked)
                    break;
            }
    }

    // Update the selected state of all our categories which have at least one
    // selectable plugin

    foreach (QStandardItem *categoryItem, mPluginCategories) {
        int nbOfPlugins = categoryItem->rowCount();

        if (nbOfPlugins) {
            int nbOfSelectablePlugins = 0;
            int nbOfUnselectablePlugins = 0;
            int nbOfSelectedSelectablePlugins = 0;

            for (int i = 0; i < nbOfPlugins; ++i) {
                QStandardItem *pluginItem = categoryItem->child(i);

                if (pluginItem->isCheckable()) {
                    ++nbOfSelectablePlugins;

                    if (pluginItem->checkState() == Qt::Checked)
                        ++nbOfSelectedSelectablePlugins;
                } else {
                    ++nbOfUnselectablePlugins;
                }
            }

            if (nbOfPlugins != nbOfUnselectablePlugins)
                categoryItem->setCheckState(nbOfSelectedSelectablePlugins?
                                                (nbOfSelectedSelectablePlugins == nbOfSelectablePlugins)?
                                                    Qt::Checked:
                                                    Qt::PartiallyChecked:
                                                Qt::Unchecked);
        }
    }

    // Re-enable the updating of the list view

    mGui->pluginsTreeView->setUpdatesEnabled(true);

    // Check whether the OK and apply buttons should be enabled

    if (pInitializing)
        // We are initialising the plugins window, so retrieve the initial
        // loading state of the plugins

        foreach (QStandardItem *plugin,
                 mSelectablePluginItems+mUnselectablePluginItems)
            mInitialLoadingStates.insert(plugin->text(),
                                         plugin->checkState() == Qt::Checked);

    bool buttonsEnabled = false;

    foreach (QStandardItem *plugin, mSelectablePluginItems+mUnselectablePluginItems)
        if (   mInitialLoadingStates.value(plugin->text())
            != (plugin->checkState() == Qt::Checked)) {
            // The loading state of the plugin has changed, so...

            buttonsEnabled = true;

            break;
        }

    mGui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(buttonsEnabled);
    mGui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(buttonsEnabled);

    // Re-enable the connection that handles a change in a plugin's loading
    // state

    connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(updatePluginsSelectedState(QStandardItem *)));
}

//==============================================================================

void PluginsWindow::openLink(const QString &pLink) const
{
    // Open the link in the user's browser

    QDesktopServices::openUrl(QUrl(pLink));
}

//==============================================================================

void PluginsWindow::on_buttonBox_accepted()
{
    // Keep track of the loading state of the various selectable plugins

    foreach (QStandardItem *selectablePluginItem, mSelectablePluginItems)
        Plugin::setLoad(selectablePluginItem->text(),
                        selectablePluginItem->checkState() == Qt::Checked);

    // Confirm that we accepted the changes

    accept();
}

//==============================================================================

void PluginsWindow::on_buttonBox_rejected()
{
    // Simple cancel whatever was done here

    reject();
}

//==============================================================================

void PluginsWindow::apply()
{
    if (QMessageBox::question(this, qApp->applicationName(),
                              tr("<strong>%1</strong> must be restarted for your changes to take effect. Do you wish to proceed?").arg(qApp->applicationName()),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // Do what is done when clicking on the OK button

        on_buttonBox_accepted();

        // Exit OpenCOR with the request to restart it after having saved its
        // settings

        mMainWindow->restart(true);
    }
}

//==============================================================================

void PluginsWindow::newPluginCategory(const PluginInfo::Category &pCategory,
                                      const QString &pName)
{
    // Create and add a category item to our data model

    QStandardItem *categoryItem = new QStandardItem(pName);

    categoryItem->setTristate(true);

    mModel->invisibleRootItem()->appendRow(categoryItem);

    // Add the category item to our list of plugin categories

    mPluginCategories.insert(pCategory, categoryItem);
}

//==============================================================================

void PluginsWindow::on_selectablePluginsCheckBox_toggled(bool pChecked)
{
    // Show/hide our unselectable plugins

    foreach (QStandardItem *unselectablePluginItem, mUnselectablePluginItems)
        mGui->pluginsTreeView->setRowHidden(unselectablePluginItem->row(),
                                            unselectablePluginItem->parent()->index(), pChecked);

    // Show/hide our categories, based on whether they contain visible plugins

    foreach (QStandardItem *categoryItem, mPluginCategories)
        if (categoryItem->hasChildren()) {
            // The category contains plugins, but the question is whether they
            // are visible
            // Note: if the category doesn't contain any plugin, then we don't
            //       need to worry about it since it is already hidden

            bool hideCategory = true;

            for (int i = 0, iMax = categoryItem->rowCount(); i < iMax; ++i)
                if (!mGui->pluginsTreeView->isRowHidden(categoryItem->child(i)->row(),
                                                        categoryItem->index())) {
                    // There is at least one plugin which is visible, so...

                    hideCategory = false;

                    break;
                }

            mGui->pluginsTreeView->setRowHidden(categoryItem->row(),
                                                mModel->invisibleRootItem()->index(),
                                                hideCategory);
        }

    // Select the first category item

    selectFirstVisibleCategory();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
