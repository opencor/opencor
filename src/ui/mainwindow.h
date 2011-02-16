#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
    class MainWindow;
}

class CentralWidget;
class PmrExplorerWindow;
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
    explicit MainWindow(QWidget *pParent = 0);
    ~MainWindow();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);

    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    Ui::MainWindow *mUi;

    CentralWidget *mCentralWidget;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    PmrExplorerWindow *mPmrExplorerWindow;
    FileBrowserWindow *mFileBrowserWindow;
    FileOrganiserWindow *mFileOrganiserWindow;

    ViewerWindow *mViewerWindow;

    QHelpEngine *mHelpEngine;
    HelpWindow *mHelpWindow;

    QxtTemporaryDir *mTempDir;
    QString mQchFileName, mQhcFileName;

    void notYetImplemented(const QString &method);

    void defaultSettingsForDockWindow(QDockWidget *pDockWindow, const Qt::DockWidgetArea &pDockArea);
    void defaultSettings();

    void loadSettings();
    void saveSettings();

    void setLocale(const QString &pLocale);

public Q_SLOTS:
    void singleAppMsgRcvd(const QString&);

private Q_SLOTS:
    void on_actionFullScreen_triggered();
    void on_actionNew_triggered();
    void on_actionSaveAll_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSave_triggered();
    void on_actionCloseAll_triggered();
    void on_actionClose_triggered();
    void on_actionReopen_triggered();
    void on_actionOpen_triggered();
    void on_actionCellML11File_triggered();
    void on_actionCellML10File_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();

    void resetAll();
};

#endif
