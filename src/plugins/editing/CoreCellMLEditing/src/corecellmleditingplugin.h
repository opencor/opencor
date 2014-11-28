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
// CoreCellMLEditing plugin
//==============================================================================

#ifndef CORECELLMLEDITINGPLUGIN_H
#define CORECELLMLEDITINGPLUGIN_H

//==============================================================================

#include "cellmlfile.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class CellmlEditingInterface;

//==============================================================================

namespace CoreCellMLEditing {

//==============================================================================

PLUGININFO_FUNC CoreCellMLEditingPluginInfo();

//==============================================================================

class CoreCellMLEditingPlugin : public QObject, public GuiInterface,
                                public I18nInterface, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CoreCellMLEditingPlugin" FILE "corecellmleditingplugin.json")

    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit CoreCellMLEditingPlugin();

#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    QMainWindow *mMainWindow;

    QAction *mFileNewCellml1_0FileAction;
    QAction *mFileNewCellml1_1FileAction;

    QAction *mToolsCellmlValidationAction;

    QString mFileName;
    CellmlEditingInterface *mCellmlEditingInterface;

    void newCellmlFile(const CellMLSupport::CellmlFile::Version &pVersion);

private Q_SLOTS:
    void newCellml1_0File();
    void newCellml1_1File();

    void cellmlValidation();
};

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
