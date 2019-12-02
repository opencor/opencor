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
// Simulation manager
//==============================================================================

#pragma once

//==============================================================================

#include "simulationsupportglobal.h"

//==============================================================================

#include <QMap>
#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace SimulationSupport {

//==============================================================================

class Simulation;

//==============================================================================

class SIMULATIONSUPPORT_EXPORT SimulationManager : public QObject
{
    Q_OBJECT

public:
    static SimulationManager * instance();

    void manage(const QString &pFileName);
    void unmanage(const QString &pFileName);

    void save(const QString &pFileName);
    void reload(const QString &pFileName);
    void rename(const QString &pOldFileName, const QString &pNewFileName);

    Simulation * simulation(const QString &pFileName) const;

private:
    QMap<QString, Simulation *> mSimulations;
};

//==============================================================================

} // namespace SimulationSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
