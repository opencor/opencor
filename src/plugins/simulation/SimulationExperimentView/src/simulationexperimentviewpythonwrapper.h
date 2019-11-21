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
// Simulation Experiment view Python wrapper
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

#include "pythonbegin.h"
    #include "PythonQt/PythonQtPythonInclude.h"
#include "pythonend.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewPythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewPythonWrapper(void *pModule,
                                                   QObject *pParent);
};

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

