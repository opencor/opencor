/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Pretty CellML view tests
//==============================================================================

#include "prettycellmlviewcellmltoprettycellmlconverter.h"
#include "tests.h"

//==============================================================================

#include "../../../../tests/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::cellml10Tests()
{
    // Test the conversion of a CellML 1.0 model

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter("../src/plugins/editing/PrettyCellMLView/tests/data/cellml_1_0.cellml");

    QVERIFY(converter.execute());

    QCOMPARE(converter.output(),
             OpenCOR::fileContents("../src/plugins/editing/PrettyCellMLView/tests/data/cellml_1_0.out"));
}

//==============================================================================

void Tests::cellml11Tests()
{
    // Test the conversion of a CellML 1.1 model

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter("../src/plugins/editing/PrettyCellMLView/tests/data/cellml_1_1.cellml");

    QVERIFY(converter.execute());

    QCOMPARE(converter.output(),
             OpenCOR::fileContents("../src/plugins/editing/PrettyCellMLView/tests/data/cellml_1_1.out"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
