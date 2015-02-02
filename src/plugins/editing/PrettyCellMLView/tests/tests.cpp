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

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_cor.cellml")));

    QCOMPARE(converter.output().split("\n"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_cor.out")));
}

//==============================================================================

void Tests::cellmlOpencorTests()
{
    // Test the conversion of a CellML file that only works with OpenCOR

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_opencor.cellml")));

    QCOMPARE(converter.output().split("\n"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_opencor.out")));
}

//==============================================================================

void Tests::failingTests()
{
    // Test the conversion of various invalid CellML files

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_base_units.cellml")));
    QCOMPARE(converter.errorLine(), 3);
    QCOMPARE(converter.errorMessage(),
             QString("A 'base_units' attribute must have a value of 'yes' or 'no'."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_apply.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("An 'apply' element must have at least one child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_eq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'eq' operator must have two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_neq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'neq' operator must have two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_gt.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'gt' operator must have two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_lt.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'lt' operator must have two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_geq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'geq' operator must have two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_leq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'leq' operator must have two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_plus.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'plus' operator must have at least one operand."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_minus.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'minus' operator must have at least one operand."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_times.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'times' operator must have at least two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_divide.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'divide' operator must have at least two operands."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_power.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'power' function must have two arguments."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_root.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'root' function must have either one or two arguments."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_abs.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'abs' function must have one argument."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_exp.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'exp' function must have one argument."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_ln.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'ln' function must have one argument."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_log.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'log' function must have either one or two arguments."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cn' element must have a value."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_1.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cn' element with an 'e-notation' type must have three child elements."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_2.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The first child element of a 'cn' element with an 'e-notation' type must be of 'text' type."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_3.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The second child element of a 'cn' element with an 'e-notation' type must be of 'element' type."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_4.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The name of the second child element of a 'cn' element with an 'e-notation' type must be 'sep'."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_5.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The third child element of a 'cn' element with an 'e-notation' type must be of 'text' type."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_unsupported_type.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The 'cn' element uses a 'complex-polar' type, which is unsupported."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_cn_e_notation_unknown_type.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The 'cn' element uses a 'some-unknown-type' type, which is unknown."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/mathml_ci.cellml")));
    QCOMPARE(converter.errorLine(), 7);
    QCOMPARE(converter.errorMessage(),
             QString("A 'ci' element must have a value."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_relationship.cellml")));
    QCOMPARE(converter.errorLine(), 4);
    QCOMPARE(converter.errorMessage(),
             QString("A 'relationship' attribute in the CellML namespace must have a value of 'encapsulation' or 'containment'."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_relationship_ref.cellml")));
    QCOMPARE(converter.errorLine(), 4);
    QCOMPARE(converter.errorMessage(),
             QString("A 'relationship_ref' element with a 'relationship' attribute value of 'encapsulation' must not define a 'name' attribute."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_connection.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'connection' element must contain exactly one 'map_components' element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_unsupported_element.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'unsupported_element' element was found in the original CellML file, but it is not supported and cannot therefore be processed."));
}

//==============================================================================

void Tests::warningTests()
{
    // Test the conversion of various CellML files that generate warnings

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/cellml_unknown_element.cellml")));
    QCOMPARE(converter.warningLines().first(), 3);
    QCOMPARE(converter.warningMessages().first(),
             QString("A 'unknown_element' element was found%2, but it is not known and cannot therefore be processed."));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
