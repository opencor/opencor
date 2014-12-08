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

#ifndef PRETTYCELLMLVIEWCELLMLTOPRETTYCELLMLCONVERTER_H
#define PRETTYCELLMLVIEWCELLMLTOPRETTYCELLMLCONVERTER_H

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

class PrettyCellMLViewCellmlToPrettyCellmlConverter
{
public:
    explicit PrettyCellMLViewCellmlToPrettyCellmlConverter(const QString &pFileName);

    bool execute();

    QString output() const;

    int errorLine() const;
    int errorColumn() const;

    QString errorMessage() const;

private:
    QString mFileName;

    QString mOutput;

    int mErrorLine;
    int mErrorColumn;

    QString mErrorMessage;
};

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
