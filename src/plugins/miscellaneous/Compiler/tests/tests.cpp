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

#ifdef _WIN32
    #define _SCL_SECURE_NO_WARNINGS
#endif

//==============================================================================

#include "compilerengine.h"
#include "compilermath.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "llvmbegin.h"
    #include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvmend.h"

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
    double arrayA[3];
    double arrayB[3];

    // Initialise arrayA

    QVERIFY(mCompilerEngine->compileCode("void function(double *pArrayA)\n"
                                         "{\n"
                                         "    pArrayA[0] = 123.0;\n"
                                         "    pArrayA[1] = 456.0;\n"
                                         "    pArrayA[2] = 789.0;\n"
                                         "}"));

    ((void (*)(double *)) (intptr_t) mCompilerEngine->getFunction("function"))(arrayA);

    QCOMPARE(arrayA[0], 123.0);
    QCOMPARE(arrayA[1], 456.0);
    QCOMPARE(arrayA[2], 789.0);

    // Initialise arrayB in reverse order of arrayA

    QVERIFY(mCompilerEngine->compileCode("void function(double *pArrayA, double *pArrayB)\n"
                                         "{\n"
                                         "    pArrayB[0] = pArrayA[2];\n"
                                         "    pArrayB[1] = pArrayA[1];\n"
                                         "    pArrayB[2] = pArrayA[0];\n"
                                         "}"));

    ((void (*)(double *, double *)) (intptr_t) mCompilerEngine->getFunction("function"))(arrayA, arrayB);

    QCOMPARE(arrayB[0], 789.0);
    QCOMPARE(arrayB[1], 456.0);
    QCOMPARE(arrayB[2], 123.0);
}

//==============================================================================

void Tests::timesOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1*pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0*5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             mA*mB);
}

//==============================================================================

void Tests::divideOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1/pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0/5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             mA/mB);
}

//==============================================================================

void Tests::moduloOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (double) ((int) pNb1 % (int) pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(11.0*3.0, 5.0),
             double(int(11.0*3.0) % int(5.0)));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(11.0*mA, mB),
             double(int(11.0*mA) % int(mB)));
}

//==============================================================================

void Tests::plusOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1+pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0+5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             mA+mB);
}

//==============================================================================

void Tests::minusOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1-pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0-5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             mA-mB);
}

//==============================================================================

void Tests::notOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return !pNb;\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             0.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0),
             1.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             0.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA),
             1.0);
}

//==============================================================================

void Tests::orOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 || pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 0.0),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0, 0.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, !mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA, !mB),
             0.0);
}

//==============================================================================

void Tests::xorOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (pNb1 != 0) ^ (pNb2 != 0);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 0.0),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0, 0.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, !mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA, !mB),
             0.0);
}

//==============================================================================

void Tests::andOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 && pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 0.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(0.0, 0.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, !mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!mA, !mB),
             0.0);
}

//==============================================================================

void Tests::equalEqualOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 == pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA+2.9, mB),
             1.0);
}

//==============================================================================

void Tests::notEqualOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 != pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA+2.9, mB),
             0.0);
}

//==============================================================================

void Tests::lowerThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 < pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7, 3.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA+2.9, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mB, mA),
             0.0);
}

//==============================================================================

void Tests::greaterThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 > pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7, 3.0),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA+2.9, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mB, mA),
             1.0);
}

//==============================================================================

void Tests::lowerOrEqualThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 <= pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7, 3.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA+2.9, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mB, mA),
             0.0);
}

//==============================================================================

void Tests::greaterOrEqualThanOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (double) (pNb1 >= pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7, 3.0),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA+2.9, mB),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mB, mA),
             1.0);
}

//==============================================================================

void Tests::absFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return fabs(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             3.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(-3.0),
             3.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mMinusA),
             5.0);
}

//==============================================================================

void Tests::expFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return exp(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_exp(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_exp(mA));
}

//==============================================================================

void Tests::logFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return log(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_log(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_log(mA));
}

//==============================================================================

void Tests::ceilFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return ceil(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7),
             6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mB),
             8.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(-5.7),
             -5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mMinusB),
             -7.0);
}

//==============================================================================

void Tests::floorFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return floor(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7),
             5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mB),
             7.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(-5.7),
             -6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mMinusB),
             -8.0);
}

//==============================================================================

void Tests::factorialFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return factorial(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             120.0);
}

//==============================================================================

void Tests::sinFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sin(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_sin(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_sin(mA));
}

//==============================================================================

void Tests::sinhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sinh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_sinh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_sinh(mA));
}

//==============================================================================

void Tests::asinFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asin(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             compiler_asin(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/mA),
             compiler_asin(1.0/mA));
}

//==============================================================================

void Tests::asinhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asinh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_asinh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_asinh(mA));
}

//==============================================================================

void Tests::cosFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return cos(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_cos(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_cos(mA));
}

//==============================================================================

void Tests::coshFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return cosh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_cosh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_cosh(mA));
}

//==============================================================================

void Tests::acosFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acos(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             compiler_acos(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/mA),
             compiler_acos(1.0/mA));
}

//==============================================================================

void Tests::acoshFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acosh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_acosh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_acosh(mA));
}

//==============================================================================

void Tests::tanFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return tan(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_tan(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_tan(mA));
}

//==============================================================================

void Tests::tanhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return tanh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_tanh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_tanh(mA));
}

//==============================================================================

void Tests::atanFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return atan(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_atan(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_atan(mA));
}

//==============================================================================

void Tests::atanhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return atanh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             compiler_atanh(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/mA),
             compiler_atanh(1.0/mA));
}

//==============================================================================

void Tests::secFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sec(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_sec(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_sec(mA));
}

//==============================================================================

void Tests::sechFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return sech(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_sech(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_sech(mA));
}

//==============================================================================

void Tests::asecFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asec(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_asec(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_asec(mA));
}

//==============================================================================

void Tests::asechFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return asech(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             compiler_asech(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/mA),
             compiler_asech(1.0/mA));
}

//==============================================================================

void Tests::cscFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return csc(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_csc(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_csc(mA));
}

//==============================================================================

void Tests::cschFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return csch(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_csch(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_csch(mA));
}

//==============================================================================

void Tests::acscFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acsc(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_acsc(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_acsc(mA));
}

//==============================================================================

void Tests::acschFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acsch(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_acsch(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_acsch(mA));
}

//==============================================================================

void Tests::cotFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return cot(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_cot(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_cot(mA));
}

//==============================================================================

void Tests::cothFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return coth(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_coth(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_coth(mA));
}

//==============================================================================

void Tests::acotFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acot(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             compiler_acot(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/mA),
             compiler_acot(1.0/mA));
}

//==============================================================================

void Tests::acothFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return acoth(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             compiler_acoth(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA),
             compiler_acoth(mA));
}

//==============================================================================

void Tests::powFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pow(pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.0),
             compiler_pow(3.0, 5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             compiler_pow(mA, mB));
}

//==============================================================================

void Tests::arbitraryLogFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return arbitrary_log(pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.0),
             compiler_log(3.0)/log(5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mA, mB),
             compiler_log(mA)/log(mB));
}

//==============================================================================

void Tests::minFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return multi_min(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             9282.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB),
             9282.0);

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return multi_min(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             595.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB, mBigC),
             595.0);
}

//==============================================================================

void Tests::maxFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return multi_max(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             15015.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB),
             15015.0);

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return multi_max(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             15015.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB, mBigC),
             15015.0);
}

//==============================================================================

void Tests::gcdFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return gcd_multi(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             273.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB),
             273.0);

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return gcd_multi(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             7.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB, mBigC),
             7.0);
}

//==============================================================================

void Tests::lcmFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return lcm_multi(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             510510.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB),
             510510.0);

    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return lcm_multi(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             510510.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(mBigA, mBigB, mBigC),
             510510.0);
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
