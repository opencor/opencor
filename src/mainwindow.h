/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Main window
//==============================================================================

#pragma once

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
    explicit MainWindow(const QString &pApplicationDate);
    ~MainWindow();

    void handleArguments(const QStringList &pArguments);

    void showSelf();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);

private:
    Ui::MainWindow *mGui;

    QString mApplicationDate;

    QSettings *mSettings;

    PluginManager *mPluginManager;

    Plugins mLoadedPluginPlugins;
    Plugins mLoadedI18nPlugins;
    Plugins mLoadedGuiPlugins;
    Plugins mLoadedPreferencesPlugins;
    Plugins mLoadedWindowPlugins;

    QString mRawLocale;

    QTranslator mQtBaseTranslator;
    QTranslator mQtHelpTranslator;
    QTranslator mQtXmlPatternsTranslator;
    QTranslator mAppTranslator;

    QMap<QString, QMenu *> mMenus;

    QMenu *mFileNewMenu;

    QMenu *mViewWindowsMenu;
    QAction *mViewSeparator;

    Plugin *mViewPlugin;

    bool mDockedWindowsVisible;

    QByteArray mDockedWindowsState;

    void registerOpencorUrlScheme();

    void initializeGuiPlugin(Plugin *pPlugin);

    void loadSettings();
    void saveSettings() const;

    void setLocale(const QString &pRawLocale = QString(),
                   const bool &pForceSetting = false);

    void reorderViewWindowsMenu();

    void updateViewWindowsMenu(QAction *pAction);

    void restart(const bool &pSaveSettings) const;

    void showEnableActions(const QList<QAction *> &pActions);

    void showPreferencesDialog(const QString &pPluginName = QString());

private slots:
    void openFileOrHandleUrl(const QString &pFileNameOrOpencorUrl);
    void handleMessage(const QString &pMessage);

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

    void handleUrl(const QUrl &pUrl);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
