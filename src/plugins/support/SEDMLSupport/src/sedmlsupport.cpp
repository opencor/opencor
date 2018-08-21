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

int lineStyleIntegerValue(const QString &pLineStyleValue)
{
    // Return the index of the given line style value

    return lineStyles().indexOf(pLineStyleValue);
}

//==============================================================================

QString lineStyleStringValue(int pLineStyleValueIndex)
{
    // Return the line style value for the given index

    if (   (pLineStyleValueIndex >= 0)
        && (pLineStyleValueIndex < lineStyles().count())) {
        return lineStyles()[pLineStyleValueIndex];
    } else {
        return QString();
    }
}

//==============================================================================

QString lineStyleStringValue(Qt::PenStyle pLineStyle)
{
    // Return the line style value for the given line style

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

int symbolStyleIntegerValue(const QString &pSymbolStyleValue)
{
    // Return the index of the given symbol style value

    return symbolStyles().indexOf(pSymbolStyleValue);
}

//==============================================================================

QString symbolStyleStringValue(int pSymbolStyleValueIndex)
{
    // Return the symbol style value for the given index

    if (   (pSymbolStyleValueIndex >= 0)
        && (pSymbolStyleValueIndex < symbolStyles().count())) {
        return symbolStyles()[pSymbolStyleValueIndex];
    } else {
        return QString();
    }
}

//==============================================================================

QString symbolStyleStringValue(QwtSymbol::Style pSymbolStyle)
{
    // Return the symbol style value for the given symbol style

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
