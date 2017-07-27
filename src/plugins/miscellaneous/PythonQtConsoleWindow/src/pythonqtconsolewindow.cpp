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
// An dockable iPython console window for Qt
//==============================================================================

#include "borderedwidget.h"
#include "CTK/ctkAbstractPythonManager.h"
#include "pythonqtconsolewindow.h"
#include "pythonsupportplugin.h"

//==============================================================================

#include <PythonQt/PythonQt.h>

//==============================================================================

#include "ui_pythonqtconsolewindow.h"

//==============================================================================

namespace OpenCOR {
namespace PythonQtConsoleWindow {

//==============================================================================

// An IPython console

static QString PythonQtConsole = R"PYTHON(
import logging
import sys

from qtconsole.inprocess import QtInProcessKernelManager
from qtconsole.inprocess import QtInProcessRichJupyterWidget

# The widget we are creating -- it's a global so it doesn't get
# garbage collected.

ipython_widget = None

def create_ipython_widget():
    global ipython_widget

    # Create and start an in-process kernel manager

    kernel_manager = QtInProcessKernelManager()
    kernel_manager.start_kernel(show_banner=False)

    # Set the kernel's GUI to PythonQt

    kernel = kernel_manager.kernel
    kernel.gui = 'pythonqt'

    # Start the kernel's client

    kernel_client = kernel_manager.client()
    kernel_client.start_channels()

    # Create a Jupyter widget and connect it with our kernel and
    # its client

    ipython_widget = QtInProcessRichJupyterWidget(font_size=12)
    ipython_widget.kernel_manager = kernel_manager
    ipython_widget.kernel_client = kernel_client

    # `sys.stderr` will now be redirected to the console, however
    # logging is still going to PythonQt (and the terminal shell)
    # so we redirect logging output to the console.

    logging.getLogger().handlers[0].stream = sys.stderr

    return ipython_widget
)PYTHON";


//==============================================================================

PythonQtConsoleWindow::PythonQtConsoleWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::PythonQtConsoleWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Get the PythonQt Qt manager

    PythonQt *pythonQtInstance = PythonQt::self();

    // Create a Python module to setup the console

    PythonQtObjectPtr qtConsoleModule = pythonQtInstance->createModuleFromScript("opencor.qtconsole", PythonQtConsole);

    // Create and retrive an IPython widget

    PyObject *createWidget = pythonQtInstance->lookupObject(qtConsoleModule, "create_ipython_widget");

    PyObject *ipythonWidget = pythonQtInstance->callAndReturnPyObject(createWidget);

    if (ipythonWidget && PyObject_TypeCheck(ipythonWidget, &PythonQtInstanceWrapper_Type)) {

        PythonQtInstanceWrapper *widgetWrapper = (PythonQtInstanceWrapper*)ipythonWidget;

        mPythonQtConsoleWidget = (QWidget *)widgetWrapper->_objPointerCopy;

        // Add the widget to our window

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
        mGui->layout->addWidget(new Core::BorderedWidget(mPythonQtConsoleWidget,
                                                         true, true, true, true));
#elif defined(Q_OS_MAC)
        mGui->layout->addWidget(new Core::BorderedWidget(mPythonQtConsoleWidget,
                                                         true, false, false, false));
#else
        #error Unsupported platform
#endif
    }
}

//==============================================================================

PythonQtConsoleWindow::~PythonQtConsoleWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

}   // namespace PythonQtConsoleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
