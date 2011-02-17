#include "filebrowserwidget.h"

#include <QFileSystemModel>

#include <QDesktopServices>
#include <QHeaderView>
#include <QSettings>

#define SETTINGS_SORTCOLUMN "_SortColumn"
#define SETTINGS_SORTORDER  "_SortOrder"

FileBrowserWidget::FileBrowserWidget(QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pParent)
{
    mFileSystemModel = new QFileSystemModel;

    // We want acces to the full file system

    mFileSystemModel->setRootPath(QDir::rootPath());

    // Set some properties for the file browser widget itself

    setModel(mFileSystemModel);   // Associate ourselves to mFileSystemModel
    setSortingEnabled(true);      // Allow sorting
    setUniformRowHeights(true);   // Everything ought to be of the same height,
                                  // so set this property to true since it can
                                  // only speed things up which can't harm!
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
}

void FileBrowserWidget::loadSettings(const QSettings &pSettings,
                                     const QString &pKey)
{
    // Retrieve the sorting information

    sortByColumn(pSettings.value(pKey+SETTINGS_SORTCOLUMN, 0).toInt(),
                 Qt::SortOrder(pSettings.value(pKey+SETTINGS_SORTORDER, Qt::AscendingOrder).toInt()));
}

void FileBrowserWidget::saveSettings(QSettings &pSettings, const QString &pKey)
{
    // Keep track of the sorting information

    pSettings.setValue(pKey+SETTINGS_SORTCOLUMN, header()->sortIndicatorSection());
    pSettings.setValue(pKey+SETTINGS_SORTORDER, header()->sortIndicatorOrder());
}

QSize FileBrowserWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void FileBrowserWidget::paintEvent(QPaintEvent *pEvent)
{
    QTreeView::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();
}
