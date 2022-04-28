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
// Compiler tests
//==============================================================================

#include "compilerengine.h"
#include "compilermath.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <array>

//==============================================================================

void Tests::initTestCase()
{
    // Create our compiler engine

    mCompilerEngine = new OpenCOR::Compiler::CompilerEngine();

    // Initialise some values

    mA = 5.0;
    mB = 7.9;

    mMinusA = -5.0;
    mMinusB = -7.9;

    mBigA = 3.0*5.0*7.0*11.0*13.0;
    mBigB = 2.0*3.0*7.0*13.0*17.0;
    mBigC = 5.0*7.0*17.0;
}

//==============================================================================

void Tests::cleanupTestCase()
{
    // Delete some internal objects

    delete mCompilerEngine;
}

//==============================================================================

void Tests::basicTests()
{
    // Check what happens when using an empty string to add a function

    QVERIFY(mCompilerEngine->compileCode(""));

    // Add 'void' to our string

    QVERIFY(!mCompilerEngine->compileCode("void"));

    // Add an identifier to our string

    QVERIFY(!mCompilerEngine->compileCode("void function"));

    // Add a '(' to our string

    QVERIFY(!mCompilerEngine->compileCode("void function("));

    // Add a ')' to our string

    QVERIFY(!mCompilerEngine->compileCode("void function()"));

    // Add a '{' to our string

    QVERIFY(!mCompilerEngine->compileCode("void function() {"));

    // Add a '}' to our string which should make it a valid void function

    QVERIFY(mCompilerEngine->compileCode("void function() {}"));

    // Make the function a double function

    QVERIFY(!mCompilerEngine->compileCode("double function() {}"));

    // Add 'return' to our string

    QVERIFY(!mCompilerEngine->compileCode("double function() { return"));

    // Add '3' (as the RHS of an equation) to our string

    QVERIFY(!mCompilerEngine->compileCode("double function() { return 3.0"));

    // Add ';' to our string

    QVERIFY(!mCompilerEngine->compileCode("double function() { return 3.0;"));

    // Add a '}' to our string which should make it a valid double function

    QVERIFY(mCompilerEngine->compileCode("double function() { return 3.0; }"));

    // Check what happens when using an invalid function name

    QVERIFY(!mCompilerEngine->compileCode("double .function() { return 3.0; }"));

    // Check what happens when using an invalid RHS of an equation

    QVERIFY(!mCompilerEngine->compileCode("double function() { return 3.0*/a; }"));
}

//==============================================================================

void Tests::voidFunctionTests()
{
    std::array<double, 3> arrayA = {};
    std::array<double, 3> arrayB = {};

    // Initialise arrayA

    QVERIFY(mCompilerEngine->compileCode("void function(double *pArrayA)\n"
                                         "{\n"
                                         "    pArrayA[0] = 123.0;\n"
                                         "    pArrayA[1] = 456.0;\n"
                                         "    pArrayA[2] = 789.0;\n"
                                         "}"));

    reinterpret_cast<void (*)(double *)>(mCompilerEngine->function("function"))(arrayA.data());

    QVERIFY(qFuzzyCompare(arrayA[0], 123.0));
    QVERIFY(qFuzzyCompare(arrayA[1], 456.0));
    QVERIFY(qFuzzyCompare(arrayA[2], 789.0));

    // Initialise arrayB in reverse order of arrayA

    QVERIFY(mCompilerEngine->compileCode("void function(double *pArrayA, double *pArrayB)\n"
                                         "{\n"
                                         "    pArrayB[0] = pArrayA[2];\n"
                                         "    pArrayB[1] = pArrayA[1];\n"
                                         "    pArrayB[2] = pArrayA[0];\n"
                                         "}"));

    reinterpret_cast<void (*)(double *, double *)>(mCompilerEngine->function("function"))(arrayA.data(), arrayB.data());

    QVERIFY(qFuzzyCompare(arrayB[0], 789.0));
    QVERIFY(qFuzzyCompare(arrayB[1], 456.0));
    QVERIFY(qFuzzyCompare(arrayB[2], 123.0));
}

//==============================================================================

void Tests::timesOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1*pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          3.0*5.7));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          mA*mB));
}

//==============================================================================

void Tests::divideOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1/pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          3.0/5.7));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          mA/mB));
}

//==============================================================================

void Tests::moduloOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (double) ((int) pNb1 % (int) pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(11.0*3.0, 5.0),
                          double(int(11.0*3.0)%int(5.0))));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(11.0*mA, mB),
                          double(int(11.0*mA)%int(mB))));
}

//==============================================================================

void Tests::plusOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1+pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          3.0+5.7));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          mA+mB));
}

//==============================================================================

void Tests::minusOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1-pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          3.0-5.7));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          mA-mB));
}

//==============================================================================

void Tests::notOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return !pNb;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(0.0),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA)),
                          1.0));
}

//==============================================================================

void Tests::orOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 || pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(0.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 0.0),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(0.0, 0.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA), mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, qFuzzyIsNull(mB)),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA), qFuzzyIsNull(mB)),
                          0.0));
}

//==============================================================================

void Tests::xorOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (pNb1 != 0) ^ (pNb2 != 0);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(0.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 0.0),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(0.0, 0.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA), mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, qFuzzyIsNull(mB)),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA), qFuzzyIsNull(mB)),
                          0.0));
}

//==============================================================================

void Tests::andOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 && pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(0.0, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 0.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(0.0, 0.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA), mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, qFuzzyIsNull(mB)),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(qFuzzyIsNull(mA), qFuzzyIsNull(mB)),
                          0.0));
}

//==============================================================================

void Tests::equalEqualOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 == pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0+2.7, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA+2.9, mB),
                          1.0));
}

//==============================================================================

void Tests::notEqualOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 != pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0+2.7, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA+2.9, mB),
                          0.0));
}

//==============================================================================

void Tests::lowerThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 < pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0+2.7, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(5.7, 3.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA+2.9, mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mB, mA),
                          0.0));
}

//==============================================================================

void Tests::greaterThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 > pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0+2.7, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(5.7, 3.0),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA+2.9, mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mB, mA),
                          1.0));
}

//==============================================================================

void Tests::lowerOrEqualThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 <= pNb2;\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0+2.7, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(5.7, 3.0),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA+2.9, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mB, mA),
                          0.0));
}

//==============================================================================

void Tests::greaterOrEqualThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (double) (pNb1 >= pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.7),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0+2.7, 5.7),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(5.7, 3.0),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          0.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA+2.9, mB),
                          1.0));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mB, mA),
                          1.0));
}

//==============================================================================

void Tests::absFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return fabs(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          fabs(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(-3.0),
                          fabs(-3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          fabs(mA)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mMinusA),
                          fabs(mMinusA)));
}

//==============================================================================

void Tests::expFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return exp(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          exp(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          exp(mA)));
}

//==============================================================================

void Tests::logFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return log(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          log(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          log(mA)));
}

//==============================================================================

void Tests::ceilFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return ceil(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(5.7),
                          ceil(5.7)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mB),
                          ceil(mB)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(-5.7),
                          ceil(-5.7)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mMinusB),
                          ceil(mMinusB)));
}

//==============================================================================

void Tests::floorFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return floor(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(5.7),
                          floor(5.7)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mB),
                          floor(mB)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(-5.7),
                          floor(-5.7)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mMinusB),
                          floor(mMinusB)));
}

//==============================================================================

void Tests::factorialFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return factorial(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          factorial(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.7),
                          factorial(3.7)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          factorial(mA)));
}

//==============================================================================

void Tests::sinFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sin(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          sin(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          sin(mA)));
}

//==============================================================================

void Tests::sinhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sinh(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          sinh(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          sinh(mA)));
}

//==============================================================================

void Tests::asinFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asin(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/3.0),
                          asin(1.0/3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/mA),
                          asin(1.0/mA)));
}

//==============================================================================

void Tests::asinhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asinh(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          asinh(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          asinh(mA)));
}

//==============================================================================

void Tests::cosFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return cos(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          cos(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          cos(mA)));
}

//==============================================================================

void Tests::coshFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return cosh(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          cosh(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          cosh(mA)));
}

//==============================================================================

void Tests::acosFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acos(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/3.0),
                          acos(1.0/3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/mA),
                          acos(1.0/mA)));
}

//==============================================================================

void Tests::acoshFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acosh(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          acosh(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          acosh(mA)));
}

//==============================================================================

void Tests::tanFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return tan(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          tan(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          tan(mA)));
}

//==============================================================================

void Tests::tanhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return tanh(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          tanh(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          tanh(mA)));
}

//==============================================================================

void Tests::atanFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return atan(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          atan(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          atan(mA)));
}

//==============================================================================

void Tests::atanhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return atanh(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/3.0),
                          atanh(1.0/3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/mA),
                          atanh(1.0/mA)));
}

//==============================================================================

void Tests::secFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sec(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          sec(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          sec(mA)));
}

//==============================================================================

void Tests::sechFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sech(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          sech(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          sech(mA)));
}

//==============================================================================

void Tests::asecFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asec(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          asec(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          asec(mA)));
}

//==============================================================================

void Tests::asechFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asech(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/3.0),
                          asech(1.0/3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/mA),
                          asech(1.0/mA)));
}

//==============================================================================

void Tests::cscFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return csc(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          csc(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          csc(mA)));
}

//==============================================================================

void Tests::cschFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return csch(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          csch(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          csch(mA)));
}

//==============================================================================

void Tests::acscFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acsc(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          acsc(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          acsc(mA)));
}

//==============================================================================

void Tests::acschFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acsch(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          acsch(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          acsch(mA)));
}

//==============================================================================

void Tests::cotFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return cot(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          cot(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          cot(mA)));
}

//==============================================================================

void Tests::cothFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return coth(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          coth(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          coth(mA)));
}

//==============================================================================

void Tests::acotFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acot(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/3.0),
                          acot(1.0/3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(1.0/mA),
                          acot(1.0/mA)));
}

//==============================================================================

void Tests::acothFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acoth(pNb);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(3.0),
                          acoth(3.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double)>(mCompilerEngine->function("function"))(mA),
                          acoth(mA)));
}

//==============================================================================

void Tests::powFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pow(pNb1, pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.0),
                          pow(3.0, 5.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          pow(mA, mB)));
}

//==============================================================================

void Tests::arbitraryLogFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return arbitrary_log(pNb1, pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0, 5.0),
                          arbitrary_log(3.0, 5.0)));
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mA, mB),
                          arbitrary_log(mA, mB)));
}

//==============================================================================

void Tests::minFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return multi_min(2, pNb1, pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
                          multi_min(2, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB),
                          multi_min(2, mBigA, mBigB))); // NOLINT(cppcoreguidelines-pro-type-vararg)

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return multi_min(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
                          multi_min(3, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB, mBigC),
                          multi_min(3, mBigA, mBigB, mBigC))); // NOLINT(cppcoreguidelines-pro-type-vararg)
}

//==============================================================================

void Tests::maxFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return multi_max(2, pNb1, pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
                          multi_max(2, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB),
                          multi_max(2, mBigA, mBigB))); // NOLINT(cppcoreguidelines-pro-type-vararg)

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return multi_max(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
                          multi_max(3, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB, mBigC),
                          multi_max(3, mBigA, mBigB, mBigC))); // NOLINT(cppcoreguidelines-pro-type-vararg)
}

//==============================================================================

void Tests::gcdFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return gcd_multi(2, pNb1, pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
                          gcd_multi(2, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB),
                          gcd_multi(2, mBigA, mBigB))); // NOLINT(cppcoreguidelines-pro-type-vararg)

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return gcd_multi(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
                          gcd_multi(3, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB, mBigC),
                          gcd_multi(3, mBigA, mBigB, mBigC))); // NOLINT(cppcoreguidelines-pro-type-vararg)
}

//==============================================================================

void Tests::lcmFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return lcm_multi(2, pNb1, pNb2);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
                          lcm_multi(2, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB),
                          lcm_multi(2, mBigA, mBigB))); // NOLINT(cppcoreguidelines-pro-type-vararg)

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return lcm_multi(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
                          lcm_multi(3, 3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0))); // NOLINT(cppcoreguidelines-pro-type-vararg)
    QVERIFY(qFuzzyCompare(reinterpret_cast<double (*)(double, double, double)>(mCompilerEngine->function("function"))(mBigA, mBigB, mBigC),
                          lcm_multi(3, mBigA, mBigB, mBigC))); // NOLINT(cppcoreguidelines-pro-type-vararg)
}

//==============================================================================

QTEST_APPLESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
