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
    // Note: if the given string line style is invalid then we return the index
    //       line style for a solid line...

    int res = lineStyles().indexOf(pStringLineStyle);

    return (res != -1)?
                res:
                1;
}

//==============================================================================

int indexLineStyle(Qt::PenStyle pLineStyle)
{
    // Return the given line style as an index line style

    return indexLineStyle(stringLineStyle(pLineStyle));
}

//==============================================================================

QString stringLineStyle(int pIndexLineStyle)
{
    // Return the given index line style as a string line style
    // Note: if the given index line style is invalid then we return the string
    //       line style for a solid line...

    return lineStyles()[(   (pIndexLineStyle >= 0)
                         && (pIndexLineStyle < lineStyles().count()))?
                            pIndexLineStyle:
                            1];
}

//==============================================================================

QString stringLineStyle(Qt::PenStyle pLineStyle)
{
    // Return the given line style as a string line style
    // Note: if the given line style is invalid then we return the string line
    //       style for a solid line...

    return lineStyles()[int((pLineStyle > Qt::DashDotDotLine)?
                                Qt::SolidLine:
                                pLineStyle)];
}

//==============================================================================

Qt::PenStyle lineStyle(const QString &pStringLineStyle)
{
    // Return the given string line style as a line style

    return Qt::PenStyle(indexLineStyle(pStringLineStyle));
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
    // Note: if the given string symbol style is invalid then we return the
    //       index symbol style for a "none" symbol...

    int res = symbolStyles().indexOf(pStringSymbolStyle);

    return (res != -1)?
                res:
                0;
}

//==============================================================================

int indexSymbolStyle(QwtSymbol::Style pSymbolStyle)
{
    // Return the given symbol style as an index symbol style

    return indexSymbolStyle(stringSymbolStyle(pSymbolStyle));
}

//==============================================================================

QString stringSymbolStyle(int pIndexSymbolStyle)
{
    // Return the given index symbol style as a string symbol style
    // Note: if the given index symbol style is invalid then we return the
    //       string symbol style for a "none" symbol...

    return symbolStyles()[(   (pIndexSymbolStyle >= 0)
                           && (pIndexSymbolStyle < symbolStyles().count()))?
                              pIndexSymbolStyle:
                              0];
}

//==============================================================================

QString stringSymbolStyle(QwtSymbol::Style pSymbolStyle)
{
    // Return the given symbol style as a string symbol style
    // Note: if the given symbol style is invalid then we return the string
    //       symbol style for a "none" symbol...

    return symbolStyles()[int((pSymbolStyle <= QwtSymbol::DTriangle)?
                                  pSymbolStyle+1:
                                  ((pSymbolStyle >= QwtSymbol::Cross) && (pSymbolStyle <= QwtSymbol::Star1))?
                                      pSymbolStyle-2:
                                      QwtSymbol::NoSymbol)];
}

//==============================================================================

QwtSymbol::Style symbolStyle(const QString &pStringSymbolStyle)
{
    // Return the given string symbol style as a symbol style
    // Note: the shifting is because indices in our list of symbols don't match
    //       those of QwtSymbol::Style...

    int res = indexSymbolStyle(pStringSymbolStyle);

    return QwtSymbol::Style((res <= 5)?
                                res-1:
                                res+2);
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
