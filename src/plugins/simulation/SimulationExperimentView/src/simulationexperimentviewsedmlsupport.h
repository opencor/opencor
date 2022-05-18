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

#pragma once

//==============================================================================

#include <QString>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_symbol.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

QString stringLineStyleFromQtPenStyle(Qt::PenStyle pQtPenStyle,
                                      bool pFormatted = false);
Qt::PenStyle qtPenStyleFromIndex(int pIndexLineStyle);
Qt::PenStyle qtPenStyleFromString(const QString &pStringLineStyle);
int indexLineStyleFromQtPenStyle(Qt::PenStyle pQtPenStyle);

QString stringSymbolStyleFromQwtSymbolStyle(QwtSymbol::Style pQwtSymbolStyle,
                                            bool pFormatted = false);
QwtSymbol::Style qwtSymbolStyleFromIndex(int pIndexSymbolStyle);
QwtSymbol::Style qwtSymbolStyleFromString(const QString &pStringSymbolStyle);
int indexSymbolStyleFromQwtSymbolStyle(QwtSymbol::Style pQwtSymbolStyle);

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
