#include "pmrexplorerwidget.h"

PmrExplorerWidget::PmrExplorerWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent)
{
}

void PmrExplorerWidget::retranslateUi()
{
    // Nothing to do for now...
}

void PmrExplorerWidget::defaultSettings()
{
    // Nothing to do for now...
}

void PmrExplorerWidget::loadSettings(const QSettings &, const QString &)
{
    // Nothing to do for now...
}

void PmrExplorerWidget::saveSettings(QSettings &, const QString &)
{
    // Nothing to do for now...
}

QSize PmrExplorerWidget::sizeHint() const
{
    // Suggest a default size for the PMR explorer widget
    // Note: this is critical if we want a docked widget, with a PMR explorer
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void PmrExplorerWidget::paintEvent(QPaintEvent *pEvent)
{
    QWidget::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();
}
