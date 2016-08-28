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
// Common widget
//==============================================================================

#include "busywidget.h"
#include "centralwidget.h"
#include "commonwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CommonWidget::CommonWidget(QWidget *pParent) :
    mParent(pParent),
    mBusyWidget(0)
{
}

//==============================================================================

QSize CommonWidget::defaultSize(const double &pRatio) const
{
    // The default size of a widget is a ratio of the available geometry

    QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(pRatio*desktopGeometry.width(),
                 pRatio*desktopGeometry.height());
}

//==============================================================================

void CommonWidget::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::retranslateUi()
{
    // Nothing to do by default...
}

//==============================================================================

bool CommonWidget::isBusyWidgetVisible() const
{
    // Return whether our busy widget is visible, i.e. whether it exists

    return mBusyWidget;
}

//==============================================================================

void CommonWidget::doShowBusyWidget(const double &pProgress)
{
    // Make sure that our previous busy widget, if any, is hidden (and deleted)

    hideBusyWidget();

    // Create and show our new busy widget resized, and then disable our parent

    mBusyWidget = new BusyWidget(mParent, pProgress);

    resizeBusyWidget();

    if (mParent == centralWidget())
        mainWindow()->setEnabled(false);
    else
        mParent->setEnabled(false);

    // Make sure that our busy widget is shown straightaway
    // Note: indeed, depending on the operating system (e.g. OS X) and on what
    //       we do next (e.g. retrieving a remote file), our busy widget may or
    //       not show straightaway...

    QCoreApplication::sendPostedEvents();
    QCoreApplication::processEvents();
}

//==============================================================================

void CommonWidget::showBusyWidget()
{
    // Show a 'normal' busy widget

    doShowBusyWidget();
}

//==============================================================================

void CommonWidget::showProgressBusyWidget()
{
    // Show a progress busy widget

    doShowBusyWidget(0.0);
}

//==============================================================================

void CommonWidget::hideBusyWidget()
{
    // Make sure that we have a busy widget

    if (!mBusyWidget)
        return;

    // Enable ourselves (or OpenCOR itself in case we are the central widget)
    // and hide our busy widget by deleting it

    if (mParent == centralWidget())
        mainWindow()->setEnabled(true);
    else
        mParent->setEnabled(true);

    delete mBusyWidget;

    mBusyWidget = 0;
}

//==============================================================================

void CommonWidget::resizeBusyWidget()
{
    // Make sure that we have a busy widget

    if (!mBusyWidget)
        return;

    // Resize our busy widget

    mBusyWidget->resize();
}

//==============================================================================

void CommonWidget::setBusyWidgetProgress(const double &pProgress)
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
