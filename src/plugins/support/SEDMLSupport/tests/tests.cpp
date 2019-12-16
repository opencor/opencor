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
// SED-ML support tests
//==============================================================================

#include "sedmlsupport.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_symbol.h"
#include "qwtend.h"

//==============================================================================

void Tests::lineStyleTests()
{
    // Convert a string line style to an index line style

    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("none"), 0);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("solid"), 1);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("dash"), 2);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("dot"), 3);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("dashdot"), 4);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("dashdotdot"), 5);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle("unknown"), 1);

    // Convert a line style to an index line style

    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle(Qt::NoPen), 0);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle(Qt::SolidLine), 1);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle(Qt::DashLine), 2);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle(Qt::DotLine), 3);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle(Qt::DashDotLine), 4);
    QCOMPARE(OpenCOR::SEDMLSupport::indexLineStyle(Qt::DashDotDotLine), 5);

    // Convert an index line style to a non-formatted string line style

    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(-1), "solid");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(0), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(1), "solid");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(2), "dash");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(3), "dot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(4), "dashdot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(5), "dashdotdot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(6), "solid");

    // Convert an index line style to a formatted string line style

    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(-1, true), "Solid");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(0, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(1, true), "Solid");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(2, true), "Dash");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(3, true), "Dot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(4, true), "DashDot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(5, true), "DashDotDot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(6, true), "Solid");

    // Convert a line style to a non-formatted string line style

    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::NoPen), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::SolidLine), "solid");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DashLine), "dash");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DotLine), "dot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DashDotLine), "dashdot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DashDotDotLine), "dashdotdot");

    // Convert a line style to a formatted string line style

    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::NoPen, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::SolidLine, true), "Solid");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DashLine, true), "Dash");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DotLine, true), "Dot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DashDotLine, true), "DashDot");
    QCOMPARE(OpenCOR::SEDMLSupport::stringLineStyle(Qt::DashDotDotLine, true), "DashDotDot");

    // Convert an index line style to a line style

    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(-1), Qt::SolidLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(0), Qt::NoPen);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(1), Qt::SolidLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(2), Qt::DashLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(3), Qt::DotLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(4), Qt::DashDotLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(5), Qt::DashDotDotLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle(6), Qt::SolidLine);

    // Convert a string line style to a line style

    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("none"), Qt::NoPen);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("solid"), Qt::SolidLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("dash"), Qt::DashLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("dot"), Qt::DotLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("dashdot"), Qt::DashDotLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("dashdotdot"), Qt::DashDotDotLine);
    QCOMPARE(OpenCOR::SEDMLSupport::lineStyle("unknown"), Qt::SolidLine);
}

//==============================================================================

void Tests::symbolStyleTests()
{
    // Convert a string symbol style to an index symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("none"), 0);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("circle"), 1);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("square"), 2);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("diamond"), 3);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("triangle"), 4);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("downTriangle"), 5);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("leftTriangle"), 6);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("rightTriangle"), 7);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("cross"), 8);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("xCross"), 9);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("horizontalLine"), 10);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("verticalLine"), 11);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("star"), 12);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle("unknown"), 0);

    // Convert a symbol style to an index symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::NoSymbol), 0);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::Ellipse), 1);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::Rect), 2);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::Diamond), 3);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::Triangle), 4);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::DTriangle), 5);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::LTriangle), 6);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::RTriangle), 7);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::Cross), 8);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::XCross), 9);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::HLine), 10);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::VLine), 11);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyle(QwtSymbol::Star1), 12);

    // Convert an index symbol style to a non-formatted string symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(-1), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(0), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(1), "circle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(2), "square");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(3), "diamond");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(4), "triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(5), "downTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(6), "leftTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(7), "rightTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(8), "cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(9), "xCross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(10), "horizontalLine");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(11), "verticalLine");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(12), "star");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(13), "none");

    // Convert an index symbol style to a formatted string symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(-1, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(0, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(1, true), "Circle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(2, true), "Square");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(3, true), "Diamond");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(4, true), "Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(5, true), "Down Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(6, true), "Left Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(7, true), "Right Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(8, true), "Cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(9, true), "X Cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(10, true), "Horizontal Line");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(11, true), "Vertical Line");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(12, true), "Star");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(13, true), "None");

    // Convert a line style to a non-formatted string line style

    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::NoSymbol), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Ellipse), "circle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Rect), "square");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Diamond), "diamond");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Triangle), "triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::DTriangle), "downTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::LTriangle), "leftTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::RTriangle), "rightTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Cross), "cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::XCross), "xCross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::HLine), "horizontalLine");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::VLine), "verticalLine");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Star1), "star");

    // Convert a line style to a formatted string line style

    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::NoSymbol, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Ellipse, true), "Circle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Rect, true), "Square");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Diamond, true), "Diamond");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Triangle, true), "Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::DTriangle, true), "Down Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::LTriangle, true), "Left Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::RTriangle, true), "Right Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Cross, true), "Cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::XCross, true), "X Cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::HLine, true), "Horizontal Line");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::VLine, true), "Vertical Line");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyle(QwtSymbol::Star1, true), "Star");

    // Convert an index symbol style to a symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(-1), QwtSymbol::NoSymbol);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(0), QwtSymbol::NoSymbol);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(1), QwtSymbol::Ellipse);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(2), QwtSymbol::Rect);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(3), QwtSymbol::Diamond);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(4), QwtSymbol::Triangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(5), QwtSymbol::DTriangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(6), QwtSymbol::LTriangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(7), QwtSymbol::RTriangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(8), QwtSymbol::Cross);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(9), QwtSymbol::XCross);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(10), QwtSymbol::HLine);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(11), QwtSymbol::VLine);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(12), QwtSymbol::Star1);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle(13), QwtSymbol::NoSymbol);

    // Convert a string line style to a line style

    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("none"), QwtSymbol::NoSymbol);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("circle"), QwtSymbol::Ellipse);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("square"), QwtSymbol::Rect);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("diamond"), QwtSymbol::Diamond);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("triangle"), QwtSymbol::Triangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("downTriangle"), QwtSymbol::DTriangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("leftTriangle"), QwtSymbol::LTriangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("rightTriangle"), QwtSymbol::RTriangle);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("cross"), QwtSymbol::Cross);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("xCross"), QwtSymbol::XCross);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("horizontalLine"), QwtSymbol::HLine);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("verticalLine"), QwtSymbol::VLine);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("star"), QwtSymbol::Star1);
    QCOMPARE(OpenCOR::SEDMLSupport::symbolStyle("unknown"), QwtSymbol::NoSymbol);
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
