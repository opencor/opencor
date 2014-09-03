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
// Spinner support widget
//==============================================================================

#include "spinnersupportwidget.h"
#include "spinnerwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

SpinnerSupportWidget::SpinnerSupportWidget(QWidget *pParent)
{
    // Create and customise our spinner widget

    mSpinnerWidget = new Core::SpinnerWidget(pParent);

    mSpinnerWidget->setVisible(false);

    setSpinnerWidgetParent(pParent);
}

//==============================================================================

void SpinnerSupportWidget::setSpinnerWidgetParent(QWidget *pParent)
{
    // Set the parent of our spinner widget and (re-)center it

    mSpinnerWidgetParent = pParent;

    mSpinnerWidget->setParent(pParent);

    centerSpinnerWidget();
}

//==============================================================================

void SpinnerSupportWidget::setSpinnerWidgetVisible(const bool &pVisible)
{
    // Show/hide our spinner widget

    mSpinnerWidget->setVisible(pVisible);
}

//==============================================================================

void SpinnerSupportWidget::centerSpinnerWidget()
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
