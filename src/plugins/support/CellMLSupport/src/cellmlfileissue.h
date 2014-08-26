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

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

static const auto Cellml_1_0 = QStringLiteral("1.0");
static const auto Cellml_1_1 = QStringLiteral("1.1");

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileIssue
{
public:
    enum Type {
        Error,
        Warning
    };

    explicit CellmlFileIssue(const Type &pType, const int &pLine,
                             const int &pColumn, const QString &pMessage,
                             const QString &pImportedFile);
    explicit CellmlFileIssue(const Type &pType, const QString &pMessage);

    bool operator<(const CellmlFileIssue &pIssue) const;

    Type type() const;
    int line() const;
    int column() const;
    QString message() const;
    QString formattedMessage() const;
    QString importedFile() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
    QString mImportedFile;

    void constructor(const Type &pType, const int &pLine,
                     const int &pColumn, const QString &pMessage,
                     const QString &pImportedFile);
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
