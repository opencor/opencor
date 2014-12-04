/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Bordered widget
//==============================================================================

#include "borderedwidget.h"
#include "coreguiutils.h"

//==============================================================================

#include <QFrame>
#include <QGridLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

BorderedWidget::BorderedWidget(QWidget *pWidget,
                               const bool &pTop, const bool &pLeft,
                               const bool &pBottom, const bool &pRight) :
    Widget(pWidget->parentWidget()),
    mWidget(pWidget)
{
    // We want our widget to be bordered by a single line which colour matches
    // that of the application's theme. Because of that colour requirement, we
    // can't use a QFrame, so instead we rely on the use of a grid layout and
    // newLineWidget()...

    // Create a grid layout for ourselves

    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    setLayout(gridLayout);

    // Add some real line widgets to the top, left, bottom and/or right of
    // ourselves, if required

    mTopBorder    = newLineWidget(this);
    mLeftBorder   = newLineWidget(false, this);
    mBottomBorder = newLineWidget(this);
    mRightBorder  = newLineWidget(false, this);

    gridLayout->addWidget(mTopBorder, 0, 0, 1, 3);
    gridLayout->addWidget(mLeftBorder, 1, 0);
    gridLayout->addWidget(pWidget, 1, 1);
    gridLayout->addWidget(mBottomBorder, 2, 0, 1, 3);
    gridLayout->addWidget(mRightBorder, 1, 2);

    setTopBorderVisible(pTop);
    setLeftBorderVisible(pLeft);
    setBottomBorderVisible(pBottom);
    setRightBorderVisible(pRight);

    // Make sure that our widget takes as much space as possible

    gridLayout->setRowStretch(1, 1);
    gridLayout->setColumnStretch(1, 1);
}

//==============================================================================

QWidget * BorderedWidget::widget()
{
    // Return our bordered widget

    return mWidget;
}

//==============================================================================

void BorderedWidget::setTopBorderVisible(const bool &pVisible) const
{
    // Show/hide the top border

    mTopBorder->setVisible(pVisible);
}

//==============================================================================

void BorderedWidget::setLeftBorderVisible(const bool &pVisible) const
{
    // Show/hide the left border

    mLeftBorder->setVisible(pVisible);
}

//==============================================================================

void BorderedWidget::setBottomBorderVisible(const bool &pVisible) const
{
    // Show/hide the bottom border

    mBottomBorder->setVisible(pVisible);
}

//==============================================================================

void BorderedWidget::setRightBorderVisible(const bool &pVisible) const
{
    // Show/hide the right border

    mRightBorder->setVisible(pVisible);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
