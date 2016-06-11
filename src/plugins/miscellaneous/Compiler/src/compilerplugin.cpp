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
// Compiler plugin
//==============================================================================

#include "compilerplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

PLUGININFO_FUNC CompilerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support code compilation."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter la compilation de code."));

    return new PluginInfo("Miscellaneous", false, false,
                          QStringList() << "Core" << "LLVM",
                          descriptions);
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
