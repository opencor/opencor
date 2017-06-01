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
// CLI utilities
//==============================================================================

#pragma once

//==============================================================================

#include "coreglobal.h"
#include "plugininfo.h"

//==============================================================================

#include <QSslError>
#include <QString>

//==============================================================================

class QCoreApplication;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {

//==============================================================================

#include "corecliutils.h.inl"

//==============================================================================

static const auto HomePageUrl = QStringLiteral("http://www.opencor.ws/");

//==============================================================================
// Note: both cliutils.h and corecliutils.h must specifically define
//       SynchronousFileDownloader. To have it in corecliutils.h.inl is NOT good
//       enough since the MOC won't pick it up...

class SynchronousFileDownloader : public QObject
{
    Q_OBJECT

public:
    bool download(const QString &pUrl, QByteArray &pContents,
                  QString *pErrorMessage) const;

private slots:
    void networkAccessManagerSslErrors(QNetworkReply *pNetworkReply,
                                       const QList<QSslError> &pSslErrors);
};

//==============================================================================

void initQtMessagePattern();

void initPluginsPath(const QString &pAppFileName);

void initApplication(QString *pAppDate = 0);

QString applicationDescription(const bool &pGuiMode = true);

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
