#include "fileorganiserwidget.h"

#include <QPaintEvent>

FileOrganiserWidget::FileOrganiserWidget(const QString &pName,
                                         QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pName, this, pParent)
{
    // Set some properties for the file organiser widget itself

#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);   // Remove the focus border
                                                // since it messes up our
                                                // toolbar
#endif
    setSortingEnabled(true);      // Allow sorting
    setUniformRowHeights(true);   // Everything ought to be of the same height,
                                  // so set this property to true since it can
                                  // only speed things up which can't harm!
}

QSize FileOrganiserWidget::sizeHint() const
{
    // Suggest a default size for the file organiser widget
    // Note: this is critical if we want a docked widget, with a file organiser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}
