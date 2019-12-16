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
// PythonQt support
//==============================================================================

#pragma once

//==============================================================================

#include "pythonqtsupportglobal.h"

//==============================================================================

#include <QObject>
#include <QMetaObject>
#include <QString>
#include <QVariant>

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQt.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

void PYTHONQTSUPPORT_EXPORT addInstanceDecorators(QObject *pObject);
void PYTHONQTSUPPORT_EXPORT addObject(PyObject *pPyObject, const QString &pName,
                                      QObject *pObject);

void PYTHONQTSUPPORT_EXPORT evaluateFile(const QString &pFileName);
QVariant PYTHONQTSUPPORT_EXPORT evaluateScript(const QString &pScript);

PythonQtObjectPtr PYTHONQTSUPPORT_EXPORT importModule(const QString &pModule);

void PYTHONQTSUPPORT_EXPORT registerClass(const QMetaObject *pMetaObject);

PyObject PYTHONQTSUPPORT_EXPORT * wrapQObject(QObject *pObject);

PythonQtInstanceWrapper PYTHONQTSUPPORT_EXPORT * getInstanceWrapper(PyObject *pSelf);

//==============================================================================

} // namespace PythonQtSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

