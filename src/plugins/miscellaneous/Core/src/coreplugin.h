/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Core plugin
//==============================================================================

#pragma once

//==============================================================================

#include "coreinterface.h"
#include "filehandlinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PLUGININFO_FUNC CorePluginInfo();

//==============================================================================

static const auto FileGroup = QStringLiteral("File");

//==============================================================================

class CentralWidget;

//==============================================================================

class CorePlugin : public QObject, public CoreInterface,
                   public FileHandlingInterface, public GuiInterface,
                   public I18nInterface, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CorePlugin" FILE "coreplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
#include "coreinterface.inl"
#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    CentralWidget *mCentralWidget = nullptr;

    QAction *mFileNewFileAction = nullptr;

    QAction *mFileOpenAction = nullptr;
    QAction *mFileOpenRemoteAction = nullptr;

    QAction *mFileReloadAction = nullptr;

    QAction *mFileDuplicateAction = nullptr;

    QAction *mFileLockedAction = nullptr;

    QAction *mFileSaveAction = nullptr;
    QAction *mFileSaveAsAction = nullptr;
    QAction *mFileSaveAllAction = nullptr;

    QAction *mFilePreviousAction = nullptr;
    QAction *mFileNextAction = nullptr;

    QAction *mFileCloseAction = nullptr;
    QAction *mFileCloseAllAction = nullptr;

    QAction *mFileOpenReloadSeparator = nullptr;

    QMenu *mFileReopenSubMenu = nullptr;

    QAction *mFileReopenMostRecentFileAction = nullptr;
    QAction *mFileReopenSubMenuSeparator1 = nullptr;
    QAction *mFileReopenSubMenuSeparator2 = nullptr;
    QAction *mFileClearReopenSubMenuAction = nullptr;

    QStringList mRecentFiles;

    QList<QAction *> mRecentFileActions;

    void updateNewModifiedSensitiveActions();

    void reopenFile(const QString &pFileName);

    void updateFileReopenMenu();

private slots:
    void newFile();

    void reopenRecentFile();
    void reopenMostRecentFile();
    void clearReopenSubMenu();
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
