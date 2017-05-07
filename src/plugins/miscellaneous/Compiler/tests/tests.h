/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

private slots:
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
