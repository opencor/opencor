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
// View widget
//==============================================================================

#include "viewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ViewWidget::ViewWidget(QWidget *pParent) :
    Widget(pParent)
{
}

//==============================================================================

QList<QWidget *> ViewWidget::statusBarWidgets() const
{
    // No status bar widgets by default

    return QList<QWidget *>();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
