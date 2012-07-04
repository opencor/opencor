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
#include <QStandardItemModel>
#include <QUrl>

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginDelegate::PluginDelegate(QStandardItemModel *pDataModel) :
    QStyledItemDelegate(),
    mDataModel(pDataModel)
{
}

//==============================================================================

void PluginDelegate::paint(QPainter *pPainter,
                           const QStyleOptionViewItem &pOption,
                           const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for the items which are not checkable
    // (i.e. plugins which the user cannot decide whether to load) in which case
    // we paint them as if they were disabled

    QStandardItem *pluginItem = mDataModel->itemFromIndex(pIndex);

    QStyleOptionViewItemV4 option(pOption);

    initStyleOption(&option, pIndex);

    if (pluginItem->parent()) {
        // This is a plugin item, so check whether it should look enabled

        if (!pluginItem->isCheckable())
            // No, it shouldn't...

            option.state &= ~QStyle::State_Enabled;
    } else {
        // This is not a plugin item, but a category item, so prevent it from
        // being hoverable. Otherwise, show the category item enabled since it's
        // actually disabled (so we can't select it), yet we want to see as if
        // it was enabled. Finally, make the category item's text bold...

        option.state &= ~QStyle::State_MouseOver;
        option.state |=  QStyle::State_Enabled;

        option.font.setBold(true);
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

PluginsWindow::PluginsWindow(QWidget *pParent, PluginManager *pPluginManager) :
    QDialog(pParent),
    CommonWidget(pParent),
    mGui(new Ui::PluginsWindow),
    mPluginManager(pPluginManager),
    mMainWindow(qobject_cast<MainWindow *>(pParent))
{
    // Set up the GUI

    mGui->setupUi(this);

    // Update the note label

    mGui->noteLabel->setText(mGui->noteLabel->text().arg(qApp->applicationName()));

    // Set up the tree view with a delegate, so that we can select plugins that
    // are shown as 'disabled' (to reflect the fact that users cannot decide
    // whether they should be loaded)

    mDataModel = new QStandardItemModel(mGui->pluginsTreeView);
    mPluginDelegate = new PluginDelegate(mDataModel);

    mGui->pluginsTreeView->setModel(mDataModel);
    mGui->pluginsTreeView->setItemDelegate(mPluginDelegate);

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

        PluginInfo pluginInfo = plugin->info();

        pluginItem->setCheckable(   pluginInfo.manageable()
                                 && (pluginInfo.type() != PluginInfo::Console));

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

        mPluginCategories.value(pluginInfo.category())->appendRow(pluginItem);
    }

    // Hide any category which doesn't have any plugin

    foreach (QStandardItem *categoryItem, mPluginCategories)
        if (!categoryItem->hasChildren())
            mGui->pluginsTreeView->setRowHidden(categoryItem->row(),
                                                mDataModel->invisibleRootItem()->index(),
                                                true);

    // Make sure that the loading state of all the plugins is right, including
    // that of the plugins which the user cannot manage

    updatePluginsLoadingState(0, true);

    // Expand the whole tree view and make sure that it only takes as much width
    // as necessary
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
            this, SLOT(updatePluginInfo(const QModelIndex &, const QModelIndex &)));

    // Connection to handle the activation of a link in the description

    connect(mGui->descriptionValue, SIGNAL(linkActivated(const QString &)),
            this, SLOT(openLink(const QString &)));

    // Connection to handle the window's buttons

    connect(mGui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)),
            this, SLOT(apply()));
}

//==============================================================================

PluginsWindow::~PluginsWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PluginsWindow::selectFirstVisiblePlugin()
{
    // Select the first plugin by looking at the first visible category
    // Note: if no category is visible, then it means that no plugin is visible,
    //       hence nothing needs to be done indeed...

    foreach (QStandardItem *categoryItem, mPluginCategories)
        if (!mGui->pluginsTreeView->isRowHidden(categoryItem->row(),
                                                mDataModel->invisibleRootItem()->index()))
            // We have found the first visible category, so now find its first
            // visible plugin

            for (int i = 0, iMax = categoryItem->rowCount(); i < iMax; ++i)
                if (!mGui->pluginsTreeView->isRowHidden(categoryItem->child(i)->row(),
                                                        categoryItem->index())) {
                    // We have found our first visible plugin, so...

                    mGui->pluginsTreeView->setCurrentIndex(categoryItem->child(i)->index());

                    // We are done, so...

                    return;
                }
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

QString PluginsWindow::versionAsString(const PluginInfo::Version &pVersion) const
{
    // Return the given version as a string
    // Note: ideally, this function would be part of the CoreInterface class,
    //       but this class is used by all the plugins and because this method
    //       requires a translation, well... we can't have it there since a
    //       translation will otherwise be required for each plugin, so...
    //       another solution is to have it here...

    switch (pVersion) {
    case PluginInfo::V001:
        return tr("Version 001");
    default:
        return tr("Unknown version");
    }
}

//==============================================================================

QString PluginsWindow::statusDescription(Plugin *pPlugin) const
{
    // Return the plugin's status' description, if any

    switch (pPlugin->status()) {
    case Plugin::NotFound:
        return tr("The %1 plugin could not be found").arg(pPlugin->name());
    case Plugin::IncompatibleVersion:
        return tr("The version used by the %1 plugin (%2) is not compatible with that of %3 (%4)").arg(pPlugin->name(),
                                                                                                       versionAsString(pPlugin->info().version()),
                                                                                                       qApp->applicationName(),
                                                                                                       versionAsString(mPluginManager->version()));
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
            return  tr("The %1 plugin could not be loaded due to the following problem: %2").arg(pPlugin->name(), pPlugin->statusErrors());
        else
            return  tr("The %1 plugin could not be loaded due to the following problems:").arg(pPlugin->name())+"\n"
                   +pPlugin->statusErrors();
    case Plugin::NotPlugin:
        return tr("The %1 library is not a plugin").arg(pPlugin->name());
    case Plugin::MissingDependencies:
        if (pPlugin->statusErrorsCount() == 1)
            return  tr("The %1 plugin could not be loaded due to the %2 plugin missing").arg(pPlugin->name(), pPlugin->statusErrors());
        else
            return  tr("The %1 plugin could not be loaded due to missing plugins:").arg(pPlugin->name())+"\n"
                   +pPlugin->statusErrors();
    case Plugin::NotPluginOrMissingDependencies:
        return tr("The %1 library is not a plugin or it is, but it could not be loaded due to a/some missing plugin/s").arg(pPlugin->name());
    default:   // Plugin::UndefinedStatus
        return tr("The status of the %1 plugin status is undefined").arg(pPlugin->name());
    }
}

//==============================================================================

void PluginsWindow::updatePluginInfo(const QModelIndex &pNewIndex,
                                     const QModelIndex &pOldIndex) const
{
    Q_UNUSED(pOldIndex);

    // Update the information view with the plugin's information

    Plugin *plugin = mPluginManager->plugin(mDataModel->itemFromIndex(pNewIndex)->text());
    PluginInfo pluginInfo = plugin->info();

    // The plugin's name

    mGui->nameValue->setText(plugin->name());

    // The plugin's type

    switch (pluginInfo.type()) {
    case PluginInfo::General:
        mGui->typeValue->setText(tr("General"));

        break;
    case PluginInfo::Console:
        mGui->typeValue->setText(tr("Console"));

        break;
    case PluginInfo::Gui:
        mGui->typeValue->setText(tr("GUI"));

        break;
    default:
        mGui->typeValue->setText(tr("Undefined"));
    }

    // The plugin's dependencies

    QStringList dependencies = pluginInfo.dependencies();

    if (dependencies.isEmpty())
        dependencies << tr("None");

    if (dependencies.count() > 1)
        mGui->dependenciesValue->setText("- "+dependencies.join("\n- "));
    else
        mGui->dependenciesValue->setText(dependencies.first());

    // The plugin's description

    QString description = pluginInfo.description(mMainWindow->locale());

    mGui->descriptionValue->setText(description.isEmpty()?
                                        tr("None"):
                                        description);

    // The plugin's status

    mGui->statusValue->setText(statusDescription(plugin));
}

//==============================================================================

void PluginsWindow::updatePluginsLoadingState(QStandardItem *pChangedPluginItem,
                                              const bool &pInitializing)
{
    // Disable the connection that handles a change in a plugin's loading state
    // (otherwise what we are doing here is going to be completely uneffective)

    disconnect(mDataModel, SIGNAL(itemChanged(QStandardItem *)),
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
                 mPluginManager->plugin(pChangedPluginItem->text())->info().fullDependencies())
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
                     mPluginManager->plugin(pluginItem->text())->info().fullDependencies())
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
                         mPluginManager->plugin(otherPluginItem->text())->info().fullDependencies())
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

    connect(mDataModel, SIGNAL(itemChanged(QStandardItem *)),
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
    if( QMessageBox::question(this, qApp->applicationName(),
                              tr("%1 must be restarted for your changes to be effective. Are you sure that this is what you want?").arg(qApp->applicationName()),
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
    // Create the category item, disable it (but it will be seen as enabled, we
    // just don't want to allow the user to select it), add it to our data model
    // and then to our list of plugin categories

    QStandardItem *categoryItem = new QStandardItem(pName);

    categoryItem->setEnabled(false);

    mDataModel->invisibleRootItem()->appendRow(categoryItem);

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
                                                mDataModel->invisibleRootItem()->index(),
                                                hideCategory);
        }

    // Select the first plugin item

    selectFirstVisiblePlugin();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
