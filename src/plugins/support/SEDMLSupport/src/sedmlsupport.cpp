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
// SED-ML support
//==============================================================================

#include "sedmlsupport.h"

//==============================================================================

#include <QObject>
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

QStringList formattedLineStyles()
{
    // Return our list of formatted line styles

    return QStringList() << QObject::tr("None")
                         << QObject::tr("Solid")
                         << QObject::tr("Dash")
                         << QObject::tr("Dot")
                         << QObject::tr("DashDot")
                         << QObject::tr("DashDotDot");
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

QString stringLineStyle(int pIndexLineStyle, bool pFormatted)
{
    // Return the given index line style as a string line style
    // Note: if the given index line style is invalid then we return the string
    //       line style for a solid line...

    QStringList res = pFormatted?formattedLineStyles():lineStyles();

    return res[(   (pIndexLineStyle >= 0)
                && (pIndexLineStyle < res.count()))?
                   pIndexLineStyle:
                   1];
}

//==============================================================================

QString stringLineStyle(Qt::PenStyle pLineStyle, bool pFormatted)
{
    // Return the given line style as a string line style
    // Note: if the given line style is invalid then we return the string line
    //       style for a solid line...

    QStringList res = pFormatted?formattedLineStyles():lineStyles();

    return res[int((pLineStyle > Qt::DashDotDotLine)?
                       Qt::SolidLine:
                       pLineStyle)];
}

//==============================================================================

Qt::PenStyle lineStyle(int pIndexLineStyle)
{
    // Return the given index line style as a line style
    // Note: if the given index line style is invalid then we return the line
    //       style for a solid line...

    return Qt::PenStyle((   (pIndexLineStyle >= 0)
                         && (pIndexLineStyle < lineStyles().count()))?
                            pIndexLineStyle:
                            1);
}

//==============================================================================

Qt::PenStyle lineStyle(const QString &pStringLineStyle)
{
    // Return the given string line style as a line style

    return lineStyle(indexLineStyle(pStringLineStyle));
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
                                                          << "leftTriangle"
                                                          << "rightTriangle"
                                                          << "cross" << "xCross"
                                                          << "horizontalLine"
                                                          << "verticalLine"
                                                          << "star";

    return SymbolStyles;
}

//==============================================================================

QStringList formattedSymbolStyles()
{
    // Return our list of formatted symbol styles

    return QStringList() << QObject::tr("None")
                         << QObject::tr("Circle")
                         << QObject::tr("Square")
                         << QObject::tr("Diamond")
                         << QObject::tr("Triangle")
                         << QObject::tr("Down Triangle")
                         << QObject::tr("Left Triangle")
                         << QObject::tr("Right Triangle")
                         << QObject::tr("Cross")
                         << QObject::tr("X Cross")
                         << QObject::tr("Horizontal Line")
                         << QObject::tr("Vertical Line")
                         << QObject::tr("Star");
}

//==============================================================================

int indexSymbolStyle(const QString &pStringSymbolStyle)
{
    // Return the given string symbol style as an index symbol style
    // Note: if the given string symbol style is invalid then we return the
    //       index symbol style for a "none" symbol...

    int res = symbolStyles().indexOf(pStringSymbolStyle);

    return (res != -1)?
               res :
               0;
}

//==============================================================================

int indexSymbolStyle(QwtSymbol::Style pSymbolStyle)
{
    // Return the given symbol style as an index symbol style

    return indexSymbolStyle(stringSymbolStyle(pSymbolStyle));
}

//==============================================================================

QString stringSymbolStyle(int pIndexSymbolStyle, bool pFormatted)
{
    // Return the given index symbol style as a string symbol style
    // Note: if the given index symbol style is invalid then we return the
    //       string symbol style for a "none" symbol...

    QStringList res = pFormatted?formattedSymbolStyles():symbolStyles();

    return res[(   (pIndexSymbolStyle >= 0)
                && (pIndexSymbolStyle < res.count()))?
                   pIndexSymbolStyle:
                   0];
}

//==============================================================================

QString stringSymbolStyle(QwtSymbol::Style pSymbolStyle, bool pFormatted)
{
    // Return the given symbol style as a string symbol style
    // Note: if the given symbol style is invalid then we return the string
    //       symbol style for a "none" symbol...

    QStringList res = pFormatted?formattedSymbolStyles():symbolStyles();

    return res[int((pSymbolStyle <= QwtSymbol::DTriangle)?
                   pSymbolStyle+1:
                   ((pSymbolStyle >= QwtSymbol::LTriangle) && (pSymbolStyle <= QwtSymbol::Star1))?
                       pSymbolStyle:
                       QwtSymbol::NoSymbol)];
}

//==============================================================================

QwtSymbol::Style symbolStyle(int pIndexSymbolStyle)
{
    // Return the given index symbol style as a string symbol style
    // Note #1: if the given index symbol style is invalid then we return the
    //          string symbol style for a "none" symbol...
    // Note #2: the shifting is because indices in our list of symbols don't
    //          match those of QwtSymbol::Style...

    return QwtSymbol::Style(   (   (pIndexSymbolStyle >= 0)
                            && (pIndexSymbolStyle < symbolStyles().count()))?
                                (pIndexSymbolStyle <= 5)?
                                    pIndexSymbolStyle-1:
                                    pIndexSymbolStyle:
                                -1);
}

//==============================================================================

QwtSymbol::Style symbolStyle(const QString &pStringSymbolStyle)
{
    // Return the given string symbol style as a symbol style

    return symbolStyle(indexSymbolStyle(pStringSymbolStyle));
}

//==============================================================================

} // namespace SEDMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
