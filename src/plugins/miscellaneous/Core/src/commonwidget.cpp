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
// Common widget
//==============================================================================

#include "busywidget.h"
#include "centralwidget.h"
#include "commonwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"

//==============================================================================

#include <QApplication>
#include <QDockWidget>
#include <QMainWindow>
#include <QPainter>
#include <QPen>
#include <QScreen>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CommonWidget::CommonWidget(QWidget *pParent) :
    mParent(pParent)
{
}

//==============================================================================

CommonWidget::~CommonWidget() = default;

//==============================================================================

QSize CommonWidget::defaultSize(double pRatio) const
{
    // The default size of a widget is a ratio of the available geometry

    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();

    return { int(pRatio*availableGeometry.width()),
             int(pRatio*availableGeometry.height()) };
}

//==============================================================================

void CommonWidget::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

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

    return mBusyWidget != nullptr;
}

//==============================================================================

void CommonWidget::showBusyWidget(double pProgress)
{
    // Create and show our new busy widget resized, and then disable our parent,
    // but only if we are visible (i.e. our parent is visible)

    if (mParent->isVisible() && (++mCounter == 1)) {
        mBusyWidget = new BusyWidget(mParent, pProgress);

        resizeBusyWidget();

        if (mParent == centralWidget()) {
            mainWindow()->setEnabled(false);
        } else {
            mParent->setEnabled(false);
        }

        // Make sure that our busy widget is shown straightaway
        // Note: indeed, depending on the operating system (e.g. macOS) and on
        //       what we do next (e.g. retrieving a remote file), our busy
        //       widget may or not show straightaway...

        QCoreApplication::sendPostedEvents();
        QCoreApplication::processEvents();
    }
}

//==============================================================================

void CommonWidget::showBusyWidget()
{
    // Show a 'normal' busy widget

    showBusyWidget(-1.0);
}

//==============================================================================

void CommonWidget::showProgressBusyWidget()
{
    // Show a progress busy widget

    showBusyWidget(0.0);
}

//==============================================================================

void CommonWidget::setBusyWidgetProgress(double pProgress)
{
    // Make sure that we have a busy widget

    if (mBusyWidget == nullptr) {
        return;
    }

    // Set the progress of our busy widget

    mBusyWidget->setProgress(pProgress);
}

//==============================================================================

void CommonWidget::hideBusyWidget(bool pForceHiding)
{
    // Make sure that we have a busy widget

    if (mBusyWidget == nullptr) {
        return;
    }

    // Check whether we want to force hiding our busy widget

    if (pForceHiding) {
        mCounter = 1;
    }

    // Enable ourselves (or OpenCOR itself in case we are the central widget)
    // and hide our busy widget by deleting it
    // Note: we call QCoreApplication::processEvents() in case we have a
    //       progress busy widget, so that we can see its final state...

    if (--mCounter == 0) {
        QTime timeLimit= QTime::currentTime().addMSecs(39);

        while (QTime::currentTime() < timeLimit) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 13);
        }

        if (mParent == centralWidget()) {
            mainWindow()->setEnabled(true);
        } else {
            mParent->setEnabled(true);
        }

        delete mBusyWidget;

        mBusyWidget = nullptr;
    }
}

//==============================================================================

void CommonWidget::resizeBusyWidget()
{
    // Make sure that we have a busy widget

    if (mBusyWidget == nullptr) {
        return;
    }

    // Resize our busy widget

    mBusyWidget->resize();
}

//==============================================================================

void CommonWidget::processEvents()
{
    // Process events, but only if our parent is visible and updates are enabled
    // for it, and we or our central widget is not showing our or its busy
    // widget

    if (   mParent->isVisible() && mParent->updatesEnabled()
        && (mBusyWidget == nullptr) && !centralWidget()->isBusyWidgetVisible()) {
        QCoreApplication::processEvents();
    }
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
