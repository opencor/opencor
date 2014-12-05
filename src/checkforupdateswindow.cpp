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
// Check for updates window
//==============================================================================

#include "checkforupdateswindow.h"
#include "cliutils.h"
#include "mainwindow.h"

//==============================================================================

#include "ui_checkforupdateswindow.h"

//==============================================================================

#include <QJsonObject>
#include <QTextEdit>
#include <QVariantMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CheckForUpdatesEngine::CheckForUpdatesEngine(const QString &pApplicationVersion,
                                             const QString &pApplicationDate) :
    mApplicationVersion(pApplicationVersion),
    mApplicationDate(pApplicationDate),
    mVersions(QJsonDocument()),
    mWhatIsNew(QJsonDocument()),
    mNewerVersions(QStringList())
{
}

//==============================================================================

bool CheckForUpdatesEngine::check()
{
    // Retrieve some information about the different versions of OpenCOR that
    // are available

    QString fileContents = QString();

    bool res = OpenCOR::readTextFromUrl("http://www.opencor.ws/downloads/index.js", fileContents);

    // Determine which versions are newer than the current version of OpenCOR

    mVersions = QJsonDocument();

    mNewerVersions.clear();

    if (res) {
        mVersions = QJsonDocument::fromJson(QString(fileContents.mid(15, fileContents.length()-17)).toUtf8());

        QVariantMap versionsMap;
        int major, minor, patch;
        int day, month, year;
        QString version;
        QString date;

        foreach (const QVariant &versions, mVersions.object().toVariantMap()["versions"].toList()) {
            // Retrieve the version and date of the current version

            versionsMap = versions.toMap();

            major = versionsMap["major"].toInt();
            minor = versionsMap["minor"].toInt();
            patch = versionsMap["patch"].toInt();

            day   = versionsMap["day"].toInt();
            month = versionsMap["month"].toInt();
            year  = versionsMap["year"].toInt();

            date = QString("%1-%2-%3").arg(year)
                                      .arg(month, 2, 10, QChar('0'))
                                      .arg(day, 2, 10, QChar('0'));

            if (!major && !minor && !patch) {
                version = date;
            } else {
                version = QString("%1.%2").arg(major).arg(minor);

                if (patch)
                    version = QString("%1.%2").arg(version).arg(patch);
            }

            // Check whether the version is newer and, if so, add it to our list

            if (mApplicationDate.compare(date) < 0) {
                if (!version.compare(date))
                    // We are dealing with the latest snapshot

                    mNewerVersions << "latest";
                else
                    // We are dealing with an official version of OpenCOR

                    mNewerVersions << version;
            }
        }
    }

    return res;
}

//==============================================================================

bool CheckForUpdatesEngine::updateAvailable(const bool &pDoCheck)
{
    // Return whether one or several updates are available, after checking if
    // required

    if (pDoCheck)
        check();

    return !mNewerVersions.isEmpty();
}

//==============================================================================

void CheckForUpdatesWindow::constructor(const QString &pApplicationVersion,
                                        const QString &pApplicationDate,
                                        CheckForUpdatesEngine *pEngine)
{
    // Set up the GUI

    mGui = new Ui::CheckForUpdatesWindow;

    mGui->setupUi(this);

    // Create and initialise our engine, if needed

    if (pEngine) {
        mEngine = pEngine;
    } else {
        mEngine = new CheckForUpdatesEngine(pApplicationVersion, pApplicationDate);

        if (!mEngine->check())
            mGui->textEdit->setText("Problem with the Internet connection?...");
    }

    if (mGui->textEdit->toPlainText().isEmpty())
        mGui->textEdit->setText(mEngine->updateAvailable()?"An update is available...":"No update is available...");
}

//==============================================================================

CheckForUpdatesWindow::CheckForUpdatesWindow(const QString &pApplicationVersion,
                                             const QString &pApplicationDate,
                                             MainWindow *pMainWindow) :
    QDialog(pMainWindow)
{
    // Construct our window

    constructor(pApplicationVersion, pApplicationDate, 0);
}

//==============================================================================

CheckForUpdatesWindow::CheckForUpdatesWindow(CheckForUpdatesEngine *pEngine,
                                             MainWindow *pMainWindow) :
    QDialog(pMainWindow)
{
    // Construct our window

    constructor(QString(), QString(), pEngine);
}

//==============================================================================

CheckForUpdatesWindow::~CheckForUpdatesWindow()
{
    // Delete the GUI

    delete mGui;

    // Delete some internal objects

    delete mEngine;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
