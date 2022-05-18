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

    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("none"), 0);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("circle"), 1);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("square"), 2);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("diamond"), 3);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("triangle"), 4);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("downTriangle"), 5);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("leftTriangle"), 6);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("rightTriangle"), 7);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("cross"), 8);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("xCross"), 9);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("horizontalLine"), 10);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("verticalLine"), 11);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("star"), 12);
    QCOMPARE(OpenCOR::SEDMLSupport::indexSymbolStyleFromString("unknown"), 0);

    // Convert an index symbol style to a non-formatted string symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(-1), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(0), "none");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(1), "circle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(2), "square");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(3), "diamond");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(4), "triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(5), "downTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(6), "leftTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(7), "rightTriangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(8), "cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(9), "xCross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(10), "horizontalLine");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(11), "verticalLine");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(12), "star");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(13), "none");

    // Convert an index symbol style to a formatted string symbol style

    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(-1, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(0, true), "None");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(1, true), "Circle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(2, true), "Square");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(3, true), "Diamond");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(4, true), "Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(5, true), "Down Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(6, true), "Left Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(7, true), "Right Triangle");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(8, true), "Cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(9, true), "X Cross");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(10, true), "Horizontal Line");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(11, true), "Vertical Line");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(12, true), "Star");
    QCOMPARE(OpenCOR::SEDMLSupport::stringSymbolStyleFromIndex(13, true), "None");
}

//==============================================================================

QTEST_APPLESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
