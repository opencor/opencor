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

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, 5);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, 5);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(3, !5);

    QVERIFY2(mResult == 1, qPrintable(QString("res = %1").arg(mResult)));

    mResult = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(mFunction))(!3, !5);

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

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
