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
// File Browser window widget
//==============================================================================

#pragma once

//==============================================================================

#include "filebrowserwindowmodel.h"
#include "treeviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

class FileBrowserWindowWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindowWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    QString currentPath() const;

    void goToHomeFolder();
    void goToParentFolder();

    void goToPreviousFileOrFolder();
    void goToNextFileOrFolder();

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mouseMoveEvent(QMouseEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);
    virtual bool viewportEvent(QEvent *pEvent);

private:
    FileBrowserWindowModel *mModel;

    bool mNeedDefColWidth;

    QStringList mInitPathDirs;
    QString mInitPathDir;
    QString mInitPath;

    QStringList mPreviousItems;
    QStringList mNextItems;

    void goToPath(const QString &pPath, const bool &pExpand = false);

    QString currentPathParent() const;

    QString pathOf(const QModelIndex &pIndex) const;

    void deselectFolders() const;

    QStringList selectedFiles() const;

    void emitItemChangedRelatedSignals();

    void updateItems(const QString &pItemPath, QStringList &pItems) const;
    void goToOtherItem(QStringList &pItems, QStringList &pOtherItems);

signals:
    void filesOpenRequested(const QStringList &pFileNames);

    void notHomeFolder(const bool &pNotHomeFolder);
    void goToParentFolderEnabled(const bool &pEnabled);

    void goToPreviousFileOrFolderEnabled(const bool &pEnabled);
    void goToNextFileOrFolderEnabled(const bool &pEnabled);

private slots:
    void itemChanged(const QModelIndex &pNewIndex,
                     const QModelIndex &pOldIndex);

    void directoryLoaded(const QString &pPath);
};

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
