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
// File handling interface
//==============================================================================

#include "interfacebegin.h"
    // Note: make sure to update fileHandlingInterfaceVersion() whenever you
    //       update this interface...

    VIRTUAL bool importFile(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName,
                          bool &pNeedFeedback) PURE_OR_OVERRIDE;

    VIRTUAL void fileOpened(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL void filePermissionsChanged(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL void fileModified(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL void fileSaved(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL void fileReloaded(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE_OR_OVERRIDE;
    VIRTUAL void fileClosed(const QString &pFileName) PURE_OR_OVERRIDE;
#include "interfaceend.h"

//==============================================================================
// End of file
//==============================================================================
