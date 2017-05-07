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
// PMR exposure
//==============================================================================

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWorkspace;
class PmrWebService;

//==============================================================================

class PMRSUPPORT_EXPORT PmrExposure : public QObject
{
    Q_OBJECT

public:
    explicit PmrExposure(const QString &pUrl, const QString &pName,
                         PmrWebService *pParent);

    static bool compare(PmrExposure *pExposure1, PmrExposure *pExposure2);

    QString url() const;
    QString name() const;

    PmrWorkspace * workspace() const;
    void setWorkspace(PmrWorkspace *pWorkspace);

    QStringList exposureFiles() const;
    void addExposureFile(const QString &pFileName);

    QString toHtml() const;

private:
    QString mUrl;
    QString mName;

    PmrWorkspace *mWorkspace;

    QStringList mExposureFiles;
};

//==============================================================================

typedef QList<PmrExposure *> PmrExposures;

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
