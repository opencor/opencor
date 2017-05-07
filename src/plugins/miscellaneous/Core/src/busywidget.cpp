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
// Busy widget
//==============================================================================
// Note: this widget is adapted from http://fgnass.github.io/spin.js/...
//==============================================================================

#include "busywidget.h"
#include "centralwidget.h"

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

BusyWidget::BusyWidget(QWidget *pParent, const double &pProgress) :
    QWidget(pParent),
    mParent(pParent),
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

    enum {
        Margin = 5
    };

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

        if (mProgress == 1.0) {
            painter.drawEllipse(QPointF(0.0, 0.0), 0.5*size, 0.5*size);
        } else {
            painter.drawPie(QRectF(-0.5*size, -0.5*size, size, size),
                            90*16, -mProgress*360*16);
        }
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
