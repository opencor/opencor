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
// Python wrapper for web browser window
//==============================================================================

#include "webbrowserwindowplugin.h"
#include "webbrowserwindowwidget.h"

//==============================================================================

#include <QWebView>

//==============================================================================

#include "pythonqtsupport.h"
#include "webbrowserwindowpythonwrapper.h"

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

static PyObject *browserWebView(PyObject *pSelf, PyObject *pArgs)
{
    Q_UNUSED(pSelf)
    Q_UNUSED(pArgs)

    return PythonQtSupport::wrapQObject(WebBrowserWindowPlugin::instance()->browserWidget()->webView());
}

//==============================================================================

static PyMethodDef pythonWebBrowserWindowMethods[] = {
    { "browserWebView",  browserWebView, METH_VARARGS, "browserWebView()\n\nReturn a QWebView of OpenCOR's web browser." },
    { nullptr, nullptr, 0, nullptr }
};

//==============================================================================

WebBrowserWindowPythonWrapper::WebBrowserWindowPythonWrapper(PyObject *pModule,
                                                             QObject *pParent) :
    QObject(pParent)
{
    PyModule_AddFunctions(pModule, pythonWebBrowserWindowMethods);
}

//==============================================================================
}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
