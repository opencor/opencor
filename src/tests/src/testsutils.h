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
// Some useful tests-related functions
//==============================================================================

#pragma once

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {

//==============================================================================

QString targetPlatformDir();

QString dirName(const QString &pDirName = QString());
QString fileName(const QString &pFileName);

QByteArray rawFileContents(const QString &pFileName);
QStringList fileContents(const QString &pFileName);

QString fileSha1(const QString &pFileName);

int runCli(const QStringList &pArguments, QStringList &pOutput);

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
