#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QStandardItemModel>

namespace Ui {
    class PluginsWindow;
}

namespace OpenCOR {

class MainWindow;
class PluginManager;

class PluginDelegate : public QStyledItemDelegate
{
public:
    explicit PluginDelegate(QStandardItemModel *pDataModel,
                            QObject *pParent = 0);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

private:
    QStandardItemModel *mDataModel;
};

class PluginsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PluginsWindow(PluginManager *pPluginManager,
                           QWidget *pParent = 0);
    ~PluginsWindow();

    virtual void retranslateUi();

private:
    Ui::PluginsWindow *mUi;

    PluginManager *mPluginManager;

    QStandardItemModel *mDataModel;

    PluginDelegate *mPluginDelegate;

    QList<QStandardItem *> mManageablePlugins;
    QList<QStandardItem *> mUnmanageablePlugins;

    MainWindow *mMainWindow;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void updatePluginInfo(const QModelIndex &pNewIndex, const QModelIndex &) const;
    void updatePluginsLoadingState(QStandardItem *pChangedPluginItem = 0) const;
    void openLink(const QString &pLink) const;
    void apply();
};

}

#endif
