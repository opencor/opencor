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
#include <QLabel>
#include <QSettings>
#include <QTextEdit>
#include <QVariantMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CheckForUpdatesEngine::CheckForUpdatesEngine(const QString &pApplicationVersion,
                                             const QString &pApplicationDate) :
    mApplicationVersion(pApplicationVersion),
    mApplicationDate(pApplicationDate),
    mStatus(QString()),
    mVersions(QJsonDocument()),
    mWhatIsNew(QJsonDocument()),
    mNewerVersions(QStringList())
{
}

//==============================================================================

void CheckForUpdatesEngine::check()
{
    // Retrieve some information about the different versions of OpenCOR that
    // are available

    QString fileContents = QString();
    QString errorMessage = QString();

    mStatus = QString();
    mVersions = QJsonDocument();

    mNewerVersions.clear();

    if (OpenCOR::readTextFromUrl("http://www.opencor.ws/downloads/index.js", fileContents, &errorMessage)) {
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
    } else {
        mStatus = QObject::tr("<strong>Error:</strong>")+" "+formatErrorMessage(errorMessage, true, true);
    }
}

//==============================================================================

QString CheckForUpdatesEngine::applicationVersion() const
{
    // Return our application version

    return mApplicationVersion;
}

//==============================================================================

QString CheckForUpdatesEngine::applicationDate() const
{
    // Return our application date

    return mApplicationDate;
}

//==============================================================================

QString CheckForUpdatesEngine::status() const
{
    // Return our status

    return mStatus;
}

//==============================================================================

QJsonDocument CheckForUpdatesEngine::versions() const
{
    // Return the JSON document for our versions

    return mVersions;
}

//==============================================================================

QJsonDocument CheckForUpdatesEngine::whatIsNew() const
{
    // Return the JSON document for our what is new

    return mWhatIsNew;
}

//==============================================================================

QStringList CheckForUpdatesEngine::newerVersions() const
{
    // Return our newer versions

    return mNewerVersions;
}

//==============================================================================

bool CheckForUpdatesEngine::hasNewerVersion() const
{
    // Return whether we have a newer version

    return mNewerVersions.size();
}

//==============================================================================

bool CheckForUpdatesEngine::hasNewerOfficialVersion() const
{
    // Return whether we have a newer official version

    foreach (const QString &newerVersion, mNewerVersions)
        if (newerVersion.compare("latest"))
            return true;

    return false;
}

//==============================================================================

void CheckForUpdatesWindow::constructor(const QString &pApplicationVersion,
                                        const QString &pApplicationDate,
                                        CheckForUpdatesEngine *pEngine)
{
    // Set up the GUI

    mGui = new Ui::CheckForUpdatesWindow;

    mGui->setupUi(this);

    // Create and initialise our engine, if needed, and update our GUI

    if (pEngine) {
        mEngine = pEngine;
    } else {
        mEngine = new CheckForUpdatesEngine(pApplicationVersion, pApplicationDate);

        mEngine->check();
    }

    updateGui();
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

void CheckForUpdatesWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve and set some properties

    mGui->checkForUpdatesAtStartupCheckBox->setChecked(pSettings->value(SettingsCheckForUpdatesAtStartup, true).toBool());
    mGui->includeSnapshotsCheckBox->setChecked(pSettings->value(SettingsIncludeSnapshots, false).toBool());
}

//==============================================================================

void CheckForUpdatesWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of some properties

    pSettings->setValue(SettingsCheckForUpdatesAtStartup,
                        mGui->checkForUpdatesAtStartupCheckBox->isChecked());
    pSettings->setValue(SettingsIncludeSnapshots,
                        mGui->includeSnapshotsCheckBox->isChecked());
}

//==============================================================================

void CheckForUpdatesWindow::updateGui()
{
    // Determine the status of our check

    if (mEngine->status().isEmpty()) {
        mGui->statusLabel->setText(QString());
    } else {
        mGui->statusLabel->setText(mEngine->status());
    }
}

//==============================================================================

void CheckForUpdatesWindow::on_recheckButton_clicked()
{
    // Recheck for updates and then update our GUI accordingly

    mEngine->check();

    updateGui();
}

//==============================================================================

void CheckForUpdatesWindow::on_checkForUpdatesAtStartupCheckBox_clicked()
{
}

//==============================================================================

void CheckForUpdatesWindow::on_includeSnapshotsCheckBox_clicked()
{
    // Update the GUI

    updateGui();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
