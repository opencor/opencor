//==============================================================================
// Computer test
//==============================================================================

#include "computerengine.h"
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
    OpenCOR::Computer::ComputerErrors parserErrors;

    // Check what happens when using an empty string to add a function

    mFunction = mComputerEngine->addFunction("");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("either 'void' or 'double' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add 'void' to our string

    mFunction = mComputerEngine->addFunction("void");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("an identifier was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add an identifier to our string

    mFunction = mComputerEngine->addFunction("void voidFunc");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'(' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '(' to our string

    mFunction = mComputerEngine->addFunction("void voidFunc(");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'double' or ')' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a ')' to our string

    mFunction = mComputerEngine->addFunction("void voidFunc()");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'{' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '{' to our string

    mFunction = mComputerEngine->addFunction("void voidFunc() {");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'}' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '}' to our string which should make it a valid void function

    mFunction = mComputerEngine->addFunction("void voidFunc() {}");

    QVERIFY(mFunction);

    // Make the function a double function

    mFunction = mComputerEngine->addFunction("double doubleFunc() {}");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("an identifier or 'return' was expected, but '}' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add 'return' to our string

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("the RHS of an equation was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add '3' (as the RHS of an equation) to our string

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("';' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add ';' to our string

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3;");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'}' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '}' to our string which should make it a valid double function

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3; }");

    QVERIFY(mFunction);

    // Check that we cannot redefine a function with the same function name

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3; }");

    QVERIFY(!mFunction);
    QVERIFY2(!mComputerEngine->error().message().compare("there is already a function called 'doubleFunc'"),
             qPrintable(QString("mComputerEngine->error().message() = %1").arg(mComputerEngine->error().message())));

    // Check that the function cannot work as an integer function

    mFunction = mComputerEngine->addFunction("int intFunc() { return 3; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("either 'void' or 'double' was expected, but 'int' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Check what happens when using an invalid function name

    mFunction = mComputerEngine->addFunction("double .doubleFunc() { return 3; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("an identifier was expected, but '.' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Check what happens when using an invalid RHS of an equation

    mFunction = mComputerEngine->addFunction("double doubleFunc() { return 3*/a; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!mFunction);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("the RHS of an equation was expected, but '/' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));
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

    QVERIFY2(arrayA[0] == 123,
             qPrintable(QString("arrayA[0] = %1").arg(arrayA[0])));
    QVERIFY2(arrayA[1] == 456,
             qPrintable(QString("arrayA[1] = %1").arg(arrayA[1])));
    QVERIFY2(arrayA[2] == 789,
             qPrintable(QString("arrayA[2] = %1").arg(arrayA[2])));

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

    QVERIFY2(arrayB[0] == 789,
             qPrintable(QString("arrayB[0] = %1").arg(arrayB[0])));
    QVERIFY2(arrayB[1] == 456,
             qPrintable(QString("arrayB[1] = %1").arg(arrayB[1])));
    QVERIFY2(arrayB[2] == 123,
             qPrintable(QString("arrayB[2] = %1").arg(arrayB[2])));
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

    QVERIFY2(mResult == 3*5.7, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == A*B, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 3/5.7, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == A/B, qPrintable(QString("res = %1 != %2").arg(mResult)));
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

    QVERIFY2(mResult == fmod(11*3, 5), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(11*A, B);

    QVERIFY2(mResult == fmod(11*A, B), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 3+5.7, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == A+B, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 3-5.7, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == A-B, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, !B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, !B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, !B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, !B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, !B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!A, !B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3+2.7, 5.7);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(5.7, 3);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A, B);

    QVERIFY2(mResult == 0, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A+2.9, B);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B, A);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 3, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(-3);

    QVERIFY2(mResult == 3, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == 5, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(MinusA);

    QVERIFY2(mResult == 5, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == exp(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == exp(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == log(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == log(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 6, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B);

    QVERIFY2(mResult == 8, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(-5.7);

    QVERIFY2(mResult == -5, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(MinusB);

    QVERIFY2(mResult == -7, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 5, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(B);

    QVERIFY2(mResult == 7, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(-5.7);

    QVERIFY2(mResult == -6, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(MinusB);

    QVERIFY2(mResult == -8, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == 6, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == 120, qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == sin(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == sin(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == cos(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == cos(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == tan(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == tan(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == sinh(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == sinh(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == cosh(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == cosh(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == tanh(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == tanh(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == asin(1/3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/A);

    QVERIFY2(mResult == asin(1/A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == acos(1/3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/A);

    QVERIFY2(mResult == acos(1/A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == atan(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == atan(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == asinh(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == asinh(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == acosh(3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(A);

    QVERIFY2(mResult == acosh(A), qPrintable(QString("res = %1").arg(mResult)));
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

    QVERIFY2(mResult == atanh(1/3), qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(1/A);

    QVERIFY2(mResult == atanh(1/A), qPrintable(QString("res = %1").arg(mResult)));
}

//==============================================================================

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
