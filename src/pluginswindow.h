#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include "commonwidget.h"

#include <QDialog>
#include <QStyledItemDelegate>
#include <QStandardItemModel>

namespace Ui {
    class PluginsWindow;
}

namespace OpenCOR {

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

    PluginManager *mPluginManager;

    QStandardItemModel *mDataModel;

    PluginDelegate *mPluginDelegate;

private slots:
    void updatePluginInfo(const QModelIndex &pNewIndex, const QModelIndex &);
    void openLink(const QString &pLink);
};

}

#endif
