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

#include "corecliutils.h"
#include "filebrowserwindowwidget.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHeaderView>
#include <QHelpEvent>
#include <QModelIndex>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

FileBrowserWindowWidget::FileBrowserWindowWidget(QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Customise ourselves

    mModel = new FileBrowserWindowModel(this);

    setDragDropMode(QAbstractItemView::DragOnly);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    setFrameShape(QFrame::StyledPanel);
#else
    setFrameShape(QFrame::Panel);
#endif
    setModel(mModel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSortingEnabled(true);

    header()->setSectionsMovable(false);

    // Some connections

    connect(selectionModel(), &QItemSelectionModel::currentChanged,
            this, &FileBrowserWindowWidget::itemChanged);

    connect(mModel, &FileBrowserWindowModel::directoryLoaded,
            this, &FileBrowserWindowWidget::directoryLoaded);
}

//==============================================================================

static const char *SettingsColumnWidth = "ColumnWidth%1";
static const char *SettingsInitialPath = "InitialPath";
static const char *SettingsSortColumn  = "SortColumn";
static const char *SettingsSortOrder   = "SortOrder";

//==============================================================================

void FileBrowserWindowWidget::loadSettings(QSettings &pSettings)
{
    // We are about to begin loading the settings, so we don't want to keep
    // track of the change of item

    disconnect(selectionModel(), &QItemSelectionModel::currentChanged,
               this, &FileBrowserWindowWidget::itemChanged);

    // Retrieve the width of each column

    QString columnWidthKey;

    for (int i = 0, iMax = header()->count(); i < iMax; ++i) {
        columnWidthKey = QString(SettingsColumnWidth).arg(i);

        mNeedDefColWidth =     mNeedDefColWidth
                           && !pSettings.contains(columnWidthKey);

        setColumnWidth(i, pSettings.value(columnWidthKey, columnWidth(i)).toInt());
    }

    // Retrieve the sorting information

    sortByColumn(pSettings.value(SettingsSortColumn, 0).toInt(),
                 Qt::SortOrder(pSettings.value(SettingsSortOrder, Qt::AscendingOrder).toInt()));

    // Retrieve the initial path

    mInitPath = pSettings.value(SettingsInitialPath, QDir::homePath()).toString();

    QFileInfo initPathFileInfo = mInitPath;

    if (!initPathFileInfo.exists()) {
        // The initial path doesn't exist, so just revert to the home path

        mInitPathDir = QDir::homePath();
        mInitPath = "";
    } else {
        // The initial path exists, so retrieve some information about the
        // folder and/or file (depending on whether the initial path refers to a
        // file)
        // Note: indeed, should mInitPath refer to a file, then to directly set
        //       the current index of the tree view widget to that of a file
        //       won't give us the expected behaviour (i.e. the parent folder
        //       being open and expanded, and the file selected), so instead
        //       one must set the current index to that of the parent folder
        //       and then select the file

        if (initPathFileInfo.isDir()) {
            // We are dealing with a folder

            mInitPathDir = initPathFileInfo.canonicalFilePath();
            mInitPath = "";
        } else {
            // We are dealing with a file

            mInitPathDir = initPathFileInfo.canonicalPath();
            mInitPath = initPathFileInfo.canonicalFilePath();
        }
    }

    // On Windows, if mInitPathDir refers to the root of a particular drive
    // (e.g. the C: drive), then it won't include a trailing separator (i.e.
    // "C:" instead of "C:/") and this causes problems below (when wanting to
    // retrieve the different folders), so we must make sure that mInitPathDir
    // contains a trailing separator
    // Note: this is clearly not needed on Linux and macOS, but it doesn't
    //       harm doing it for these platforms too...

    mInitPathDir = QDir(mInitPathDir+"/").canonicalPath();

    // Create a list of the different folders that need to be loaded to consider
    // that the loading of the settings is finished (see just below and the
    // directoryLoaded slot)

    mInitPathDirs << mInitPathDir;

    QDir initPathDir = mInitPathDir;

    while (initPathDir.cdUp()) {
        mInitPathDirs << initPathDir.canonicalPath();
    }

    // Set the current index to that of the folder (and file, if it exists) in
    // which we are interested
    // Note: this will result in the directoryLoaded signal being emitted and,
    //       us, to take advantage of it to scroll to the right
    //       directory/file...

    setCurrentIndex(mModel->index(mInitPathDir));

    if (!mInitPath.isEmpty()) {
        setCurrentIndex(mModel->index(mInitPath));
    }

    // Make sure that the current path is expanded
    // Note: this is important in case the current path is that of the C: drive
    //       or the root of the file system, in which case this won't result in
    //       the directoryLoaded signal being emitted

    if (!isExpanded(currentIndex())) {
        setExpanded(currentIndex(), true);
    }

    // Make sure that the current path is visible
    // Note: indeed, to process pending events only in directoryLoaded() is not
    //       good enough (anymore?!) on macOS...

    processEvents();

    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emitItemChangedRelatedSignals();
}

//==============================================================================

void FileBrowserWindowWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of the width of each column

    for (int i = 0, iMax = header()->count(); i < iMax; ++i) {
        pSettings.setValue(QString(SettingsColumnWidth).arg(i), columnWidth(i));
    }

    // Keep track of the sorting information

    pSettings.setValue(SettingsSortColumn, header()->sortIndicatorSection());
    pSettings.setValue(SettingsSortOrder, header()->sortIndicatorOrder());

    // Keep track of what will be our future initial folder/file path

    pSettings.setValue(SettingsInitialPath, mModel->filePath(currentIndex()));
}

//==============================================================================

QStringList FileBrowserWindowWidget::selectedFiles() const
{
    // Retrieve all the files that are currently selected
    // Note: if there is a non-file among the selected items, then we return an
    //       empty list

    QStringList res;
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i) {
        QString fileName = pathOf(selectedIndexes[i]);
        QFileInfo fileInfo = fileName;

        if (fileInfo.isFile()) {
            if (fileInfo.isSymLink()) {
                // The current item is a symbolic link, so retrieve its target
                // and check that it exists, and if it does then add it to the
                // list

                fileName = fileInfo.symLinkTarget();

                if (QFile::exists(fileName)) {
                    res << fileName;
                }
            } else {
                // The current item is a file, so just add to the list

                res << fileName;
            }
        }
    }

    // Remove duplicates (which might be present as a result of symbolic links)

    res.removeDuplicates();

    return res;
}

//==============================================================================

void FileBrowserWindowWidget::deselectFolders() const
{
    // Check whether more than one item is selected with the view of dragging
    // them and, if so, unselect any folder item since we don't allow them to be
    // dragged

    if (selectionModel()->selectedRows().count() > 1) {
        QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

        for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i) {
            QFileInfo fileInfo = pathOf(selectedIndexes[i]);

            if (fileInfo.isDir() || !fileInfo.exists()) {
                // Either we are dealing with a directory or an entry that
                // doesn't actually exist (e.g. on Windows, it could be a drive
                // for a removable device with the device not being present)

                selectionModel()->select(selectedIndexes[i],
                                         QItemSelectionModel::Deselect);
            }
        }
    }
}

//==============================================================================

void FileBrowserWindowWidget::emitItemChangedRelatedSignals()
{
    // Let the user know whether the path of the new item is that of our home
    // folder, as well as whether we could go to the parent item

    emit homeFolder(currentPath() == QDir::homePath());
    emit goToParentFolderEnabled(!currentPathParent().isEmpty());

    // Let the user know whether we can go to the previous/next file/folder

    emit goToPreviousFileOrFolderEnabled(!mPreviousItems.isEmpty());
    emit goToNextFileOrFolderEnabled(!mNextItems.isEmpty());
}

//==============================================================================

void FileBrowserWindowWidget::updateItems(const QString &pItemPath,
                                          QStringList &pItems) const
{
    // Remove any instance of pItemPath in pItems

    pItems.removeAll(pItemPath);

    // Because of the above, we may have two or more consective identital items
    // in the list, so we must reduce that to one

    if (pItems.count() > 1) {
        QStringList newItems;
        QString prevItem = pItems.first();

        newItems << prevItem;

        for (int i = 1, iMax = pItems.count(); i < iMax; ++i) {
            QString crtItem = pItems[i];

            if (crtItem != prevItem) {
                // The current and previous items are different, so we want to
                // keep track of it and add it to our new list

                newItems << crtItem;

                prevItem = crtItem;
            }
        }

        // Update the old list of items with our new one

        pItems = newItems;
    }
}

//==============================================================================

void FileBrowserWindowWidget::goToOtherItem(QStringList &pItems,
                                            QStringList &pOtherItems)
{
    if (pItems.isEmpty()) {
        return;
    }

    // Go to the previous/next item and move the last item from our list of
    // items to our list of other items

    // First, we must stop keeping track of the change of item otherwise it's
    // going to mess things up

    disconnect(selectionModel(), &QItemSelectionModel::currentChanged,
               this, &FileBrowserWindowWidget::itemChanged);

    // Retrieve our current path and add it to our list of other items

    QString crtPath = currentPath();

    pOtherItems << crtPath;

    // Retrieve the new path and check whether it still exists

    QString newItemPath = pItems.last();

    while (!pItems.isEmpty() && !QFile::exists(newItemPath)) {
        // The new item doesn't exist anymore, so remove it from our list of
        // items and other items

        updateItems(newItemPath, pItems);
        updateItems(newItemPath, pOtherItems);

        // Try with the next new item

        if (!pItems.isEmpty()) {
            // The list is not empty, so make the last item our next new item

            newItemPath = pItems.last();

            // The next new item cannot, however, be the same as the current
            // path

            while (!pItems.isEmpty() && (newItemPath == crtPath)) {
                pItems.removeLast();

                if (!pItems.isEmpty()) {
                    newItemPath = pItems.last();
                } else {
                    newItemPath = "";
                }
            }
        } else {
            newItemPath = "";
        }
    }

    if (!newItemPath.isEmpty()) {
        // We have a valid new item, so go to its path and remove it from our
        // list of items

        goToPath(newItemPath);

        pItems.removeLast();
    }

    // Make sure that the last item in the lists of items and other items isn't
    // that of the current path (this may happen if some items got deleted by
    // the user)

    crtPath = currentPath();

    if (!pItems.isEmpty() && (pItems.last() == crtPath)) {
        pItems.removeLast();
    }

    if (!pOtherItems.isEmpty() && (pOtherItems.last() == crtPath)) {
        pOtherItems.removeLast();
    }

    // Now that we are done, we can once again keep track of the change of item

    connect(selectionModel(), &QItemSelectionModel::currentChanged,
            this, &FileBrowserWindowWidget::itemChanged);

    // Let the user know about a few item changed related things

    emitItemChangedRelatedSignals();
}

//==============================================================================

void FileBrowserWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::keyPressEvent(pEvent);

    // Deselect folders, if required

    deselectFolders();

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    QStringList crtSelectedFiles = selectedFiles();

    if (   !crtSelectedFiles.isEmpty()
        &&  ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit filesOpenRequested(crtSelectedFiles);
    }
}

//==============================================================================

void FileBrowserWindowWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::mouseMoveEvent(pEvent);

    // Deselect folders, if required

    deselectFolders();
}

//==============================================================================

void FileBrowserWindowWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::mousePressEvent(pEvent);

    // Deselect folders, if required

    deselectFolders();
}

//==============================================================================

bool FileBrowserWindowWidget::viewportEvent(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ToolTip) {
        // We need to show a tool tip, so make sure that it's up to date by
        // setting it to the path of the current item

        auto helpEvent = static_cast<QHelpEvent *>(pEvent);

        setToolTip(QDir::toNativeSeparators(Core::canonicalFileName(mModel->filePath(indexAt(helpEvent->pos())))));
    }

    // Default handling of the event

    return TreeViewWidget::viewportEvent(pEvent);
}

//==============================================================================

void FileBrowserWindowWidget::itemChanged(const QModelIndex &pNewIndex,
                                          const QModelIndex &pOldIndex)
{
    Q_UNUSED(pNewIndex)

    // A new item has been selected, so we need to keep track of the old one in
    // case we want to go back to it

    mPreviousItems << pathOf(pOldIndex);

    // Reset the list of next items since that list doesn't make sense anymore

    mNextItems.clear();

    // Let the user know about a few item changed related things

    emitItemChangedRelatedSignals();
}

//==============================================================================

void FileBrowserWindowWidget::directoryLoaded(const QString &pPath)
{
    static bool needInitializing = true;

    if (needInitializing
        && (   ( mInitPath.isEmpty() && mInitPathDir.contains(pPath))
            || (!mInitPath.isEmpty() && mInitPath.contains(pPath)))) {
        // mModel is still loading the initial path, so we try to expand it and
        // scroll to it, but first we process pending events (indeed, though
        // Windows doesn't need this, Linux and macOS definitely do and it can't
        // harm having it for all three environments)

        processEvents();

        QModelIndex initPathDirIndex = mModel->index(mInitPathDir);

        setExpanded(initPathDirIndex, true);
        scrollTo(initPathDirIndex);
        setCurrentIndex(initPathDirIndex);

        if (!mInitPath.isEmpty()) {
            // The initial path is that of a file and it exists, so select it

            QModelIndex initPathIndex = mModel->index(mInitPath);

            scrollTo(initPathIndex);
            setCurrentIndex(initPathIndex);
        }

        // Set the default width of the columns, if needed, so that it fits
        // their contents

        if (mNeedDefColWidth) {
            resizeColumnsToContents();
        }

        // Remove the loaded directory from mInitPathDirs

        mInitPathDirs.removeOne(QDir(pPath+"/").canonicalPath());
        // Note #1: it is very important, on Windows, to add "/" to pPath.
        //          Indeed, say that mInitPathDir is on the C: drive, then
        //          eventually pPath will be equal to "C:" while mInitPathDirs
        //          will know about "C:/"...
        // Note #2: this is clearly not needed on Linux and macOS, but it
        //          doesn't harm adding it for these platforms too...

        // Check whether we are done initializing

        if (mInitPathDirs.isEmpty()) {
            // We are now done loading the settings for the file browser widget,
            // so we can now keep track of the change of item

            connect(selectionModel(), &QItemSelectionModel::currentChanged,
                    this, &FileBrowserWindowWidget::itemChanged);

            needInitializing = false;
        }
    }
}

//==============================================================================

void FileBrowserWindowWidget::goToPath(const QString &pPath, bool pExpand)
{
    // Set the current index to that of the provided path

    QModelIndex pathIndex = mModel->index(pPath);

    if ((pathIndex.isValid()) && (pathIndex != currentIndex())) {
        // The path exists, so we can go to it

        if (pExpand) {
            setExpanded(pathIndex, true);
        }

        setCurrentIndex(pathIndex);
    }
}

//==============================================================================

void FileBrowserWindowWidget::goToHomeFolder()
{
    // Go to the home folder

    goToPath(QDir::homePath(), true);
}

//==============================================================================

void FileBrowserWindowWidget::goToParentFolder()
{
    // Go to the parent folder

    goToPath(currentPathParent());
}

//==============================================================================

void FileBrowserWindowWidget::goToPreviousFileOrFolder()
{
    // Go to the previous file/folder

    goToOtherItem(mPreviousItems, mNextItems);
}

//==============================================================================

void FileBrowserWindowWidget::goToNextFileOrFolder()
{
    // Go to the next file/folder

    goToOtherItem(mNextItems, mPreviousItems);
}

//==============================================================================

QString FileBrowserWindowWidget::currentPath() const
{
    // Return the current path

    return mModel->filePath(currentIndex());
}

//==============================================================================

QString FileBrowserWindowWidget::currentPathParent() const
{
    // Return the current path parent, if any

    QModelIndex crtIndexParent = currentIndex().parent();

    return crtIndexParent.isValid()?mModel->filePath(crtIndexParent):QString();
}

//==============================================================================

QString FileBrowserWindowWidget::pathOf(const QModelIndex &pIndex) const
{
    // Return the file path of pIndex, if it exists

    return pIndex.isValid()?mModel->filePath(pIndex):QString();
}

//==============================================================================

} // namespace FileBrowserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
