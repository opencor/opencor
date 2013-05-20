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

//==============================================================================

class QStandardItem;
class QStandardItemModel;

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

class PluginItemDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class PluginsWindow : public QDialog, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit PluginsWindow(PluginManager *pPluginManager,
                           MainWindow *pMainWindow = 0);
    ~PluginsWindow();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::PluginsWindow *mGui;

    MainWindow *mMainWindow;

    PluginManager *mPluginManager;

    QStandardItemModel *mModel;
    PluginItemDelegate *mPluginItemDelegate;

    QList<QStandardItem *> mManageablePluginItems;
    QList<QStandardItem *> mUnmanageablePluginItems;

    QMap<QString, bool> mInitialLoadingStates;

    QMap<PluginInfo::Category, QStandardItem *> mPluginCategories;

    void newPluginCategory(const PluginInfo::Category &pCategory,
                           const QString &pName);

    QString interfaceVersionAsString(const PluginInfo::InterfaceVersion &pInterfaceVersion) const;
    QString statusDescription(Plugin *pPlugin) const;

    void selectFirstVisibleCategory();

private Q_SLOTS:
    void on_selectablePluginsCheckBox_toggled(bool pChecked);

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void updateInformation(const QModelIndex &pNewIndex,
                           const QModelIndex &pOldIndex) const;
    void updatePluginsSelectedState(QStandardItem *pItem = 0,
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
