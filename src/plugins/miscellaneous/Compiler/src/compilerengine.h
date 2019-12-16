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
// Compiler engine
//==============================================================================

#pragma once

//==============================================================================

#include "compilerglobal.h"

//==============================================================================

#include <QObject>
#include <QString>

//==============================================================================

namespace llvm {
    class ExecutionEngine;
} // namespace llvm

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class COMPILER_EXPORT CompilerEngine : public QObject
{
    Q_OBJECT

public:
    ~CompilerEngine() override;

    bool hasError() const;
    QString error() const;

    bool compileCode(const QString &pCode);

    void * getFunction(const QString &pFunctionName);

private:
    llvm::ExecutionEngine *mExecutionEngine = nullptr;

    QString mError;
};

//==============================================================================

} // namespace Compiler
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
