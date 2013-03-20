//==============================================================================
// Main window
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//==============================================================================

#include "guiinterface.h"

//==============================================================================

#include <QMainWindow>
#include <QMap>
#include <QTranslator>
#include <QUrl>

//==============================================================================

namespace Ui {
    class MainWindow;
}

//==============================================================================

class QSettings;

//==============================================================================

namespace SharedTools {
    class QtSingleApplication;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

class GuiInterface;
class PluginManager;

//==============================================================================

static const int NeedRestart = 1789;

//==============================================================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(SharedTools::QtSingleApplication *pApp);
    ~MainWindow();

    QString locale() const;

    void restart(const bool &pSaveSettings) const;

    void handleArguments(const QString &pArguments);

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);

private:
    Ui::MainWindow *mGui;

    QSettings *mSettings;

    PluginManager *mPluginManager;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    QMap<QString, QMenu *> mMenus;

    QMenu *mFileNewMenu;

    QMenu *mViewOrganisationMenu;
    QAction *mViewSeparator;

    QList<QMenu *> mViewMenus;
    QMap<Plugin *, QAction *> mViewActions;

    Plugin *mViewPlugin;
    bool mNeedViewPluginInitialisation;

    void initializeGuiPlugin(Plugin *pPlugin, GuiSettings *pGuiSettings);

    void loadSettings();
    void saveSettings() const;

    void setLocale(const QString &pLocale, const bool &pForceSetting = false);

    void reorderViewMenu(QMenu *pViewMenu);
    void reorderViewMenus();

    void updateViewMenu(const GuiWindowSettings::GuiWindowSettingsType &pMenuType,
                        QAction *pAction);

    void handleAction(const QUrl &pUrl);

    void showSelf();

    void checkViewMenu(QMenu *pViewMenu);

private Q_SLOTS:
    void fileOpenRequest(const QString &pFileName);
    void messageReceived(const QString &pMessage);

    void on_actionFullScreen_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionPlugins_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();

    void updateGui(Plugin *pViewPlugin);

    void resetAll();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
