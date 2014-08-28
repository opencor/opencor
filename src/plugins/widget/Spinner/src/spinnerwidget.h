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

#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H

//==============================================================================

#include "spinnerglobal.h"

//==============================================================================

#include <QColor>
#include <QWidget>

//==============================================================================

class QTimer;

//==============================================================================

namespace OpenCOR {
namespace Spinner {

//==============================================================================

class SPINNER_EXPORT SpinnerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpinnerWidget(QWidget *pParent);

    int fps() const;
    void setFps(const int &pFps);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &pBackgroundColor);

    double backgroundRoundness() const;
    void setBackgroundRoundness(const double &pBackgroundRoundness);

    int lineCount() const;
    void setLineCount(const int &pLineCount);

    QColor lineColor() const;
    void setLineColor(const QColor &pLineColor);

    int lineLength() const;
    void setLineLength(const int &pLineLength);

    int lineWidth() const;
    void setLineWidth(const int &pLineWidth);

    double lineRoundness() const;
    void setLineRoundness(const double &pLineRoundness);

    int lineTrail() const;
    void setLineTrail(const int &pLineTrail);

    double lineOpacity() const;
    void setLineOpacity(const double &pLineOpacity);

    int radius() const;
    void setRadius(const int &pRadius);

protected:
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QTimer *mTimer;

    int mFps;

    QColor mBackgroundColor;
    double mBackgroundRoundness;

    int mMainLine;

    int mLineCount;
    QColor mLineColor;
    int mLineLength;
    int mLineWidth;
    double mLineRoundness;
    int mLineTrail;
    double mLineOpacity;

    int mRadius;

public Q_SLOTS:
    virtual void setVisible(bool pVisible);

private Q_SLOTS:
    void rotate();
};

//==============================================================================

}   // namespace Spinner
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
