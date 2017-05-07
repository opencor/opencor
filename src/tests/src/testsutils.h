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
// Some useful tests-related functions
//==============================================================================

#pragma once

//==============================================================================

#include <QString>

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

namespace OpenCOR {

//==============================================================================

QString dirName(const QString &pDirName);
QString fileName(const QString &pFilePath);

QByteArray rawFileContents(const QString &pFileName);
QStringList fileContents(const QString &pFileName);

QString fileSha1(const QString &pFileName);

QStringList runCli(const QStringList pArguments);

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
