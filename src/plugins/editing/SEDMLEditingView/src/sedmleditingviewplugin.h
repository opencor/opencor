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

private slots:
    void reformat();

    void sedmlValidation();
};

//==============================================================================

}   // namespace SEDMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
