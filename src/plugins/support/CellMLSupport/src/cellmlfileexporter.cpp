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
