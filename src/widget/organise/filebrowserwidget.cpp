#include "filebrowserwidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QMouseEvent>
#include <QSettings>

#define SETTINGS_COLUMNWIDTH "_ColumnWidth"
#define SETTINGS_INITIALPATH "_InitialPath"
#define SETTINGS_SORTCOLUMN  "_SortColumn"
#define SETTINGS_SORTORDER   "_SortOrder"

FileBrowserWidget::FileBrowserWidget(QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pParent)
{
    mFileSystemModel = new QFileSystemModel;

    // We want acces to the full file system

    mFileSystemModel->setRootPath("");

    // Set some properties for the file browser widget itself

#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);   // Remove the focus border since
                                                // it messes up our toolbar
#endif
    setModel(mFileSystemModel);   // Associate ourselves to mFileSystemModel
    setSortingEnabled(true);      // Allow sorting
    setUniformRowHeights(true);   // Everything ought to be of the same height,
                                  // so set this property to true since it can
                                  // only speed things up which can't harm!

    // Connection to keep track of the directory loading progress of
    // mFileSystemModel

    connect(mFileSystemModel, SIGNAL(directoryLoaded(const QString &)),
            this, SLOT(directoryLoaded(const QString &)));
}

FileBrowserWidget::~FileBrowserWidget()
{
    delete mFileSystemModel;
}

void FileBrowserWidget::loadSettings(const QSettings &pSettings,
                                     const QString &pKey)
{
    // Retrieve the width of each column

    mNeedDefaultColumnWidth = true;

    QString columnWidthKey;

    for (int i = 0; i < header()->count(); ++i) {
        columnWidthKey = pKey+SETTINGS_COLUMNWIDTH+QString::number(i);

        mNeedDefaultColumnWidth =     mNeedDefaultColumnWidth
                                  && !pSettings.contains(columnWidthKey);

        setColumnWidth(i, pSettings.value(columnWidthKey,
                                          columnWidth(i)).toInt());
    }

    // Retrieve the sorting information

    sortByColumn(pSettings.value(pKey+SETTINGS_SORTCOLUMN, 0).toInt(),
                 Qt::SortOrder(pSettings.value(pKey+SETTINGS_SORTORDER,
                                               Qt::AscendingOrder).toInt()));

    // Retrieve the initial path

    mInitialPath = pSettings.value(pKey+SETTINGS_INITIALPATH,
                                   QDir::homePath()).toString();
    QFileInfo initialPathFileInfo = mInitialPath;

    if (!initialPathFileInfo.exists()) {
        // The initial path doesn't exist, so just revert to the home path

        mInitialPathDir = QDir::homePath();
        mInitialPath    = "";
    } else {
        // The initial path exists, so retrieve some information about the
        // folder and/or file (depending on whether the initial path refers to
        // a file or not)
        // Note: indeed, should mInitialPath refer to a file, then to directly
        //       set the current index of the tree view to that of a file won't
        //       give us the expected behaviour (i.e. the parent folder being
        //       open and expanded, and the file selected), so instead one must
        //       set the current index to that of the parent folder and then
        //       select the file

        if (initialPathFileInfo.isDir()) {
            // We are dealing with a folder, so...

            mInitialPathDir = initialPathFileInfo.canonicalFilePath();
            mInitialPath    = "";
        } else {
            // We are dealing with a file, so...

            mInitialPathDir = initialPathFileInfo.canonicalPath();
            mInitialPath    = initialPathFileInfo.canonicalFilePath();
        }
    }

    // Set the current index to that of the folder (and file, if it exists) we
    // are interested in
    // Note: this will result in the directoryLoaded signal to be emitted and,
    //       us, to take advantage of it to scroll to the right directory/file

    setCurrentIndex(mFileSystemModel->index(mInitialPathDir));

    if (!mInitialPath.isEmpty())
        // The initial path is that of a file, so...

        setCurrentIndex(mFileSystemModel->index(mInitialPath));
}

void FileBrowserWidget::saveSettings(QSettings &pSettings, const QString &pKey)
{
    // Retrieve the width of each column

    for (int i = 0; i < header()->count(); ++i)
        pSettings.setValue(pKey+SETTINGS_COLUMNWIDTH+QString::number(i),
                           columnWidth(i));

    // Keep track of the sorting information

    pSettings.setValue(pKey+SETTINGS_SORTCOLUMN,
                       header()->sortIndicatorSection());
    pSettings.setValue(pKey+SETTINGS_SORTORDER, header()->sortIndicatorOrder());

    // Keep track of what will be our future initial folder/file path

    pSettings.setValue(pKey+SETTINGS_INITIALPATH,
                       mFileSystemModel->filePath(currentIndex()));
}

QSize FileBrowserWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void FileBrowserWidget::mousePressEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::RightButton)
        // We are pressing the right mouse button which may be used to display a
        // context menu and we don't want the row beneath the mouse to be
        // selected (in case it isn't already), so...

        return;
    else
        // We are not pressing the right mouse button, so carry on with the
        // default handling of the event

        QTreeView::mousePressEvent(pEvent);
}

void FileBrowserWidget::directoryLoaded(const QString &pPath)
{
    static bool needInitializing = true;

    if (needInitializing
        && (   ( mInitialPath.isEmpty() && mInitialPathDir.contains(pPath))
            || (!mInitialPath.isEmpty() && mInitialPath.contains(pPath)))) {
        // mFileSystemModel is still loading the initial path, so we try to
        // expand it and scroll to it, but first we process any pending event
        // (indeed, Windows doesn't need this, but Linux and Mac OS X definitely
        // do and it can't harm having it for all three environments, so...)

        qApp->processEvents();

        QModelIndex initialPathDirModelIndex = mFileSystemModel->index(mInitialPathDir);

        setExpanded(initialPathDirModelIndex, true);
        scrollTo(initialPathDirModelIndex);

        if (!mInitialPath.isEmpty()) {
            // The initial path is that of a file and it exists, so select it

            QModelIndex initialPathModelIndex = mFileSystemModel->index(mInitialPath);

            setExpanded(initialPathModelIndex, true);
            scrollTo(initialPathModelIndex);
        }

        // Set the default width of the columns so that it fits their contents

        if (mNeedDefaultColumnWidth) {
            header()->setResizeMode(QHeaderView::ResizeToContents);

            qApp->processEvents();
            // Note: this ensures that the columns actually get resized

            header()->setResizeMode(QHeaderView::Interactive);
        }
    } else {
        // We are done initializing, so...

        needInitializing = false;
    }
}

bool FileBrowserWidget::gotoFolder(const QString &pFolder, const bool &pExpand)
{
    // Set the current index to that of the home folder and expand the folder

    QModelIndex folderModelIndex = mFileSystemModel->index(pFolder);

    if (folderModelIndex != QModelIndex()) {
        // The folder exists, so we can go to it

        setCurrentIndex(folderModelIndex);

        if (pExpand)
            setExpanded(folderModelIndex, true);

        scrollTo(folderModelIndex);

        return true;
    } else {
        // The folder doesn't exist, so...

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

    gotoFolder(homeFolder(), pExpand);
}

QString FileBrowserWidget::path(const QModelIndex &pIndex)
{
    // Return the path for the given index

    return mFileSystemModel->filePath(pIndex);
}

QString FileBrowserWidget::currentPath()
{
    // Return the current path

    return mFileSystemModel->filePath(currentIndex());
}

bool FileBrowserWidget::currentPathVisible()
{
    // Return whether the current path is visible or not, i.e. whether all of
    // the parent nodes are expanded or not

    bool result = true;
    QModelIndex crtIndex = currentIndex();

    while (result && (crtIndex.parent() != QModelIndex())) {
        crtIndex = crtIndex.parent();

        if (!isExpanded(crtIndex))
            // The current parent is not expanded, meaning the current path
            // cannot be visible, so...

            result = false;
    }

    return result;
}
