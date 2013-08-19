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

PluginsWindow::PluginsWindow(PluginManager *pPluginManager,
                             MainWindow *pMainWindow) :
    QDialog(pMainWindow),
    CommonWidget(pMainWindow),
    mGui(new Ui::PluginsWindow),
    mMainWindow(pMainWindow),
    mPluginManager(pPluginManager),
    mManageablePluginItems(QList<QStandardItem *>()),
    mUnmanageablePluginItems(QList<QStandardItem *>())
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

        if (pluginInfo) {
            pluginItem->setCheckable(pluginInfo->isManageable());

            if (pluginItem->isCheckable()) {
                // Retrieve the loading state of the plugin

                pluginItem->setCheckState((Plugin::load(plugin->name()))?
                                              Qt::Checked:
                                              Qt::Unchecked);

                // We are dealing with a manageable plugin, so add it to our list of
                // manageable plugins

                mManageablePluginItems << pluginItem;
            } else {
                // We are dealing with an unmanageable plugin, so add it to our list
                // of unmanageable plugins

                mUnmanageablePluginItems << pluginItem;
            }

            // Add the plugin to the right category

            mPluginCategories.value(pluginInfo->category())->appendRow(pluginItem);
        } else {
            // We are not actually dealing with a plugin, so add it to the
            // Miscellaneous category

            mUnmanageablePluginItems << pluginItem;

            mPluginCategories.value(PluginInfo::Miscellaneous)->appendRow(pluginItem);
        }
    }

    // Make a category checkable if it contains manageable plugins and hide it,
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
        return tr("the plugin could not be found.");
    case Plugin::InvalidInterfaceVersion:
        return tr("the version of the interface used by the plugin (%1) is not valid (%2 is expected).").arg(interfaceVersionAsString(pPlugin->info()->interfaceVersion()),
                                                                                                             interfaceVersionAsString(mPluginManager->interfaceVersion()));
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
        return tr("this library is not a plugin.");
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

            QStringList dependencies = pluginInfo->fullDependencies();

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

            mGui->fieldOneLabel->setText(tr("Status:"));
            mGui->fieldOneValue->setText(statusDescription(plugin));
        }
    } else if (atLeastOneItem) {
        // We are not dealing with a plugin, but a plugin category

        validItem = true;

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

    // Show/hide the different fields

    mGui->fieldOneLabel->setVisible(atLeastOneItem);
    mGui->fieldOneValue->setVisible(atLeastOneItem);

    mGui->fieldTwoLabel->setVisible(atLeastOneItem && validItem);
    mGui->fieldTwoValue->setVisible(atLeastOneItem && validItem);

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

    // In case we un/select a category, then go through its manageable plugins
    // and un/select them accordingly

    if (pItem && !pItem->parent())
        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            QStandardItem *pluginItem = pItem->child(i);

            if (pluginItem->isCheckable())
                pluginItem->setCheckState(pItem->checkState());
        }

    // Update the selected state of all our unmanageable plugins

    foreach (QStandardItem *unmanageablePluginItem, mUnmanageablePluginItems) {
        // First, reset the selected state of our unamanageable plugin

        unmanageablePluginItem->setCheckState(Qt::Unchecked);

        // Next, go through our manageable plugins and check whether one of them
        // needs our unmanageable plugin

        foreach (QStandardItem *manageablePluginItem, mManageablePluginItems)
            if (manageablePluginItem->checkState() == Qt::Checked) {
                // The manageable plugin is selected, so go through its plugin
                // dependencies and check whether one of them is our
                // unmanageable plugin

                foreach (const QString &requiredPlugin,
                         mPluginManager->plugin(manageablePluginItem->text())->info()->fullDependencies())
                    if (!requiredPlugin.compare(unmanageablePluginItem->text())) {
                        // The manageable plugin requires our unamanageable
                        // plugin, so...

                        unmanageablePluginItem->setCheckState(Qt::Checked);

                        break;
                    }

                if (unmanageablePluginItem->checkState() == Qt::Checked)
                    break;
            }
    }

    // Update the selected state of all our categories which have at least one
    // manageable plugin

    foreach (QStandardItem *categoryItem, mPluginCategories) {
        int nbOfPlugins = categoryItem->rowCount();

        if (nbOfPlugins) {
            int nbOfManageablePlugins = 0;
            int nbOfUnmanageablePlugins = 0;
            int nbOfSelectedManageablePlugins = 0;

            for (int i = 0; i < nbOfPlugins; ++i) {
                QStandardItem *pluginItem = categoryItem->child(i);

                if (pluginItem->isCheckable()) {
                    ++nbOfManageablePlugins;

                    if (pluginItem->checkState() == Qt::Checked)
                        ++nbOfSelectedManageablePlugins;
                } else {
                    ++nbOfUnmanageablePlugins;
                }
            }

            if (nbOfPlugins != nbOfUnmanageablePlugins)
                categoryItem->setCheckState(nbOfSelectedManageablePlugins?
                                                (nbOfSelectedManageablePlugins == nbOfManageablePlugins)?
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
                 mManageablePluginItems+mUnmanageablePluginItems)
            mInitialLoadingStates.insert(plugin->text(),
                                         plugin->checkState() == Qt::Checked);

    bool buttonsEnabled = false;

    foreach (QStandardItem *plugin, mManageablePluginItems+mUnmanageablePluginItems)
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
    // Keep track of the loading state of the various manageable plugins

    foreach (QStandardItem *manageablePluginItem, mManageablePluginItems)
        Plugin::setLoad(manageablePluginItem->text(),
                        manageablePluginItem->checkState() == Qt::Checked);

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
                              tr("<strong>%1</strong> must be restarted for your changes to be effective. Do you wish to proceed?").arg(qApp->applicationName()),
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
    // Show/hide our unmanageable plugins

    foreach (QStandardItem *unmanageablePluginItem, mUnmanageablePluginItems)
        mGui->pluginsTreeView->setRowHidden(unmanageablePluginItem->row(),
                                            unmanageablePluginItem->parent()->index(), pChecked);

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
