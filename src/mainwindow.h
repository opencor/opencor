#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
    class MainWindow;
}

class QSettings;

namespace OpenCOR {
namespace Core {
    class DockWidget;
}

class GuiInterface;
class PluginManager;

static const int NeedRestart = 1789;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *pParent = 0);
    ~MainWindow();

    QString locale() const;

    void restart(const bool &pSaveSettings) const;

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

    void initializePlugin(GuiInterface *pGuiInterface);

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
