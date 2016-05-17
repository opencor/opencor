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
// Busy widget
//==============================================================================
// Note: this widget is adapted from http://fgnass.github.io/spin.js/...
//==============================================================================

#include "busywidget.h"
#include "centralwidget.h"

//==============================================================================

#include <Qt>
#include <QtMath>

//==============================================================================

#include <QApplication>
#include <QColor>
#include <QDesktopWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPoint>
#include <QRectF>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const int Margin = 5;

//==============================================================================

BusyWidget::BusyWidget(QWidget *pParent, const bool &pGlobal,
                       const double &pProgress) :
    QWidget(pParent),
    mParent(pParent),
    mGlobal(pGlobal),
    mFps(0),
    mForegroundColor(Qt::white),
    mBackgroundColor(QColor(54, 96, 146)),
    mBackgroundRoundness(0.25),
    mMainLine(0),
    mCount(12),
    mLength(5),
    mThickness(5),
    mRoundness(1.0),
    mTrail(100),
    mOpacity(0.25),
    mRadius(15),
    mProgress(pProgress)
{
    // Create our timer and a connection to handle its timing out

    mTimer = new QTimer(this);

    setFps(10);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(rotate()));

    // Make ourselves visible and start our timer, if needed

    setVisible(true);

    if (pProgress == -1.0)
        mTimer->start();
}

//==============================================================================

QWidget * BusyWidget::effectiveParentWidget() const
{
    // Return our effective parent widget, which if we are global means
    // returning the central widget, if possible

    if (mGlobal) {
        QWidget *currentParentWidget = parentWidget();

        if (currentParentWidget) {
            forever {
                if (currentParentWidget) {
                    CentralWidget *centralWidget = qobject_cast<CentralWidget *>(currentParentWidget);

                    if (centralWidget)
                        return centralWidget;
                    else
                        currentParentWidget = currentParentWidget->parentWidget();
                } else {
                    return parentWidget();
                }
            }
        } else {
            return 0;
        }
    } else {
        return parentWidget();
    }
}

//==============================================================================

int BusyWidget::fps() const
{
    // Return our FPS

    return mFps;
}

//==============================================================================

void BusyWidget::setFps(const int &pFps)
{
    // Set our FPS

    if (pFps != mFps) {
        mFps = pFps;

        mTimer->setInterval(1000/pFps);
    }
}

//==============================================================================

QColor BusyWidget::foregroundColor() const
{
    // Return our foreground colour

    return mForegroundColor;
}

//==============================================================================

void BusyWidget::setForegroundColor(const QColor &pForegroundColor)
{
    // Set our foreground colour

    if (pForegroundColor != mForegroundColor) {
        mForegroundColor = pForegroundColor;

        update();
    }
}

//==============================================================================

QColor BusyWidget::backgroundColor() const
{
    // Return our background colour

    return mBackgroundColor;
}

//==============================================================================

void BusyWidget::setBackgroundColor(const QColor &pBackgroundColor)
{
    // Set our background colour

    if (pBackgroundColor != mBackgroundColor) {
        mBackgroundColor = pBackgroundColor;

        update();
    }
}

//==============================================================================

double BusyWidget::backgroundRoundness() const
{
    // Return our background roundness

    return mBackgroundRoundness;
}

//==============================================================================

void BusyWidget::setBackgroundRoundness(const double &pBackgroundRoundness)
{
    // Set our background roundness

    if (pBackgroundRoundness != mBackgroundRoundness) {
        mBackgroundRoundness = pBackgroundRoundness;

        update();
    }
}

//==============================================================================

int BusyWidget::count() const
{
    // Return our number of lines

    return mCount;
}

//==============================================================================

void BusyWidget::setCount(const int &pCount)
{
    // Set our number of lines

    if ((pCount != mCount) && (pCount > 1)) {
        mCount = pCount;
        mMainLine = 0;

        update();
    }
}

//==============================================================================

int BusyWidget::length() const
{
    // Return the length of our lines

    return mLength;
}

//==============================================================================

void BusyWidget::setLength(const int &pLength)
{
    // Set the length of our lines

    if (pLength != mLength) {
        mLength = pLength;

        update();
    }
}

//==============================================================================

int BusyWidget::thickness() const
{
    // Return the thickness of our lines

    return mThickness;
}

//==============================================================================

void BusyWidget::setThickness(const int &pThickness)
{
    // Set the thickness of our lines

    if (pThickness != mThickness) {
        mThickness = pThickness;

        update();
    }
}

//==============================================================================

double BusyWidget::roundness() const
{
    // Return the roundness of our lines

    return mRoundness;
}

//==============================================================================

void BusyWidget::setRoundness(const double &pRoundness)
{
    // Set the roundness of our lines

    if (pRoundness != mRoundness) {
        mRoundness = pRoundness;

        update();
    }
}

//==============================================================================

int BusyWidget::trail() const
{
    // Return the trail of our lines

    return mTrail;
}

//==============================================================================

void BusyWidget::setTrail(const int &pTrail)
{
    // Set the trail of our lines

    if (pTrail != mTrail) {
        mTrail = pTrail;

        update();
    }
}

//==============================================================================

double BusyWidget::opacity() const
{
    // Return the opacity of our lines

    return mOpacity;
}

//==============================================================================

void BusyWidget::setOpacity(const double &pOpacity)
{
    // Set the opacity of our lines

    if (pOpacity != mOpacity) {
        mOpacity = pOpacity;

        update();
    }
}

//==============================================================================

int BusyWidget::radius() const
{
    // Return our radius

    return mRadius;
}

//==============================================================================

void BusyWidget::setRadius(const int &pRadius)
{
    // Set our radius

    if (pRadius != mRadius) {
        mRadius = pRadius;

        update();
    }
}

//==============================================================================

double BusyWidget::progress() const
{
    // Return our progress

    return mProgress;
}

//==============================================================================

void BusyWidget::setProgress(const double &pProgress)
{
    // Set our progress and enable/disable our timer, depending on the case

    if (pProgress != mProgress) {
        mProgress = pProgress;

        if (pProgress == -1.0) {
            mMainLine = 0;

            mTimer->start();
        } else {
            mTimer->stop();
        }

        update();
    }
}

//==============================================================================

void BusyWidget::resize()
{
    // Resize ourselves against our parent/desktop

    if (mParent) {
        QWidget::resize(mParent->width(), mParent->height());
    } else {
        QRect desktopGeometry = qApp->desktop()->availableGeometry();

        QWidget::resize(desktopGeometry.width(), desktopGeometry.height());
    }
}

//==============================================================================

void BusyWidget::rotate()
{
    // Rotate ourselves

    ++mMainLine;

    if (mMainLine >= mCount)
        mMainLine = 0;

    update();
}

//==============================================================================

void BusyWidget::paintEvent(QPaintEvent *pEvent)
{
    // Get a painter that supports antialiasing

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw a background for ourselves

    painter.translate(0.5*width(), 0.5*height());

    QPainterPath painterPath;
    int backgroundSize = 2*(mRadius+mLength+Margin);
    double backgroundCornerRadius = mBackgroundRoundness*(backgroundSize >> 1);

    painterPath.addRoundedRect(QRectF(-0.5*backgroundSize, -0.5*backgroundSize,
                                      backgroundSize, backgroundSize),
                               backgroundCornerRadius, backgroundCornerRadius);

    painter.fillPath(painterPath, mBackgroundColor);

    // Draw ourselves

    if (mProgress == -1.0) {
        double lineCornerRadius = mRoundness*(mThickness >> 1);

        for (int i = 0; i < mCount; ++i) {
            painter.save();

            int relativeLine = i-mMainLine;

            if (relativeLine < 0)
                relativeLine += mCount;

            painter.rotate(-90-relativeLine*360.0/mCount);
            painter.translate(mRadius, 0);

            QColor foregroundColor = mForegroundColor;

            foregroundColor.setAlphaF(qMax(1.0-(100.0-100.0*(mCount-1-i)/(mCount-1))*(1.0-mOpacity)/mTrail, mOpacity));

            painter.setBrush(foregroundColor);

            painter.drawRoundedRect(QRectF(0.0, -0.5*mThickness, mLength, mThickness),
                                    lineCornerRadius, lineCornerRadius);

            painter.restore();
        }
    } else {
        int size = 2*(mRadius+mLength);

        painter.setBrush(mForegroundColor);

        painter.drawPie(QRectF(-0.5*size, -0.5*size, size, size),
                        90*16, -mProgress*360*16);
    }

    // Accept the event

    pEvent->accept();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
