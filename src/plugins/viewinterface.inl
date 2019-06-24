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
// View interface
//==============================================================================

#ifdef INTERFACE_DEFINITION
    #define VIRTUAL virtual
    #define PURE_OR_OVERRIDE = 0
#else
    #define VIRTUAL
    #define PURE_OR_OVERRIDE override
#endif

    // Note: make sure to update viewInterfaceVersion() whenever you update this
    //       interface...

    VIRTUAL Mode viewMode() const PURE_OR_OVERRIDE;
    VIRTUAL QStringList viewMimeTypes() const PURE_OR_OVERRIDE;
    VIRTUAL QString viewMimeType(const QString &pFileName) const PURE_OR_OVERRIDE;
    VIRTUAL QString viewDefaultFileExtension() const PURE_OR_OVERRIDE;

    VIRTUAL QWidget * viewWidget(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL void removeViewWidget(const QString &pFileName) PURE_OR_OVERRIDE;
    VIRTUAL QString viewName() const PURE_OR_OVERRIDE;

    VIRTUAL QIcon fileTabIcon(const QString &pFileName) const PURE_OR_OVERRIDE;

#undef VIRTUAL
#undef PURE_OR_OVERRIDE

//==============================================================================
// End of file
//==============================================================================
