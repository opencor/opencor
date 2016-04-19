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
// File browser widget
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

Q_SIGNALS:
    void filesOpenRequested(const QStringList &pFileNames);

    void notHomeFolder(const bool &pNotHomeFolder);
    void goToParentFolderEnabled(const bool &pEnabled);

    void goToPreviousFileOrFolderEnabled(const bool &pEnabled);
    void goToNextFileOrFolderEnabled(const bool &pEnabled);

private Q_SLOTS:
    void itemChanged(const QModelIndex &, const QModelIndex &pPrevItem);

    void directoryLoaded(const QString &pPath);
};

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
