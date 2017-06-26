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
// Pendulum window
//==============================================================================

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

#include <QTimer>

//==============================================================================

#include "opencmiss/zinc/timekeeper.hpp"

//==============================================================================

class QCheckBox;
class QLabel;
class QSlider;

//==============================================================================

namespace Ui {
    class PendulumWindowWindow;
}

//==============================================================================

namespace OpenCMISS {
namespace Zinc {
    class Context;
}   // namespace Zinc
}   // namespace OpenCMISS

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace ZincWidget {
    class ZincWidget;
}   // namespace ZincWidget

//==============================================================================

namespace PendulumWindow {

//==============================================================================

class PendulumWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit PendulumWindowWindow(QWidget *pParent);
    ~PendulumWindowWindow();

    virtual void retranslateUi();

    void setDataSize(const int &pDataSize);

    void setTimeValues(double *pTimeValues);
    void setQ1Values(double *pQ1Values);
    void setThetaValues(double *pThetaValues);

    void setR0Value(const double &pR0Value);

private:
    Ui::PendulumWindowWindow *mGui;

    ZincWidget::ZincWidget *mZincWidget;
    OpenCMISS::Zinc::Context *mZincContext;

    QTimer mTimer;

    QLabel *mTimeLabel;
    QSlider *mTimeSlider;
    QCheckBox *mTimeCheckBox;
    OpenCMISS::Zinc::Timekeeper mTimeKeeper;

    char *mZincSceneViewerDescription;

    int mAxesFontPointSize;

    int mDataSize;

    double *mTimeValues;
    double *mQ1Values;
    double *mThetaValues;

    double mR0Value;

private slots:
    void createAndSetZincContext();
    void graphicsInitialized();
    void devicePixelRatioChanged(const int &pDevicePixelRatio);

    void updateScene(const int &pTime);
    void updateScene();

    void autoMode();
};

//==============================================================================

}   // namespace PendulumWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
