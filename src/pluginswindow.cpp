#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"

#include "ui_pluginswindow.h"

namespace OpenCOR {

PluginsWindow::PluginsWindow(PluginManager *pPluginManager,
                             QWidget *pParent) :
    QDialog(pParent),
    CommonWidget(pParent),
    mUi(new Ui::PluginsWindow)
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

}
