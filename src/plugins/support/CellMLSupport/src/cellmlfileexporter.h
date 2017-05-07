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
// CellML file exporter
//==============================================================================

#pragma once

//==============================================================================

#include <QString>

//==============================================================================

#include "cellmlapibegin.h"
    #include "IfaceCellML_APISPEC.hxx"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CellmlFileExporter
{
public:
    explicit CellmlFileExporter();

    bool result() const;

    QString errorMessage() const;

protected:
    bool mResult;

    QString mErrorMessage;

    bool saveModel(iface::cellml_api::Model *pModel, const QString &pFileName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
