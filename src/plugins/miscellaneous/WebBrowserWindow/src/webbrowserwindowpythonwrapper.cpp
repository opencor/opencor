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

#include "pythonsupport.h"
#include "webbrowserwindowplugin.h"
#include "webbrowserwindowpythonwrapper.h"
#include "webbrowserwindowwidget.h"

//==============================================================================

#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

static PyObject *browserWebView(PyObject *self, PyObject *args)
{
    Q_UNUSED(self);
    Q_UNUSED(args);

    return PythonSupport::wrapQObject(WebBrowserWindowPlugin::instance()->browserWidget()->webView());
}

//==============================================================================

static PyMethodDef pythonWebBrowserWindowMethods[] = {
    {"browserWebView",  browserWebView, METH_VARARGS, "browserWebView()\n\nReturn a QWebView of OpenCOR's web browser."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

WebBrowserWindowPythonWrapper::WebBrowserWindowPythonWrapper(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PyModule_AddFunctions(pModule, pythonWebBrowserWindowMethods);
}

//==============================================================================
}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
