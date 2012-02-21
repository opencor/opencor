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
    llvm::Function * function;
    OpenCOR::Computer::ComputerErrors parserErrors;

    // Check what happens when using an empty string to add a function

    function = mComputerEngine->addFunction("");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("either 'void' or 'double' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add 'void' to our string

    function = mComputerEngine->addFunction("void");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("an identifier was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add an identifier to our string

    function = mComputerEngine->addFunction("void voidFunction");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'(' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '(' to our string

    function = mComputerEngine->addFunction("void voidFunction(");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'double' or ')' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a ')' to our string

    function = mComputerEngine->addFunction("void voidFunction()");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'{' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '{' to our string

    function = mComputerEngine->addFunction("void voidFunction() {");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'}' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '}' to our string which should make it a valid void function

    function = mComputerEngine->addFunction("void voidFunction() {}");

    QVERIFY(function);

    // Make the function a double function

    function = mComputerEngine->addFunction("double doubleFunction() {}");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("an identifier or 'return' was expected, but '}' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add 'return' to our string

    function = mComputerEngine->addFunction("double doubleFunction() { return");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("the RHS of an equation was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add '3' (as the RHS of an equation) to our string

    function = mComputerEngine->addFunction("double doubleFunction() { return 3");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("';' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add ';' to our string

    function = mComputerEngine->addFunction("double doubleFunction() { return 3;");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("'}' was expected, but 'EoF' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Add a '}' to our string which should make it a valid double function

    function = mComputerEngine->addFunction("double doubleFunction() { return 3; }");

    QVERIFY(function);

    // Check that we cannot redefine a function with the same function name

    function = mComputerEngine->addFunction("double doubleFunction() { return 3; }");

    QVERIFY(!function);
    QVERIFY2(!mComputerEngine->error().message().compare("there is already a function called 'doubleFunction'"),
             qPrintable(QString("mComputerEngine->error().message() = %1").arg(mComputerEngine->error().message())));

    // Check that the function cannot work as an integer function

    function = mComputerEngine->addFunction("int intFunction() { return 3; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("either 'void' or 'double' was expected, but 'int' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Check what happens when using an invalid function name

    function = mComputerEngine->addFunction("double .doubleFunction() { return 3; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("an identifier was expected, but '.' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));

    // Check what happens when using an invalid RHS of an equation

    function = mComputerEngine->addFunction("double doubleFunction() { return 3*/a; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY2(parserErrors.count() == 1,
             qPrintable(QString("parserErrors.count() = %1").arg(parserErrors.count())));
    QVERIFY2(!parserErrors.at(0).message().compare("the RHS of an equation was expected, but '/' was found instead"),
             qPrintable(QString("parserErrors.at(0).message() = %1").arg(parserErrors.at(0).message())));
}

//==============================================================================

void Test::voidFunctionTests()
{
    llvm::Function * function;
    double arrayA[3];
    double arrayB[3];

    // Initialise arrayA

    function = mComputerEngine->addFunction(
                   "void initA(double *pArrayA)\n"
                   "{\n"
                   "    pArrayA[0] = 123;\n"
                   "    pArrayA[1] = 456;\n"
                   "    pArrayA[2] = 789;\n"
                   "}"
               );

    QVERIFY(function);

    ((void (*)(double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(arrayA);

    QVERIFY2(arrayA[0] == 123,
             qPrintable(QString("arrayA[0] = %1").arg(arrayA[0])));
    QVERIFY2(arrayA[1] == 456,
             qPrintable(QString("arrayA[1] = %1").arg(arrayA[1])));
    QVERIFY2(arrayA[2] == 789,
             qPrintable(QString("arrayA[2] = %1").arg(arrayA[2])));

    // Initialise arrayB in reverse order of arrayA

    function = mComputerEngine->addFunction(
                   "void initB(double *pArrayA, double *pArrayB)\n"
                   "{\n"
                   "    pArrayB[0] = pArrayA[2];\n"
                   "    pArrayB[1] = pArrayA[1];\n"
                   "    pArrayB[2] = pArrayA[0];\n"
                   "}"
               );

    QVERIFY(function);

    ((void (*)(double *, double *))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(arrayA, arrayB);

    QVERIFY2(arrayB[0] == 789,
             qPrintable(QString("arrayB[0] = %1").arg(arrayB[0])));
    QVERIFY2(arrayB[1] == 456,
             qPrintable(QString("arrayB[1] = %1").arg(arrayB[1])));
    QVERIFY2(arrayB[2] == 123,
             qPrintable(QString("arrayB[2] = %1").arg(arrayB[2])));
}

//==============================================================================

void Test::mathematicalOperatorTests()
{
    llvm::Function * function;
    static const double A = 5;
    static const double B = 7.9;
    double res;

    // Compute and return the sum of two numbers

    function = mComputerEngine->addFunction(
                   "double add(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1+pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3, 5.7);

    QVERIFY2(res == 3+5.7, qPrintable(QString("res = %1").arg(res)));

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B);

    QVERIFY2(res == A+B, qPrintable(QString("res = %1").arg(res)));

    // Compute and return the subtraction of two numbers

    function = mComputerEngine->addFunction(
                   "double sub(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1-pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3, 5.7);

    QVERIFY2(res == 3-5.7, qPrintable(QString("res = %1").arg(res)));

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B);

    QVERIFY2(res == A-B, qPrintable(QString("res = %1").arg(res)));

    // Compute and return the product of two numbers

    function = mComputerEngine->addFunction(
                   "double mult(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1*pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3, 5.7);

    QVERIFY2(res == 3*5.7, qPrintable(QString("res = %1").arg(res)));

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B);

    QVERIFY2(res == A*B, qPrintable(QString("res = %1").arg(res)));

    // Compute and return the division of two numbers

    function = mComputerEngine->addFunction(
                   "double div(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1/pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(3, 5.7);

    QVERIFY2(res == 3/5.7, qPrintable(QString("res = %1").arg(res)));

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(A, B);

    QVERIFY2(res == A/B, qPrintable(QString("res = %1 != %2").arg(res)));

    // Compute and return the modulo of two numbers

    function = mComputerEngine->addFunction(
                   "double mod(double pNb1, double pNb2)\n"
                   "{\n"
                   "    return pNb1%pNb2;\n"
                   "}"
               );

    QVERIFY(function);

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(11*3, 5);

    QVERIFY2(res == fmod(11*3, 5), qPrintable(QString("res = %1").arg(res)));

    res = ((double (*)(double, double))(intptr_t) mComputerEngine->executionEngine()->getPointerToFunction(function))(11*A, B);

    QVERIFY2(res == fmod(11*A, B), qPrintable(QString("res = %1").arg(res)));
}

//==============================================================================

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
