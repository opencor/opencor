#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "cellml.h"
#include "commonwidget.h"
#include "dockwidget.h"

namespace Ui {
    class FileBrowserWindow;
}

class DocumentManager;
class FileBrowserWidget;
class MainWindow;

class QModelIndex;

class FileBrowserWindow : public DockWidget, public CommonWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindow(MainWindow *pParent);
    ~FileBrowserWindow();

    virtual void retranslateUi();

    virtual void loadSettings(const QSettings &pSettings, const QString &);
    virtual void saveSettings(QSettings &pSettings, const QString &);

protected:
    virtual void updateActions();

private:
    Ui::FileBrowserWindow *mUi;

    FileBrowserWidget *mFileBrowserWidget;

    QStringList mPrevFolders;
    QStringList mNextFolders;

    QString mPrevFolder;

    bool mKeepTrackOfPrevFolder;

    DocumentManager *mDocumentManager;

    void newCellmlFile(const CellmlVersion &pCellmlVersion);

    void updateFolders(const QString &pFolderName, QStringList &pFolders);

private Q_SLOTS:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();
    void on_actionNew_triggered();
    void on_actionNewFolder_triggered();
    void on_actionNewCellML10File_triggered();
    void on_actionNewCellML11File_triggered();
    void on_actionDelete_triggered();

    void customContextMenu(const QPoint &);

    void needUpdateActions();
    void currentItemChanged(const QModelIndex &, const QModelIndex &);
};

#endif
