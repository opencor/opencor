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
// COMBINE archive issue
//==============================================================================

#pragma once

//==============================================================================

#include "combinesupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

class COMBINESUPPORT_EXPORT CombineArchiveIssue
{
public:
    enum Type {
        Information,
        Error,
        Warning,
        Fatal
    };

    explicit CombineArchiveIssue(const Type &pType, const QString &pMessage);

    Type type() const;
    QString message() const;

private:
    Type mType;
    QString mMessage;
};

//==============================================================================

typedef QList<CombineArchiveIssue> CombineArchiveIssues;

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
