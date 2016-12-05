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
// Plugin interface
//==============================================================================

#pragma once

//==============================================================================

#include "plugin.h"

//==============================================================================

#include <QUrl>

//==============================================================================

class QMainWindow;
class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int pluginInterfaceVersion();

//==============================================================================

class PluginInterface
{
public:
#define INTERFACE_DEFINITION
    #include "plugininterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

typedef QList<PluginInterface *> PluginInterfaces;

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR::PluginInterface")

//==============================================================================
// End of file
//==============================================================================
