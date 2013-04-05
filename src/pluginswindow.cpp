//==============================================================================
// Plugins window
//==============================================================================

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
    // (i.e. plugins which the user cannot decide whether to load) in which case
    // we paint them as if they were disabled

    QStandardItem *pluginItem = qobject_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex);

    QStyleOptionViewItemV4 option(pOption);

    initStyleOption(&option, pIndex);

    if (pluginItem->parent()) {
        // We are dealing with a plugin item, so check whether it should look
        // enabled

        if (!pluginItem->isCheckable())
            // No, it shouldn't...

            option.state &= ~QStyle::State_Enabled;
    } else {
        // We are dealing with a category item, so show it bold

        option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

PluginsWindow::PluginsWindow(PluginManager *pPluginManager, QWidget *pParent) :
    QDialog(pParent),
    CommonWidget(pParent),
    mGui(new Ui::PluginsWindow),
    mPluginManager(pPluginManager),
    mMainWindow(qobject_cast<MainWindow *>(pParent))
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

    // Populate the data model with our different categories of plugins
    // Note: we create all of them in one go (rather than when required), so
    //       that they are in the order we want them to be

    newPluginCategory(PluginInfo::Organisation, tr("Organisation"));
    newPluginCategory(PluginInfo::Editing, tr("Editing"));
    newPluginCategory(PluginInfo::Simulation, tr("Simulation"));
    newPluginCategory(PluginInfo::Analysis, tr("Analysis"));
    newPluginCategory(PluginInfo::Miscellaneous, tr("Miscellaneous"));
    newPluginCategory(PluginInfo::Api, tr("API"));
    newPluginCategory(PluginInfo::ThirdParty, tr("Third-party"));

    // Populate the data model with our different plugins

    foreach (Plugin *plugin, mPluginManager->plugins()) {
        // Create the item corresponding to the current plugin

        QStandardItem *pluginItem = new QStandardItem((plugin->status() == Plugin::Loaded)?
                                                          QIcon(":/oxygen/actions/dialog-ok-apply.png"):
                                                          QIcon(":/oxygen/actions/edit-delete.png"),
                                                      plugin->name());

        // Only manageable plugins and plugins that are of the right type are
        // checkable

        PluginInfo *pluginInfo = plugin->info();

        pluginItem->setCheckable(   pluginInfo->manageable()
                                 && (pluginInfo->type() != PluginInfo::Console));

        if (pluginItem->isCheckable()) {
            // Retrieve the loading state of the plugin

            pluginItem->setCheckState((Plugin::load(plugin->name()))?
                                          Qt::Checked:
                                          Qt::Unchecked);

            // We are dealing with a manageable plugin, so add it to our list of
            // manageable plugins

            mManageablePlugins << pluginItem;
        } else {
            // We are dealing with an unmanageable plugin, so add it to our list
            // of unmanageable plugins

            mUnmanageablePlugins << pluginItem;
        }

        // Add the plugin to the right category

        mPluginCategories.value(pluginInfo->category())->appendRow(pluginItem);
    }

    // Hide any category which doesn't have any plugin

    foreach (QStandardItem *categoryItem, mPluginCategories)
        if (!categoryItem->hasChildren())
            mGui->pluginsTreeView->setRowHidden(categoryItem->row(),
                                                mModel->invisibleRootItem()->index(),
                                                true);

    // Make sure that the loading state of all the plugins is right, including
    // that of the plugins which the user cannot manage

    updatePluginsLoadingState(0, true);

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

    connect(mGui->fieldFourValue, SIGNAL(linkActivated(const QString &)),
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

static const QString SettingsShowOnlySelectablePlugins = "ShowOnlySelectablePlugins";

//==============================================================================

void PluginsWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve whether to show selectable plugins

    mGui->selectablePluginsCheckBox->setChecked(pSettings->value(SettingsShowOnlySelectablePlugins,
                                                                 true).toBool());

    // Show/hide our unmanageable plugins

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

QString PluginsWindow::interfaceVersionAsString(const PluginInfo::InterfaceVersion &pInterfaceVersion) const
{
    // Return the given interface version as a string
    // Note: ideally, this function would be part of the CoreInterface class,
    //       but this class is used by all the plugins and because this method
    //       requires a translation, well... we can't have it there since a
    //       translation will otherwise be required for each plugin, so...
    //       another solution is to have it here...

    switch (pInterfaceVersion) {
    case PluginInfo::InterfaceVersion001:
        return tr("001");
    default:
        return tr("unknown");
    }
}

//==============================================================================

QString PluginsWindow::statusDescription(Plugin *pPlugin) const
{
    // Return the plugin's status' description, if any

    switch (pPlugin->status()) {
    case Plugin::NotFound:
        return tr("The %1 plugin could not be found").arg(pPlugin->name());
    case Plugin::InvalidInterfaceVersion:
        return tr("The version of the interface used by the plugin (%1) is not valid (%2 is expected)").arg(interfaceVersionAsString(pPlugin->info()->interfaceVersion()),
                                                                                                            interfaceVersionAsString(mPluginManager->interfaceVersion()));
    case Plugin::NotSuitable:
        return tr("The %1 plugin is not of the right type").arg(pPlugin->name());
    case Plugin::NotWanted:
        return tr("The %1 plugin is not wanted").arg(pPlugin->name());
    case Plugin::NotNeeded:
        return tr("The %1 plugin is not needed").arg(pPlugin->name());
    case Plugin::Loaded:
        return tr("The %1 plugin is loaded and fully functional").arg(pPlugin->name());
    case Plugin::NotLoaded:
        if (pPlugin->statusErrorsCount() == 1)
            return tr("The %1 plugin could not be loaded due to the following problem: %2").arg(pPlugin->name(), pPlugin->statusErrors());
        else
            return tr("The %1 plugin could not be loaded due to the following problems:\n%2").arg(pPlugin->name(), pPlugin->statusErrors());
    case Plugin::NotPlugin:
        return tr("The %1 library is not a plugin").arg(pPlugin->name());
    case Plugin::MissingOrInvalidDependencies:
        if (pPlugin->statusErrorsCount() == 1)
            return tr("The %1 plugin could not be loaded due to the %2 plugin being missing or invalid").arg(pPlugin->name(), pPlugin->statusErrors());
        else
            return tr("The %1 plugin could not be loaded due to missing or invalid plugins:\n%2").arg(pPlugin->name(), pPlugin->statusErrors());
    default:   // Plugin::UndefinedStatus
        return tr("The status of the %1 plugin status is undefined").arg(pPlugin->name());
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

    bool validItem = pNewIndex.isValid();
    bool pluginItem = false;

    if (validItem) {
        // Update the information view with the category's or plugin's information

        QString itemText = mModel->itemFromIndex(pNewIndex)->text();
        Plugin *plugin = mPluginManager->plugin(itemText);

        if (plugin) {
            // We are dealing with a plugin, so retrieve the plugin's information

            pluginItem = true;

            PluginInfo *pluginInfo = plugin->info();

            // The plugin's name

            mGui->fieldOneLabel->setText(tr("Plugin:"));
            mGui->fieldOneValue->setText(plugin->name());

            // The plugin's type

            mGui->fieldTwoLabel->setText(tr("Type:"));

            switch (pluginInfo->type()) {
            case PluginInfo::General:
                mGui->fieldTwoValue->setText(tr("General"));

                break;
            case PluginInfo::Console:
                mGui->fieldTwoValue->setText(tr("Console"));

                break;
            case PluginInfo::Gui:
                mGui->fieldTwoValue->setText(tr("GUI"));

                break;
            default:
                mGui->fieldTwoValue->setText(tr("Undefined"));
            }

            // The plugin's dependencies

            QStringList dependencies = pluginInfo->dependencies();

            mGui->fieldThreeLabel->setText(tr("Dependencies:"));

            if (dependencies.isEmpty())
                mGui->fieldThreeValue->setText("None");
            else
                mGui->fieldThreeValue->setText("- "+dependencies.join("\n- "));

            // The plugin's description

            QString description = pluginInfo->description(mMainWindow->locale());

            mGui->fieldFourLabel->setText(tr("Description:"));
            mGui->fieldFourValue->setText(description.isEmpty()?
                                              tr("None"):
                                              description);

            // The plugin's status

            mGui->fieldFiveLabel->setText(tr("Status:"));
            mGui->fieldFiveValue->setText(statusDescription(plugin));
        } else {
            // We are not dealing with a plugin, but a plugin category

            // The category's name

            mGui->fieldOneLabel->setText(tr("Category:"));
            mGui->fieldOneValue->setText(itemText);

            // The category's description

            mGui->fieldTwoLabel->setText(tr("Description:"));

            if (!itemText.compare(tr("Organisation")))
                mGui->fieldTwoValue->setText(tr("Organisation plugins are used to search, open, organise, etc. your files."));
            else if (!itemText.compare(tr("Editing")))
                mGui->fieldTwoValue->setText(tr("Editing plugins are used to edit part or all of your files using one of several possible views."));
            else if (!itemText.compare(tr("Simulation")))
                mGui->fieldTwoValue->setText(tr("Simulation plugins are used to simulate your files."));
            else if (!itemText.compare(tr("Analysis")))
                mGui->fieldTwoValue->setText(tr("Analysis plugins are used to analyse your data files."));
            else if (!itemText.compare(tr("Miscellaneous")))
                mGui->fieldTwoValue->setText(tr("Miscellaneous plugins are used for various purposes."));
            else if (!itemText.compare(tr("API")))
                mGui->fieldTwoValue->setText(tr("API plugins are used to provide access to external APIs."));
            else if (!itemText.compare(tr("Third-party")))
                mGui->fieldTwoValue->setText(tr("Third-party plugins are used to provide access to third-party libraries."));
        }
    }

    // Show/hide the different fields

    mGui->fieldOneLabel->setVisible(validItem);
    mGui->fieldOneValue->setVisible(validItem);

    mGui->fieldTwoLabel->setVisible(validItem);
    mGui->fieldTwoValue->setVisible(validItem);

    mGui->fieldThreeLabel->setVisible(validItem && pluginItem);
    mGui->fieldThreeValue->setVisible(validItem && pluginItem);

    mGui->fieldFourLabel->setVisible(validItem && pluginItem);
    mGui->fieldFourValue->setVisible(validItem && pluginItem);

    mGui->fieldFiveLabel->setVisible(validItem && pluginItem);
    mGui->fieldFiveValue->setVisible(validItem && pluginItem);
}

//==============================================================================

void PluginsWindow::updatePluginsLoadingState(QStandardItem *pChangedPluginItem,
                                              const bool &pInitializing)
{
    // Disable the connection that handles a change in a plugin's loading state
    // (otherwise what we are doing here is going to be completely uneffective)

    disconnect(mModel, SIGNAL(itemChanged(QStandardItem *)),
               this, SLOT(updatePluginsLoadingState(QStandardItem *)));

    // Prevent the list view from being updated, since we may end up changing
    // quite a bit of its visual contents

    mGui->pluginsTreeView->setUpdatesEnabled(false);

    // Check whether we came here as a result of checking a plugin and, if so,
    // then make sure that all of that plugin's dependencies (i.e. both direct
    // and indirect dependencies) are also checked

    if (   pChangedPluginItem
        && (pChangedPluginItem->checkState() == Qt::Checked))
        foreach (const QString &requiredPlugin,
                 mPluginManager->plugin(pChangedPluginItem->text())->info()->fullDependencies())
            foreach (QStandardItem *pluginItem, mManageablePlugins)
                if (!pluginItem->text().compare(requiredPlugin))
                    // We are dealing with one of the plugin's dependencies, so
                    // make sure it's checked

                    pluginItem->setCheckState(Qt::Checked);

    // At this stage, all the plugins which should be checked are checked, so
    // now we must update the manageable plugins that are currently checked to
    // make sure that they should still be checked indeed. This means going
    // through each of the plugins and keep them checked only if all of their
    // dependencies are checked. Note that it is fine to do it this way since
    // all we need is one plugin's dependency to be unchecked for the plugin
    // itself to also be unchecked, so...

    foreach (QStandardItem *pluginItem, mManageablePlugins)
        if (pluginItem->checkState() == Qt::Checked)
            foreach (const QString &requiredPlugin,
                     mPluginManager->plugin(pluginItem->text())->info()->fullDependencies())
                foreach (QStandardItem *otherPluginItem, mManageablePlugins)
                    if (!otherPluginItem->text().compare(requiredPlugin)) {
                        // We have found the plugin's dependency

                        if (otherPluginItem->checkState() == Qt::Unchecked)
                            // The plugin's dependency is unchecked which means
                            // that the plugin cannot be checked, so...

                            pluginItem->setCheckState(Qt::Unchecked);

                        break;
                    }

    // Finally, we need to see whether our unmanageable plugins should be
    // checked or unchecked

    foreach (QStandardItem *pluginItem, mUnmanageablePlugins) {
        // First, reset the loading state of the unamanageable plugin

        pluginItem->setCheckState(Qt::Unchecked);

        // Next, go through the checked manageable plugins' dependencies

        foreach (QStandardItem *otherPluginItem, mManageablePlugins)
            if (otherPluginItem->checkState() == Qt::Checked)
                // The manageable plugin is checked, so carry on...

                foreach (const QString &requiredPlugin,
                         mPluginManager->plugin(otherPluginItem->text())->info()->fullDependencies())
                    if (!requiredPlugin.compare(pluginItem->text())) {
                        // The manageable plugin does require the unamanageable
                        // plugin, so...

                        pluginItem->setCheckState(Qt::Checked);

                        break;
                    }
    }

    // Re-enable the updating of the list view

    mGui->pluginsTreeView->setUpdatesEnabled(true);

    // Check whether the OK and apply buttons should be enabled

    if (pInitializing)
        // We are initialising the plugins window, so retrieve the initial
        // loading state of the plugins

        foreach (QStandardItem *pluginItem,
                 mManageablePlugins+mUnmanageablePlugins)
            mInitialLoadingStates.insert(pluginItem->text(),
                                         pluginItem->checkState() == Qt::Checked);

    bool buttonsEnabled = false;

    foreach (QStandardItem *pluginItem, mManageablePlugins+mUnmanageablePlugins)
        if (   mInitialLoadingStates.value(pluginItem->text())
            != (pluginItem->checkState() == Qt::Checked)) {
            // The loading state of the plugin has changed, so...

            buttonsEnabled = true;

            break;
        }

    mGui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(buttonsEnabled);
    mGui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(buttonsEnabled);

    // Re-enable the connection that handles a change in a plugin's loading
    // state

    connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(updatePluginsLoadingState(QStandardItem *)));
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
    // Keep track of the loading state of the various manageable plugins

    foreach (QStandardItem *pluginItem, mManageablePlugins)
        Plugin::setLoad(pluginItem->text(),
                        pluginItem->checkState() == Qt::Checked);

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
                              tr("%1 must be restarted for your changes to be effective. Do you wish to proceed?").arg(qApp->applicationName()),
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

    mModel->invisibleRootItem()->appendRow(categoryItem);

    // Add the category item to our list of plugin categories

    mPluginCategories.insert(pCategory, categoryItem);
}

//==============================================================================

void PluginsWindow::on_selectablePluginsCheckBox_toggled(bool pChecked)
{
    // Show/hide our unmanageable plugins

    foreach (QStandardItem *pluginItem, mUnmanageablePlugins)
        mGui->pluginsTreeView->setRowHidden(pluginItem->row(),
                                            pluginItem->parent()->index(), pChecked);

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
