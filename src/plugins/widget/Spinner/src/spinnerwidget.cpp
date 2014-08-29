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
// Spinner widget
//==============================================================================
// Note: this widget is adapted from http://fgnass.github.io/spin.js/...
//==============================================================================

#include "spinnerwidget.h"

//==============================================================================

#include <Qt>
#include <QtMath>

//==============================================================================

#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPoint>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace Spinner {

//==============================================================================

SpinnerWidget::SpinnerWidget(QWidget *pParent) :
    QWidget(pParent),
    mFps(20),
    mBackgroundColor(QColor(54, 96, 146)),
    mBackgroundRoundness(0.25),
    mMainLine(0),
    mLineCount(12),
    mLineColor(Qt::white),
    mLineLength(5),
    mLineWidth(5),
    mLineRoundness(1.0),
    mLineTrail(100),
    mLineOpacity(0.25),
    mRadius(15)
{
    // Set our default size

    int size = 2*(mRadius+mLineLength+5);

    resize(size, size);

    // Create our timer and a connection to handle its timing out

    mTimer = new QTimer(this);

    mTimer->setInterval(100);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(rotate()));
}

//==============================================================================

int SpinnerWidget::fps() const
{
    // Return our FPS

    return mFps;
}

//==============================================================================

void SpinnerWidget::setFps(const int &pFps)
{
    // Set our FPS

    if (pFps != mFps) {
        mFps = pFps;

        mTimer->setInterval(1000/pFps);
    }
}

//==============================================================================

QColor SpinnerWidget::backgroundColor() const
{
    // Return our background colour

    return mBackgroundColor;
}

//==============================================================================

void SpinnerWidget::setBackgroundColor(const QColor &pBackgroundColor)
{
    // Set our background colour

    if (pBackgroundColor != mBackgroundColor) {
        mBackgroundColor = pBackgroundColor;

        update();
    }
}

//==============================================================================

double SpinnerWidget::backgroundRoundness() const
{
    // Return our background roundness

    return mBackgroundRoundness;
}

//==============================================================================

void SpinnerWidget::setBackgroundRoundness(const double &pBackgroundRoundness)
{
    // Set our background roundness

    if (pBackgroundRoundness != mBackgroundRoundness) {
        mBackgroundRoundness = pBackgroundRoundness;

        update();
    }
}

//==============================================================================

int SpinnerWidget::lineCount() const
{
    // Return our number of lines

    return mLineCount;
}

//==============================================================================

void SpinnerWidget::setLineCount(const int &pLineCount)
{
    // Set our number of lines

    if (pLineCount != mLineCount) {
        mLineCount = pLineCount;
        mMainLine = 0;

        update();
    }
}

//==============================================================================

QColor SpinnerWidget::lineColor() const
{
    // Return our line colour

    return mLineColor;
}

//==============================================================================

void SpinnerWidget::setLineColor(const QColor &pLineColor)
{
    // Set our line colour

    if (pLineColor != mLineColor) {
        mLineColor = pLineColor;

        update();
    }
}

//==============================================================================

int SpinnerWidget::lineLength() const
{
    // Return the length of our lines

    return mLineLength;
}

//==============================================================================

void SpinnerWidget::setLineLength(const int &pLineLength)
{
    // Set the length of our lines

    if (pLineLength != mLineLength) {
        mLineLength = pLineLength;

        update();
    }
}

//==============================================================================

int SpinnerWidget::lineWidth() const
{
    // Return the width of our lines

    return mLineWidth;
}

//==============================================================================

void SpinnerWidget::setLineWidth(const int &pLineWidth)
{
    // Set the width of our lines

    if (pLineWidth != mLineWidth) {
        mLineWidth = pLineWidth;

        update();
    }
}

//==============================================================================

double SpinnerWidget::lineRoundness() const
{
    // Return the roundness of our lines

    return mLineRoundness;
}

//==============================================================================

void SpinnerWidget::setLineRoundness(const double &pLineRoundness)
{
    // Set the roundness of our lines

    if (pLineRoundness != mLineRoundness) {
        mLineRoundness = pLineRoundness;

        update();
    }
}

//==============================================================================

int SpinnerWidget::lineTrail() const
{
    // Return the trail of our lines

    return mLineTrail;
}

//==============================================================================

void SpinnerWidget::setLineTrail(const int &pLineTrail)
{
    // Set the trail of our lines

    if (pLineTrail != mLineTrail) {
        mLineTrail = pLineTrail;

        update();
    }
}

//==============================================================================

double SpinnerWidget::lineOpacity() const
{
    // Return the opacity of our lines

    return mLineOpacity;
}

//==============================================================================

void SpinnerWidget::setLineOpacity(const double &pLineOpacity)
{
    // Set the opacity of our lines

    if (pLineOpacity != mLineOpacity) {
        mLineOpacity = pLineOpacity;

        update();
    }
}

//==============================================================================

int SpinnerWidget::radius() const
{
    // Return our radius

    return mRadius;
}

//==============================================================================

void SpinnerWidget::setRadius(const int &pRadius)
{
    // Set our radius

    if (pRadius != mRadius) {
        mRadius = pRadius;

        update();
    }
}

//==============================================================================

void SpinnerWidget::rotate()
{
    // Rotate ourselves

    ++mMainLine;

    if (mMainLine >= mLineCount)
        mMainLine = 0;

    update();
}

//==============================================================================

void SpinnerWidget::paintEvent(QPaintEvent *pEvent)
{
    // Get a painter that supports antialiasing

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draw a background for our spinner

    QPainterPath painterPath;

    double backgroundCornerRadius = mBackgroundRoundness*(width() >> 1);

    painterPath.addRoundedRect(rect(), backgroundCornerRadius, backgroundCornerRadius);

    painter.setPen(Qt::NoPen);
    painter.setBrush(mBackgroundColor);

    painter.drawPath(painterPath);

    // Draw our spinner

    double lineCornerRadius = mLineRoundness*(mLineWidth >> 1);

    painter.translate(0.5*width(), 0.5*height());

    for (int i = 0; i < mLineCount; ++i) {
        painter.save();

        painter.rotate(i*360.0/mLineCount);
        painter.translate(mRadius, 0);

        int relativeLine = i-mMainLine;

        if (relativeLine < 0)
            relativeLine += mLineCount;

        QColor lineColor = mLineColor;

        lineColor.setAlphaF(qMax(1.0-(100.0-100.0*relativeLine/(mLineCount-1))*(1.0-mLineOpacity)/mLineTrail, mLineOpacity));

        painter.setBrush(lineColor);

        painter.drawRoundedRect(QRectF(0.0, -0.5*mLineWidth, mLineLength, mLineWidth),
                                lineCornerRadius, lineCornerRadius);

        painter.restore();
    }

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void SpinnerWidget::setVisible(bool pVisible)
{
    // Reset ourselves, if needed

    if (pVisible)
        mMainLine = 0;

    // Default handling of the method

    QWidget::setVisible(pVisible);

    // Start/stop our timer

    if (pVisible) {
        if (!mTimer->isActive())
            mTimer->start();
    } else {
        mTimer->stop();
    }
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
