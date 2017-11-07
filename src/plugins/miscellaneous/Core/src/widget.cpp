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

#include "widget.h"

//==============================================================================

#include <QBoxLayout>
#include <QFormLayout>
#include <QResizeEvent>
#include <QStackedLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

Widget::Widget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(this),
    mSizeHint(defaultSize(0.15))
{
}

//==============================================================================

Widget::Widget(const QSize &pSizeHint, QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(this),
    mSizeHint(pSizeHint)
{
}

//==============================================================================

QLayout * Widget::createLayout(const LayoutType &pLayoutType)
{
    // Create and set a layout

    QLayout *layout = 0;

    switch (pLayoutType) {
    case VerticalLayout:
        layout = new QVBoxLayout(this);

        break;
    case HorizontalLayout:
        layout = new QHBoxLayout(this);

        break;
    case FormLayout:
        layout = new QFormLayout(this);

        break;
    case GridLayout:
        layout = new QGridLayout(this);

        break;
    case StackedLayout:
        layout = new QStackedLayout(this);

        break;
    }

    layout->setContentsMargins(QMargins());

    if ((pLayoutType == VerticalLayout) || (pLayoutType == HorizontalLayout))
        layout->setSpacing(0);

    setLayout(layout);

    return layout;
}

//==============================================================================

void Widget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QWidget::resizeEvent(pEvent);

    // Resize our busy widget

    resizeBusyWidget();
}

//==============================================================================

QSize Widget::sizeHint() const
{
    // Suggest our default size for the widget

    if (mSizeHint.isValid())
        return mSizeHint;
    else
        return QWidget::sizeHint();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
