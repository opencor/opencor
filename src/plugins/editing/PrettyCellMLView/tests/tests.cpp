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

void Tests::cellmlCorTests()
{
    // Test the conversion of a CellML file that works with COR

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_cor.cellml"));

    QVERIFY(converter.execute());

    QCOMPARE(converter.output().split("\n"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_cor.out")));
}

//==============================================================================

void Tests::cellmlOpencorTests()
{
    // Test the conversion of a CellML file that only works with OpenCOR

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_opencor.cellml"));

    QVERIFY(converter.execute());

    QCOMPARE(converter.output().split("\n"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_opencor.out")));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
