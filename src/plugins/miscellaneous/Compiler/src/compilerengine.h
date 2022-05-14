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

#include "llvmclangbegin.h"
    #include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvmclangend.h"

//==============================================================================

#include <QObject>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class COMPILER_EXPORT CompilerEngine : public QObject
{
    Q_OBJECT

public:
    bool hasError() const;
    QString error() const;

    bool addFunction(const QString &pName, void *pFunction);

    bool compileCode(const QString &pCode);

    void * function(const QString &pName);

private:
    std::unique_ptr<llvm::orc::LLJIT> mLljit;

    QString mError;
};

//==============================================================================

} // namespace Compiler
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
