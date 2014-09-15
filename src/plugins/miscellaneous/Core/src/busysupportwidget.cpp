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
// Busy support widget
//==============================================================================

#include "busysupportwidget.h"
#include "busywidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

BusySupportWidget::BusySupportWidget() :
    mBusyWidget(0)
{
}

//==============================================================================

bool BusySupportWidget::isBusyWidgetVisible() const
{
    // Return whether our busy widget is visible

    return mBusyWidget?mBusyWidget->isVisible():false;
}

//==============================================================================

void BusySupportWidget::showBusyWidget(QWidget *pParent, const double &pProgress)
{
    // Show our busy widget, which implies deleting its previous instance (if
    // any), creating a new one and centering it

    delete mBusyWidget;

    mBusyWidget = new Core::BusyWidget(pParent, pProgress);

    centerBusyWidget();
}

//==============================================================================

void BusySupportWidget::hideBusyWidget()
{
    // Hide our busy widget by deleting it

    delete mBusyWidget;

    mBusyWidget = 0;
}

//==============================================================================

void BusySupportWidget::centerBusyWidget()
{
    // Make sure that we have a busy widget

    if (!mBusyWidget)
        return;

    // Center our busy widget

    QRect desktopGeometry = qApp->desktop()->availableGeometry();
    int parentWidth = mBusyWidget->parentWidget()?mBusyWidget->parentWidget()->width():desktopGeometry.width();
    int parentHeight = mBusyWidget->parentWidget()?mBusyWidget->parentWidget()->height():desktopGeometry.height();

    mBusyWidget->move(0.5*(parentWidth-mBusyWidget->width()),
                      0.5*(parentHeight-mBusyWidget->height()));
}

//==============================================================================

void BusySupportWidget::setBusyWidgetProgress(const double &pProgress)
{
    // Make sure that we have a busy widget

    if (!mBusyWidget)
        return;

    // Set the progress of our busy widget

    mBusyWidget->setProgress(pProgress);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
