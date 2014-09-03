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
// Window widget
//==============================================================================

#include "spinnerwidget.h"
#include "windowwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

WindowWidget::WindowWidget(QWidget *pParent) :
    QDockWidget(pParent),
    CommonWidget(pParent)
{
    // Create and customise our spinner widget

    mSpinnerWidget = new Core::SpinnerWidget(this);

    mSpinnerWidget->setVisible(false);

    setSpinnerWidgetParent(this);
}

//==============================================================================

void WindowWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QDockWidget::resizeEvent(pEvent);

    // (Re-)center our spinner widget

    recenterSpinnerWidget();
}

//==============================================================================

void WindowWidget::setSpinnerWidgetParent(QWidget *pParent)
{
    // Set the parent of our spinner widget and (re-)center it

    mSpinnerWidgetParent = pParent;

    mSpinnerWidget->setParent(pParent);

    recenterSpinnerWidget();
}

//==============================================================================

void WindowWidget::setSpinnerWidgetVisible(const bool &pVisible)
{
    // Show/hide our spinner widget

    mSpinnerWidget->setVisible(pVisible);
}

//==============================================================================

void WindowWidget::recenterSpinnerWidget()
{
    // (Re-)center our spinner widget

    QRect desktopGeometry = qApp->desktop()->availableGeometry();
    int parentWidth = mSpinnerWidgetParent?mSpinnerWidgetParent->width():desktopGeometry.width();
    int parentHeight = mSpinnerWidgetParent?mSpinnerWidgetParent->height():desktopGeometry.height();

    mSpinnerWidget->move(0.5*(parentWidth-mSpinnerWidget->width()),
                         0.5*(parentHeight-mSpinnerWidget->height()));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
