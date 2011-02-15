#include "commandviewerwidget.h"

void CommandViewerWidget::retranslateUi()
{
    // Nothing to do for now...
}

void CommandViewerWidget::defaultSettings()
{
    // Nothing to do for now...
}

void CommandViewerWidget::loadSettings(const QSettings &, const QString &)
{
    // Nothing to do for now...
}

void CommandViewerWidget::saveSettings(QSettings &, const QString &)
{
    // Nothing to do for now...
}

QSize CommandViewerWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.1);
}
