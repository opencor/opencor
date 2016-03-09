/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// GUI application
//==============================================================================

#include "guiapplication.h"

//==============================================================================

#include <QFileOpenEvent>

//==============================================================================

namespace OpenCOR {

//==============================================================================

GuiApplication::GuiApplication(const QString &pId, int &pArgC, char **pArgV) :
    QtSingleApplication(pId, pArgC, pArgV),
    mCanEmitFileOpenRequestSignal(false),
    mFileNamesOrOpencorUrls(QStringList())
{
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
