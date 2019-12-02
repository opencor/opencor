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
// SUNDIALS plugin
//==============================================================================

#include "sundialsplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SUNDIALS {

//==============================================================================

PLUGININFO_FUNC SUNDIALSPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin to access the <a href="https://computation.llnl.gov/projects/sundials/arkode">ARKode</a>, <a href="https://computation.llnl.gov/projects/sundials/cvodes">CVODES</a>, <a href="https://computation.llnl.gov/projects/sundials/idas">IDAS</a> and <a href="https://computation.llnl.gov/projects/sundials/kinsol">KINSOL</a> solvers from <a href="https://computation.llnl.gov/projects/sundials">SUNDIALS</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension pour accéder les solveurs <a href="https://computation.llnl.gov/projects/sundials/arkode">ARKode</a>, <a href="https://computation.llnl.gov/projects/sundials/cvodes">CVODES</a>, <a href="https://computation.llnl.gov/projects/sundials/idas">IDAS</a> et <a href="https://computation.llnl.gov/projects/sundials/kinsol">KINSOL</a> de <a href="https://computation.llnl.gov/projects/sundials">SUNDIALS</a>.)"));

    return new PluginInfo(PluginInfo::Category::ThirdParty, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

} // namespace SUNDIALS
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
