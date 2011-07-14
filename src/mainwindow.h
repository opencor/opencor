#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QTranslator>
#include <QWidget>

class QSettings;

namespace OpenCOR {

class MainWindow;
class PluginManager;

class MainWindowUi : public QObject
{
    Q_OBJECT

    friend class MainWindow;

public:

private:
    // General

    QMenuBar *menuBar;
    QToolBar *helpToolbar;

    QWidget *centralWidget;

    QStatusBar *statusBar;

    // File menu

    QMenu *menuFile;
        QAction *actionExit;

    // View menu

    QMenu *menuView;
        QMenu *menuToolbars;
            QAction *actionHelpToolbar;
        QAction *actionStatusBar;
        // Separator
        QAction *actionFullScreen;

    // Tools menu

    QMenu *menuTools;
        QMenu *menuLanguage;
            QAction *actionSystem;
            QAction *actionEnglish;
            QAction *actionFrench;
        // Separator
        QAction *actionPlugins;
        // Separator
        QAction *actionPreferences;
        // Separator
        QAction *actionResetAll;

    // Help menu

    QMenu *menuHelp;
        QAction *actionHomePage;
        // Separator
        QAction *actionCheckForUpdates;
        // Separator
        QAction *actionAbout;

    // Methods

    void setupUi(MainWindow *pMainWindow);
    void retranslateUi(MainWindow *pMainWindow);
};

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

    QString locale();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    MainWindowUi *mUi;

    QSettings *mSettings;

    PluginManager *mPluginManager;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString &pLocale);

public Q_SLOTS:
    void singleAppMsgRcvd(const QString &);

private Q_SLOTS:
    void on_actionFullScreen_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionPlugins_triggered();
    void on_actionPreferences_triggered();
    void on_actionHomePage_triggered();
    void on_actionCheckForUpdates_triggered();
    void on_actionAbout_triggered();

    void resetAll();
};

}

#endif
