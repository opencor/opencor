#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include "commonwidget.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
    class PluginsWindow;
}

namespace OpenCOR {

class PluginManager;

class PluginsWindow : public QDialog, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit PluginsWindow(PluginManager *pPluginManager,
                           QWidget *pParent = 0);
    ~PluginsWindow();

    virtual void retranslateUi();

private:
    Ui::PluginsWindow *mUi;

    QStandardItemModel *mDataModel;

    PluginManager *mPluginManager;

private slots:
    void updatePluginInfo(const QModelIndex &pNewIndex, const QModelIndex &);
    void openLink(const QString &pLink);
};

}

#endif
