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
// Widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT Widget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    enum LayoutType {
        VerticalLayout,
        HorizontalLayout,
        FormLayout,
        GridLayout,
        StackedLayout
    };

    explicit Widget(QWidget *pParent);
    explicit Widget(const QSize &pSizeHint, QWidget *pParent);

    QLayout * createLayout(const LayoutType &pLayoutType = VerticalLayout);

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

    virtual QSize sizeHint() const;

private:
    QSize mSizeHint;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
