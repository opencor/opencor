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
// Busy widget
//==============================================================================

#pragma once

//==============================================================================

#include <QColor>
#include <QWidget>

//==============================================================================

class QTimer;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class BusyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BusyWidget(QWidget *pParent, double pProgress);

    int fps() const;
    void setFps(int pFps);

    QColor foregroundColor() const;
    void setForegroundColor(const QColor &pForegroundColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &pBackgroundColor);

    double backgroundRoundness() const;
    void setBackgroundRoundness(double pBackgroundRoundness);

    int count() const;
    void setCount(int pCount);

    int length() const;
    void setLength(int pLength);

    int thickness() const;
    void setThickness(int pThickness);

    double roundness() const;
    void setRoundness(double pRoundness);

    int trail() const;
    void setTrail(int pTrail);

    double opacity() const;
    void setOpacity(double pOpacity);

    int radius() const;
    void setRadius(int pRadius);

    double progress() const;
    void setProgress(double pProgress);

    void resize();

protected:
    void paintEvent(QPaintEvent *pEvent) override;

private:
    QWidget *mParent;

    QTimer *mTimer;

    int mFps = 0;

    QColor mForegroundColor = Qt::white;
    QColor mBackgroundColor = QColor(54, 96, 146);

    double mBackgroundRoundness = 0.25;

    int mMainLine = 0;

    int mCount = 12;
    int mLength = 5;
    int mThickness = 5;
    double mRoundness = 1.0;
    int mTrail = 100;
    double mOpacity = 0.25;

    int mRadius = 15;

    double mProgress;

private slots:
    void rotate();
};

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
