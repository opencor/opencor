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
// Editing view interface
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EditorWidget;

//==============================================================================

} // namespace EditorWidget

//==============================================================================

extern "C" Q_DECL_EXPORT int editingViewInterfaceVersion();

//==============================================================================

class EditingViewInterface
{
public:
    virtual ~EditingViewInterface();

#define INTERFACE_DEFINITION
    #include "editingviewinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::EditingViewInterface, "OpenCOR::EditingViewInterface")

//==============================================================================
// End of file
//==============================================================================
