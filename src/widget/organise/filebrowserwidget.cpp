#include "filebrowserwidget.h"

void FileBrowserWidget::retranslateUi()
{
    // Nothing to do for now...
}

void FileBrowserWidget::defaultSettings()
{
    // Nothing to do for now...
}

void FileBrowserWidget::loadSettings(const QSettings &, const QString &)
{
    // Nothing to do for now...
}

void FileBrowserWidget::saveSettings(QSettings &, const QString &)
{
    // Nothing to do for now...
}

QSize FileBrowserWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}
