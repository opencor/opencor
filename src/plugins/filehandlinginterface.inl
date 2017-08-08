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
// File handling interface
//==============================================================================

#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    // Note: make sure to update fileHandlingInterfaceVersion() whenever you
    //       update this interface...

    virtual bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName,
                          bool &pNeedFeedback) PURE;

    virtual void fileOpened(const QString &pFileName) PURE;
    virtual void filePermissionsChanged(const QString &pFileName) PURE;
    virtual void fileModified(const QString &pFileName) PURE;
    virtual void fileReloaded(const QString &pFileName,
                              const bool &pFileChanged) PURE;
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE;
    virtual void fileClosed(const QString &pFileName) PURE;

#undef PURE

//==============================================================================
// End of file
//==============================================================================
