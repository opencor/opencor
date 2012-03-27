//==============================================================================
// Plugins window
//==============================================================================

#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

//==============================================================================

#include "commonwidget.h"
#include "coreinterface.h"
#include "plugininfo.h"

//==============================================================================

#include <QDialog>
#include <QStyledItemDelegate>
#include <QStandardItemModel>

//==============================================================================

namespace Ui {
    class PluginsWindow;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

class MainWindow;
class PluginManager;

//==============================================================================

class PluginDelegate : public QStyledItemDelegate
{
public:
    explicit PluginDelegate(QStandardItemModel *pDataModel);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

private:
    QStandardItemModel *mDataModel;
};

//==============================================================================

class PluginsWindow : public QDialog, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit PluginsWindow(QWidget *pParent, PluginManager *pPluginManager);
    ~PluginsWindow();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::PluginsWindow *mUi;

    PluginManager *mPluginManager;

    QStandardItemModel *mDataModel;

    PluginDelegate *mPluginDelegate;

    QList<QStandardItem *> mManageablePlugins;
    QList<QStandardItem *> mUnmanageablePlugins;

    MainWindow *mMainWindow;

    QMap<QString, bool> mInitialLoadingStates;

    QMap<PluginInfo::Category, QStandardItem *> mPluginCategories;

    void newPluginCategory(const PluginInfo::Category &pCategory,
                           const QString &pName);

    QString versionAsString(const PluginInfo::Version &pVersion) const;
    QString statusDescription(Plugin *pPlugin) const;

    void selectFirstVisiblePlugin();

private Q_SLOTS:
    void on_selectablePluginsCheckBox_toggled(bool pChecked);

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void updatePluginInfo(const QModelIndex &pNewIndex, const QModelIndex &) const;
    void updatePluginsLoadingState(QStandardItem *pChangedPluginItem = 0,
                                   const bool &pInitializing = false);
    void openLink(const QString &pLink) const;
    void apply();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
