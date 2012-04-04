//==============================================================================
// Computer test
//==============================================================================

#include "computerengine.h"
#include "computermath.h"
#include "test.h"

//==============================================================================

#include "../../../../test/testutils.h"

//==============================================================================

#include "llvm/ExecutionEngine/ExecutionEngine.h"

//==============================================================================

static const double A = 5;
static const double B = 7.9;

static const double MinusA = -5;
static const double MinusB = -7.9;

static const double BigA = 3*5*7*11*13;
static const double BigB = 2*3*7*13*17;
static const double BigC = 5*7*17;

//==============================================================================

void Test::initTestCase()
{
    // Load the CellMLModelSupport plugin

    OpenCOR::loadPlugin("Computer");

    // Create our computer engine

    mComputerEngine = new OpenCOR::Computer::ComputerEngine;
}

//==============================================================================

void Test::cleanupTestCase()
{
    // Delete some internal objects

    delete mComputerEngine;
}

//==============================================================================

void Test::basicTests()
{
    // Check what happens when using an empty string to add a function

    mFunction = mComputerEngine->addFunction("");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "either 'void' or 'double' was expected, but 'EoF' was found instead");

    // Add 'void' to our string

    mFunction = mComputerEngine->addFunction("void");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "an identifier was expected, but 'EoF' was found instead");

    // Add an identifier to our string

    mFunction = mComputerEngine->addFunction("void voidFunc");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "'(' was expected, but 'EoF' was found instead");

    // Add a '(' to our string

    mFunction = mComputerEngine->addFunction("void voidFunc(");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "'double' or ')' was expected, but 'EoF' was found instead");

    // Add a ')' to our string

    mFunction = mComputerEngine->addFunction("void voidFunc()");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "'{' was expected, but 'EoF' was found instead");

    // Add a '{' to our string

    mFunction = mComputerEngine->addFunction("void voidFunc() {");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "'}' was expected, but 'EoF' was found instead");

    // Add a '}' to our string which should make it a valid void function

    mFunction = mComputerEngine->addFunction("void voidFunc() {}");

    QVERIFY(mFunction);

    // Make the function a double function

    mFunction = mComputerEngine->addFunction("double doubleFunc() {}");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "an identifier or 'return' was expected, but '}' was found instead");

    // Add 'return' to our string

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "the RHS of an equation was expected, but 'EoF' was found instead");

    // Add '3' (as the RHS of an equation) to our string

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "';' was expected, but 'EoF' was found instead");

    // Add ';' to our string

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3;");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "'}' was expected, but 'EoF' was found instead");

    // Add a '}' to our string which should make it a valid double function

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3; }");

    QVERIFY(mFunction);

    // Check that we cannot redefine a function with the same function name

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3; }");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->error().message()),
            "there is already a function called 'doubleFunc'");

    // Check that the function cannot work as an integer function

    mFunction = mComputerEngine->addFunction("int intFunc() { return 3; }");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "either 'void' or 'double' was expected, but 'int' was found instead");

    // Check what happens when using an invalid function name

    mFunction = mComputerEngine->addFunction("double .doubleFunc() { return 3; }");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "an identifier was expected, but '.' was found instead");

    // Check what happens when using an invalid RHS of an equation

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3*/a; }");

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "the RHS of an equation was expected, but '/' was found instead");
}

//==============================================================================

void Test::voidFunctionTests()
{
    double arrayA[3];
    double arrayB[3];

    // Initialise arrayA

    mFunction = mComputerEngine->addFunction(
                    "void aInitFunc(double *pArrayA)\n"
                    "{\n"
                    "    pArrayA[0] = 123;\n"
                    "    pArrayA[1] = 456;\n"
                    "    pArrayA[2] = 789;\n"
                    "}"
                );

    QVERIFY(mFunction);

    ((void (*)(double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(arrayA);

    QCOMPARE(arrayA[0], 123.0);
    QCOMPARE(arrayA[1], 456.0);
    QCOMPARE(arrayA[2], 789.0);

    // Initialise arrayB in reverse order of arrayA

    mFunction = mComputerEngine->addFunction(
                    "void bInitFunc(double *pArrayA, double *pArrayB)\n"
                    "{\n"
                    "    pArrayB[0] = pArrayA[2];\n"
                    "    pArrayB[1] = pArrayA[1];\n"
                    "    pArrayB[2] = pArrayA[0];\n"
                    "}"
                );

    QVERIFY(mFunction);

    ((void (*)(double *, double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(arrayA, arrayB);

    QCOMPARE(arrayB[0], 789.0);
    QCOMPARE(arrayB[1], 456.0);
    QCOMPARE(arrayB[2], 123.0);
}

//==============================================================================

void Test::timesOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double multFunc(double pNb1, double pNb2)\n"
                    "{\n"
                     "    return pNb1*pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 3*5.7);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, A*B);
}

//==============================================================================

void Test::divideOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double divFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1/pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 3/5.7);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, A/B);
}

//==============================================================================

void Test::moduloOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double modFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1%pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*3, 5);

    QCOMPARE(mResult, fmod(11*3, 5));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*A, B);

    QCOMPARE(mResult, fmod(11*A, B));
}

//==============================================================================

void Test::plusOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double addFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1+pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 3+5.7);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, A+B);
}

//==============================================================================

void Test::minusOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double subFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1-pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 3-5.7);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, A-B);
}

//==============================================================================

void Test::notOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double notFunc(double pNb)\n"
                    "{\n"
                    "    return !pNb;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A);

    QCOMPARE(mResult, 1.0);
}

//==============================================================================

void Test::orOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double orFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 || pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, !B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, !B);

    QCOMPARE(mResult, 0.0);
}

//==============================================================================

void Test::xorOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double xorFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 ^ pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, !B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, !B);

    QCOMPARE(mResult, 0.0);
}

//==============================================================================

void Test::andOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double andFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 && pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, !B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, !B);

    QCOMPARE(mResult, 0.0);
}

//==============================================================================

void Test::equalEqualOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double equalEqualFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 == pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QCOMPARE(mResult, 1.0);
}

//==============================================================================

void Test::notEqualOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double notEqualFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 != pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QCOMPARE(mResult, 0.0);
}

//==============================================================================

void Test::lowerThanOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double lowerThanFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 < pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QCOMPARE(mResult, 0.0);
}

//==============================================================================

void Test::greaterThanOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double greaterThanFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 > pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QCOMPARE(mResult, 1.0);
}

//==============================================================================

void Test::lowerOrEqualThanOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double lowerOrEqualThanFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 <= pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QCOMPARE(mResult, 0.0);
}

//==============================================================================

void Test::greaterOrEqualThanOperatorTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double greaterOrEqualThanFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pNb1 >= pNb2;\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5.7);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, 0.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QCOMPARE(mResult, 1.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QCOMPARE(mResult, 1.0);
}

//==============================================================================

void Test::absFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double absFunc(double pNb)\n"
                    "{\n"
                    "    return fabs(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, 3.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(-3);

    QCOMPARE(mResult, 3.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, 5.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(MinusA);

    QCOMPARE(mResult, 5.0);
}

//==============================================================================

void Test::expFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double expFunc(double pNb)\n"
                    "{\n"
                    "    return exp(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, exp(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, exp(A));
}

//==============================================================================

void Test::logFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double logFunc(double pNb)\n"
                    "{\n"
                    "    return log(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, log(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, log(A));
}

//==============================================================================

void Test::ceilFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double ceilFunc(double pNb)\n"
                    "{\n"
                    "    return ceil(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7);

    QCOMPARE(mResult, 6.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B);

    QCOMPARE(mResult, 8.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(-5.7);

    QCOMPARE(mResult, -5.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(MinusB);

    QCOMPARE(mResult, -7.0);
}

//==============================================================================

void Test::floorFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double floorFunc(double pNb)\n"
                    "{\n"
                    "    return floor(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7);

    QCOMPARE(mResult, 5.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B);

    QCOMPARE(mResult, 7.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(-5.7);

    QCOMPARE(mResult, -6.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(MinusB);

    QCOMPARE(mResult, -8.0);
}

//==============================================================================

void Test::factorialFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double factorialFunc(double pNb)\n"
                    "{\n"
                    "    return factorial(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, 6.0);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, 120.0);
}

//==============================================================================

void Test::sinFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double sinFunc(double pNb)\n"
                    "{\n"
                    "    return sin(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, sin(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, sin(A));
}

//==============================================================================

void Test::cosFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double cosFunc(double pNb)\n"
                    "{\n"
                    "    return cos(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, cos(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, cos(A));
}

//==============================================================================

void Test::tanFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double tanFunc(double pNb)\n"
                    "{\n"
                    "    return tan(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, tan(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, tan(A));
}

//==============================================================================

void Test::sinhFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double sinhFunc(double pNb)\n"
                    "{\n"
                    "    return sinh(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, sinh(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, sinh(A));
}

//==============================================================================

void Test::coshFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double coshFunc(double pNb)\n"
                    "{\n"
                    "    return cosh(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, cosh(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, cosh(A));
}

//==============================================================================

void Test::tanhFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double tanhFunc(double pNb)\n"
                    "{\n"
                    "    return tanh(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, tanh(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, tanh(A));
}

//==============================================================================

void Test::asinFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double asinFunc(double pNb)\n"
                    "{\n"
                    "    return asin(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/3);

    QCOMPARE(mResult, asin(1/3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/A);

    QCOMPARE(mResult, asin(1/A));
}

//==============================================================================

void Test::acosFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double acosFunc(double pNb)\n"
                    "{\n"
                    "    return acos(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/3);

    QCOMPARE(mResult, acos(1/3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/A);

    QCOMPARE(mResult, acos(1/A));
}

//==============================================================================

void Test::atanFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double atanFunc(double pNb)\n"
                    "{\n"
                    "    return atan(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, atan(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, atan(A));
}

//==============================================================================

void Test::asinhFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double asinhFunc(double pNb)\n"
                    "{\n"
                    "    return asinh(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, asinh(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, asinh(A));
}

//==============================================================================

void Test::acoshFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double acoshFunc(double pNb)\n"
                    "{\n"
                    "    return acosh(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3);

    QCOMPARE(mResult, acosh(3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QCOMPARE(mResult, acosh(A));
}

//==============================================================================

void Test::atanhFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double atanhFunc(double pNb)\n"
                    "{\n"
                    "    return atanh(pNb);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/3);

    QCOMPARE(mResult, atanh(1/3));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/A);

    QCOMPARE(mResult, atanh(1/A));
}

//==============================================================================

void Test::arbitraryLogFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double arbitraryLogFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return arbitraryLog(pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5);

    QCOMPARE(mResult, log(3)/log(5));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, log(A)/log(B));
}

//==============================================================================

void Test::powFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double powFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return pow(pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5);

    QCOMPARE(mResult, pow(3, 5));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QCOMPARE(mResult, pow(A, B));
}

//==============================================================================

void Test::quotFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double quotFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return quot(pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*3, 5);

    QCOMPARE(mResult, 6.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*A, B);

    QCOMPARE(mResult, 6.0);
}

//==============================================================================

void Test::remFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double remFunc(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return rem(pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*3, 5);

    QCOMPARE(mResult, 3.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*A, B);

    QCOMPARE(mResult, 7.6);
}

//==============================================================================

void Test::gcdFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double gcdFunc2(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return gcd(2, pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17);

    QCOMPARE(mResult, 273.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB);

    QCOMPARE(mResult, 273.0);

    mFunction = mComputerEngine->addFunction(
                    "double gcdFunc3(double pNb1, double pNb2, double pNb3)\n"
                    "{\n"
                    "    return gcd(3, pNb1, pNb2, pNb3);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17, 5*7*17);

    QCOMPARE(mResult, 7.0);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB, BigC);

    QCOMPARE(mResult, 7.0);
}

//==============================================================================

void Test::lcmFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double lcmFunc2(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return lcm(2, pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17);

    QCOMPARE(mResult, 510510.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB);

    QCOMPARE(mResult, 510510.0);

    mFunction = mComputerEngine->addFunction(
                    "double lcmFunc3(double pNb1, double pNb2, double pNb3)\n"
                    "{\n"
                    "    return lcm(3, pNb1, pNb2, pNb3);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17, 5*7*17);

    QCOMPARE(mResult, 510510.0);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB, BigC);

    QCOMPARE(mResult, 510510.0);
}

//==============================================================================

void Test::maxFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double maxFunc2(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return max(2, pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17);

    QCOMPARE(mResult, 15015.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB);

    QCOMPARE(mResult, 15015.0);

    mFunction = mComputerEngine->addFunction(
                    "double maxFunc3(double pNb1, double pNb2, double pNb3)\n"
                    "{\n"
                    "    return max(3, pNb1, pNb2, pNb3);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17, 5*7*17);

    QCOMPARE(mResult, 15015.0);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB, BigC);

    QCOMPARE(mResult, 15015.0);
}

//==============================================================================

void Test::minFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double minFunc2(double pNb1, double pNb2)\n"
                    "{\n"
                    "    return min(2, pNb1, pNb2);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17);

    QCOMPARE(mResult, 9282.0);

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB);

    QCOMPARE(mResult, 9282.0);

    mFunction = mComputerEngine->addFunction(
                    "double minFunc3(double pNb1, double pNb2, double pNb3)\n"
                    "{\n"
                    "    return min(3, pNb1, pNb2, pNb3);\n"
                    "}"
                );

    QVERIFY(mFunction);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3*5*7*11*13, 2*3*7*13*17, 5*7*17);

    QCOMPARE(mResult, 595.0);

    mResult = ((double (*)(double, double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(BigA, BigB, BigC);

    QCOMPARE(mResult, 595.0);
}

//==============================================================================

void Test::defIntFunctionTests()
{
    mFunction = mComputerEngine->addFunction(
                    "double defIntFunc()\n"
                    "{\n"
                    "    return defint(0, 2, x, x);\n"
                    "}"
                );

    QVERIFY(!mFunction);
    QCOMPARE(qPrintable(mComputerEngine->parserError().message()),
             "definite integrals are not yet supported");

}

//==============================================================================

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
