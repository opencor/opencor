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
// Compiler engine
//==============================================================================

#pragma once

//==============================================================================

#include "compilerglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "llvmbegin.h"
    #include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvmend.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class COMPILER_EXPORT CompilerEngine
{
public:
    explicit CompilerEngine();

    bool hasError() const;
    QString error() const;

    bool compileCode(const QString &pCode);

    void * getFunction(const QString &pFunctionName);

private:
    std::unique_ptr<llvm::ExecutionEngine> mExecutionEngine;

    QString mError;
};

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
