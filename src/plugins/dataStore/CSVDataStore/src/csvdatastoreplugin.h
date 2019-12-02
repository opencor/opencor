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
// CSV data store plugin
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"
#include "filetypeinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace CSVDataStore {

//==============================================================================

PLUGININFO_FUNC CSVDataStorePluginInfo();

//==============================================================================

static const auto CsvMimeType      = QStringLiteral("text/csv");
static const auto CsvFileExtension = QStringLiteral("csv");

//==============================================================================

class CSVDataStorePlugin : public QObject, public DataStoreInterface,
                           public FileTypeInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CSVDataStorePlugin" FILE "csvdatastoreplugin.json")

    Q_INTERFACES(OpenCOR::FileTypeInterface)
    Q_INTERFACES(OpenCOR::DataStoreInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit CSVDataStorePlugin();

#include "datastoreinterface.inl"
#include "filetypeinterface.inl"
#include "i18ninterface.inl"
};

//==============================================================================

} // namespace CSVDataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
