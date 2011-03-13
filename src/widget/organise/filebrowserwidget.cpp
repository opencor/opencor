#include "filebrowserwidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QMouseEvent>
#include <QSettings>

static const QString SettingsColumnWidth = "_ColumnWidth";
static const QString SettingsInitialPath = "_InitialPath";
static const QString SettingsSortColumn  = "_SortColumn";
static const QString SettingsSortOrder   = "_SortOrder";

FileBrowserWidget::FileBrowserWidget(QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pParent),
    mNeedDefColWidth(true)
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

    QString columnWidthKey;

    for (int i = 0; i < header()->count(); ++i) {
        columnWidthKey = pKey+SettingsColumnWidth+QString::number(i);

        mNeedDefColWidth =     mNeedDefColWidth
                           && !pSettings.contains(columnWidthKey);

        setColumnWidth(i, pSettings.value(columnWidthKey,
                                          columnWidth(i)).toInt());
    }

    // Retrieve the sorting information

    sortByColumn(pSettings.value(pKey+SettingsSortColumn, 0).toInt(),
                 Qt::SortOrder(pSettings.value(pKey+SettingsSortOrder,
                                               Qt::AscendingOrder).toInt()));

    // Retrieve the initial path

    mInitPath = pSettings.value(pKey+SettingsInitialPath,
                                QDir::homePath()).toString();

    QFileInfo initPathFileInfo = mInitPath;

    if (!initPathFileInfo.exists()) {
        // The initial path doesn't exist, so just revert to the home path

        mInitPathDir = QDir::homePath();
        mInitPath    = "";
    } else {
        // The initial path exists, so retrieve some information about the
        // folder and/or file (depending on whether the initial path refers to
        // a file or not)
        // Note: indeed, should mInitPath refer to a file, then to directly set
        //       the current index of the tree view to that of a file won't give
        //       us the expected behaviour (i.e. the parent folder being open
        //       and expanded, and the file selected), so instead one must set
        //       the current index to that of the parent folder and then select
        //       the file

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

    // Set the current index to that of the folder (and file, if it exists) we
    // are interested in
    // Note: this will result in the directoryLoaded signal to be emitted and,
    //       us, to take advantage of it to scroll to the right directory/file

    setCurrentIndex(mFileSystemModel->index(mInitPathDir));

    if (!mInitPath.isEmpty())
        // The initial path is that of a file, so...

        setCurrentIndex(mFileSystemModel->index(mInitPath));
}

void FileBrowserWidget::saveSettings(QSettings &pSettings, const QString &pKey)
{
    // Retrieve the width of each column

    for (int i = 0; i < header()->count(); ++i)
        pSettings.setValue(pKey+SettingsColumnWidth+QString::number(i),
                           columnWidth(i));

    // Keep track of the sorting information

    pSettings.setValue(pKey+SettingsSortColumn,
                       header()->sortIndicatorSection());
    pSettings.setValue(pKey+SettingsSortOrder, header()->sortIndicatorOrder());

    // Keep track of what will be our future initial folder/file path

    pSettings.setValue(pKey+SettingsInitialPath,
                       mFileSystemModel->filePath(currentIndex()));
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
        // setting it to the file path of the current item

        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(pEvent);

        setToolTip(QDir::toNativeSeparators(mFileSystemModel->filePath(indexAt(QPoint(helpEvent->x(),
                                                                                      helpEvent->y())))));
    }

    // Default handling of the event

    return QTreeView::viewportEvent(pEvent);
}

void FileBrowserWidget::mousePressEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::RightButton)
        // We are pressing the right mouse button which may be used to display a
        // context menu and we don't want the row beneath the mouse to be
        // selected (in case it isn't already), so...

        pEvent->accept();
    else
        // We are not pressing the right mouse button, so carry on with the
        // default handling of the event

        QTreeView::mousePressEvent(pEvent);
}

void FileBrowserWidget::directoryLoaded(const QString &pPath)
{
    static bool needInitializing = true;

    if (needInitializing
        && (   ( mInitPath.isEmpty() && mInitPathDir.contains(pPath))
            || (!mInitPath.isEmpty() && mInitPath.contains(pPath)))) {
        // mFileSystemModel is still loading the initial path, so we try to
        // expand it and scroll to it, but first we process any pending event
        // (indeed, Windows doesn't need this, but Linux and Mac OS X definitely
        // do and it can't harm having it for all three environments, so...)

        qApp->processEvents();

        QModelIndex initPathDirModelIndex = mFileSystemModel->index(mInitPathDir);

        setExpanded(initPathDirModelIndex, true);
        scrollTo(initPathDirModelIndex);

        if (!mInitPath.isEmpty()) {
            // The initial path is that of a file and it exists, so select it

            QModelIndex initPathModelIndex = mFileSystemModel->index(mInitPath);

            setExpanded(initPathModelIndex, true);
            scrollTo(initPathModelIndex);
        }

        // Set the default width of the columns so that it fits their contents

        if (mNeedDefColWidth) {
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

void FileBrowserWidget::showCurrentPath()
{
    // Show the current path by scrolling to it

    scrollTo(currentIndex());
}

bool FileBrowserWidget::gotoPath(const QString &pPath, const bool &pExpand)
{
    // Set the current index to that of the provided path

    QModelIndex pathModelIndex = mFileSystemModel->index(pPath);

    if (pathModelIndex != QModelIndex()) {
        // The path exists, so we can go to it

        setCurrentIndex(pathModelIndex);

        if (pExpand)
            setExpanded(pathModelIndex, true);

        scrollTo(pathModelIndex);

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

    return mFileSystemModel->filePath(currentIndex());
}

QString FileBrowserWidget::currentPathDir()
{
    // Return the directory of the current path

    QString crtIndexPath  = mFileSystemModel->filePath(currentIndex());
    QFileInfo crtIndexFileInfo  = crtIndexPath;

    return crtIndexFileInfo.isDir()?
               crtIndexPath:
               crtIndexFileInfo.dir().canonicalPath();
}

QString FileBrowserWidget::currentPathParent()
{
    // Return the current path parent, if any

    QModelIndex crtIndexParent = currentIndex().parent();

    return (crtIndexParent != QModelIndex())?
               mFileSystemModel->filePath(crtIndexParent):
               "";
}

bool FileBrowserWidget::isCurrentPathVisible()
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

bool FileBrowserWidget::isCurrentPathDirWritable()
{
    // Return whether the directory of the current path is writable or not

    return QFileInfo(currentPathDir()).isWritable();
}

bool FileBrowserWidget::isCurrentPathWritable()
{
    // Return whether the current path is writable or not

    return QFileInfo(currentPath()).isWritable();
}
