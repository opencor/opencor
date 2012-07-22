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
    // Load the Computer plugin

    OpenCOR::loadPlugin("Computer");

    // Create our computer engine

    mComputerEngine = new OpenCOR::Computer::ComputerEngine();
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

    QVERIFY(!mComputerEngine->addFunction("voidFunc",
                                          ""));

    // Add 'void' to our string

    QVERIFY(!mComputerEngine->addFunction("voidFunc",
                                          "void"));

    // Add an identifier to our string

    QVERIFY(!mComputerEngine->addFunction("voidFunc",
                                          "void voidFunc"));

    // Add a '(' to our string

    QVERIFY(!mComputerEngine->addFunction("voidFunc",
                                          "void voidFunc("));

    // Add a ')' to our string

    QVERIFY(!mComputerEngine->addFunction("voidFunc",
                                          "void voidFunc()"));

    // Add a '{' to our string

    QVERIFY(!mComputerEngine->addFunction("voidFunc",
                                          "void voidFunc() {"));

    // Add a '}' to our string which should make it a valid void function

    QVERIFY(mComputerEngine->addFunction("voidFunc",
                                         "void voidFunc() {}"));

    // Make the function a double function

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double doubleFunc() {}"));

    // Add 'return' to our string

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double doubleFunc() { return"));

    // Add '3' (as the RHS of an equation) to our string

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double doubleFunc() { return 3.0"));

    // Add ';' to our string

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double doubleFunc() { return 3.0;"));

    // Add a '}' to our string which should make it a valid double function

    QVERIFY(mComputerEngine->addFunction("doubleFunc",
                                         "double doubleFunc() { return 3.0; }"));

    // Check that we cannot redefine a function with the same function name

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double doubleFunc() { return 3.0; }"));

    // Check what happens when using an invalid function name

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double .doubleFunc() { return 3.0; }"));

    // Check what happens when using an invalid RHS of an equation

    QVERIFY(!mComputerEngine->addFunction("doubleFunc",
                                          "double doubleFunc() { return 3.0*/a; }"));
}

//==============================================================================

void Test::voidFunctionTests()
{
    llvm::Function *function;

    double arrayA[3];
    double arrayB[3];

    // Initialise arrayA

    function = mComputerEngine->addFunction("aInitFunc",
                   "void aInitFunc(double *pArrayA)\n"
                   "{\n"
                   "    pArrayA[0] = 123.0;\n"
                   "    pArrayA[1] = 456.0;\n"
                   "    pArrayA[2] = 789.0;\n"
                   "}"
               );

    QVERIFY(function);

    ((void (*)(double *)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(arrayA);

    QCOMPARE(arrayA[0], 123.0);
    QCOMPARE(arrayA[1], 456.0);
    QCOMPARE(arrayA[2], 789.0);

    // Initialise arrayB in reverse order of arrayA

    function = mComputerEngine->addFunction("bInitFunc",
                   "void bInitFunc(double *pArrayA, double *pArrayB)\n"
                   "{\n"
                   "    pArrayB[0] = pArrayA[2];\n"
                   "    pArrayB[1] = pArrayA[1];\n"
                   "    pArrayB[2] = pArrayA[0];\n"
                   "}"
               );

    QVERIFY(function);

    ((void (*)(double *, double *)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(arrayA, arrayB);

    QCOMPARE(arrayB[0], 789.0);
    QCOMPARE(arrayB[1], 456.0);
    QCOMPARE(arrayB[2], 123.0);
}

//==============================================================================

void Test::timesOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("multFunc",
                   "double multFunc(double pNb1, double pNb2)\n"
                   "{\n"
                    "    return pNb1*pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             3.0*5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             A*B);
}

//==============================================================================

void Test::divideOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("divFunc",
                   "double divFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1/pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             3.0/5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             A/B);
}

//==============================================================================

void Test::moduloOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("modFunc",
                   "double modFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return (double) ((int) pNb1 % (int) pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(11.0*3.0, 5.0),
             (double) ((int) (11.0*3.0) % (int) 5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(11.0*A, B),
             (double) ((int) (11.0*A) % (int) B));
}

//==============================================================================

void Test::plusOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("addFunc",
                   "double addFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1+pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             3.0+5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             A+B);
}

//==============================================================================

void Test::minusOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("subFunc",
                   "double subFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1-pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             3.0-5.7);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             A-B);
}

//==============================================================================

void Test::notOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("notFunc",
                   "double notFunc(double pNb)\n"
                   "{\n"
                   "    return !pNb;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             0.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0),
             1.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             0.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A),
             1.0);
}

//==============================================================================

void Test::orOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("orFunc",
                   "double orFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 || pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, !5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, !B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A, !B),
             0.0);
}

//==============================================================================

void Test::xorOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("xorFunc",
                   "double xorFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return (pNb1 != 0) ^ (pNb2 != 0);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, !5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, !B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A, !B),
             0.0);
}

//==============================================================================

void Test::andOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("andFunc",
                   "double andFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 && pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!3.0, !5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, !B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(!A, !B),
             0.0);
}

//==============================================================================

void Test::equalEqualOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("equalEqualFunc",
                   "double equalEqualFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 == pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A+2.9, B),
             1.0);
}

//==============================================================================

void Test::notEqualOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("notEqualFunc",
                   "double notEqualFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 != pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A+2.9, B),
             0.0);
}

//==============================================================================

void Test::lowerThanOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("lowerThanFunc",
                   "double lowerThanFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 < pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(5.7, 3.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A+2.9, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(B, A),
             0.0);
}

//==============================================================================

void Test::greaterThanOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("greaterThanFunc",
                   "double greaterThanFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 > pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0+2.7, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(5.7, 3.0),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A+2.9, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(B, A),
             1.0);
}

//==============================================================================

void Test::lowerOrEqualThanOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("lowerOrEqualThanFunc",
                   "double lowerOrEqualThanFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1 <= pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(5.7, 3.0),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A+2.9, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(B, A),
             0.0);
}

//==============================================================================

void Test::greaterOrEqualThanOperatorTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("greaterOrEqualThanFunc",
                   "double greaterOrEqualThanFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return (double) (pNb1 >= pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.7),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0+2.7, 5.7),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(5.7, 3.0),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             0.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A+2.9, B),
             1.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(B, A),
             1.0);
}

//==============================================================================

void Test::absFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("absFunc",
                   "double absFunc(double pNb)\n"
                   "{\n"
                   "    return fabs(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             3.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(-3.0),
             3.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(MinusA),
             5.0);
}

//==============================================================================

void Test::expFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("expFunc",
                   "double expFunc(double pNb)\n"
                   "{\n"
                   "    return exp(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             exp(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             exp(A));
}

//==============================================================================

void Test::logFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("logFunc",
                   "double logFunc(double pNb)\n"
                   "{\n"
                   "    return log(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             log(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             log(A));
}

//==============================================================================

void Test::ceilFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("ceilFunc",
                   "double ceilFunc(double pNb)\n"
                   "{\n"
                   "    return ceil(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(5.7),
             6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(B),
             8.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(-5.7),
             -5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(MinusB),
             -7.0);
}

//==============================================================================

void Test::floorFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("floorFunc",
                   "double floorFunc(double pNb)\n"
                   "{\n"
                   "    return floor(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(5.7),
             5.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(B),
             7.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(-5.7),
             -6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(MinusB),
             -8.0);
}

//==============================================================================

void Test::factorialFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("factorialFunc",
                   "double factorialFunc(double pNb)\n"
                   "{\n"
                   "    return factorial(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             6.0);
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             120.0);
}

//==============================================================================

void Test::sinFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("sinFunc",
                   "double sinFunc(double pNb)\n"
                   "{\n"
                   "    return sin(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             sin(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             sin(A));
}

//==============================================================================

void Test::cosFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("cosFunc",
                   "double cosFunc(double pNb)\n"
                   "{\n"
                   "    return cos(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             cos(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             cos(A));
}

//==============================================================================

void Test::tanFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("tanFunc",
                   "double tanFunc(double pNb)\n"
                   "{\n"
                   "    return tan(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             tan(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             tan(A));
}

//==============================================================================

void Test::sinhFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("sinhFunc",
                   "double sinhFunc(double pNb)\n"
                   "{\n"
                   "    return sinh(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             sinh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             sinh(A));
}

//==============================================================================

void Test::coshFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("coshFunc",
                   "double coshFunc(double pNb)\n"
                   "{\n"
                   "    return cosh(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             cosh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             cosh(A));
}

//==============================================================================

void Test::tanhFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("tanhFunc",
                   "double tanhFunc(double pNb)\n"
                   "{\n"
                   "    return tanh(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             tanh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             tanh(A));
}

//==============================================================================

void Test::asinFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("asinFunc",
                   "double asinFunc(double pNb)\n"
                   "{\n"
                   "    return asin(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(1.0/3.0),
             asin(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(1.0/A),
             asin(1.0/A));
}

//==============================================================================

void Test::acosFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("acosFunc",
                   "double acosFunc(double pNb)\n"
                   "{\n"
                   "    return acos(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(1.0/3.0),
             acos(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(1.0/A),
             acos(1.0/A));
}

//==============================================================================

void Test::atanFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("atanFunc",
                   "double atanFunc(double pNb)\n"
                   "{\n"
                   "    return atan(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             atan(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             atan(A));
}

//==============================================================================

void Test::asinhFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("asinhFunc",
                   "double asinhFunc(double pNb)\n"
                   "{\n"
                   "    return asinh(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             asinh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             asinh(A));
}

//==============================================================================

void Test::acoshFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("acoshFunc",
                   "double acoshFunc(double pNb)\n"
                   "{\n"
                   "    return acosh(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0),
             acosh(3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A),
             acosh(A));
}

//==============================================================================

void Test::atanhFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("atanhFunc",
                   "double atanhFunc(double pNb)\n"
                   "{\n"
                   "    return atanh(pNb);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(1.0/3.0),
             atanh(1.0/3.0));
    QCOMPARE(((double (*)(double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(1.0/A),
             atanh(1.0/A));
}

//==============================================================================

void Test::arbitraryLogFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("arbitraryLogFunc",
                   "double arbitraryLogFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return arbitrary_log(pNb1, pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.0),
             log(3.0)/log(5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             log(A)/log(B));
}

//==============================================================================

void Test::powFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("powFunc",
                   "double powFunc(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pow(pNb1, pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0, 5.0),
             pow(3.0, 5.0));
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B),
             pow(A, B));
}

//==============================================================================

void Test::gcdFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("gcdFunc2",
                   "double gcdFunc2(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return gcd_multi(2, pNb1, pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             273.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB),
             273.0);

    function = mComputerEngine->addFunction("gcdFunc3",
                   "double gcdFunc3(double pNb1, double pNb2, double pNb3)\n"
                   "{\n"
                   "    return gcd_multi(3, pNb1, pNb2, pNb3);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             7.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB, BigC),
             7.0);
}

//==============================================================================

void Test::lcmFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("lcmFunc2",
                   "double lcmFunc2(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return lcm_multi(2, pNb1, pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             510510.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB),
             510510.0);

    function = mComputerEngine->addFunction("lcmFunc3",
                   "double lcmFunc3(double pNb1, double pNb2, double pNb3)\n"
                   "{\n"
                   "    return lcm_multi(3, pNb1, pNb2, pNb3);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             510510.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB, BigC),
             510510.0);
}

//==============================================================================

void Test::maxFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("maxFunc2",
                   "double maxFunc2(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return multi_max(2, pNb1, pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             15015.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB),
             15015.0);

    function = mComputerEngine->addFunction("maxFunc3",
                   "double maxFunc3(double pNb1, double pNb2, double pNb3)\n"
                   "{\n"
                   "    return multi_max(3, pNb1, pNb2, pNb3);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             15015.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB, BigC),
             15015.0);
}

//==============================================================================

void Test::minFunctionTests()
{
    llvm::Function *function;

    function = mComputerEngine->addFunction("minFunc2",
                   "double minFunc2(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return multi_min(2, pNb1, pNb2);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0),
             9282.0);
    QCOMPARE(((double (*)(double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB),
             9282.0);

    function = mComputerEngine->addFunction("minFunc3",
                   "double minFunc3(double pNb1, double pNb2, double pNb3)\n"
                   "{\n"
                   "    return multi_min(3, pNb1, pNb2, pNb3);\n"
                   "}"
               );

    QVERIFY(function);

    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3.0*5.0*7.0*11.0*13.0, 2.0*3.0*7.0*13.0*17.0, 5.0*7.0*17.0),
             595.0);
    QCOMPARE(((double (*)(double, double, double)) (intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(BigA, BigB, BigC),
             595.0);
}

//==============================================================================

void Test::defIntFunctionTests()
{
    QVERIFY(!mComputerEngine->addFunction("defIntFunc",
                 "double defIntFunc()\n"
                 "{\n"
                 "    return defint();\n"
                 "}"
             ));
}

//==============================================================================

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
