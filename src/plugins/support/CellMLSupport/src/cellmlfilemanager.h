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
// CellML file manager
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "cellmlsupportglobal.h"
#include "standardfilemanager.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

typedef QMap<QString, CellmlFile *> CellmlFiles;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileManager : public StandardSupport::StandardFileManager
{
    Q_OBJECT

public:
    static CellmlFileManager * instance();

    bool isCellmlFile(const QString &pFileName) const;

    CellmlFile * cellmlFile(const QString &pFileName);

protected:
    virtual bool canLoadFile(const QString &pFileName) const;

    virtual QObject * newFile(const QString &pFileName) const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
