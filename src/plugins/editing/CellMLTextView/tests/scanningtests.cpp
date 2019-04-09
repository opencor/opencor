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

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // CellML Text keyword

    scanner.setText("def");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // Non-CellML Text keyword, but identifier, since within a parameter block

    scanner.setText("{ def }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // Non-CellML Text keyword, but identifier, since outside a parameter block

    scanner.setText("pref");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // CellML Text parameter keyword

    scanner.setText("{ pref }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PrefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // Identifier

    scanner.setText("OpenCOR");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // Unknown token

    scanner.setText("_________");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // Miscellaneous tokens

    scanner.setText("' , = == <> < <= > >= + - * / : ; ( ) { }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::QuoteToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CommaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EqEqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NeqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LtToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LeqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GtToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GeqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PlusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MinusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TimesToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DivideToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ColonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SemiColonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

void ScanningTests::scanningCommentTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    // Single line comments

    scanner.setText("Some stuff... // A single line comment...");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("Some"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("stuff"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SingleLineCommentToken);
    QCOMPARE(scanner.tokenString(), QString(" A single line comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    // Multiline comments

    scanner.setText("/*A multiline comment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A multiline comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("/*A\nmultiline\ncomment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A\nmultiline\ncomment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("/*A multi/*line* comment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A multi/*line* comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("/*A multi/*line*");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("A multi/*line*"));
    QCOMPARE(scanner.tokenComment(), QString("The comment is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("/*A multi/*line*/ comment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A multi/*line"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("comment"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TimesToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DivideToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("/*An incomplete multiline comment...");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("An incomplete multiline comment..."));
    QCOMPARE(scanner.tokenComment(), QString("The comment is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

void ScanningTests::scanningKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("and as between case comp def endcomp enddef endsel for group import incl map model otherwise sel unit using var vars");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AndToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AsToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::BetweenToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CaseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CompToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EndCompToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EndDefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EndSelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ForToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GroupToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ImportToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InclToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MapToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ModelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OtherwiseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnitToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UsingToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::VarToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::VarsToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("abs ceil exp fact floor ln log pow rem root sqr sqrt");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AbsToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CeilToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ExpToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::FactToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::FloorToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LnToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LogToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PowToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::RemToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::RootToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SqrToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SqrtToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("and or xor not");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AndToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OrToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::XorToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NotToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("ode");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OdeToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("min max");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MaxToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("gcd lcm");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GcdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LcmToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("sin cos tan sec csc cot sinh cosh tanh sech csch coth asin acos atan asec acsc acot asinh acosh atanh asech acsch acoth");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CosToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TanToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SecToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CscToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CotToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SinhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CoshToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TanhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SechToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CschToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CothToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AsinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AcosToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AtanToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AsecToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AcscToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AcotToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AsinhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AcoshToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AtanhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AsechToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AcschToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AcothToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("true false nan pi inf e");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TrueToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::FalseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NanToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PiToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InfToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("base encapsulation containment");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::BaseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EncapsulationToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ContainmentToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

void ScanningTests::scanningSiUnitKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("ampere becquerel candela celsius coulomb dimensionless farad gram gray henry hertz joule katal kelvin kilogram liter litre lumen lux meter metre mole newton ohm pascal radian second siemens sievert steradian tesla volt watt weber");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AmpereToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::BecquerelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CandelaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CelsiusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CoulombToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DimensionlessToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::FaradToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GrayToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::HenryToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::HertzToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::JouleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KatalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KelvinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KilogramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LiterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LitreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LumenToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LuxToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MeterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MetreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MoleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NewtonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OhmToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PascalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::RadianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SecondToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SiemensToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SievertToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SteradianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TeslaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::VoltToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::WattToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::WeberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("{ ampere becquerel candela celsius coulomb dimensionless farad gram gray henry hertz joule katal kelvin kilogram liter litre lumen lux meter metre mole newton ohm pascal radian second siemens sievert steradian tesla volt watt weber }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AmpereToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::BecquerelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CandelaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CelsiusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CoulombToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DimensionlessToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::FaradToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GrayToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::HenryToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::HertzToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::JouleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KatalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KelvinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KilogramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LiterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LitreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LumenToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::LuxToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MeterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MetreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MoleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NewtonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OhmToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PascalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::RadianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SecondToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SiemensToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SievertToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::SteradianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TeslaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::VoltToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::WattToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::WeberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

void ScanningTests::scanningParameterKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("{ pref expo mult off }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PrefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ExpoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MultToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OffToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("{ init pub priv }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InitToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PubToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PrivToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("{ yotta zetta exa peta tera giga mega kilo hecto deka deci centi milli micro nano pico femto atto zepto yocto }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::YottaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ZettaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ExaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PetaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::TeraToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::GigaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MegaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::KiloToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::HectoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DekaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::DeciToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::CentiToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MilliToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::MicroToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NanoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::PicoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::FemtoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::AttoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ZeptoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::YoctoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("{ in out none }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::OutToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NoneToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

void ScanningTests::scanningStringTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("\"This is a string...\"");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::StringToken);
    QCOMPARE(scanner.tokenString(), QString("This is a string..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("A string: \"This is a string...\".");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("A"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("string"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::ColonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::StringToken);
    QCOMPARE(scanner.tokenString(), QString("This is a string..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("\"This is an incomplete string...");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("This is an incomplete string..."));
    QCOMPARE(scanner.tokenComment(), QString("The string is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

void ScanningTests::scanningNumberTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("123");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString("123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123.");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString("123."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123.456");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString("123.456"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText(".");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText(".123");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText(".123.");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText(".123.456");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".456"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123e");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("123e"));
    QCOMPARE(scanner.tokenComment(), QString("The exponent has no digits."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123e45");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123e+45");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123e-45");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);

    scanner.setText("123e456789");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::NumberToken);
    QCOMPARE(scanner.tokenComment(), QString("The number is not valid (e.g. too big, too small)."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TokenType::EofToken);
}

//==============================================================================

QTEST_GUILESS_MAIN(ScanningTests)

//==============================================================================
// End of file
//==============================================================================
