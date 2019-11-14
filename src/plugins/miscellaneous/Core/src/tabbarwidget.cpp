/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Tab bar widget
//==============================================================================

#include "tabbarwidget.h"
#ifdef Q_OS_MAC
    #include "tabbarwidgetmacstyle.h"
#endif

//==============================================================================

#include <QWheelEvent>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

TabBarWidget::TabBarWidget(QWidget *pParent) :
    QTabBar(pParent)
{
    // Customise our style, but only if we are on macOS
    // Note #1: indeed, between Qt 5.6.x LTS and Qt 5.9.x LTS, the styling of a
    //          QTabBar has changed quite a bit and is now not working properly,
    //          hence we fix things by having 'our' own style (see
    //          https://bugreports.qt.io/browse/QTBUG-61742)...
    // Note #2: this eventually got "fixed", but it still doesn't look right, so
    //          we keep using our customised style...

#ifdef Q_OS_MAC
    setStyle(new TabBarWidgetMacStyle());
#endif

    // Force the size of the icons to be 16 by 16 pixels
    // Note: this ensures that our icons have a decent size on HiDPI screens...

    setIconSize(QSize(16, 16));

    // Prevent our tabs from expanding
    // Note: if we didn't do this and had many tabs, our widget would widen,
    //       which in turn would reduce the size of any adjacent widget,
    //       something that we don't want (indeed, if OpenCOR had many files
    //       opened, then our central widget would widen and the width of any
    //       docked window would be reduced)...

    setExpanding(false);

    // Enable the scrolling of tabs
    // Note: this is style dependent and, by default, not enabled on macOS, but
    //       we want to be consistent throughout, so set it in all cases, even
    //       though it's already set on Windows and Linux (better be safe than
    //       sorry)...

    setUsesScrollButtons(true);
}

//==============================================================================

#ifdef Q_OS_MAC
TabBarWidget::~TabBarWidget()
{
    // Delete some internal objects

    delete style();
}
#endif

//==============================================================================

int TabBarWidget::oldIndex() const
{
    // Return our old index

    return mOldIndex;
}

//==============================================================================

void TabBarWidget::setOldIndex(int pOldIndex)
{
    // Set our old index

    mOldIndex = pOldIndex;
}

//==============================================================================

void TabBarWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Switch to the next/previous tab, if possible

    int offset = (pEvent->delta() < 0)?1:-1;

    for (int i = currentIndex()+offset; (i >= 0) && (i < count()); i += offset) {
        if (isTabEnabled(i)) {
            setCurrentIndex(i);

            break;
        }
    }

    QTabBar::wheelEvent(pEvent);
}

//==============================================================================

QSize TabBarWidget::tabSizeHint(int pIndex) const
{
    // Determine and return our tab size hint, keeping in mind our style when on
    // macOS

#ifdef Q_OS_MAC
    int shift = tabsClosable()?12:0;
    QIcon icon = tabIcon(pIndex);

    if (!icon.isNull()) {
        int iconExtent = style()->pixelMetric(QStyle::PM_SmallIconSize);

        shift += icon.actualSize(QSize(iconExtent, iconExtent)).width()+4;
    }

    if (   (shape() == RoundedNorth) || (shape() == RoundedSouth)
        || (shape() == TriangularNorth) || (shape() == TriangularSouth)) {
        return QTabBar::tabSizeHint(pIndex)-QSize(shift, 0);
    }

    return QTabBar::tabSizeHint(pIndex)-QSize(0, shift);
#else
    return QTabBar::tabSizeHint(pIndex);
#endif
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
