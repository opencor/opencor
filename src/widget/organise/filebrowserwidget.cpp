#include "filebrowserwidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QSettings>

#define SETTINGS_COLUMNWIDTH "_ColumnWidth"
#define SETTINGS_CURRENTPATH "_CurrentPath"
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

void FileBrowserWidget::retranslateUi()
{
    // Nothing to do for now...
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

    // Retrieve the current path

    mCurrentPath = pSettings.value(pKey+SETTINGS_CURRENTPATH,
                                   QDir::homePath()).toString();
    QFileInfo currentPathFileInfo = mCurrentPath;

    if (!currentPathFileInfo.exists()) {
        // The current path doesn't exist, so just revert to the home path

        mCurrentPathDir = QDir::homePath();
        mCurrentPath    = "";
    } else {
        // The current path exists, so retrieve some information about the
        // folder and/or file (depending on whether the current path refers to
        // a file or not)
        // Note: indeed, should mCurrentPath refer to a file, then to directly
        //       set the current index of the tree view to that of a file won't
        //       give us the expected behaviour (i.e. the parent folder being
        //       open and expanded, and the file selected), so instead one must
        //       set the current index to that of the parent folder and then
        //       select the file

        if (currentPathFileInfo.isDir()) {
            // We are dealing with a folder, so...

            mCurrentPathDir = currentPathFileInfo.canonicalFilePath();
            mCurrentPath    = "";
        } else {
            // We are dealing with a file, so...

            mCurrentPathDir = currentPathFileInfo.canonicalPath();
            mCurrentPath    = currentPathFileInfo.canonicalFilePath();
        }
    }

    // Set the current index to that of the folder (and file, if it exists) we
    // are interested in
    // Note: this will result in the directoryLoaded signal to be emitted and,
    //       us, to take advantage of it to scroll to the right directory/file

    setCurrentIndex(mFileSystemModel->index(mCurrentPathDir));

    if (!mCurrentPath.isEmpty())
        // The current path is that of a file, so...

        setCurrentIndex(mFileSystemModel->index(mCurrentPath));
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

    // Keep track of the current folder/file path

    pSettings.setValue(pKey+SETTINGS_CURRENTPATH,
                       mFileSystemModel->filePath(currentIndex()));
}

QSize FileBrowserWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void FileBrowserWidget::directoryLoaded(const QString &pPath)
{
    static bool needInitializing = true;

    if (needInitializing
        && (   ( mCurrentPath.isEmpty() && mCurrentPathDir.contains(pPath))
            || (!mCurrentPath.isEmpty() && mCurrentPath.contains(pPath)))) {
        // mFileSystemModel is still loading the current path, so we try to
        // expand it and scroll to it, but first we process any pending event
        // (indeed, Windows doesn't need this, but Linux and Mac OS X definitely
        // do and it can't harm having it for all three environments, so...)

        qApp->processEvents();

        QModelIndex currentPathDirModelIndex = mFileSystemModel->index(mCurrentPathDir);

        setExpanded(currentPathDirModelIndex, true);
        scrollTo(currentPathDirModelIndex);

        if (!mCurrentPath.isEmpty()) {
            // The current path is that of a file and it exists, so select it

            QModelIndex currentPathModelIndex = mFileSystemModel->index(mCurrentPath);

            setExpanded(currentPathModelIndex, true);
            scrollTo(currentPathModelIndex);
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

QString FileBrowserWidget::homeFolder()
{
    // Return the path to the home folder

    return QDir::homePath();
}

void FileBrowserWidget::gotoHomeFolder()
{
    // Set the current index to that of the home folder and expand the folder

    QModelIndex homePathModelIndex = mFileSystemModel->index(homeFolder());

    setCurrentIndex(homePathModelIndex);
    setExpanded(homePathModelIndex, true);
    scrollTo(homePathModelIndex);
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
            // The current parent is not expanded, so the current path cannot be
            // visible, so...

            result = false;
    }

    return result;
}
