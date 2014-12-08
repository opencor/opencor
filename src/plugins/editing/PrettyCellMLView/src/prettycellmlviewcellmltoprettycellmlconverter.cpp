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
// CellML to pretty CellML converter
//==============================================================================

#include "corecliutils.h"
#include "prettycellmlviewcellmltoprettycellmlconverter.h"

//==============================================================================

#include <QDomDocument>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellMLViewCellmlToPrettyCellmlConverter::PrettyCellMLViewCellmlToPrettyCellmlConverter(const QString &pFileName) :
    mFileName(pFileName),
    mOutput(QString()),
    mErrorLine(-1),
    mErrorColumn(-1),
    mErrorMessage(QString())
{
}

//==============================================================================

bool PrettyCellMLViewCellmlToPrettyCellmlConverter::execute()
{
    // Convert the CellML file to pretty CellML by first getting a DOM
    // representation of the file

    QString fileContents;
    QDomDocument domDocument;

    Core::readTextFromFile(mFileName, fileContents);

    if (domDocument.setContent(fileContents, &mErrorMessage, &mErrorLine, &mErrorColumn)) {
        mOutput = QString();

        return true;
    } else {
        mOutput = fileContents;

        return false;
    }
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::output() const
{
    // Return our output

    return mOutput;
}

//==============================================================================

int PrettyCellMLViewCellmlToPrettyCellmlConverter::errorLine() const
{
    // Return our error line

    return mErrorLine;
}

//==============================================================================

int PrettyCellMLViewCellmlToPrettyCellmlConverter::errorColumn() const
{
    // Return our error column

    return mErrorColumn;
}

//==============================================================================

QString PrettyCellMLViewCellmlToPrettyCellmlConverter::errorMessage() const
{
    // Return our error message

    return mErrorMessage;
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
