//==============================================================================
// CellML support test
//==============================================================================

#include "computerengine.h"
#include "test.h"

//==============================================================================

#include "../../../../test/testutils.h"

//==============================================================================

void Test::initTestCase()
{
    // Load the CellMLModelSupport plugin

    OpenCOR::loadPlugin("CellMLSupport");

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

QTEST_MAIN(Test)

//==============================================================================
// End of file
//==============================================================================
