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

#include "opencmiss/zinc/fieldfiniteelement.hpp"
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

    void initData(const int &pDataSize, const double &pMinimumTime,
                  const double &pMaximumTime, const double &pTimeInterval,
                  double *pTimeValues, double *pR0Values, double *pQ1Values,
                  double *pThetaValues);
    void addData(const int &pCurrentDataSize);

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

    int mCurrentDataSize;

    double *mTimeValues;
    double *mR0Values;
    double *mQ1Values;
    double *mThetaValues;

    void customizeZincContext();

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
