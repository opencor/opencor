/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

    descriptions.insert("en", QString::fromUtf8("a plugin to access the <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a>, <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> and <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_kinsol\">KINSOL</a> solvers from <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a> 2.6.2."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder les solveurs <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a>, <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> et <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_kinsol\">KINSOL</a> de <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a> 2.6.2."));

    return new PluginInfo("Third-party", false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace SUNDIALS
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
