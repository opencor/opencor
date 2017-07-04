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
// Zinc window
//==============================================================================

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

namespace Ui {
    class ZincWindowWindow;
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

namespace ZincWindow {

//==============================================================================

class ZincWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit ZincWindowWindow(QWidget *pParent);
    ~ZincWindowWindow();

    virtual void retranslateUi();

private:
    Ui::ZincWindowWindow *mGui;

    ZincWidget::ZincWidget *mZincWidget;
    OpenCMISS::Zinc::Context *mZincContext;

    char *mZincSceneViewerDescription;

    int mAxesFontPointSize;

private slots:
    void createAndSetZincContext();
    void graphicsInitialized();
    void devicePixelRatioChanged(const int &pDevicePixelRatio);
};

//==============================================================================

}   // namespace ZincWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
