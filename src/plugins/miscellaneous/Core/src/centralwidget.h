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
// Central widget
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "file.h"
#include "guiinterface.h"
#include "widget.h"
#include "viewinterface.h"

//==============================================================================

#include <QDir>
#include <QMap>
#include <QTabBar>

//==============================================================================

class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QStackedWidget;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

namespace Core {

//==============================================================================

class CentralWidget;
class Dialog;
class UserMessageWidget;
class TabBarWidget;

//==============================================================================

class CentralWidgetMode
{
public:
    explicit CentralWidgetMode(CentralWidget *pOwner);
    ~CentralWidgetMode();

    bool isEnabled() const;
    void setEnabled(const bool &pEnabled);

    TabBarWidget * viewTabs() const;

    Plugins viewPlugins() const;
    void addViewPlugin(Plugin *pViewPlugin);

private:
    bool mEnabled;

    TabBarWidget *mViewTabs;

    Plugins mViewPlugins;
};

//==============================================================================

class CORE_EXPORT CentralWidget : public Widget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *pParent);
    ~CentralWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void settingsLoaded(const Plugins &pLoadedPlugins);

    virtual void retranslateUi();

    void addView(Plugin *pPlugin);

    TabBarWidget * newTabBarWidget(const QTabBar::Shape &pShape,
                                   const bool &pMovable = false,
                                   const bool &pTabsClosable = false);

    QString currentFileName() const;
    QString localFileName(const QString &pUrl) const;

    void openFile(const QString &pFileName,
                  const File::Type &pType = File::Local,
                  const QString &pUrl = QString());
    void openFiles(const QStringList &pFileNames);

    void openRemoteFile(const QString &pUrl, const bool &pShowWarning = true);

    bool canClose();

    bool selectMode(const QString &pModeName);
    bool selectView(const QString &pViewName);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual void dragMoveEvent(QDragMoveEvent *pEvent);
    virtual void dropEvent(QDropEvent *pEvent);

private:
    enum State {
        Starting,
        Idling,
        UpdatingGui,
        Stopping
    };

    State mState;

    QSettings *mSettings;

    Plugins mLoadedFileHandlingPlugins;
    Plugins mLoadedFileTypePlugins;
    Plugins mLoadedGuiPlugins;
    Plugins mLoadedViewPlugins;

    TabBarWidget *mModeTabs;
    TabBarWidget *mFileTabs;

    QMap<int, ViewInterface::Mode> mModeTabIndexModes;
    QMap<ViewInterface::Mode, int> mModeModeTabIndexes;

    QMap<QString, int> mFileModeTabIndexes;
    QMap<QString, QMap<int, int>> mFileModeViewTabIndexes;

    QStringList mFileNames;

    QStackedWidget *mContents;

    QWidget *mLogoView;

    UserMessageWidget *mNoViewMsg;

    QMap<ViewInterface::Mode, CentralWidgetMode *> mModes;

    Dialog *mRemoteFileDialog;
    QLabel *mRemoteFileDialogUrlLabel;
    QLineEdit *mRemoteFileDialogUrlValue;
    QDialogButtonBox *mRemoteFileDialogButtonBox;

    QMap<QString, QString> mRemoteLocalFileNames;

    QMap<QString, QWidget *> mViews;

    Plugin * viewPlugin(const int &pIndex) const;
    Plugin * viewPlugin(const QString &pFileName) const;

    void updateNoViewMsg();

    bool saveFile(const int &pIndex, const bool &pNeedNewFileName = false);

    bool canCloseFile(const int &pIndex);

    void updateFileTab(const int &pIndex, const bool &pIconOnly = false);

    void updateStatusBarWidgets(QList<QWidget *> pWidgets);

    QString viewKey(const int &pMode, const int &pView,
                    const QString &pFileName);

signals:
    void guiUpdated(Plugin *pViewPlugin, const QString &pFileName);

    void atLeastOneView(const bool &pAtLeastOneView);

    void canSave(const bool &pEnabled);
    void canSaveAs(const bool &pEnabled);
    void canSaveAll(const bool &pEnabled);

    void atLeastOneFile(const bool &pAtLeastOneFile);
    void atLeastTwoFiles(const bool &pAtLeastTwoFiles);

private slots:
    void updateGui();

    void openFile();

    void openRemoteFileChanged();
    void doOpenRemoteFile();
    void cancelOpenRemoteFile();
    void openRemoteFile();

    void reloadFile(const int &pIndex = -1, const bool &pForce = false);

    void duplicateFile();

    void toggleLockedFile();

    void fileChanged(const QString &pFileName, const bool &pFileChanged,
                     const bool &pDependenciesChanged);
    void fileDeleted(const QString &pFileName);

    void updateModifiedSettings();

    void filePermissionsChanged(const QString &pFileName);
    void fileModified(const QString &pFileName);

    void fileReloaded(const QString &pFileName, const bool &pFileChanged,
                      const bool &pExcludeFileViewPlugin = false);

    void fileCreated(const QString &pFileName, const QString &pUrl);
    void fileDuplicated(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    void fileSaved(const QString &pFileName);

    void saveFile();
    void saveFileAs();
    void saveAllFiles();

    void previousFile();
    void nextFile();

    bool closeFile(const int &pIndex = -1, const bool &pForceClosing = false);
    void closeAllFiles(const bool &pForceClosing = false);

    void moveFile(const int &pFromIndex, const int &pToIndex);

    void updateFileTabIcon(const QString &pViewName, const QString &pFileName,
                           const QIcon &pIcon);
    void updateFileTabIcons();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
