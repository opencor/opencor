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
// Simulation Experiment view SED-ML support
//==============================================================================

#include "sedmlsupport.h"
#include "simulationexperimentviewsedmlsupport.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

QString stringLineStyleFromQtPenStyle(Qt::PenStyle pQtPenStyle, bool pFormatted)
{
    // Return the given Qt pen style as a string line style
    // Note: if the given Qt pen style is invalid then we return the string line
    //       style for a solid line...

    return SEDMLSupport::stringLineStyleFromIndex(indexLineStyleFromQtPenStyle(pQtPenStyle),
                                                  pFormatted);
}

//==============================================================================

Qt::PenStyle qtPenStyleFromIndex(int pIndexLineStyle)
{
    // Return the given index line style as a line style
    // Note: if the given index line style is invalid then we return the line
    //       style for a solid line...

    static const int LineStylesCount = SEDMLSupport::lineStyles().count();

    return Qt::PenStyle((   (pIndexLineStyle >= 0)
                         && (pIndexLineStyle < LineStylesCount))?
                            pIndexLineStyle:
                            1);
}

//==============================================================================

Qt::PenStyle qtPenStyleFromString(const QString &pStringLineStyle)
{
    // Return the given string line style as a line style

    return qtPenStyleFromIndex(SEDMLSupport::indexLineStyleFromString(pStringLineStyle));
}

//==============================================================================

int indexLineStyleFromQtPenStyle(Qt::PenStyle pQtPenStyle)
{
    // Return the given line style as an index line style

    return int((pQtPenStyle > Qt::DashDotDotLine)?
                   Qt::SolidLine:
                   pQtPenStyle);
}

//==============================================================================

QString stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Style pQwtSymbolStyle,
                                            bool pFormatted)
{
    // Return the given Qwt symbol style as a string symbol style
    // Note: if the given symbol style is invalid then we return the none
    //       string...

    return SEDMLSupport::stringSymbolStyleFromIndex(indexSymbolStyleFromQwtSymbolStyle(pQwtSymbolStyle),
                                                    pFormatted);
}

//==============================================================================

QwtSymbol::Style qwtSymbolStyleFromIndex(int pIndexSymbolStyle)
{
    // Return the given index symbol style as a Qwt symbol style
    // Note #1: if the given index symbol style is invalid then we return the
    //          string symbol style for a "none" symbol...
    // Note #2: the shifting is because indices in our list of symbols don't
    //          match those of QwtSymbol::Style...

    static const int SymbolStylesCount = SEDMLSupport::symbolStyles().count();

    return QwtSymbol::Style(   (   (pIndexSymbolStyle >= 0)
                            && (pIndexSymbolStyle < SymbolStylesCount))?
                                (pIndexSymbolStyle <= 5)?
                                    pIndexSymbolStyle-1:
                                    pIndexSymbolStyle:
                                -1);
}

//==============================================================================

QwtSymbol::Style qwtSymbolStyleFromString(const QString &pStringSymbolStyle)
{
    // Return the given string symbol style as a Qwt symbol style

    return qwtSymbolStyleFromIndex(SEDMLSupport::indexSymbolStyleFromString(pStringSymbolStyle));
}

//==============================================================================

int indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Style pQwtSymbolStyle)
{
    // Return the given Qwt symbol style as an index symbol style

    return int((pQwtSymbolStyle <= QwtSymbol::DTriangle)?
                   pQwtSymbolStyle+1:
                   ((pQwtSymbolStyle >= QwtSymbol::LTriangle) && (pQwtSymbolStyle <= QwtSymbol::Star1))?
                       pQwtSymbolStyle:
                       QwtSymbol::NoSymbol+1);
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
