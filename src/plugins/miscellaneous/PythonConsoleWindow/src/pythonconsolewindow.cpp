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
// Python Console window
//==============================================================================
// Note: to debug the Python code, use `import pdb; pdb.set_trace()` to set a
//       breakpoint and start OpenCOR from the command line...
//==============================================================================

#include "borderedwidget.h"
#include "pythonconsolewindow.h"
#include "pythonqtsupport.h"

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

    // Create a Python module to setup the console

    static const QString PythonConsole = R"PYTHON(
import logging
import sys

# Add the current working directory to IPython's search path

sys.path.insert(0, '.')

# Create an in-process kernel manager and Jupyter widget

from qtconsole.inprocess import QtInProcessKernelManager
from qtconsole.inprocess import QtInProcessRichJupyterWidget

class QtConsole(object):
    def __init__(self):
        # Create and start an in-process kernel manager

        self._kernel_manager = QtInProcessKernelManager()

        self._kernel_manager.start_kernel(show_banner=False)

        # Set the kernel's GUI to PythonQt

        self._kernel = self._kernel_manager.kernel
        self._kernel.gui = 'pythonqt'

        self._kernel.shell.enable_gui()
        self._kernel.shell.enable_matplotlib()

        # Start the kernel's client

        self._kernel_client = self._kernel_manager.client()

        self._kernel_client.start_channels()

        # Create a Jupyter widget and connect it with our kernel and its client

        self._widget = QtInProcessRichJupyterWidget(font_size=10, enable_calltips=False)
        self._widget.kernel_client = self._kernel_client
        self._widget.kernel_manager = self._kernel_manager

        # sys.stderr will now be redirected to the console, however logging is
        # still going to PythonQt (and the terminal shell), so we redirect
        # logging output to the console

        logging.getLogger().handlers[0].stream = sys.stderr

    def widget(self):
        return self._widget

# The console we are creating
# Note: we make it a global so it doesn't get garbage collected...

qt_console = None

def create_ipython_widget():
    global qt_console

    qt_console = QtConsole()

    return qt_console.widget()
)PYTHON";

    PythonQt *pythonQtInstance = PythonQt::self();
    PythonQtObjectPtr qtConsoleModule = pythonQtInstance->createModuleFromScript("opencor.qtconsole", PythonConsole);

    if (qtConsoleModule == nullptr) {
        if (PyErr_Occurred() != nullptr) {
            PyErr_Print();
        } else {
            std::cerr << "The Qt Console module could not be created." << std::endl;
        }

        return;
    }

    // IPython tracebacks are noisy if modules have an empty filename, so set
    // one

    qtConsoleModule.addVariable("__file__", "QtConsole");

    // Create and retrieve an IPython widget

    PyObject *createWidget = pythonQtInstance->lookupObject(qtConsoleModule, "create_ipython_widget");
    PyObject *ipythonWidget = pythonQtInstance->callAndReturnPyObject(createWidget);
    auto widgetWrapper = reinterpret_cast<PythonQtInstanceWrapper *>(ipythonWidget);

    if (widgetWrapper == nullptr) {
        if (PyErr_Occurred() != nullptr) {
            PyErr_Print();
        } else {
            std::cerr << "An IPython widget could not be created." << std::endl;
        }

        return;
    }

    mPythonConsoleWidget = static_cast<QWidget *>(widgetWrapper->_objPointerCopy);

    mPythonConsoleWidget->setStyleSheet("QWidget {"
                                        "    border: none;"
                                        "}");

    setFocusProxy(mPythonConsoleWidget);

    // Add the IPython widget to our layout

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPythonConsoleWidget,
                                                     true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(new Core::BorderedWidget(mPythonConsoleWidget,
                                                     false, false, false, false));
#endif
}

//==============================================================================

PythonConsoleWindow::~PythonConsoleWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PythonConsoleWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);
}

//==============================================================================

} // namespace PythonConsoleWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
