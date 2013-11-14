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
// CellML file issue
//==============================================================================

#ifndef CELLMLFILEISSUE_H
#define CELLMLFILEISSUE_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "disableunusedvariablewarning.inl"
    namespace OpenCOR {
    namespace CellMLSupport {
        static const char *Cellml_1_0 = "1.0";
        static const char *Cellml_1_1 = "1.1";
    }   // namespace CellMLSupport
    }   // namespace OpenCOR
#include "enableunusedvariablewarning.inl"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileIssue
{
public:
    enum Type {
        Error,
        Warning
    };

    explicit CellmlFileIssue(const Type &pType, const QString &pMessage,
                             const int &pLine = 0, const int &pColumn = 0,
                             const QString &pImportedFile = QString());

    Type type() const;
    QString message() const;
    QString formattedMessage() const;
    int line() const;
    int column() const;
    QString importedFile() const;

private:
    Type mType;
    QString mMessage;
    int mLine;
    int mColumn;
    QString mImportedFile;
};

//==============================================================================

typedef QList<CellmlFileIssue> CellmlFileIssues;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
