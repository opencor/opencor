#include "filebrowserwidget.h"

#include <QFileSystemModel>

#include <QDesktopServices>
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

    // Have the columns resize themselves automatically, based on their contents
//---GRY--- IS IT REALLY WHAT WE WANT?! IDEALLY, IT WOULD BE AUTOMATICALLY
//          RESIZED THE FIRST TIME WE SHOW THE WIDGET (AS A RESULT OF THE CALL
//          TO DefaultSettings), BUT THEN IT WOULD BE UP TO THE USER TO RESIZE
//          THEM IF HE SO DESIRES

    header()->setResizeMode(QHeaderView::ResizeToContents);
}

FileBrowserWidget::~FileBrowserWidget()
{
    delete mFileSystemModel;
}

void FileBrowserWidget::retranslateUi()
{
    // Nothing to do for now...
}

void FileBrowserWidget::defaultSettings()
{
    // Sort by ascending drive letters / paths

    sortByColumn(0, Qt::AscendingOrder);

    // Start in the user's home directory and expand it

    QModelIndex homePathModelIndex = mFileSystemModel->index(QDir::homePath());

    setCurrentIndex(homePathModelIndex);
    setExpanded(homePathModelIndex, true);
}

void FileBrowserWidget::loadSettings(const QSettings &pSettings,
                                     const QString &pKey)
{
    // Retrieve the sorting information

    sortByColumn(pSettings.value(pKey+SETTINGS_SORTCOLUMN, 0).toInt(),
                 Qt::SortOrder(pSettings.value(pKey+SETTINGS_SORTORDER,
                                               Qt::AscendingOrder).toInt()));

    // Retrieve the current path and make it visible to the user

    QString currentPath = pSettings.value(pKey+SETTINGS_CURRENTPATH,
                                          QDir::homePath()).toString();
    QString currentPathDir = currentPath;
    QFileInfo currentPathInfo = currentPath;
    bool currentPathIsDir = currentPathInfo.isDir();

    if (!currentPathIsDir) {
        // The current path is not that of a directory, but that of a file (or
        // it doesn't exist), so retrieve the name of the parent folder
        // Note: indeed, to directly set the current index to that of a file
        //       won't give us the expected behaviour (i.e. the parent folder
        //       being open and expanded, and the file selected), so instead one
        //       must set the current index to that of the parent folder and
        //       then select the file

        QDir filePathUp = currentPath;

        filePathUp.cdUp();

        currentPathDir = filePathUp.path();

        // If the parent folder doesn't exist, then we give up and go for the
        // home path

        if (!QFileInfo(currentPathDir).exists())
            currentPathDir = QDir::homePath();
    }

    QModelIndex currentPathModelIndex = mFileSystemModel->index(currentPathDir);

    setCurrentIndex(currentPathModelIndex);
    scrollTo(currentPathModelIndex);

    if (!currentPathIsDir && currentPathInfo.exists())
    {
        // The current path is that of a file and it exists, so select it (see
        // the above note for the reasoning behind this)

        QModelIndex currentPathIndex = mFileSystemModel->index(currentPath);

        setCurrentIndex(currentPathIndex);
        scrollTo(currentPathIndex);
    }
}

void FileBrowserWidget::saveSettings(QSettings &pSettings, const QString &pKey)
{
    // Keep track of the sorting information

    pSettings.setValue(pKey+SETTINGS_SORTCOLUMN,
                       header()->sortIndicatorSection());
    pSettings.setValue(pKey+SETTINGS_SORTORDER, header()->sortIndicatorOrder());

    // Keep track of the current folder/file path

    QString currentPath = mFileSystemModel->filePath(currentIndex());

    pSettings.setValue(pKey+SETTINGS_CURRENTPATH, currentPath);
}

QSize FileBrowserWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}
