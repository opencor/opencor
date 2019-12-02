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
// PythonQt support plugin
//==============================================================================

#pragma once

//==============================================================================

#include "plugininfo.h"
#include "plugininterface.h"
#include "pythonqtsupportglobal.h"

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQt.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

PLUGININFO_FUNC PythonQtSupportPluginInfo();

//==============================================================================

class PYTHONQTSUPPORT_EXPORT PythonQtSupportPlugin : public QObject, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PythonQtSupportPlugin" FILE "pythonqtsupportplugin.json")

    Q_INTERFACES(OpenCOR::PluginInterface)

public:
#include "plugininterface.inl"

private:
    wchar_t **mArgV = nullptr;
    PythonQtObjectPtr mModule = nullptr;

private slots:
    void printStdOut(const QString &pString);
    void printStdErr(const QString &pString);
};

//==============================================================================

} // namespace PythonQtSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
