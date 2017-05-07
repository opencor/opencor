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
// Standard file manager
//==============================================================================

#pragma once

//==============================================================================

#include "standardfile.h"
#include "standardsupportglobal.h"

//==============================================================================

#include <QMap>
#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

typedef QMap<QString, QObject *> Files;

//==============================================================================

class STANDARDSUPPORT_EXPORT StandardFileManager : public QObject
{
    Q_OBJECT

public:
    bool isFile(const QString &pFileName);

    QObject * file(const QString &pFileName);

protected:
    Files mFiles;

    explicit StandardFileManager();
    ~StandardFileManager();

    virtual bool canLoadFile(const QString &pFileName) const = 0;

    virtual QObject * newFile(const QString &pFileName) const = 0;

private slots:
    void manageFile(const QString &pFileName);
    void unmanageFile(const QString &pFileName);

    void reloadFile(const QString &pFileName, const bool &pFileChanged);
    void renameFile(const QString &pOldFileName, const QString &pNewFileName);

    void saveFile(const QString &pFileName);
};

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
