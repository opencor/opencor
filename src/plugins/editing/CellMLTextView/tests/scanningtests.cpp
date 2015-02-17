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
// CellML Text view scanning tests
//==============================================================================

#include "cellmltextviewscanner.h"
#include "scanningtests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void ScanningTests::scanningWordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    // CellML text keyword

    scanner.setText("def");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DefToken);

    // Non-CellML text keyword, but identifier, since within a parameter block

    scanner.setText("{ def }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);

    // Non-CellML text keyword, but identifier, since outside a parameter block

    scanner.setText("pref");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierToken);

    // CellML text parameter keyword

    scanner.setText("{ pref }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PrefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);

    // Identifier

    scanner.setText("OpenCOR");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierToken);

    // Unknown token

    scanner.setText("_________");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
}

//==============================================================================

QTEST_GUILESS_MAIN(ScanningTests)

//==============================================================================
// End of file
//==============================================================================
