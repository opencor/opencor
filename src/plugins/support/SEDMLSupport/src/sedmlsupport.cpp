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
// SED-ML support
//==============================================================================

#include "sedmlsupport.h"

//==============================================================================

#include <QStringList>

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

QStringList lineStyles()
{
    // Return our list of line styles

    static const QStringList LineStyles = QStringList() << "none" << "solid"
                                                        << "dash" << "dot"
                                                        << "dashdot"
                                                        << "dashdotdot";

    return LineStyles;
}

//==============================================================================

int indexLineStyle(const QString &pStringLineStyle)
{
    // Return the given string line style as an index line style

    return lineStyles().indexOf(pStringLineStyle);
}

//==============================================================================

QString stringLineStyle(int pIndexLineStyle)
{
    // Return the given index line style as a string line style

    return lineStyles()[(   (pIndexLineStyle >= 0)
                         && (pIndexLineStyle < lineStyles().count()))?
                            pIndexLineStyle:
                            0];
}

//==============================================================================

QString stringLineStyle(Qt::PenStyle pLineStyle)
{
    // Return the given line style as a string line style

    return lineStyles()[int((pLineStyle > Qt::DashDotDotLine)?
                                Qt::SolidLine:
                                pLineStyle)];
}

//==============================================================================

QStringList symbolStyles()
{
    // Return our list of symbol styles

    static const QStringList SymbolStyles = QStringList() << "none" << "circle"
                                                          << "square"
                                                          << "diamond"
                                                          << "triangle"
                                                          << "downTriangle"
                                                          << "cross" << "xCross"
                                                          << "horizontalLine"
                                                          << "verticalLine"
                                                          << "star";

    return SymbolStyles;
}

//==============================================================================

int indexSymbolStyle(const QString &pStringSymbolStyle)
{
    // Return the given string symbol style as an index symbol style

    return symbolStyles().indexOf(pStringSymbolStyle);
}

//==============================================================================

QString stringSymbolStyle(int pIndexSymbolStyle)
{
    // Return the given index symbol style as a string symbol style

    return symbolStyles()[(   (pIndexSymbolStyle >= 0)
                           && (pIndexSymbolStyle < symbolStyles().count()))?
                              pIndexSymbolStyle:
                              0];
}

//==============================================================================

QString stringSymbolStyle(QwtSymbol::Style pSymbolStyle)
{
    // Return the given symbol style as a string symbol style

    return symbolStyles()[int((pSymbolStyle <= QwtSymbol::DTriangle)?
                                  pSymbolStyle+1:
                                  ((pSymbolStyle >= QwtSymbol::Cross) && (pSymbolStyle <= QwtSymbol::Star1))?
                                      pSymbolStyle-2:
                                      QwtSymbol::NoSymbol)];
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
