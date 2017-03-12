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
// Python wrapper for Core plugin
//==============================================================================

#include "centralwidget.h"
#include "coreguiutils.h"
#include "pythonwrappercore.h"
#include "pythonwrapperplugin.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewsimulationwidget.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace PythonWrapper {

//==============================================================================

static PyObject *initializeSimulation(const QString &pFileName)
{
    SingleCellView::SingleCellViewWidget *singleCellViewWidget = PythonWrapperPlugin::instance()->singleCellViewWidget();
    if (singleCellViewWidget) {
        singleCellViewWidget->initialize(pFileName);
        auto simulation = singleCellViewWidget->simulation(pFileName);
        if (simulation) return PythonQt::priv()->wrapQObject(simulation);
    }
    Py_RETURN_NONE;
}

//==============================================================================

static PyObject *openSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self);

    PyObject *bytes;
    char *name;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
        Py_RETURN_NONE;
    }
    PyBytes_AsStringAndSize(bytes, &name, &len);
    QString fileName = QString::fromUtf8(name, len);
    Py_DECREF(bytes);

    Core::centralWidget()->openFile(fileName);
    return initializeSimulation(fileName);
}

//==============================================================================

static PyObject *openRemoteSimulation(PyObject *self, PyObject *args)
{
    Q_UNUSED(self);

    PyObject *bytes;
    char *name;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "O&", PyUnicode_FSConverter, &bytes)) {
        Py_RETURN_NONE;
    }
    PyBytes_AsStringAndSize(bytes, &name, &len);
    QString url = QString::fromUtf8(name, len);
    Py_DECREF(bytes);

    Core::centralWidget()->openRemoteFile(url);
    return initializeSimulation(Core::centralWidget()->localFileName(url));
}

//==============================================================================

static PyMethodDef wrappedCoreMethods[] = {
    {"openSimulation",  openSimulation, METH_VARARGS, "Open a simulation."},
    {"openRemoteSimulation",  openRemoteSimulation, METH_VARARGS, "Open a remote simulation."},
    {NULL, NULL, 0, NULL}
};

//==============================================================================

PythonWrapperCore::PythonWrapperCore(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    PyModule_AddFunctions(pModule, wrappedCoreMethods);
}

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
