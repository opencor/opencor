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
// SED-ML file issue
//==============================================================================

#pragma once

//==============================================================================

#include "sedmlsupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

class SEDMLSUPPORT_EXPORT SedmlFileIssue
{
public:
    enum Type {
        Information,
        Error,
        Warning,
        Fatal
    };

    explicit SedmlFileIssue(const Type &pType, const int &pLine,
                            const int &pColumn, const QString &pMessage);
    explicit SedmlFileIssue(const Type &pType, const QString &pMessage);

    bool operator<(const SedmlFileIssue &pIssue) const;

    Type type() const;
    int line() const;
    int column() const;
    QString message() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;

    void constructor(const Type &pType, const int &pLine,
                     const int &pColumn, const QString &pMessage);
};

//==============================================================================

typedef QList<SedmlFileIssue> SedmlFileIssues;

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
