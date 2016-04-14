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
// MathML viewer plugin
//==============================================================================

#include "mathmlviewerplugin.h"

//==============================================================================

namespace OpenCOR {
namespace MathMLViewer {

//==============================================================================

PLUGININFO_FUNC MathMLViewerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to visualise MathML documents."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour visualiser des documents MathML."));

    return new PluginInfo("Widget", false, false,
                          QStringList() << "Core" << "Qwt",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void MathMLViewerPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================

}   // namespace MathMLViewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
