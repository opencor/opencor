#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"

#include "ui_pluginswindow.h"

#include <QDesktopServices>
#include <QUrl>

namespace OpenCOR {

PluginDelegate::PluginDelegate(QStandardItemModel *pDataModel,
                               QObject *pParent) :
    QStyledItemDelegate(pParent),
    mDataModel(pDataModel)
{
}

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

    if (!pluginItem->isCheckable())
        option.state &= ~QStyle::State_Enabled;

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

PluginsWindow::PluginsWindow(PluginManager *pPluginManager,
                             QWidget *pParent) :
    QDialog(pParent),
    mUi(new Ui::PluginsWindow),
    mPluginManager(pPluginManager)
{
    // Set up the UI

    mUi->setupUi(this);

    // Update the note label

    mUi->noteLabel->setText(mUi->noteLabel->text().arg(qApp->applicationName()));

    // Set up the tree view with a delegate, so that we can select plugins that
    // are shown as 'disabled' (to reflect the fact that users cannot decide
    // whether they should be loaded)

    mDataModel = new QStandardItemModel;
    mPluginDelegate = new PluginDelegate(mDataModel);

    mUi->listView->setModel(mDataModel);
    mUi->listView->setItemDelegate(mPluginDelegate);

    // Populate the data model

    foreach (Plugin *plugin, mPluginManager->plugins()) {
        QStandardItem *pluginItem = new QStandardItem(plugin->name());

        // Only plugins that have dependencies are checkable

        pluginItem->setCheckable(plugin->info().dependencies().count());

        // Retrieve the loading state of the plugin, in case it is a plugin the
        // user can manage

        if (pluginItem->isCheckable())
            pluginItem->setCheckState((Plugin::load(mPluginManager->settings(),
                                                    plugin->name()))?
                                          Qt::Checked:
                                          Qt::Unchecked);

        // Add the plugin to our data model

        mDataModel->invisibleRootItem()->appendRow(pluginItem);
    }

    // Make sure that the loading state of all the plugins is right, including
    // that of the plugins which the user cannot manage

    updatePluginsLoadingState();

    // Select the first plugin

    mUi->listView->selectionModel()->select(mDataModel->index(0, 0),
                                            QItemSelectionModel::Select);

    // Make sure that the list view only takes as much width as necessary
    // Note: for some reasons (maybe because we have check boxes?), the
    //       retrieved column size gives us a width that is slightly too small
    //       and therefore requires a horizontal scroll bar, hence we add 15% to
    //       it (the extra 15% seems to be enough to even account for a big
    //       number of plugins which would then require a vertical scroll bar)

    mUi->listView->setMinimumWidth(1.15*mUi->listView->sizeHintForColumn(0));
    mUi->listView->setMaximumWidth(mUi->listView->minimumWidth());

    // Make, through the note label, sure that the window has a minimum width

    mUi->noteLabel->setMinimumWidth(2.75*mUi->listView->minimumWidth());

    // Connection to handle a plugin's information

    connect(mUi->listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updatePluginInfo(const QModelIndex &, const QModelIndex &)));

    // Connection to handle a change in a plugin's loading state

    connect(mDataModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(updatePluginsLoadingState(QStandardItem *)));

    // Connection to handle the activation of a link in the description

    connect(mUi->descriptionValue, SIGNAL(linkActivated(const QString &)),
            this, SLOT(openLink(const QString &)));

    // Make sure that the window has a reasonable starting size

    mUi->verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

PluginsWindow::~PluginsWindow()
{
    // Delete some internal objects

    delete mDataModel;
    delete mPluginDelegate;
    delete mUi;
}

void PluginsWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);
}

void PluginsWindow::updatePluginInfo(const QModelIndex &pNewIndex,
                                     const QModelIndex &) const
{
    // Update the information view with the plugin's information

    QString pluginName = mDataModel->itemFromIndex(pNewIndex)->text();
    Plugin *plugin = mPluginManager->plugin(pluginName);
    PluginInfo pluginInfo = plugin->info();

    // The plugin's name

    mUi->nameValue->setText(pluginName);

    // The plugin's type

    switch (pluginInfo.type()) {
    case PluginInfo::General:
        mUi->typeValue->setText(tr("General"));

        break;
    case PluginInfo::Console:
        mUi->typeValue->setText(tr("Console"));

        break;
    case PluginInfo::Gui:
        mUi->typeValue->setText(tr("GUI"));

        break;
    default:
        mUi->typeValue->setText(tr("Undefined"));

        break;
    }

    // The plugin's dependencies

    QStringList dependencies = pluginInfo.dependencies();

    if (dependencies.isEmpty())
        dependencies << tr("None");

    if (dependencies.count() > 1)
        mUi->dependenciesValue->setText("- "+dependencies.join("\n- "));
    else
        mUi->dependenciesValue->setText(dependencies.first());

    // The plugin's description

    QString description = pluginInfo.description(qobject_cast<MainWindow *>(parent())->locale());

    mUi->descriptionValue->setText(description.isEmpty()?
                                       tr("None"):
                                       description);

    // The plugin's status

    mUi->statusValue->setText(plugin->statusDescription());
}

void PluginsWindow::updatePluginsLoadingState(QStandardItem *) const
{
    // Determine which plugins are required by the plugins over which the user
    // has loading control

    QStringList requiredPlugins;

    for (int i = 0; i < mDataModel->rowCount(); ++i) {
        QStandardItem *pluginItem = mDataModel->item(i);

        if (   pluginItem->isCheckable()
            && (pluginItem->checkState() == Qt::Checked))
           requiredPlugins << mPluginManager->requiredPlugins(Plugin::fileName(mPluginManager->pluginsDir(), pluginItem->text()));
    }

    requiredPlugins.removeDuplicates();

    // Update the loading state of the plugins over which the user has no
    // control

    for (int i = 0; i < mDataModel->rowCount(); ++i) {
        QStandardItem *pluginItem = mDataModel->item(i);

        if (!pluginItem->isCheckable())
            pluginItem->setCheckState(requiredPlugins.contains(pluginItem->text())?
                                          Qt::Checked:
                                          Qt::Unchecked);
    }
}

void PluginsWindow::openLink(const QString &pLink) const
{
    // Open the link in the user's browser

    QDesktopServices::openUrl(QUrl(pLink));
}

void PluginsWindow::on_buttonBox_accepted()
{
    // Keep track of the loading state of the various plugins over which the
    // user has control (i.e. the ones that are checkable)

    for (int i = 0; i < mDataModel->rowCount(); ++i) {
        QStandardItem *pluginItem = mDataModel->item(i);

        if (pluginItem->isCheckable())
            Plugin::setLoad(mPluginManager->settings(), pluginItem->text(),
                            pluginItem->checkState() == Qt::Checked);
    }

    // Confirm that we accepted the changes

    accept();
}

void PluginsWindow::on_buttonBox_rejected()
{
    // Simple cancel whatever was done here

    reject();
}

}
