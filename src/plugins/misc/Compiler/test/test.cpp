//==============================================================================
// Compiler test
//==============================================================================

#include "compilerengine.h"
#include "compilermath.h"
#include "test.h"

//==============================================================================

#include "../../../../test/testutils.h"

//==============================================================================

#include "llvm/ExecutionEngine/ExecutionEngine.h"

//==============================================================================

static const double A = 5.0;
static const double B = 7.9;

static const double MinusA = -5.0;
static const double MinusB = -7.9;

static const double BigA = 3.0*5.0*7.0*11.0*13.0;
static const double BigB = 2.0*3.0*7.0*13.0*17.0;
static const double BigC = 5.0*7.0*17.0;

//==============================================================================

void Test::initTestCase()
{
    // Load the Compiler plugin

    OpenCOR::loadPlugin("Compiler");

    // Create our compiler engine

    mCompilerEngine = new OpenCOR::Compiler::CompilerEngine();
}

//==============================================================================

void Test::cleanupTestCase()
{
    // Delete some internal objects

    delete mCompilerEngine;
}

//==============================================================================

void Test::basicTests()
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

void Test::voidFunctionTests()
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

void Test::timesOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1*pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0*5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             A*B);
}

//==============================================================================

void Test::divideOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1/pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0/5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             A/B);
}

//==============================================================================

void Test::moduloOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (double) ((int) pNb1 % (int) pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(11.0*3.0, 5.0),
             (double) ((int) (11.0*3.0) % (int) 5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(11.0*A, B),
             (double) ((int) (11.0*A) % (int) B));
}

//==============================================================================

void Test::plusOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1+pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0+5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             A+B);
}

//==============================================================================

void Test::minusOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1-pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             3.0-5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             A-B);
}

//==============================================================================

void Test::notOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb)\n"
                                         "{\n"
                                         "    return !pNb;\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             0.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0),
             1.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             0.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A),
             1.0);
}

//==============================================================================

void Test::orOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 || pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, !5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, !B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A, !B),
             0.0);
}

//==============================================================================

void Test::xorOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return (pNb1 != 0) ^ (pNb2 != 0);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, !5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, !B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A, !B),
             0.0);
}

//==============================================================================

void Test::andOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 && pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, !B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(!A, !B),
             0.0);
}

//==============================================================================

void Test::equalEqualOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 == pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A+2.9, B),
             1.0);
}

//==============================================================================

void Test::notEqualOperatorTests()
{
    QVERIFY(mCompilerEngine->compileCode("double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pNb1 != pNb2;\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A+2.9, B),
             0.0);
}

//==============================================================================

void Test::lowerThanOperatorTests()
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
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A+2.9, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(B, A),
             0.0);
}

//==============================================================================

void Test::greaterThanOperatorTests()
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
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A+2.9, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(B, A),
             1.0);
}

//==============================================================================

void Test::lowerOrEqualThanOperatorTests()
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
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A+2.9, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(B, A),
             0.0);
}

//==============================================================================

void Test::greaterOrEqualThanOperatorTests()
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
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A+2.9, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(B, A),
             1.0);
}

//==============================================================================

void Test::absFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double fabs(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return fabs(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             3.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(-3.0),
             3.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(MinusA),
             5.0);
}

//==============================================================================

void Test::expFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double exp(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return exp(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             exp(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             exp(A));
}

//==============================================================================

void Test::logFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double log(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return log(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             log(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             log(A));
}

//==============================================================================

void Test::ceilFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double ceil(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return ceil(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7),
             6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(B),
             8.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(-5.7),
             -5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(MinusB),
             -7.0);
}

//==============================================================================

void Test::floorFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double floor(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return floor(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(5.7),
             5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(B),
             7.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(-5.7),
             -6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(MinusB),
             -8.0);
}

//==============================================================================

void Test::factorialFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double factorial(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return factorial(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             120.0);
}

//==============================================================================

void Test::sinFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double sin(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return sin(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             sin(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             sin(A));
}

//==============================================================================

void Test::cosFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double cos(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return cos(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             cos(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             cos(A));
}

//==============================================================================

void Test::tanFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double tan(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return tan(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             tan(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             tan(A));
}

//==============================================================================

void Test::sinhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double sinh(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return sinh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             sinh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             sinh(A));
}

//==============================================================================

void Test::coshFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double cosh(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return cosh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             cosh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             cosh(A));
}

//==============================================================================

void Test::tanhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double tanh(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return tanh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             tanh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             tanh(A));
}

//==============================================================================

void Test::asinFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double asin(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return asin(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             asin(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/A),
             asin(1.0/A));
}

//==============================================================================

void Test::acosFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double acos(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return acos(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             acos(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/A),
             acos(1.0/A));
}

//==============================================================================

void Test::atanFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double atan(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return atan(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             atan(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             atan(A));
}

//==============================================================================

void Test::asinhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double asinh(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return asinh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             asinh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             asinh(A));
}

//==============================================================================

void Test::acoshFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double acosh(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return acosh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0),
             acosh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(A),
             acosh(A));
}

//==============================================================================

void Test::atanhFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double atanh(double);\n"
                                         "\n"
                                         "double function(double pNb)\n"
                                         "{\n"
                                         "    return atanh(pNb);\n"
                                         "}"));

    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/3.0),
             atanh(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mCompilerEngine->getFunction("function"))(1.0/A),
             atanh(1.0/A));
}

//==============================================================================

void Test::arbitraryLogFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double arbitrary_log(double, double);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return arbitrary_log(pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.0),
             log(3.0)/log(5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             log(A)/log(B));
}

//==============================================================================

void Test::powFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double pow(double, double);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return pow(pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0, 5.0),
             pow(3.0, 5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(A, B),
             pow(A, B));
}

//==============================================================================

void Test::gcdFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double gcd_multi(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return gcd_multi(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             273.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB),
             273.0);

    QVERIFY(mCompilerEngine->compileCode("extern double gcd_multi(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return gcd_multi(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             7.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB, BigC),
             7.0);
}

//==============================================================================

void Test::lcmFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double lcm_multi(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return lcm_multi(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             510510.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB),
             510510.0);

    QVERIFY(mCompilerEngine->compileCode("extern double lcm_multi(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return lcm_multi(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             510510.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB, BigC),
             510510.0);
}

//==============================================================================

void Test::maxFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double multi_max(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return multi_max(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             15015.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB),
             15015.0);

    QVERIFY(mCompilerEngine->compileCode("extern double multi_max(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return multi_max(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             15015.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB, BigC),
             15015.0);
}

//==============================================================================

void Test::minFunctionTests()
{
    QVERIFY(mCompilerEngine->compileCode("extern double multi_min(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2)\n"
                                         "{\n"
                                         "    return multi_min(2, pNb1, pNb2);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             9282.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB),
             9282.0);

    QVERIFY(mCompilerEngine->compileCode("extern double multi_min(int, ...);\n"
                                         "\n"
                                         "double function(double pNb1, double pNb2, double pNb3)\n"
                                         "{\n"
                                         "    return multi_min(3, pNb1, pNb2, pNb3);\n"
                                         "}"));

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             595.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mCompilerEngine->getFunction("function"))(BigA, BigB, BigC),
             595.0);
}

//==============================================================================

void Test::defIntFunctionTests()
{
    QVERIFY(!mCompilerEngine->compileCode("double function()\n"
                                          "{\n"
                                          "    return defint();\n"
                                          "}"));
}

//==============================================================================

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
