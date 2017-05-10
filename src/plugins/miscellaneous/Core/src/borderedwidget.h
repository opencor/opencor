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
// Bordered widget
//==============================================================================

#pragma once

//==============================================================================

#include "widget.h"
#include "coreglobal.h"

//==============================================================================

class QFrame;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT BorderedWidget : public Widget
{
    Q_OBJECT

public:
    explicit BorderedWidget(QWidget *pWidget,
                            const bool &pTop, const bool &pLeft,
                            const bool &pBottom, const bool &pRight);

    QWidget * widget();

    void setTopBorderVisible(const bool &pVisible) const;
    void setLeftBorderVisible(const bool &pVisible) const;
    void setBottomBorderVisible(const bool &pVisible) const;
    void setRightBorderVisible(const bool &pVisible) const;

private:
    QWidget *mWidget;

    QFrame *mTopBorder;
    QFrame *mLeftBorder;
    QFrame *mBottomBorder;
    QFrame *mRightBorder;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
