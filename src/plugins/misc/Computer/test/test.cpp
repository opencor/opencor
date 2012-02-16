//==============================================================================
// Computer test
//==============================================================================

#include "computerengine.h"
#include "test.h"

//==============================================================================

#include "../../../../test/testutils.h"

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
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("either 'void' or 'double' was expected, but 'EoF' was found instead"));

    // Add 'void' to our string

    function = mComputerEngine->addFunction("void");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("an identifier was expected, but 'EoF' was found instead"));

    // Add an identifier to our string

    function = mComputerEngine->addFunction("void voidFunction");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("'(' was expected, but 'EoF' was found instead"));

    // Add a '(' to our string

    function = mComputerEngine->addFunction("void voidFunction(");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("'double' or ')' was expected, but 'EoF' was found instead"));

    // Add a ')' to our string

    function = mComputerEngine->addFunction("void voidFunction()");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("'{' was expected, but 'EoF' was found instead"));

    // Add a '{' to our string

    function = mComputerEngine->addFunction("void voidFunction() {");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("'}' was expected, but 'EoF' was found instead"));

    // Add a '}' to our string which should make it a valid void function

    function = mComputerEngine->addFunction("void voidFunction() {}");

    QVERIFY(function);

    // Make the function a double function

    function = mComputerEngine->addFunction("double doubleFunction() {}");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("an identifier or 'return' was expected, but '}' was found instead"));

    // Add 'return' to our string

    function = mComputerEngine->addFunction("double doubleFunction() { return");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("the RHS of an equation was expected, but 'EoF' was found instead"));

    // Add '3' (as the RHS of an equation) to our string

    function = mComputerEngine->addFunction("double doubleFunction() { return 3");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("';' was expected, but 'EoF' was found instead"));

    // Add ';' to our string

    function = mComputerEngine->addFunction("double doubleFunction() { return 3;");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("'}' was expected, but 'EoF' was found instead"));

    // Add a '}' to our string which should make it a valid double function

    function = mComputerEngine->addFunction("double doubleFunction() { return 3; }");

    QVERIFY(function);

    // Check that the function cannot work as an integer function

    function = mComputerEngine->addFunction("int intFunction() { return 3; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("either 'void' or 'double' was expected, but 'int' was found instead"));

    // Check what happens when using an invalid function name

    function = mComputerEngine->addFunction("double .doubleFunction() { return 3; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("an identifier was expected, but '.' was found instead"));

    // Check what happens when using an invalid RHS of an equation

    function = mComputerEngine->addFunction("double doubleFunction() { return 3*/a; }");
    parserErrors = mComputerEngine->parserErrors();

    QVERIFY(!function);
    QVERIFY(parserErrors.count() == 1);
    QVERIFY(!parserErrors.at(0).message().compare("the RHS of an equation was expected, but '/' was found instead"));
}

//==============================================================================

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
