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
// Busy widget
//==============================================================================

#ifndef BUSYWIDGET_H
#define BUSYWIDGET_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QColor>
#include <QWidget>

//==============================================================================

class QTimer;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT BusyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BusyWidget(QWidget *pParent, const double &pProgress = -1.0);

    int fps() const;
    void setFps(const int &pFps);

    QColor foregroundColor() const;
    void setForegroundColor(const QColor &pForegroundColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &pBackgroundColor);

    double backgroundRoundness() const;
    void setBackgroundRoundness(const double &pBackgroundRoundness);

    int count() const;
    void setCount(const int &pCount);

    int length() const;
    void setLength(const int &pLength);

    int thickness() const;
    void setThickness(const int &pThickness);

    double roundness() const;
    void setRoundness(const double &pRoundness);

    int trail() const;
    void setTrail(const int &pTrail);

    double opacity() const;
    void setOpacity(const double &pOpacity);

    int radius() const;
    void setRadius(const int &pRadius);

    double progress() const;
    void setProgress(const double &pProgress);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QTimer *mTimer;

    int mFps;

    QColor mForegroundColor;
    QColor mBackgroundColor;

    double mBackgroundRoundness;

    int mMainLine;

    int mCount;
    int mLength;
    int mThickness;
    double mRoundness;
    int mTrail;
    double mOpacity;

    int mRadius;

    double mProgress;

private Q_SLOTS:
    void rotate();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
