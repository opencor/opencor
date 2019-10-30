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
// Web Browser window Python wrapper
//==============================================================================

#include "pythonqtsupport.h"
#include "webbrowserwindowplugin.h"
#include "webbrowserwindowpythonwrapper.h"
#include "webbrowserwindowwidget.h"

//==============================================================================

#include <QWebView>

//==============================================================================

#include <array>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

static PyObject * webBrowserWindowWebView(PyObject *pSelf, PyObject *pArgs)
{
    Q_UNUSED(pSelf)
    Q_UNUSED(pArgs)

    // Return a Python object to our Web browser window's Web view

    return PythonQtSupport::wrapQObject(WebBrowserWindowPlugin::instance()->webBrowserWindowWidget()->webView());
}
//==============================================================================

WebBrowserWindowPythonWrapper::WebBrowserWindowPythonWrapper(PyObject *pModule,
                                                             QObject *pParent) :
    QObject(pParent)
{
    // Add some Python wrappers

    static std::array<PyMethodDef, 2> PythonWebBrowserWindowMethods = {{
                                                                          { "webBrowserWindowWebView",  webBrowserWindowWebView, METH_VARARGS, "Return OpenCOR's Web Browser window's QWebView object." },
                                                                          { nullptr, nullptr, 0, nullptr }
                                                                      }};

    PyModule_AddFunctions(pModule, PythonWebBrowserWindowMethods.data());
}

//==============================================================================
}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
