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
// Python Qt support functions
//==============================================================================

#pragma once

//==============================================================================

#include "pythonqtsupportglobal.h"

//==============================================================================

#if defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wold-style-cast"
	#pragma GCC diagnostic ignored "-Wreserved-id-macro"
	#pragma GCC diagnostic ignored "-Wshift-sign-overflow"
#endif

#include <PythonQt/PythonQt.h>

#if defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif

//==============================================================================

#include <QObject>
#include <QMetaObject>
#include <QString>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

PYTHONQTSUPPORT_EXPORT
void addInstanceDecorators(QObject *pQObject);

PYTHONQTSUPPORT_EXPORT
void addObject(PyObject *pObject, const QString &pName, QObject *pQObject);

PYTHONQTSUPPORT_EXPORT
QVariant evalScript(const QString &pScript);

PYTHONQTSUPPORT_EXPORT
void registerClass(const QMetaObject *pMetaObject);

PYTHONQTSUPPORT_EXPORT
PyObject *wrapQObject(QObject *pQObject);

//==============================================================================

}   // namespace PythonQtSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

