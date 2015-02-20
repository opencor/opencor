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
// CellML Text view parsing tests
//==============================================================================

#include "cellmltextviewconverter.h"
#include "cellmltextviewparser.h"
#include "corecliutils.h"
#include "parsingtests.h"

//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void ParsingTests::parsingTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewParser parser;

    // Various tests on a minimal model definition

    QVERIFY(!parser.execute(QString()));
    QCOMPARE(parser.messages().first().message(), QString("'def' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def")));
    QCOMPARE(parser.messages().first().message(), QString("'model' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def model")));
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def model my_model")));
    QCOMPARE(parser.messages().first().message(), QString("'as' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def model my_model as")));
    QCOMPARE(parser.messages().first().message(), QString("'enddef' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def model my_model as\n"
                                    "enddef")));
    QCOMPARE(parser.messages().first().message(), QString("';' is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute(QString("def model my_model as\n"
                                   "enddef;")));

    // Various tests on a minimal model definition with a cmeta:id

    QVERIFY(!parser.execute(QString("def model{")));
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def model{my_cmeta_id")));
    QCOMPARE(parser.messages().first().message(), QString("'}' is expected, but the end of the file was found instead."));

    QVERIFY(!parser.execute(QString("def model{my_cmeta_id}")));
    QCOMPARE(parser.messages().first().message(), QString("An identifier is expected, but the end of the file was found instead."));

    QVERIFY(parser.execute(QString("def model{my_cmeta_id} my_model as\n"
                                   "enddef;")));

    // Parsing of some CellML text code and converting to raw CellML...

    QVERIFY(parser.execute(OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.in")).join("\n")));
    QCOMPARELIST(qDomDocumentToString(parser.domDocument()).split("\n"),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.cellml")));

    // ... and back

    OpenCOR::CellMLTextView::CellMLTextViewConverter converter;

    QVERIFY(converter.execute(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.cellml")));
    QCOMPARELIST(converter.output().split("\n"),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/parsing/my_model.in")));
}

//==============================================================================

QTEST_GUILESS_MAIN(ParsingTests)

//==============================================================================
// End of file
//==============================================================================
