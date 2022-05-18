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
// Simulation Experiment view tests
//==============================================================================

#include "simulationexperimentviewsedmlsupport.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::lineStyleTests()
{
    // Convert a Qt pen style to a string line style

    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::NoPen), "none");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::SolidLine), "solid");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DashLine), "dash");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DotLine), "dot");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DashDotLine), "dashdot");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DashDotDotLine), "dashdotdot");

    // Convert a Qt pen style to a formatted string line style

    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::NoPen, true), "None");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::SolidLine, true), "Solid");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DashLine, true), "Dash");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DotLine, true), "Dot");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DashDotLine, true), "DashDot");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringLineStyleFromQtPenStyle(Qt::DashDotDotLine, true), "DashDotDot");

    // Convert an index line style to a Qt pen style

    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(-1), Qt::SolidLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(0), Qt::NoPen);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(1), Qt::SolidLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(2), Qt::DashLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(3), Qt::DotLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(4), Qt::DashDotLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(5), Qt::DashDotDotLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromIndex(6), Qt::SolidLine);

    // Convert a string line style to a Qt pen style

    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("none"), Qt::NoPen);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("solid"), Qt::SolidLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("dash"), Qt::DashLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("dot"), Qt::DotLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("dashdot"), Qt::DashDotLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("dashdotdot"), Qt::DashDotDotLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qtPenStyleFromString("unknown"), Qt::SolidLine);

    // Convert a Qt pen style to an index line style

    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::NoPen), 0);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::SolidLine), 1);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::DashLine), 2);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::DotLine), 3);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::DashDotLine), 4);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::DashDotDotLine), 5);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexLineStyleFromQtPenStyle(Qt::CustomDashLine), 1);
}

//==============================================================================

void Tests::symbolStyleTests()
{
    // Convert a Qwt symbol style to a string symbol style

    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::NoSymbol), "none");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Ellipse), "circle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Rect), "square");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Diamond), "diamond");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Triangle), "triangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::DTriangle), "downTriangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::LTriangle), "leftTriangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::RTriangle), "rightTriangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Cross), "cross");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::XCross), "xCross");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::HLine), "horizontalLine");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::VLine), "verticalLine");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Star1), "star");

    // Convert a Qwt symbol style to a formatted string symbol style

    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::NoSymbol, true), "None");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Ellipse, true), "Circle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Rect, true), "Square");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Diamond, true), "Diamond");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Triangle, true), "Triangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::DTriangle, true), "Down Triangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::LTriangle, true), "Left Triangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::RTriangle, true), "Right Triangle");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Cross, true), "Cross");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::XCross, true), "X Cross");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::HLine, true), "Horizontal Line");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::VLine, true), "Vertical Line");
    QCOMPARE(OpenCOR::SimulationExperimentView::stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Star1, true), "Star");

    // Convert an index symbol style to a Qwt symbol style

    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(-1), QwtSymbol::NoSymbol);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(0), QwtSymbol::NoSymbol);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(1), QwtSymbol::Ellipse);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(2), QwtSymbol::Rect);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(3), QwtSymbol::Diamond);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(4), QwtSymbol::Triangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(5), QwtSymbol::DTriangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(6), QwtSymbol::LTriangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(7), QwtSymbol::RTriangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(8), QwtSymbol::Cross);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(9), QwtSymbol::XCross);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(10), QwtSymbol::HLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(11), QwtSymbol::VLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(12), QwtSymbol::Star1);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromIndex(13), QwtSymbol::NoSymbol);

    // Convert a string symbol style to a Qwt symbol style

    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("none"), QwtSymbol::NoSymbol);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("circle"), QwtSymbol::Ellipse);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("square"), QwtSymbol::Rect);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("diamond"), QwtSymbol::Diamond);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("triangle"), QwtSymbol::Triangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("downTriangle"), QwtSymbol::DTriangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("leftTriangle"), QwtSymbol::LTriangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("rightTriangle"), QwtSymbol::RTriangle);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("cross"), QwtSymbol::Cross);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("xCross"), QwtSymbol::XCross);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("horizontalLine"), QwtSymbol::HLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("verticalLine"), QwtSymbol::VLine);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("star"), QwtSymbol::Star1);
    QCOMPARE(OpenCOR::SimulationExperimentView::qwtSymbolStyleFromString("unknown"), QwtSymbol::NoSymbol);

    // Convert a Qwt symbol style to an index symbol style

    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::NoSymbol), 0);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Ellipse), 1);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Rect), 2);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Diamond), 3);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Triangle), 4);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::DTriangle), 5);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::LTriangle), 6);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::RTriangle), 7);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Cross), 8);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::XCross), 9);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::HLine), 10);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::VLine), 11);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Star1), 12);
    QCOMPARE(OpenCOR::SimulationExperimentView::indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Hexagon), 0);
}

//==============================================================================

QTEST_APPLESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
