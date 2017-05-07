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

#include "guiapplication.h"

//==============================================================================

#include <QFileOpenEvent>
#include <QLoggingCategory>

//==============================================================================

namespace OpenCOR {

//==============================================================================

GuiApplication::GuiApplication(const QString &pId, int &pArgC, char **pArgV) :
    QtSingleApplication(pId, pArgC, pArgV),
    mCanEmitFileOpenRequestSignal(false),
    mFileNamesOrOpencorUrls(QStringList())
{
    // Filter out OpenSSL warning messages

    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
}

//==============================================================================

bool GuiApplication::hasFileNamesOrOpencorUrls() const
{
    // Return whether we have some file name or OpenCOR URLs

    return mFileNamesOrOpencorUrls.count();
}

//==============================================================================

QString GuiApplication::firstFileNameOrOpencorUrl()
{
    // Retrieve, remove and return our first file name or OpenCOR URL, if any

    if (mFileNamesOrOpencorUrls.isEmpty()) {
        return QString();
    } else {
        QString res = mFileNamesOrOpencorUrls.first();

        mFileNamesOrOpencorUrls.removeFirst();

        return res;
    }
}

//==============================================================================

void GuiApplication::updateCanEmitFileOpenRequestSignal()
{
    // Now allow to emit the fileOpenRequest() signal

    mCanEmitFileOpenRequestSignal = true;
}

//==============================================================================

bool GuiApplication::event(QEvent *pEvent)
{
    // Check whether we have been asked to open one or several files

    if (pEvent->type() == QEvent::FileOpen) {
        QFileOpenEvent *fileOpenEvent = static_cast<QFileOpenEvent*>(pEvent);
        QString fileNameOrOpencorUrl = fileOpenEvent->file();

        if (fileNameOrOpencorUrl.isEmpty())
            fileNameOrOpencorUrl = fileOpenEvent->url().toString();

        if (mCanEmitFileOpenRequestSignal)
            emit fileOpenRequest(fileNameOrOpencorUrl);
        else
            mFileNamesOrOpencorUrls << fileNameOrOpencorUrl;

        return true;
    } else {
        return QApplication::event(pEvent);
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
