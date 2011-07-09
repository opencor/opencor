#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"

#include "ui_pluginswindow.h"

namespace OpenCOR {

PluginsWindow::PluginsWindow(PluginManager *pPluginManager,
                             QWidget *pParent) :
    QDialog(pParent),
    CommonWidget(pParent),
    mUi(new Ui::PluginsWindow),
    mPluginManager(pPluginManager)
{
    // Set up the UI

    mUi->setupUi(this);

    // Set up the tree view

    mDataModel = new QStandardItemModel;

    mUi->listView->setModel(mDataModel);

    // Populate the data model

    foreach(Plugin *plugin, pPluginManager->plugins()) {
        QStandardItem *pluginItem = new QStandardItem(plugin->name());

        pluginItem->setCheckable(true);
        pluginItem->setEnabled(plugin->info().dependencies().count());

        pluginItem->setCheckState((plugin->status() == Plugin::Loaded)?
                                      Qt::Checked:
                                      Qt::Unchecked);

        mDataModel->invisibleRootItem()->appendRow(pluginItem);
    }

    // Make sure that the list view only takes as much space as necessary
    // Note: for some reason (maybe because of the check boxes?) the retrieved
    //       column size gives us a width that is slightly too small and
    //       therefore requires a horizontal scroll bar, hence we add 10% to it

    mUi->listView->setMinimumWidth(1.1*mUi->listView->sizeHintForColumn(0));
    mUi->listView->setMaximumWidth(mUi->listView->minimumWidth());

    // Avoid issues of the information widgets resizing crazily (especially on
    // Mac OS X) by fixing the width of the description value widget

    mUi->descriptionValue->setMinimumWidth(mUi->listView->minimumWidth());
    mUi->descriptionValue->setMaximumWidth(mUi->descriptionValue->minimumWidth());

    // Connection to handle a plugin's information

    connect(mUi->listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updatePluginInfo(const QModelIndex &, const QModelIndex &)));
}

PluginsWindow::~PluginsWindow()
{
    // Delete some internal objects

    delete mDataModel;
    delete mUi;
}

void PluginsWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);
}

void PluginsWindow::updatePluginInfo(const QModelIndex &pNewIndex,
                                     const QModelIndex &)
{
    // Update the information view with the plugin's information

    PluginInfo pluginInfo = mPluginManager->plugin(mDataModel->itemFromIndex(pNewIndex)->text())->info();

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

    mUi->descriptionValue->setText(pluginInfo.description());
}

}
