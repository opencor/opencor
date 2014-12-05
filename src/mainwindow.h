/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Main window
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//==============================================================================

#include "guiinterface.h"
#include "plugin.h"

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
}   // namespace SharedTools

//==============================================================================

namespace OpenCOR {

//==============================================================================

class GuiInterface;
class PluginManager;

//==============================================================================

enum {
    CleanRestart  =  843,
    NormalRestart = 1789
};

//==============================================================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(SharedTools::QtSingleApplication *pApplication,
                        const QString &pApplicationDate);
    ~MainWindow();

    QString locale() const;

    void restart(const bool &pSaveSettings) const;

    void handleArguments(const QString &pArguments);

    bool shuttingDown() const;

    void showSelf();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);

private:
    Ui::MainWindow *mGui;

    QString mApplicationDate;

    bool mShuttingDown;

    QSettings *mSettings;

    PluginManager *mPluginManager;

    Plugins mLoadedPluginPlugins;
    Plugins mLoadedI18nPlugins;
    Plugins mLoadedGuiPlugins;
    Plugins mLoadedWindowPlugins;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    QMap<QString, QMenu *> mMenus;

    QMenu *mFileNewMenu;

    QMenu *mViewWindowsMenu;
    QAction *mViewSeparator;

    Plugin *mViewPlugin;

    bool mDockedWindowsVisible;

    QByteArray mDockedWindowsState;

    bool mStatusBarVisible;

    void initializeGuiPlugin(Plugin *pPlugin);

    void loadSettings();
    void saveSettings() const;

    void setLocale(const QString &pLocale, const bool &pForceSetting = false);

    void reorderViewWindowsMenu();

    void updateViewWindowsMenu(QAction *pAction);

    void handleAction(const QUrl &pUrl);

    void showEnableActions(const QList<QAction *> &pActions);

private Q_SLOTS:
    void fileOpenRequest(const QString &pFileName);
    void messageReceived(const QString &pMessage, QObject *pSocket);

    void on_actionFullScreen_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionFrench_triggered();
    void on_actionPlugins_triggered();
    void on_actionPreferences_triggered();
    void on_actionHomePage_triggered();
    void on_actionCheckForUpdates_triggered();
    void on_actionAbout_triggered();

    void updateGui(Plugin *pViewPlugin, const QString &pFileName);

    void showDockedWindows(const bool &pShow,
                           const bool &pInitialisation = false);
    void toggleDockedWindows();

    void updateDockWidgetsVisibility();

    void resetAll();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
