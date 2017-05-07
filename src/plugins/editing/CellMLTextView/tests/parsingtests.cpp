/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML Text view parsing tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "cellmlfile.h"
#include "cellmltextviewconverter.h"
#include "cellmltextviewparser.h"
#include "corecliutils.h"
#include "parsingtests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void ParsingTests::basicTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal model definition

    QVERIFY(!parser.execute(QString(),
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'model' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'as' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a minimal model definition with a cmeta:id

    QVERIFY(!parser.execute("def model{",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A cmeta:id is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{123",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A cmeta:id is expected, but '123' was found instead."));

    QVERIFY(!parser.execute("def model{text",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{text123",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{text_with_underscores",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{text-with-hyphens",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{text.with.periods",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{text123_with_underscores.periods.and-hyphens",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{_.-__..--___...---",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A cmeta:id is expected, but '_.-__..--___...---' was found instead."));

    QVERIFY(!parser.execute("def model{my_cmeta_id",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model{my_cmeta_id}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model{my_cmeta_id} my_model as\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Test the definition of a something unknown

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unknown as"
                            "    enddef;"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'import', 'unit', 'comp', 'group' or 'map' is expected, but 'unknown' was found instead."));
}

//==============================================================================

void ParsingTests::fileTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Parsing of some CellML Text code and converting to raw CellML...

    QVERIFY(parser.execute(OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.in")).join('\n'),
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(QString(OpenCOR::Core::serialiseDomDocument(parser.domDocument())).split('\n'),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.cellml")));
    QVERIFY(!parser.domDocument().isNull());

    // ... and back

    OpenCOR::CellMLTextView::CellMLTextViewConverter converter;

    QVERIFY(converter.execute(OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.cellml")).join('\n')));
    QCOMPARE(converter.output().split('\n'),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.in")));

    // Parsing of some CellML Text code (with some in between comments) and
    // converting it to raw CellML
    // Note: we can't convert it back to CellML Text code since the in between
    //       comments won't, as expected, be in their original location. This is
    //       because those in between comments are within a definition itself
    //       while we would normally have them before and/or after...

    QVERIFY(parser.execute(OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model_with_in_between_comments.in")).join('\n'),
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(QString(OpenCOR::Core::serialiseDomDocument(parser.domDocument())).split('\n'),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model_with_in_between_comments.cellml")));
    QVERIFY(!parser.domDocument().isNull());
}

//==============================================================================

void ParsingTests::importTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal import definition

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'using' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A string representing a URL is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\"",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'for' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit', 'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def import using \"import_model.cellml\" for\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        unit my_imported_unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'using' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        unit my_imported_unit using",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        unit my_imported_unit using unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        unit my_imported_unit using unit my_reference_unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        unit my_imported_unit using unit my_reference_unit;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit', 'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def import using \"import_model.cellml\" for\n"
                           "        unit my_imported_unit using unit my_reference_unit;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        comp",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        comp my_imported_component",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'using' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        comp my_imported_component using",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        comp my_imported_component using comp",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        comp my_imported_component using comp my_reference_component",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def import using \"import_model.cellml\" for\n"
                            "        comp my_imported_component using comp my_reference_component;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit', 'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def import using \"import_model.cellml\" for\n"
                           "        comp my_imported_component using comp my_reference_component;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Test the parsing of an originally CellML 1.0 file that now requires
    // CellML 1.1

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def import{my_import_cmeta_id} using \"imported_model.cellml\" for\n"
                           "        unit{my_imported_unit_cmeta_id} my_imported_unit using unit my_reference_unit;\n"
                           "        comp{my_imported_component_cmeta_id} my_imported_component using comp my_reference_component;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.cellmlVersion(), OpenCOR::CellMLSupport::CellmlFile::Cellml_1_1);
    QVERIFY(!parser.domDocument().isNull());
}

//==============================================================================

void ParsingTests::unitsTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal unit definition

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_base_unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'as' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_base_unit as",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'base', 'unit' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_base_unit as base",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_base_unit as base unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_base_unit as base unit;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_base_unit as base unit;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def unit my_base_unit as base unit;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier or an SI unit (e.g. 'second') is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'{' or ';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def unit my_unit as\n"
                           "       unit some_unit;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'pref', 'expo', 'mult' or 'off' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A number or a prefix (e.g. 'milli') is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or '}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli};",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def unit my_unit as\n"
                           "       unit some_unit {pref: milli};"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'pref', 'expo', 'mult' or 'off' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli, expo",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli, expo:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A number is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli, expo: 3",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or '}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli, expo: 3}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit {pref: milli, expo: 3};",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def unit my_unit as\n"
                           "       unit some_unit {pref: milli, expo: 3};"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "       unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier or an SI unit (e.g. 'second') is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "       unit some_unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'{' or ';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "       unit some_unit;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "       unit some_unit;\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "       unit some_unit;\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "       unit some_unit;\n"
                            "       unit some_unit;\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def unit my_unit as\n"
                           "       unit some_unit;\n"
                           "       unit some_unit;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Test the redefinition of an SI unit as a base unit

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit second as base unit;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but 'second' was found instead."));

    // Test the definition of a unit with no unit elements

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def unit my_unit as\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Test the definition of a unit with an invalid unit

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "        unit 123;\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier or an SI unit (e.g. 'second') is expected, but '123' was found instead."));

    // Test the definition of a unit with an unknown unit attribute

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "        unit some_unit {unknown_attribute: 3};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'pref', 'expo', 'mult' or 'off' is expected, but 'unknown_attribute' was found instead."));

    // Test the definition of a unit with an invalid prefix value

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "        unit some_unit {pref: invalid_prefix_value};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A number or a prefix (e.g. 'milli') is expected, but 'invalid_prefix_value' was found instead."));

    // Test the definition of a unit with an invalid multiplier value

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "        unit some_unit {mult: invalid_multiplier_value};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A number is expected, but 'invalid_multiplier_value' was found instead."));

    // Test the definition of a unit with twice the same attribute specification

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def unit my_unit as\n"
                            "        unit some_unit {pref: 3, pref: 3};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("The 'pref' attribute has already been specified."));
}

//==============================================================================

void ParsingTests::componentTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal component definition

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'as' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a component definition with a simple units definition
    // (since they are already tested extensively above)

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'unit' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'as' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit as",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'base', 'unit' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit as\n"
                            "        enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit as\n"
                            "        enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit as\n"
                            "        enddef;\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit as\n"
                            "        enddef;\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        def unit my_unit as\n"
                            "        enddef;\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        def unit my_unit as\n"
                           "        enddef;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a component definition with a minimal variable
    // declaration

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier or an SI unit (e.g. 'second') is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'{' or ';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second;\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second;\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second;\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        var my_variable: second;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'init', 'pub' or 'priv' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A number or an identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or '}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        var my_variable: second {init: 3};\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'init', 'pub' or 'priv' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'in', 'out' or 'none' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub: in",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or '}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub: in,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'init', 'pub' or 'priv' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub: in, priv",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub: in, priv:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'in', 'out' or 'none' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub: in, priv: out",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or '}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, pub: in, priv: out}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        var my_variable: second {init: 3, pub: in, priv: out};\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        var my_variable: second {init: 3, pub: in, priv: none};\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Test the declaration of a variable with an invalid public interface
    // attribute

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {pub: invalid_value};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'in', 'out' or 'none' is expected, but 'invalid_value' was found instead."));

    // Test the declaration of a variable with an unknown variable attribute

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {unknown_attribute: invalid_value};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'init', 'pub' or 'priv' is expected, but 'unknown_attribute' was found instead."));

    // Test the declaration of a variable with with twice the same attribute
    // specification

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        var my_variable: second {init: 3, init: 5};\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("The 'init' attribute has already been specified."));

    // Various tests on a component definition to check that we can get the LHS
    // of an equation

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        123",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but '123' was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'=' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or ')' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x)",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'=' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A strictly positive integer number is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 3.5",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A strictly positive integer number is expected, but '3.5' was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, -3",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A strictly positive integer number is expected, but '-3' was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 0",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A strictly positive integer number is expected, but '0' was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 3",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'{' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 3{",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'dimensionless' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 3{dimensionless",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 3{dimensionless}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("')' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        ode(f, x, 3{dimensionless})",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'=' is expected, but the end of the file was found instead."));

    // Various tests on a component definition to check that we can get the RHS
    // of an equation

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier, 'ode', a number, a mathematical function, a mathematical constant or '(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = b",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = b;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        a = b;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'case', 'otherwise' or 'endsel' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier, 'ode', a number, a mathematical function, a mathematical constant or '(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier, 'ode', a number, a mathematical function, a mathematical constant or '(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'case', 'otherwise' or 'endsel' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'case', 'otherwise' or 'endsel' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;\n"
                            "            otherwise",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("':' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;\n"
                            "            otherwise:",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier, 'ode', a number, a mathematical function, a mathematical constant or '(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;\n"
                            "            otherwise: otherid",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;\n"
                            "            otherwise: otherid;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'case', 'otherwise' or 'endsel' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;\n"
                            "            otherwise: otherid;\n"
                            "        endsel",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond1: id1;\n"
                            "            case cond2: id2;\n"
                            "            otherwise: otherid;\n"
                            "        endsel;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        a = sel\n"
                           "            case cond1: id1;\n"
                           "            case cond2: id2;\n"
                           "            otherwise: otherid;\n"
                           "        endsel;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = sel\n"
                            "            case cond: id;\n"
                            "            otherwise: otherid1;\n"
                            "            otherwise: otherid2;\n"
                            "        endsel;\n"
                            "    enddef;\n"
                            "enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("There can only be one 'otherwise' clause."));

    // Various tests on a component definition to check that we can have an ODE
    // on the RHS of an equation

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("',' or ')' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t)",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t);",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        a = ode(b, t);\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t,",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("A strictly positive integer number is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t, 2",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'{' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t, 2{",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'dimensionless' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t, 2{dimensionless",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t, 2{dimensionless}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("')' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t, 2{dimensionless})",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = ode(b, t, 2{dimensionless});",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        a = ode(b, t, 2{dimensionless});\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a component definition to check that we can have a
    // number on the RHS of an equation

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = 3.5",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'{' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = 3.5{",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier or an SI unit (e.g. 'second') is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = 3.5{second",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = 3.5{second}",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = 3.5{second};",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        a = 3.5{second};\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a component definition to check that we can have a
    // parenthesised mathematical expressionon the RHS of an equation

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = (",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier, 'ode', a number, a mathematical function, a mathematical constant or '(' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = (b",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("')' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = (b)",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def comp my_component as\n"
                            "        a = (b);",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def', 'var', an identifier, 'ode' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def comp my_component as\n"
                           "        a = (b);\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());
}

//==============================================================================

void ParsingTests::groupTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal group containment definition

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'as' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'containment' or 'encapsulation' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier, 'and' or 'for' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as containment for\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a group definition of containment type and with a name

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment my_containment",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'and' or 'for' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment my_containment for",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as containment my_containment for\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a group definition of encapsulation type

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as encapsulation",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'and' or 'for' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as encapsulation for",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as encapsulation for\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment and",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'containment' or 'encapsulation' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as containment and containment my_containment and encapsulation for\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a group definition of containment type with one level of
    // components in it

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "        comp",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "        comp my_component1",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'incl' or ';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "        comp my_component1;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as containment for\n"
                           "        comp my_component1;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as containment for\n"
                           "        comp my_component1;\n"
                           "        comp my_component2;\n"
                           "        comp my_component3;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    // Various tests on a group definition of containment type with several
    // levels of components in it

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "       comp my_component1 incl",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "        comp my_component1 incl\n"
                            "             comp",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "        comp my_component1 incl\n"
                            "             comp my_component11",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'incl' or ';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def group as containment for\n"
                            "        comp my_component1 incl\n"
                            "             comp my_component11;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'comp' or 'endcomp' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def group as containment for\n"
                           "        comp my_component1 incl\n"
                           "             comp my_component11;\n"
                           "             comp my_component12;\n"
                           "             comp my_component13;\n"
                           "        enddef;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());
}

//==============================================================================

void ParsingTests::mapTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal map definition

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'between' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'and' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'for' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'vars' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "    enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "    enddef;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'def' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "    enddef;\n"
                            "enddef",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def map between my_component1 and my_component2 for\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "        vars",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "        vars my_variable1",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'and' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "        vars my_variable1 and",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "        vars my_variable1 and my_variable2",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute("def model my_model as\n"
                            "    def map between my_component1 and my_component2 for\n"
                            "        vars my_variable1 and my_variable2;",
                            OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QCOMPARE(parser.messages().first().type(), OpenCOR::CellMLTextView::CellmlTextViewParserMessage::Error);
    QCOMPARE(parser.messages().first().message(), QString("'vars' or 'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute("def model my_model as\n"
                           "    def map between my_component1 and my_component2 for\n"
                           "        vars my_variable1 and my_variable2;\n"
                           "    enddef;\n"
                           "enddef;",
                           OpenCOR::CellMLSupport::CellmlFile::Cellml_1_0));
    QVERIFY(!parser.domDocument().isNull());
}

//==============================================================================

QTEST_GUILESS_MAIN(ParsingTests)

//==============================================================================
// End of file
//==============================================================================
