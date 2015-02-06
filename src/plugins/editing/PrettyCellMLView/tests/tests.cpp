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

void Tests::successfulTests()
{
    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    // Test the conversion of a CellML file that works with COR

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/successful/cellml_cor.cellml")));

    QCOMPARELIST(converter.output().split("\n"),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/successful/cellml_cor.out")));

    // Test the conversion of a CellML file that only works with OpenCOR

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/successful/cellml_opencor.cellml")));

    QCOMPARELIST(converter.output().split("\n"),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/successful/cellml_opencor.out")));
}

//==============================================================================

void Tests::failingTests()
{
    // Test the conversion of various invalid CellML files

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    // CellML base units

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/cellml_base_units.cellml")));
    QCOMPARE(converter.errorLine(), 3);
    QCOMPARE(converter.errorMessage(),
             QString("A 'base_units' attribute must have a value of 'yes' or 'no'."));

    // CellML reaction

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/cellml_reaction.cellml")));
    QCOMPARE(converter.errorLine(), 4);
    QCOMPARE(converter.errorMessage(),
             QString("A 'reaction' element was found in the original CellML file, but it is not supported and cannot therefore be processed."));

    // CellML group

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/cellml_relationship.cellml")));
    QCOMPARE(converter.errorLine(), 4);
    QCOMPARE(converter.errorMessage(),
             QString("A 'relationship' attribute in the CellML namespace must have a value of 'encapsulation' or 'containment'."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/cellml_relationship_ref.cellml")));
    QCOMPARE(converter.errorLine(), 4);
    QCOMPARE(converter.errorMessage(),
             QString("A 'relationship_ref' element with a 'relationship' attribute value of 'encapsulation' must not define a 'name' attribute."));

    // CellML connection

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/cellml_connection.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'connection' element must contain exactly one 'map_components' element."));

    // MathML token elements

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cn' element must have a value."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_1.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cn' element with an 'e-notation' type must have three child elements."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_2.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The first child element of a 'cn' element with an 'e-notation' type must be of 'text' type."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_3.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The second child element of a 'cn' element with an 'e-notation' type must be of 'element' type."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_4.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The name of the second child element of a 'cn' element with an 'e-notation' type must be 'sep'."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_5.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The third child element of a 'cn' element with an 'e-notation' type must be of 'text' type."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_unsupported_type.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The 'cn' element uses a 'complex-polar' type that is unsupported."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cn_e_notation_unknown_type.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("The 'cn' element uses a 'some-unknown-type' type that is unknown."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_ci.cellml")));
    QCOMPARE(converter.errorLine(), 7);
    QCOMPARE(converter.errorMessage(),
             QString("A 'ci' element must have a value."));

    // MathML basic content elements

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_apply.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("An 'apply' element must have at least one child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_piecewise_1.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'piecewise' element can only be used within a top-level 'apply' element that has an 'eq' element as its first child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_piecewise_2.cellml")));
    QCOMPARE(converter.errorLine(), 10);
    QCOMPARE(converter.errorMessage(),
             QString("A 'piecewise' element cannot be used within another 'piecewise' element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_piecewise_3.cellml")));
    QCOMPARE(converter.errorLine(), 8);
    QCOMPARE(converter.errorMessage(),
             QString("A 'piecewise' element must have at least one child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_piece.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'piece' element must have two child elements."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_otherwise.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("An 'otherwise' element must have one child element."));

    // MathML relational operators

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_eq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'eq' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_neq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'neq' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_gt.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'gt' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_lt.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'lt' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_geq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'geq' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_leq.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'leq' element must have two siblings."));

    // MathML arithmetic operators

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_plus.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'plus' element must have at least one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_minus.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'minus' element must have at least one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_times.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'times' element must have at least two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_divide.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'divide' element must have at least two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_power.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'power' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_root_1.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'root' element must have either one or two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_root_2.cellml")));
    QCOMPARE(converter.errorLine(), 7);
    QCOMPARE(converter.errorMessage(),
             QString("The first sibling of a 'root' element with two siblings must be a 'degree' element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_abs.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'abs' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_exp.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'exp' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_ln.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'ln' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_log.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'log' element must have either one or two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_ceiling.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'ceiling' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_floor.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'floor' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_factorial.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'factorial' element must have one sibling."));

    // MathML logical operators

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_and.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'and' element must have at least two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_or.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'or' element must have at least two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_xor.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'xor' element must have at least two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_not.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'not' element must have one sibling."));

    // MathML calculus elements

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_diff_1.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'diff' element must have two siblings."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_diff_2.cellml")));
    QCOMPARE(converter.errorLine(), 7);
    QCOMPARE(converter.errorMessage(),
             QString("The first sibling of a 'diff' element with two siblings must be a 'bvar' element."));

    // MathML qualifier elements

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_degree.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'degree' element must have one child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_logbase.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'logbase' element must have one child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_bvar_1.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'bvar' element must have one or two child elements."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_bvar_2.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("The first child element of a 'bvar' element with two child elements must be a 'degree' element."));

    // MathML trigonometric operators

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_sin.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'sin' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cos.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cos' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_tan.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'tan' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_sec.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'sec' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_csc.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'csc' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cot.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cot' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_sinh.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'sinh' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_cosh.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'cosh' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_tanh.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'tanh' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_sech.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'sech' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_csch.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'csch' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_coth.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("A 'coth' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arcsin.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arcsin' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arccos.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arccos' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arctan.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arctan' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arcsec.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arcsec' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arccsc.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arccsc' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arccot.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arccot' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arcsinh.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arcsinh' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arccosh.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arccosh' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arctanh.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arctanh' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arcsech.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arcsech' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arccsch.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arccsch' element must have one sibling."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_arccoth.cellml")));
    QCOMPARE(converter.errorLine(), 6);
    QCOMPARE(converter.errorMessage(),
             QString("An 'arccoth' element must have one sibling."));

    // MathML constants

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_constant_1.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'pi' element cannot have a child element."));

    QVERIFY(!converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/failing/mathml_constant_2.cellml")));
    QCOMPARE(converter.errorLine(), 5);
    QCOMPARE(converter.errorMessage(),
             QString("A 'pi' element cannot have child elements."));
}

//==============================================================================

void Tests::warningTests()
{
    // Unknown CellML element

    OpenCOR::PrettyCellMLView::PrettyCellMLViewCellmlToPrettyCellmlConverter converter;

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/warning/cellml_unknown_element.cellml")));
    QCOMPARE(converter.warningLines().first(), 3);
    QCOMPARE(converter.warningMessages().first(),
             QString("A 'unknown_element' element was found%2, but it is not known and cannot therefore be processed."));

    // Unknown MathML element

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/warning/mathml_unknown_element.cellml")));
    QCOMPARE(converter.warningLines().first(), 5);
    QCOMPARE(converter.warningMessages().first(),
             QString("A 'unknown_element' element was found%2, but it is not known and cannot therefore be processed."));

    // Known, but unsupported MathML elements

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/warning/mathml_semantics.cellml")));
    QCOMPARE(converter.warningLines().first(), 5);
    QCOMPARE(converter.warningMessages().first(),
             QString("A 'semantics' element was found in the original CellML file, but it is not supported and cannot therefore be processed."));

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/warning/mathml_annotation.cellml")));
    QCOMPARE(converter.warningLines().first(), 5);
    QCOMPARE(converter.warningMessages().first(),
             QString("An 'annotation' element was found in the original CellML file, but it is not supported and cannot therefore be processed."));

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/PrettyCellMLView/tests/data/warning/mathml_annotation-xml.cellml")));
    QCOMPARE(converter.warningLines().first(), 5);
    QCOMPARE(converter.warningMessages().first(),
             QString("An 'annotation-xml' element was found in the original CellML file, but it is not supported and cannot therefore be processed."));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
