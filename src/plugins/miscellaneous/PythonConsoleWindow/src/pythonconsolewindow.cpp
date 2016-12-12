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
// An dockable Python console window
//==============================================================================

#include "borderedwidget.h"
#include "ctkAbstractPythonManager.h"
#include "ctkPythonConsole.h"
#include "pythonconsolewindow.h"
#include "pythonqtsupportplugin.h"

//==============================================================================

#include "ui_pythonconsolewindow.h"

//==============================================================================

namespace OpenCOR {
namespace PythonConsoleWindow {

//==============================================================================

PythonConsoleWindow::PythonConsoleWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::PythonConsoleWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Get the Python manager

    auto pythonManager = PythonQtSupport::PythonQtSupportPlugin::pythonManager();

    // Create a new Python console widget

    mPythonConsoleWidget = new ctkPythonConsole(this);
    mPythonConsoleWidget->initialize(pythonManager);

    // Add the widget to Python's main module

    pythonManager->addObjectToPythonMain("PythonConsoleInstance", mPythonConsoleWidget);

    // Add the widget to our window

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPythonConsoleWidget,
                                                     true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(new Core::BorderedWidget(mPythonConsoleWidget,
                                                     true, false, false, false));
#else
    #error Unsupported platform
#endif

}

//==============================================================================

PythonConsoleWindow::~PythonConsoleWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

}   // namespace PythonConsoleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
