#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
    class MainWindow;
}

class CentralWidget;
class DockWidget;
class CellmlModelRepositoryWindow;
class FileBrowserWindow;
class FileOrganiserWindow;
class ViewerWindow;
class HelpWindow;

class QHelpEngine;
class QSettings;

class QxtTemporaryDir;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ExitCode
    {
        NeedRestart = 1789
    };

    explicit MainWindow(QWidget *pParent = 0);
    ~MainWindow();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    Ui::MainWindow *mUi;

    CentralWidget *mCentralWidget;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    CellmlModelRepositoryWindow *mCellmlModelRepositoryWindow;
    FileBrowserWindow *mFileBrowserWindow;
    FileOrganiserWindow *mFileOrganiserWindow;

    ViewerWindow *mViewerWindow;

    QHelpEngine *mHelpEngine;
    HelpWindow *mHelpWindow;

    QxtTemporaryDir *mTempDir;
    QString mQchFileName, mQhcFileName;

    QMenu *mActionReopenMenu;
    QMenu *mActionOpenReopenMenu;

    QStringList mRecentlyOpenedFiles;

    void loadDockWindowSettings(DockWidget *pDockWindow,
                                const bool &pNeedDefaultSettings,
                                QSettings &pSettings,
                                const Qt::DockWidgetArea &pDockArea,
                                DockWidget *pDockWidget = 0);
    void loadSettings();
    void saveSettings();

    void setLocale(const QString &pLocale);

    void updateActions();

    void addRecentlyOpenedFile(const QString &pFileName);
    void removeRecentlyOpenedFile(const QString &pFileName);

public Q_SLOTS:
    void singleAppMsgRcvd(const QString &);

private Q_SLOTS:
    void on_actionNew_triggered();
    void on_actionCellML10File_triggered();
    void on_actionCellML11File_triggered();
    void on_actionOpen_triggered();
    void on_actionOpenReopen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSaveAll_triggered();
    void on_actionPrint_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionDelete_triggered();
    void on_actionFind_triggered();
    void on_actionFindNext_triggered();
    void on_actionFindPrevious_triggered();
    void on_actionReplace_triggered();
    void on_actionSelectAll_triggered();
    void on_actionFullScreen_triggered();
    void on_actionRun_triggered();
    void on_actionStop_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionPreferences_triggered();
    void on_actionHomePage_triggered();
    void on_actionCellMLHomePage_triggered();
    void on_actionUpdates_triggered();
    void on_actionAbout_triggered();

    void updateWindowTitle();

    void fileOpened(const QString &pFileName);
    void fileClosed(const QString &pFileName);

    void reopenFile();

    void resetAll();
};

#endif
