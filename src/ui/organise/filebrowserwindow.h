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

    void gotoOtherItem(QStringList &pItems, QStringList &pOtherItems);
    void updateItems(const QString &pItemPath, QStringList &pItems);

Q_SIGNALS:
    void filesOpened(const QStringList &pFileNames);

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();

    void beginLoadingSettings();
    void endLoadingSettings();
    void customContextMenu(const QPoint &);
    void itemDoubleClicked(const QModelIndex &itemIndex);

    void itemChanged(const QModelIndex &, const QModelIndex &pPrevItem);
};

#endif
