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
// PythonQt support
//==============================================================================

#include "pythonqtsupport.h"

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

void addInstanceDecorators(QObject *pObject)
{
    // Add some instance decorators to the given object

    PythonQt::self()->addInstanceDecorators(pObject);
}

//==============================================================================

void addObject(PyObject *pPyObject, const QString &pName, QObject *pObject)
{
    // Add the given Qt object, using the given name, to the given Python object

    PythonQt::self()->addObject(pPyObject, pName, pObject);
}

//==============================================================================

void evaluateFile(const QString &pFilename)
{
    // Evaluate the script which file name is given

    PythonQt::self()->evalFile(PythonQt::self()->getMainModule(), pFilename);
}

//==============================================================================

QVariant evaluateScript(const QString &pScript)
{
    // Evaluate the given script

    return PythonQt::self()->evalScript(PythonQt::self()->getMainModule(), pScript);
}

//==============================================================================

PythonQtObjectPtr importModule(const QString &pModule)
{
    // Import the given module

    return PythonQt::self()->importModule(pModule);
}

//==============================================================================

void registerClass(const QMetaObject *pMetaObject)
{
    // Register the class which meta object is given

    PythonQt::self()->registerClass(pMetaObject);
}

//==============================================================================

PyObject * wrapQObject(QObject *pObject)
{
    // Wrap the given Qt object into a Python object

    return PythonQt::priv()->wrapQObject(pObject);
}

//==============================================================================

PythonQtInstanceWrapper * getInstanceWrapper(PyObject *pSelf)
{
    // Retrieve and return a Python wrapper object for the given Python object

    if (   (pSelf != nullptr)
#include "pythonbegin.h"
        && PyObject_TypeCheck(pSelf, &PythonQtInstanceWrapper_Type)) {
#include "pythonend.h"
        return reinterpret_cast<PythonQtInstanceWrapper *>(pSelf);
    }

    return nullptr;
}

//==============================================================================

} // namespace PythonQtSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

