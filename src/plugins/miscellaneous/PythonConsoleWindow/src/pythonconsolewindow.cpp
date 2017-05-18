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

    // Get the Python Qt manager

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

QStringList PythonConsoleWindow::getCommandHistory() const
{
    return mPythonConsoleWidget->getCommandHistory();
}

//==============================================================================

void PythonConsoleWindow::setCommandHistory(const QStringList &pCommandHistory)
{
    mPythonConsoleWidget->setCommandHistory(pCommandHistory);
}

//==============================================================================

}   // namespace PythonConsoleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
