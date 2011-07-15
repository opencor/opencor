#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

class QSettings;

namespace Ui {
    class MainWindow;
}

namespace OpenCOR {

class GuiSettings;
class PluginManager;

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

    QString locale() const;

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    Ui::MainWindow *mUi;

    QSettings *mSettings;

    PluginManager *mPluginManager;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    void initializePlugin(const GuiSettings &pGuiSettings) const;

    void loadPluginSettings(const GuiSettings &pGuiSettings,
                            const bool &pNeedDefaultSettings);
    void savePluginSettings(const GuiSettings &pGuiSettings) const;

    void loadSettings();
    void saveSettings() const;

    void setLocale(const QString &pLocale);

public Q_SLOTS:
#ifdef Q_WS_WIN
    void singleAppMsgRcvd(const QString &) const;
#else
    void singleAppMsgRcvd(const QString &);
#endif

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
