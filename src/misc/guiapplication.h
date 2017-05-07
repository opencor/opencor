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
// GUI application
//==============================================================================

#pragma once

//==============================================================================

#include <QtSingleApplication>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class GuiApplication : public QtSingleApplication
{
    Q_OBJECT

public:
    GuiApplication(const QString &pId, int &pArgC, char **pArgV);

    bool hasFileNamesOrOpencorUrls() const;
    QString firstFileNameOrOpencorUrl();

    void updateCanEmitFileOpenRequestSignal();

protected:
    virtual bool event(QEvent *pEvent);

private:
    bool mCanEmitFileOpenRequestSignal;

    QStringList mFileNamesOrOpencorUrls;

signals:
    void fileOpenRequest(const QString &pFileName);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
