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

#include "cellmlfileexporter.h"
#include "corecliutils.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileExporter::CellmlFileExporter() :
    mResult(false),
    mErrorMessage(QString())
{
}

//==============================================================================

bool CellmlFileExporter::result() const
{
    // Return the result of the conversion

    return mResult;
}

//==============================================================================

QString CellmlFileExporter::errorMessage() const
{
    // Return the error message following the conversion

    return mErrorMessage;
}

//==============================================================================

bool CellmlFileExporter::saveModel(iface::cellml_api::Model *pModel,
                                   const QString &pFileName)
{
    // Save the given model or ouput it to the console, if no file name has been
    // provided, and this after having reformatted the given model

    QDomDocument domDocument;

    domDocument.setContent(QString::fromStdWString(pModel->serialisedText()));

    if (pFileName.isEmpty()) {
        std::wcout << QString(Core::serialiseDomDocument(domDocument)).toStdWString() << std::endl;

        return true;
    } else {
        return Core::writeFileContentsToFile(pFileName, Core::serialiseDomDocument(domDocument));
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
