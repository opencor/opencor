/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML Text view scanning tests
//==============================================================================

#include "cellmltextviewscanner.h"
#include "scanningtests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void ScanningTests::basicScanningTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    // Skipping spaces, tabs, CRs and LFs, thus ending up with the EOF token

    scanner.setText("   \t\r\n   \t   \r   \n   ");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // CellML Text keyword

    scanner.setText("def");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Def);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // Non-CellML Text keyword, but identifier, since within a parameter block

    scanner.setText("{ def }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // Non-CellML Text keyword, but identifier, since outside a parameter block

    scanner.setText("pref");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // CellML Text parameter keyword

    scanner.setText("{ pref }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pref);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // Identifier

    scanner.setText("OpenCOR");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // Unknown token

    scanner.setText("_________");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // Miscellaneous tokens

    scanner.setText("' , = == <> < <= > >= + - * / : ; ( ) { }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Quote);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Comma);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eq);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::EqEq);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Neq);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Lt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Leq);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Gt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Geq);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Plus);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Minus);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Times);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Divide);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Colon);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::SemiColon);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

void ScanningTests::scanningCommentTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    // Single line comments

    scanner.setText("Some stuff... // A single line comment...");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    QCOMPARE(scanner.string(), QString("Some"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    QCOMPARE(scanner.string(), QString("stuff"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::SingleLineComment);
    QCOMPARE(scanner.string(), QString(" A single line comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    // Multiline comments

    scanner.setText("/*A multiline comment...*/");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::MultilineComment);
    QCOMPARE(scanner.string(), QString("A multiline comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("/*A\nmultiline\ncomment...*/");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::MultilineComment);
    QCOMPARE(scanner.string(), QString("A\nmultiline\ncomment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("/*A multi/*line* comment...*/");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::MultilineComment);
    QCOMPARE(scanner.string(), QString("A multi/*line* comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("/*A multi/*line*");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Invalid);
    QCOMPARE(scanner.string(), QString("A multi/*line*"));
    QCOMPARE(scanner.comment(), QString("The comment is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("/*A multi/*line*/ comment...*/");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::MultilineComment);
    QCOMPARE(scanner.string(), QString("A multi/*line"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    QCOMPARE(scanner.string(), QString("comment"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Times);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Divide);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("/*An incomplete multiline comment...");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Invalid);
    QCOMPARE(scanner.string(), QString("An incomplete multiline comment..."));
    QCOMPARE(scanner.comment(), QString("The comment is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

void ScanningTests::scanningKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("and as between case comp def endcomp enddef endsel for group import incl map model otherwise sel unit using var vars");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::And);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::As);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Between);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Case);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Comp);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Def);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::EndComp);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::EndDef);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::EndSel);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::For);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Group);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Import);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Incl);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Map);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Model);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Otherwise);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sel);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unit);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Using);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Var);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Vars);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("abs ceil exp fact floor ln log pow rem root sqr sqrt");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Abs);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ceil);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Exp);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Fact);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Floor);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ln);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Log);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pow);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Rem);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Root);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sqr);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sqrt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("and or xor not");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::And);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Or);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Xor);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Not);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("ode");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ode);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("min max");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Min);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Max);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("gcd lcm");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Gcd);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Lcm);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("sin cos tan sec csc cot sinh cosh tanh sech csch coth asin acos atan asec acsc acot asinh acosh atanh asech acsch acoth");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sin);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Cos);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Tan);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sec);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Csc);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Cot);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sinh);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Cosh);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Tanh);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sech);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Csch);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Coth);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Asin);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Acos);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Atan);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Asec);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Acsc);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Acot);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Asinh);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Acosh);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Atanh);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Asech);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Acsch);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Acoth);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("true false nan pi inf e");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::True);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::False);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Nan);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pi);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Inf);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::E);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("base encapsulation containment");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Base);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Encapsulation);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Containment);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

void ScanningTests::scanningSiUnitKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("ampere becquerel candela celsius coulomb dimensionless farad gram gray henry hertz joule katal kelvin kilogram liter litre lumen lux meter metre mole newton ohm pascal radian second siemens sievert steradian tesla volt watt weber");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ampere);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Becquerel);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Candela);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Celsius);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Coulomb);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Dimensionless);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Farad);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Gram);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Gray);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Henry);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Hertz);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Joule);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Katal);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Kelvin);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Kilogram);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Liter);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Litre);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Lumen);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Lux);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Meter);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Metre);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Mole);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Newton);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ohm);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pascal);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Radian);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Second);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Siemens);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sievert);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Steradian);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Tesla);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Volt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Watt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Weber);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("{ ampere becquerel candela celsius coulomb dimensionless farad gram gray henry hertz joule katal kelvin kilogram liter litre lumen lux meter metre mole newton ohm pascal radian second siemens sievert steradian tesla volt watt weber }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ampere);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Becquerel);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Candela);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Celsius);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Coulomb);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Dimensionless);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Farad);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Gram);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Gray);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Henry);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Hertz);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Joule);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Katal);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Kelvin);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Kilogram);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Liter);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Litre);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Lumen);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Lux);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Meter);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Metre);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Mole);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Newton);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Ohm);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pascal);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Radian);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Second);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Siemens);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Sievert);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Steradian);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Tesla);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Volt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Watt);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Weber);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

void ScanningTests::scanningParameterKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("{ pref expo mult off }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pref);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Expo);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Mult);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Off);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("{ init pub priv }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Init);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pub);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Priv);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("{ yotta zetta exa peta tera giga mega kilo hecto deka deci centi milli micro nano pico femto atto zepto yocto }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Yotta);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Zetta);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Exa);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Peta);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Tera);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Giga);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Mega);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Kilo);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Hecto);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Deka);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Deci);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Centi);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Milli);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Micro);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Nano);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Pico);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Femto);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Atto);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Zepto);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Yocto);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("{ in out none }");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::OpeningCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::In);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Out);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::None);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::ClosingCurlyBracket);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

void ScanningTests::scanningStringTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText(R"("This is a string...")");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::String);
    QCOMPARE(scanner.string(), QString("This is a string..."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText(R"(A string: "This is a string...".)");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    QCOMPARE(scanner.string(), QString("A"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::IdentifierOrCmetaId);
    QCOMPARE(scanner.string(), QString("string"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Colon);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::String);
    QCOMPARE(scanner.string(), QString("This is a string..."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText(R"("This is an incomplete string...)");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Invalid);
    QCOMPARE(scanner.string(), QString("This is an incomplete string..."));
    QCOMPARE(scanner.comment(), QString("The string is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

void ScanningTests::scanningNumberTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("123");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString("123"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123.");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString("123."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123.456");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString("123.456"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText(".");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText(".123");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText(".123.");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Unknown);
    QCOMPARE(scanner.string(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText(".123.456");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.string(), QString(".456"));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123e");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Invalid);
    QCOMPARE(scanner.string(), QString("123e"));
    QCOMPARE(scanner.comment(), QString("The exponent has no digits."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123e45");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123e+45");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123e-45");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);

    scanner.setText("123e456789");

    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Number);
    QCOMPARE(scanner.comment(), QString("The number is not valid (e.g. too big, too small)."));
    scanner.getNextToken();
    QCOMPARE(scanner.token(), OpenCOR::CellMLTextView::CellmlTextViewScanner::Token::Eof);
}

//==============================================================================

QTEST_GUILESS_MAIN(ScanningTests)

//==============================================================================
// End of file
//==============================================================================
