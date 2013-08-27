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
// Compiler test
//==============================================================================

#include <QtGlobal>

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

namespace llvm {
    class Function;
}   // namespace llvm

//==============================================================================

namespace OpenCOR {
namespace Compiler {
    class CompilerEngine;
}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

class Test : public QObject
{
    Q_OBJECT

private:
    OpenCOR::Compiler::CompilerEngine *mCompilerEngine;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void basicTests();

    void voidFunctionTests();

    void timesOperatorTests();
    void divideOperatorTests();
    void moduloOperatorTests();
    void plusOperatorTests();
    void minusOperatorTests();

    void notOperatorTests();
    void orOperatorTests();
    void xorOperatorTests();
    void andOperatorTests();
    void equalEqualOperatorTests();
    void notEqualOperatorTests();
    void lowerThanOperatorTests();
    void greaterThanOperatorTests();
    void lowerOrEqualThanOperatorTests();
    void greaterOrEqualThanOperatorTests();

    void absFunctionTests();
    void expFunctionTests();
    void logFunctionTests();
    void ceilFunctionTests();
    void floorFunctionTests();
    void factorialFunctionTests();
    void sinFunctionTests();
    void cosFunctionTests();
    void tanFunctionTests();
    void sinhFunctionTests();
    void coshFunctionTests();
    void tanhFunctionTests();
    void asinFunctionTests();
    void acosFunctionTests();
    void atanFunctionTests();
    void asinhFunctionTests();
    void acoshFunctionTests();
    void atanhFunctionTests();

    void arbitraryLogFunctionTests();
    void powFunctionTests();

    void gcdFunctionTests();
    void lcmFunctionTests();
    void maxFunctionTests();
    void minFunctionTests();

    void defIntFunctionTests();
};

//==============================================================================
// End of file
//==============================================================================
