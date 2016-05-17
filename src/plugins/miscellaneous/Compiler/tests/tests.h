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
// Compiler tests
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace Compiler {
    class CompilerEngine;
}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

class Tests : public QObject
{
    Q_OBJECT

private:
    OpenCOR::Compiler::CompilerEngine *mCompilerEngine;

    double mA;
    double mB;

    double mMinusA;
    double mMinusB;

    double mBigA;
    double mBigB;
    double mBigC;

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
    void sinhFunctionTests();
    void asinFunctionTests();
    void asinhFunctionTests();

    void cosFunctionTests();
    void coshFunctionTests();
    void acosFunctionTests();
    void acoshFunctionTests();

    void tanFunctionTests();
    void tanhFunctionTests();
    void atanFunctionTests();
    void atanhFunctionTests();

    void secFunctionTests();
    void sechFunctionTests();
    void asecFunctionTests();
    void asechFunctionTests();

    void cscFunctionTests();
    void cschFunctionTests();
    void acscFunctionTests();
    void acschFunctionTests();

    void cotFunctionTests();
    void cothFunctionTests();
    void acotFunctionTests();
    void acothFunctionTests();

    void powFunctionTests();
    void arbitraryLogFunctionTests();

    void minFunctionTests();
    void maxFunctionTests();

    void gcdFunctionTests();
    void lcmFunctionTests();
};

//==============================================================================
// End of file
//==============================================================================
