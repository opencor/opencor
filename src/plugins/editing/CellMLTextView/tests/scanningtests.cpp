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

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // CellML Text keyword

    scanner.setText("def");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // Non-CellML Text keyword, but identifier, since within a parameter block

    scanner.setText("{ def }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // Non-CellML Text keyword, but identifier, since outside a parameter block

    scanner.setText("pref");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // CellML Text parameter keyword

    scanner.setText("{ pref }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PrefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // Identifier

    scanner.setText("OpenCOR");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // Unknown token

    scanner.setText("_________");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // Miscellaneous tokens

    scanner.setText("' , = == <> < <= > >= + - * / : ; ( ) { }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::QuoteToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CommaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EqEqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NeqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LtToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LeqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GtToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GeqToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PlusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MinusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TimesToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DivideToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ColonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SemiColonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

void ScanningTests::scanningCommentTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    // Single line comments

    scanner.setText("Some stuff... // A single line comment...");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("Some"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("stuff"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SingleLineCommentToken);
    QCOMPARE(scanner.tokenString(), QString(" A single line comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    // Multiline comments

    scanner.setText("/*A multiline comment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A multiline comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("/*A\nmultiline\ncomment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A\nmultiline\ncomment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("/*A multi/*line* comment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A multi/*line* comment..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("/*A multi/*line*");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("A multi/*line*"));
    QCOMPARE(scanner.tokenComment(), QString("The comment is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("/*A multi/*line*/ comment...*/");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MultilineCommentToken);
    QCOMPARE(scanner.tokenString(), QString("A multi/*line"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("comment"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TimesToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DivideToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("/*An incomplete multiline comment...");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("An incomplete multiline comment..."));
    QCOMPARE(scanner.tokenComment(), QString("The comment is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

void ScanningTests::scanningKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("and as between case comp def endcomp enddef endsel for group import incl map model otherwise sel unit using var vars");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AndToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AsToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::BetweenToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CaseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CompToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EndCompToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EndDefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EndSelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ForToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GroupToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ImportToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InclToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MapToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ModelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OtherwiseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnitToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UsingToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::VarToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::VarsToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("abs ceil exp fact floor ln log pow rem root sqr sqrt");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AbsToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CeilToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ExpToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::FactToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::FloorToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LnToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LogToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PowToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::RemToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::RootToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SqrToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SqrtToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("and or xor not");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AndToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OrToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::XorToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NotToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("ode");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OdeToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("min max");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MaxToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("gcd lcm");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GcdToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LcmToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("sin cos tan sec csc cot sinh cosh tanh sech csch coth asin acos atan asec acsc acot asinh acosh atanh asech acsch acoth");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CosToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TanToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SecToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CscToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CotToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SinhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CoshToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TanhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SechToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CschToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CothToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AsinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AcosToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AtanToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AsecToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AcscToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AcotToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AsinhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AcoshToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AtanhToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AsechToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AcschToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AcothToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("true false nan pi inf e");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TrueToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::FalseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NanToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PiToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InfToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("base encapsulation containment");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::BaseToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EncapsulationToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ContainmentToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

void ScanningTests::scanningSiUnitKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("ampere becquerel candela celsius coulomb dimensionless farad gram gray henry hertz joule katal kelvin kilogram liter litre lumen lux meter metre mole newton ohm pascal radian second siemens sievert steradian tesla volt watt weber");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AmpereToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::BecquerelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CandelaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CelsiusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CoulombToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DimensionlessToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::FaradToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GrayToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::HenryToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::HertzToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::JouleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KatalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KelvinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KilogramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LiterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LitreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LumenToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LuxToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MeterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MetreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MoleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NewtonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OhmToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PascalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::RadianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SecondToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SiemensToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SievertToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SteradianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TeslaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::VoltToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::WattToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::WeberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("{ ampere becquerel candela celsius coulomb dimensionless farad gram gray henry hertz joule katal kelvin kilogram liter litre lumen lux meter metre mole newton ohm pascal radian second siemens sievert steradian tesla volt watt weber }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AmpereToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::BecquerelToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CandelaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CelsiusToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CoulombToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DimensionlessToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::FaradToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GrayToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::HenryToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::HertzToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::JouleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KatalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KelvinToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KilogramToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LiterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LitreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LumenToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::LuxToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MeterToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MetreToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MoleToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NewtonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OhmToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PascalToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::RadianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SecondToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SiemensToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SievertToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::SteradianToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TeslaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::VoltToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::WattToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::WeberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

void ScanningTests::scanningParameterKeywordTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("{ pref expo mult off }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PrefToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ExpoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MultToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OffToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("{ init pub priv }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InitToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PubToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PrivToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("{ yotta zetta exa peta tera giga mega kilo hecto deka deci centi milli micro nano pico femto atto zepto yocto }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::YottaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ZettaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ExaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PetaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::TeraToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::GigaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MegaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::KiloToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::HectoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DekaToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::DeciToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::CentiToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MilliToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::MicroToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NanoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::PicoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::FemtoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::AttoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ZeptoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::YoctoToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("{ in out none }");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OpeningCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::OutToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NoneToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ClosingCurlyBracketToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

void ScanningTests::scanningStringTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("\"This is a string...\"");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::StringToken);
    QCOMPARE(scanner.tokenString(), QString("This is a string..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("A string: \"This is a string...\".");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("A"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::IdentifierOrCmetaIdToken);
    QCOMPARE(scanner.tokenString(), QString("string"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::ColonToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::StringToken);
    QCOMPARE(scanner.tokenString(), QString("This is a string..."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("\"This is an incomplete string...");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("This is an incomplete string..."));
    QCOMPARE(scanner.tokenComment(), QString("The string is incomplete."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

void ScanningTests::scanningNumberTests()
{
    OpenCOR::CellMLTextView::CellmlTextViewScanner scanner;

    scanner.setText("123");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString("123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123.");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString("123."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123.456");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString("123.456"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText(".");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText(".123");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText(".123.");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::UnknownToken);
    QCOMPARE(scanner.tokenString(), QString("."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText(".123.456");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".123"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenString(), QString(".456"));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123e");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::InvalidToken);
    QCOMPARE(scanner.tokenString(), QString("123e"));
    QCOMPARE(scanner.tokenComment(), QString("The exponent has no digits."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123e45");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123e+45");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123e-45");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);

    scanner.setText("123e456789");

    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::NumberToken);
    QCOMPARE(scanner.tokenComment(), QString("The number is not valid (e.g. too big, too small)."));
    scanner.getNextToken();
    QCOMPARE(scanner.tokenType(), OpenCOR::CellMLTextView::CellmlTextViewScanner::EofToken);
}

//==============================================================================

QTEST_GUILESS_MAIN(ScanningTests)

//==============================================================================
// End of file
//==============================================================================
