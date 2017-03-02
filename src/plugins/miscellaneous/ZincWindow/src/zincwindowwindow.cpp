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
// Zinc window
//==============================================================================

#include "borderedwidget.h"
#include "zincwidget.h"
#include "zincwindowwindow.h"

//==============================================================================

#include "ui_zincwindowwindow.h"

//==============================================================================

namespace OpenCOR {
namespace ZincWindow {

//==============================================================================

ZincWindowWindow::ZincWindowWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::ZincWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and add a Zinc window widget

    mZincWidget = new ZincWidget::ZincWidget(this);
    mZincContext = new OpenCMISS::Zinc::Context("ZincWindowWindow");

    connect(mZincWidget, SIGNAL(graphicsInitialized()),
            this, SLOT(graphicsInitialized()));

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mZincWindowWidget,
                                                     false, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(mZincWidget);
#else
    #error Unsupported platform
#endif
}

//==============================================================================

ZincWindowWindow::~ZincWindowWindow()
{
    // Delete some internal objects

    delete mZincContext;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void ZincWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);
}

//==============================================================================

void ZincWindowWindow::graphicsInitialized()
{
    // Our Zinc widget has had its graphics initialised, so we can now set its
    // background colour

    double backgroundColor[3] = {0.0, 0.0, 0.0};

    mZincWidget->sceneViewer().setBackgroundColourRGB(backgroundColor);
}

//==============================================================================

}   // namespace ZincWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
