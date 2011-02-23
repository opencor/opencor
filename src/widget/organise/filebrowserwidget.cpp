#include "filebrowserwidget.h"

#include <QDesktopServices>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QSettings>

#define SETTINGS_SORTCOLUMN  "_SortColumn"
#define SETTINGS_SORTORDER   "_SortOrder"
#define SETTINGS_CURRENTPATH "_CurrentPath"

FileBrowserWidget::FileBrowserWidget(QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pParent)
{
    mFileSystemModel = new QFileSystemModel;

    // We want acces to the full file system

    mFileSystemModel->setRootPath("");

    // Set some properties for the file browser widget itself

    setModel(mFileSystemModel);   // Associate ourselves to mFileSystemModel
    setSortingEnabled(true);      // Allow sorting
    setUniformRowHeights(true);   // Everything ought to be of the same height,
                                  // so set this property to true since it can
                                  // only speed things up which can't harm!

    // Have the columns resize themselves automatically, based on their
    // contents
//---GRY--- IS IT REALLY WHAT WE WANT?! IDEALLY, IT WOULD BE AUTOMATICALLY
//          RESIZED THE FIRST TIME WE SHOW THE WIDGET (AS A RESULT OF THE CALL
//          TO loadSettings), AND THEN IT WOULD BE UP TO THE USER TO RESIZE
//          THEM THEM IF HE SO DESIRES. HOWEVER, TO DO THIS, WE NEED THE WIDGET
//          TO BE VISIBLE, SO ONE MIGHT THINK THAT OVERRIDING paintEvent WOULD
//          DO THE TRICK, BUT NO... (FROM A GOOGLE SEARCH, IT WOULD SEEM THAT
//          OTHERS HAVE THE SAME PROBLEM) SO, FOR NOW, WE LEAVE THINGS AS THEY
//          ARE

    header()->setResizeMode(QHeaderView::ResizeToContents);

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
        // expand it and scroll to it

        QModelIndex currentPathDirModelIndex = mFileSystemModel->index(mCurrentPathDir);

        setExpanded(currentPathDirModelIndex, true);
        scrollTo(currentPathDirModelIndex);

        if (!mCurrentPath.isEmpty()) {
            // The current path is that of a file and it exists, so select it

            QModelIndex currentPathModelIndex = mFileSystemModel->index(mCurrentPath);

            setExpanded(currentPathModelIndex, true);
            scrollTo(currentPathModelIndex);
        }
    } else {
        // We are done initializing, so...

        needInitializing = false;
    }
}
