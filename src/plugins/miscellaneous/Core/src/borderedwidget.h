/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Bordered widget
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "widget.h"

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
    explicit BorderedWidget(QWidget *pWidget, bool pTop, bool pLeft,
                            bool pBottom, bool pRight);

    QWidget * widget();

    void setTopBorderVisible(bool pVisible) const;
    void setLeftBorderVisible(bool pVisible) const;
    void setBottomBorderVisible(bool pVisible) const;
    void setRightBorderVisible(bool pVisible) const;

private:
    QWidget *mWidget;

    QFrame *mTopBorder;
    QFrame *mLeftBorder;
    QFrame *mBottomBorder;
    QFrame *mRightBorder;
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
