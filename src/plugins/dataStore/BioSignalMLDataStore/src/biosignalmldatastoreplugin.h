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
// BioSignalML data store plugin
//==============================================================================

#pragma once

//==============================================================================

#include "biosignalmldatastoreexporter.h"
#include "datastoreinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

PLUGININFO_FUNC BioSignalMLDataStorePluginInfo();

//==============================================================================

class BioSignalMLDataStorePlugin : public QObject, public I18nInterface,
                                   public DataStoreInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.BioSignalMLDataStorePlugin" FILE "biosignalmldatastoreplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::DataStoreInterface)

public:
#include "i18ninterface.inl"
#include "datastoreinterface.inl"
};

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
