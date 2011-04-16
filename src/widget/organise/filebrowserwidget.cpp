#include "filebrowserwidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QHelpEvent>
#include <QSettings>
#include <QUrl>

FileBrowserModel::FileBrowserModel(QObject *pParent)
    : QFileSystemModel(pParent)
{
    // We want acces to the full file system

    setRootPath("");
}

Qt::ItemFlags FileBrowserModel::flags(const QModelIndex &pIndex) const
{
    // Determine the draggable/droppable state of the item referenced by the
    // index

    Qt::ItemFlags flags = QFileSystemModel::flags(pIndex);

    // Don't allow the item to be dragged if it's a folder

    if (QFileInfo(filePath(pIndex)).isDir())
        flags &= ~Qt::ItemIsDragEnabled;

    // We are all done, so...

    return flags;
}

FileBrowserWidget::FileBrowserWidget(const QString &pName, QWidget *pParent) :
    TreeView(pName, this, pParent),
    mNeedDefColWidth(true),
    mInitPathDirs(QStringList()),
    mInitPathDir(QString()),
    mInitPath(QString())
{
    // Create an instance of the file system model that we want to view

    mDataModel = new FileBrowserModel;

    // Set some properties for the file browser widget itself

    setDragDropMode(QAbstractItemView::DragOnly);
    setModel(mDataModel);
    setSortingEnabled(true);

    // Connection to keep track of the directory loading progress of mDataModel

    connect(mDataModel, SIGNAL(directoryLoaded(const QString &)),
            this, SLOT(directoryLoaded(const QString &)));
}

FileBrowserWidget::~FileBrowserWidget()
{
    delete mDataModel;
}

static const QString SettingsColumnWidth = "ColumnWidth";
static const QString SettingsInitialPath = "InitialPath";
static const QString SettingsSortColumn  = "SortColumn";
static const QString SettingsSortOrder   = "SortOrder";

void FileBrowserWidget::loadSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Let people know that we are starting loading the settings

        emit beginLoadingSettings();

        // Retrieve the width of each column

        QString columnWidthKey;

        for (int i = 0; i < header()->count(); ++i) {
            columnWidthKey = SettingsColumnWidth+QString::number(i);

            mNeedDefColWidth =     mNeedDefColWidth
                               && !pSettings.contains(columnWidthKey);

            setColumnWidth(i, pSettings.value(columnWidthKey,
                                              columnWidth(i)).toInt());
        }

        // Retrieve the sorting information

        sortByColumn(pSettings.value(SettingsSortColumn, 0).toInt(),
                     Qt::SortOrder(pSettings.value(SettingsSortOrder,
                                                   Qt::AscendingOrder).toInt()));

        // Retrieve the initial path

        mInitPath = pSettings.value(SettingsInitialPath,
                                    QDir::homePath()).toString();

        QFileInfo initPathFileInfo = mInitPath;

        if (!initPathFileInfo.exists()) {
            // The initial path doesn't exist, so just revert to the home path

            mInitPathDir = QDir::homePath();
            mInitPath    = "";
        } else {
            // The initial path exists, so retrieve some information about the
            // folder and/or file (depending on whether the initial path refers
            // to a file or not)
            // Note: indeed, should mInitPath refer to a file, then to directly
            //       set the current index of the tree view to that of a file
            //       won't give us the expected behaviour (i.e. the parent
            //       folder being open and expanded, and the file selected), so
            //       instead one must set the current index to that of the
            //       parent folder and then select the file

            if (initPathFileInfo.isDir()) {
                // We are dealing with a folder, so...

                mInitPathDir = initPathFileInfo.canonicalFilePath();
                mInitPath    = "";
            } else {
                // We are dealing with a file, so...

                mInitPathDir = initPathFileInfo.canonicalPath();
                mInitPath    = initPathFileInfo.canonicalFilePath();
            }
        }

        // On Windows, if mInitPathDir refers to the root of a particular drive
        // (e.g. the C: drive), then it won't include a trailing separator (i.e.
        // "C:" instead of "C:/") and this causes problems below (when wanting
        // to retrieve the different folders), so we must make sure that that
        // mInitPathDir contains a trailing separator
        // Note: this is clearly not needed on Linux and Mac OS X, but it
        //       doesn't harm doing it for these platforms too, so...

        mInitPathDir = QDir(mInitPathDir+QDir::separator()).canonicalPath();

        // Create a list of the different folders that need to be loaded to
        // consider that the loading of the settings is finished (see just below
        // and the directoryLoaded slot)

        mInitPathDirs.append(mInitPathDir);

        QDir initPathDir = mInitPathDir;

        while (initPathDir.cdUp())
            mInitPathDirs.append(initPathDir.canonicalPath());

        // Set the current index to that of the folder (and file, if it exists)
        // we are interested in
        // Note: this will result in the directoryLoaded signal being emitted
        //       and, us, to take advantage of it to scroll to the right
        //       directory/file

        setCurrentIndex(mDataModel->index(mInitPathDir));

        if (!mInitPath.isEmpty())
            // The initial path is that of a file, so...

            setCurrentIndex(mDataModel->index(mInitPath));
    pSettings.endGroup();
}

void FileBrowserWidget::saveSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Retrieve the width of each column

        for (int i = 0; i < header()->count(); ++i)
            pSettings.setValue(SettingsColumnWidth+QString::number(i),
                               columnWidth(i));

        // Keep track of the sorting information

        pSettings.setValue(SettingsSortColumn,
                           header()->sortIndicatorSection());
        pSettings.setValue(SettingsSortOrder, header()->sortIndicatorOrder());

        // Keep track of what will be our future initial folder/file path

        pSettings.setValue(SettingsInitialPath,
                           mDataModel->filePath(currentIndex()));
    pSettings.endGroup();
}

QSize FileBrowserWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

bool FileBrowserWidget::viewportEvent(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ToolTip) {
        // We need to show a tool tip, so make sure that it's up to date by
        // setting it to the path of the current item

        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(pEvent);

        setToolTip(QDir::toNativeSeparators(mDataModel->filePath(indexAt(helpEvent->pos()))));
    }

    // Default handling of the event

    return TreeView::viewportEvent(pEvent);
}

QStringList FileBrowserWidget::selectedFiles()
{
    // Retrieve all the files that are currently selected
    // Note: if there is a non-file among the selected items, then we return an
    //       empty list

    QStringList selectedFiles;
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    for (int i = 0; i < selectedIndexes.count(); ++i) {
        QString fileName = pathOf(selectedIndexes.at(i));
        QFileInfo fileInfo = fileName;

        if (fileInfo.isFile()) {
            if (fileInfo.isSymLink()) {
                // The current item is a symbolic link, so retrieve its target
                // and check that it exists, and if it does then add it to the
                // list

                fileName = fileInfo.symLinkTarget();

                if (QFileInfo(fileName).exists())
                    selectedFiles.append(fileName);
            } else {
                // The current item is a file, so just add to the list

                selectedFiles.append(fileName);
            }
        } else {
            // The current item is not a file, so return an empty list

            return QStringList();
        }
    }

    // Remove duplicates (which might be present as a result of symbolic links)

    selectedFiles.removeDuplicates();

    return selectedFiles;
}

void FileBrowserWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    TreeView::keyPressEvent(pEvent);

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    QStringList crtSelectedFiles = selectedFiles();

    if (   crtSelectedFiles.count()
#ifndef Q_WS_MAC
        && (   (pEvent->key() == Qt::Key_Enter)
            || (pEvent->key() == Qt::Key_Return)))
#else
        && (   (pEvent->modifiers() & Qt::ControlModifier)
            && (pEvent->key() == Qt::Key_Down)))
#endif
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit filesOpened(crtSelectedFiles);
}

void FileBrowserWidget::directoryLoaded(const QString &pPath)
{
    static bool needInitializing = true;

    if (needInitializing
        && (   ( mInitPath.isEmpty() && mInitPathDir.contains(pPath))
            || (!mInitPath.isEmpty() && mInitPath.contains(pPath)))) {
        // mDataModel is still loading the initial path, so we try to expand it
        // and scroll to it, but first we process any pending event (indeed,
        // though Windows doesn't need this, Linux and Mac OS X definitely do
        // and it can't harm having it for all three environments, so...)

        qApp->processEvents();

        QModelIndex initPathDirModelIndex = mDataModel->index(mInitPathDir);

        setExpanded(initPathDirModelIndex, true);
        scrollTo(initPathDirModelIndex);
        setCurrentIndex(initPathDirModelIndex);

        if (!mInitPath.isEmpty()) {
            // The initial path is that of a file and it exists, so select it

            QModelIndex initPathModelIndex = mDataModel->index(mInitPath);

            scrollTo(initPathModelIndex);
            setCurrentIndex(initPathModelIndex);
        }

        // Set the default width of the columns, if needed, so that it fits
        // their contents

        if (mNeedDefColWidth) {
            header()->setResizeMode(QHeaderView::ResizeToContents);

            qApp->processEvents();
            // Note: this ensures that the columns actually get resized

            header()->setResizeMode(QHeaderView::Interactive);
        }

        // Remove the loaded directory from mInitPathDirs

        mInitPathDirs.removeOne(QDir(pPath+QDir::separator()).canonicalPath());
        // Note: it is very important, on Windows, to add QDir::separator() to
        //       pPath. Indeed, say that mInitPathDir is on the C: drive, then
        //       eventually pPath will be equal to "C:" while mInitPathDirs will
        //       know about "C:/", so... (note: this is clearly not needed on
        //       Linux and Mac OS X, but it doesn't harm adding it for these
        //       platforms too, so...)

        // Check whether or not we are done initializing (which is when
        // mInitPathDirs is empty) and, if so, let people know that
        // this means we are done loading the settings

        if (mInitPathDirs.isEmpty()) {
            emit endLoadingSettings();

            needInitializing = false;
        }
    }
}

bool FileBrowserWidget::gotoPath(const QString &pPath, const bool &pExpand)
{
    // Set the current index to that of the provided path

    QModelIndex pathModelIndex = mDataModel->index(pPath);

    if (pathModelIndex != QModelIndex()) {
        // The path exists, so we can go to it

        if (pExpand)
            setExpanded(pathModelIndex, true);

        setCurrentIndex(pathModelIndex);

        return true;
    } else {
        // The path doesn't exist, so...

        return false;
    }
}

QString FileBrowserWidget::homeFolder()
{
    // Return the path to the home folder

    return QDir::homePath();
}

void FileBrowserWidget::gotoHomeFolder(const bool &pExpand)
{
    // Go to the home folder

    gotoPath(QDir::homePath(), pExpand);
}

QString FileBrowserWidget::currentPath()
{
    // Return the current path

    return mDataModel->filePath(currentIndex());
}

QString FileBrowserWidget::currentPathDir()
{
    // Return the directory of the current path

    QString crtIndexPath = mDataModel->filePath(currentIndex());
    QFileInfo crtIndexFileInfo = crtIndexPath;

    return crtIndexFileInfo.isDir()?
               crtIndexPath:
               crtIndexFileInfo.dir().canonicalPath();
}

QString FileBrowserWidget::currentPathParent()
{
    // Return the current path parent, if any

    QModelIndex crtIndexParent = currentIndex().parent();

    return (crtIndexParent != QModelIndex())?
               mDataModel->filePath(crtIndexParent):
               "";
}

QString FileBrowserWidget::pathOf(const QModelIndex &pIndex)
{
    // Return the file path of pIndex, if it exists

    return (pIndex != QModelIndex())?
               mDataModel->filePath(pIndex):
               "";
}
