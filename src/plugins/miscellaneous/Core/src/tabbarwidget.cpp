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

//==============================================================================

#include <QWheelEvent>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

TabBarWidget::TabBarWidget(QWidget *pParent) :
    QTabBar(pParent)
{
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

    QWidget::wheelEvent(pEvent);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
