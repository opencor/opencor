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

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"http://computation.llnl.gov/projects/sundials/cvode\">CVODE</a>, <a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> and <a href=\"http://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> solvers from <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder les solveurs <a href=\"http://computation.llnl.gov/projects/sundials/cvode\">CVODE</a>, <a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> et <a href=\"http://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> de <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a>."));

    return new PluginInfo(PluginInfo::ThirdParty, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace SUNDIALS
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
