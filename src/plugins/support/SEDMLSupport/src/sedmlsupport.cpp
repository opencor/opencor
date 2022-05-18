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

    static const QStringList LineStyles = { "none", "solid", "dash", "dot",
                                            "dashdot", "dashdotdot" };

    return LineStyles;
}

//==============================================================================

QStringList formattedLineStyles()
{
    // Return our list of formatted line styles

    static const QStringList FormattedLineStyles = { QObject::tr("None"),
                                                     QObject::tr("Solid"),
                                                     QObject::tr("Dash"),
                                                     QObject::tr("Dot"),
                                                     QObject::tr("DashDot"),
                                                     QObject::tr("DashDotDot") };

    return FormattedLineStyles;
}

//==============================================================================

int indexLineStyleFromString(const QString &pStringLineStyle)
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

QString stringLineStyleFromIndex(int pIndexLineStyle, bool pFormatted)
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

QStringList symbolStyles()
{
    // Return our list of symbol styles

    static const QStringList SymbolStyles = { "none", "circle", "square",
                                              "diamond", "triangle",
                                              "downTriangle", "leftTriangle",
                                              "rightTriangle", "cross",
                                              "xCross", "horizontalLine",
                                              "verticalLine", "star" };

    return SymbolStyles;
}

//==============================================================================

QStringList formattedSymbolStyles()
{
    // Return our list of formatted symbol styles

    static const QStringList FormattedSymbolStyles = { QObject::tr("None"),
                                                       QObject::tr("Circle"),
                                                       QObject::tr("Square"),
                                                       QObject::tr("Diamond"),
                                                       QObject::tr("Triangle"),
                                                       QObject::tr("Down Triangle"),
                                                       QObject::tr("Left Triangle"),
                                                       QObject::tr("Right Triangle"),
                                                       QObject::tr("Cross"),
                                                       QObject::tr("X Cross"),
                                                       QObject::tr("Horizontal Line"),
                                                       QObject::tr("Vertical Line"),
                                                       QObject::tr("Star") };

    return FormattedSymbolStyles;
}

//==============================================================================

int indexSymbolStyleFromString(const QString &pStringSymbolStyle)
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

QString stringSymbolStyleFromIndex(int pIndexSymbolStyle, bool pFormatted)
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

} // namespace SEDMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
