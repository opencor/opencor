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

SpinnerSupportWidget::SpinnerSupportWidget() :
    mSpinnerWidget(0)
{
}

//==============================================================================

bool SpinnerSupportWidget::isSpinnerWidgetVisible() const
{
    // Return whether our spinner widget is visible

    return mSpinnerWidget?mSpinnerWidget->isVisible():false;
}

//==============================================================================

void SpinnerSupportWidget::showSpinnerWidget(QWidget *pParent)
{
    // Show our spinner widget, which implies deleting its previous instance (if
    // any), creating a new one, centering it and finally showing it

    delete mSpinnerWidget;

    mSpinnerWidget = new Core::SpinnerWidget(pParent);

    centerSpinnerWidget();

    mSpinnerWidget->setVisible(true);
}

//==============================================================================

void SpinnerSupportWidget::hideSpinnerWidget()
{
    // Hide our spinner widget by deleting it

    delete mSpinnerWidget;

    mSpinnerWidget = 0;
}

//==============================================================================

void SpinnerSupportWidget::centerSpinnerWidget()
{
    // Make sure that we have a spinner widget

    if (!mSpinnerWidget)
        return;

    // Center our spinner widget

    QRect desktopGeometry = qApp->desktop()->availableGeometry();
    int parentWidth = mSpinnerWidget->parentWidget()?mSpinnerWidget->parentWidget()->width():desktopGeometry.width();
    int parentHeight = mSpinnerWidget->parentWidget()?mSpinnerWidget->parentWidget()->height():desktopGeometry.height();

    mSpinnerWidget->move(0.5*(parentWidth-mSpinnerWidget->width()),
                         0.5*(parentHeight-mSpinnerWidget->height()));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
