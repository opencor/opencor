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

Q_SIGNALS:
    void folderCreated(const QString &pFolderName);
    void cellmlFileCreated(const QString &pCellmlFileName);

protected:
    virtual void updateActions();

private:
    Ui::FileBrowserWindow *mUi;

    FileBrowserWidget *mFileBrowserWidget;

    QStringList mPrevFolders;
    QStringList mNextFolders;

    bool mKeepTrackOfPrevFolder;

    void updateFolders(const QString &pFolderName, QStringList &pFolders);

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();

    void needUpdateActions();
    void currentItemChanged(const QModelIndex &, const QModelIndex &pPrevItem);
};

#endif
