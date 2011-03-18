#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "cellml.h"
#include "dockwidget.h"

namespace Ui {
    class FileBrowserWindow;
}

class FileBrowserWidget;

class QModelIndex;

class FileBrowserWindow : public DockWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindow(QWidget *pParent = 0);
    ~FileBrowserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings &pSettings);
    virtual void saveSettings(QSettings &pSettings);

protected:
    virtual void updateActions();

private:
    Ui::FileBrowserWindow *mUi;

    FileBrowserWidget *mFileBrowserWidget;

    QStringList mPrevItems;
    QStringList mNextItems;

    bool mKeepTrackOfPrevItem;

    void gotoOtherItem(QStringList &pItems, QStringList &pOtherItems);
    void updateItems(const QString &pItemPath, QStringList &pItems);

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();

    void customContextMenu(const QPoint &);
    void currentItemChanged(const QModelIndex &, const QModelIndex &pPrevItem);
};

#endif
