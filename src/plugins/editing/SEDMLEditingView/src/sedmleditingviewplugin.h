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
// SED-ML Editing view plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filehandlinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class SedmlEditingViewInterface;

//==============================================================================

namespace SEDMLEditingView {

//==============================================================================

PLUGININFO_FUNC SEDMLEditingViewPluginInfo();

//==============================================================================

class SEDMLEditingViewPlugin : public QObject, public FileHandlingInterface,
                               public GuiInterface, public I18nInterface,
                               public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SEDMLEditingViewPlugin" FILE "sedmleditingviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit SEDMLEditingViewPlugin();

#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    QAction *mEditReformatAction;

    QAction *mToolsSedmlValidationAction;

    QString mFileName;
    SedmlEditingViewInterface *mSedmlEditingViewInterface;

private Q_SLOTS:
    void reformat();

    void sedmlValidation();
};

//==============================================================================

}   // namespace SEDMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
