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
// CoreEditing plugin
//==============================================================================

#ifndef COREEDITINGPLUGIN_H
#define COREEDITINGPLUGIN_H

//==============================================================================

#include "coreeditingglobal.h"
#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

#include "disableunusedvariablewarning.inl"
    namespace OpenCOR {
    namespace CoreEditing {
        static const char *EditGroup = "Edit";
    }   // namespace CoreEditing
    }   // namespace OpenCOR
#include "enableunusedvariablewarning.inl"

//==============================================================================

namespace OpenCOR {
namespace CoreEditing {

//==============================================================================

PLUGININFO_FUNC CoreEditingPluginInfo();

//==============================================================================

class COREEDITING_EXPORT CoreEditingPlugin : public QObject,
                                             public CoreInterface,
                                             public GuiInterface,
                                             public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CoreEditingPlugin" FILE "coreeditingplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
#include "coreinterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"

    // Plugin specific

    void setEditMenuVisible(const bool &pVisible);

private:
    QMenu *mEditMenu;

    QAction *mEditUndoAction;
    QAction *mEditRedoAction;
    QAction *mEditCutAction;
    QAction *mEditCopyAction;
    QAction *mEditPasteAction;
    QAction *mEditDeleteAction;
    QAction *mEditFindAction;
    QAction *mEditFindNextAction;
    QAction *mEditPreviousAction;
    QAction *mEditReplaceAction;
    QAction *mEditSelectAllAction;
};

//==============================================================================

}   // namespace CoreEditing
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
