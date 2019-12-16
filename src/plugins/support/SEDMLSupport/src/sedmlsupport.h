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

#pragma once

//==============================================================================

#include "sedmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_symbol.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

QStringList SEDMLSUPPORT_EXPORT lineStyles();
QStringList SEDMLSUPPORT_EXPORT formattedLineStyles();

int SEDMLSUPPORT_EXPORT indexLineStyle(const QString &pStringLineStyle);
int SEDMLSUPPORT_EXPORT indexLineStyle(Qt::PenStyle pLineStyle);
QString SEDMLSUPPORT_EXPORT stringLineStyle(int pIndexLineStyle,
                                            bool pFormatted = false);
QString SEDMLSUPPORT_EXPORT stringLineStyle(Qt::PenStyle pLineStyle,
                                            bool pFormatted = false);
Qt::PenStyle SEDMLSUPPORT_EXPORT lineStyle(int pIndexLineStyle);
Qt::PenStyle SEDMLSUPPORT_EXPORT lineStyle(const QString &pStringLineStyle);

QStringList SEDMLSUPPORT_EXPORT symbolStyles();
QStringList SEDMLSUPPORT_EXPORT formattedSymbolStyles();

int SEDMLSUPPORT_EXPORT indexSymbolStyle(const QString &pStringSymbolStyle);
int SEDMLSUPPORT_EXPORT indexSymbolStyle(QwtSymbol::Style pSymbolStyle);
QString SEDMLSUPPORT_EXPORT stringSymbolStyle(int pIndexSymbolStyle,
                                              bool pFormatted = false);
QString SEDMLSUPPORT_EXPORT stringSymbolStyle(QwtSymbol::Style pSymbolStyle,
                                              bool pFormatted = false);
QwtSymbol::Style SEDMLSUPPORT_EXPORT symbolStyle(int pIndexSymbolStyle);
QwtSymbol::Style SEDMLSUPPORT_EXPORT symbolStyle(const QString &pStringSymbolStyle);

//==============================================================================

} // namespace SEDMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
