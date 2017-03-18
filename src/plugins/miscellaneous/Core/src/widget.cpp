/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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
