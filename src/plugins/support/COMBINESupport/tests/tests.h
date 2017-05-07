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
// COMBINE support tests
//==============================================================================

#pragma once

//==============================================================================

#include "combinearchiveissue.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {
    class CombineArchive;
}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================

class Tests : public QObject
{
    Q_OBJECT

private:
    OpenCOR::COMBINESupport::CombineArchive *mCombineArchive;
    OpenCOR::COMBINESupport::CombineArchiveIssues mIssues;

    void doBasicTests(const QString &pFileName = QString());

private slots:
    void initTestCase();
    void cleanupTestCase();

    void basicTests();
    void loadingErrorTests();
};

//==============================================================================
// End of file
//==============================================================================
