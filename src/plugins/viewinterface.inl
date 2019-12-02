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
// View interface
//==============================================================================

#include "interfacebegin.h"
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
#include "interfaceend.h"

//==============================================================================
// End of file
//==============================================================================
