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

#include "pendulumwindowglobal.h"
#include "windowwidget.h"

//==============================================================================

#include <QTimer>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/fieldfiniteelement.hpp"
    #include "opencmiss/zinc/timekeeper.hpp"
#include "zincend.h"

//==============================================================================

class QCheckBox;
class QLabel;
class QSlider;

//==============================================================================

namespace Ui {
    class PendulumWindowWindow;
} // namespace Ui

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

class PENDULUMWINDOW_EXPORT PendulumWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit PendulumWindowWindow(QWidget *pParent);
    ~PendulumWindowWindow() override;

    void retranslateUi() override;

    void initData(const quint64 &pDataSize, double pMinimumTime,
                  double pMaximumTime, double pTimeInterval, double *pR0Values,
                  double *pQ1Values, double *pThetaValues);
    void addData(int pCurrentDataSize);

private:
    Ui::PendulumWindowWindow *mGui;

    ZincWidget::ZincWidget *mZincWidget;
    OpenCMISS::Zinc::Context *mZincContext;

    QTimer mTimer;

    QLabel *mTimeLabel;
    QSlider *mTimeSlider;
    QCheckBox *mTimeCheckBox;

    OpenCMISS::Zinc::Fieldmodule mFieldModule;
    OpenCMISS::Zinc::Timekeeper mTimeKeeper;
    OpenCMISS::Zinc::FieldFiniteElement mR0;
    OpenCMISS::Zinc::FieldFiniteElement mQ1;
    OpenCMISS::Zinc::FieldFiniteElement mTheta;
    OpenCMISS::Zinc::Fieldcache mFieldCache;

    char *mZincSceneViewerDescription;

    int mAxesFontPointSize;

    bool mInitialiseZincScene;

    int mCurrentDataSize;

    double *mTimeValues;
    double *mR0Values;
    double *mQ1Values;
    double *mThetaValues;

private slots:
    void createAndSetZincContext();
    void graphicsInitialized();

    void timeSliderValueChanged(int pTime);
    void timerTimeOut();

    void autoMode();
};

//==============================================================================

}   // namespace PendulumWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
