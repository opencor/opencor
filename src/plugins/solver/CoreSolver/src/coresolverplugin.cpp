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
// CoreSolver plugin
//==============================================================================

#include "coresolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

PLUGININFO_FUNC CoreSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core solver plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension de solveur de base."));

    return new PluginInfo(PluginInfo::Solver, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
