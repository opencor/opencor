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
// Check for updates dialog
//==============================================================================

#include "checkforupdatesdialog.h"
#include "cliutils.h"
#include "mainwindow.h"

//==============================================================================

#include "ui_checkforupdatesdialog.h"

//==============================================================================

#include <QCheckBox>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QSettings>
#include <QTextEdit>
#include <QVariantMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

CheckForUpdatesEngine::CheckForUpdatesEngine(const QString &pApplicationDate) :
    mApplicationDate(pApplicationDate)
{
}

//==============================================================================

void CheckForUpdatesEngine::check()
{
    // Retrieve some information about the different versions of OpenCOR that
    // are available

    QByteArray fileVersionsContents;
    QString errorMessage;

    mNewerVersions.clear();

    if (readFile("https://opencor.ws/downloads/index.js", fileVersionsContents, &errorMessage)) {
        QJsonParseError jsonParseError;
        QJsonDocument versions = QJsonDocument::fromJson(fileVersionsContents.mid(15, fileVersionsContents.length()-17), &jsonParseError);

        if (jsonParseError.error == QJsonParseError::NoError) {
            QVariantMap versionMap;
            int versionMajor;
            int versionMinor;
            int versionPatch;
            int versionDay;
            int versionMonth;
            int versionYear;
            QString versionVersion;
            QString versionDate;

            for (const auto &version : versions.object().toVariantMap()["versions"].toList()) {
                // Retrieve the version and date of the current version

                versionMap = version.toMap();

                versionMajor = versionMap["major"].toInt();
                versionMinor = versionMap["minor"].toInt();
                versionPatch = versionMap["patch"].toInt();

                versionDay = versionMap["day"].toInt();
                versionMonth = versionMap["month"].toInt();
                versionYear = versionMap["year"].toInt();

                versionDate = QDate(versionYear, versionMonth, versionDay).toString("yyyy-MM-dd");

                if ((versionMajor == 0) && (versionMinor == 0) && (versionPatch == 0)) {
                    versionVersion = versionDate;
                } else {
                    versionVersion = QString("%1.%2").arg(versionMajor)
                                                     .arg(versionMinor);

                    if (versionPatch != 0) {
                        versionVersion = QString("%1.%2").arg(versionVersion)
                                                         .arg(versionPatch);
                    }
                }

                // Check whether the version is newer and, if so, add it to our
                // list

                if (mApplicationDate < versionDate) {
                    mNewerVersions << versionVersion;
                }
            }

            mStatus = QString();
        } else {
            mStatus =  tr("The version information is mal-formatted.")
                      +"<br/>"
                      +tr(R"(Please <a href="https://opencor.ws/user/contactUs.html">contact us</a> about this error.)");
        }
    } else {
        mStatus = formatMessage(errorMessage, false, true);
    }
}

//==============================================================================

QString CheckForUpdatesEngine::status() const
{
    // Return our status

    return mStatus;
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

    return !mNewerVersions.isEmpty();
}

//==============================================================================

bool CheckForUpdatesEngine::hasNewerOfficialVersion() const
{
    // Return whether we have a newer official version

    for (const auto &newerVersion : mNewerVersions) {
        if (!newerVersion.contains('-')) {
            return true;
        }
    }

    return false;
}

//==============================================================================

CheckForUpdatesDialog::CheckForUpdatesDialog(const QString &pApplicationDate,
                                             CheckForUpdatesEngine *pEngine,
                                             QWidget *pParent) :
    Dialog(pParent)
{
    // We are not yet initialised

    mInitialized = false;

    // Customise our settings

    mSettings.beginGroup(SettingsCheckForUpdatesDialog);

    // Set up the GUI

    mGui = new Ui::CheckForUpdatesDialog;

    mGui->setupUi(this);

    connect(mGui->statusLabel, &QLabel::linkActivated,
            this, &CheckForUpdatesDialog::statusLabelLinkActivated);
    connect(mGui->recheckButton, &QPushButton::clicked,
            this, &CheckForUpdatesDialog::recheckButtonClicked);
    connect(mGui->checkForUpdatesAtStartupCheckBox, &QCheckBox::toggled,
            this, &CheckForUpdatesDialog::checkForUpdatesAtStartupCheckBoxToggled);
    connect(mGui->includeSnapshotsCheckBox, &QCheckBox::toggled,
            this, &CheckForUpdatesDialog::includeSnapshotsCheckBoxToggled);
    connect(mGui->buttonBox, &QDialogButtonBox::accepted,
            this, &CheckForUpdatesDialog::accept);

    // Create/set our engine and check for updates in the former case

    if (pEngine != nullptr) {
        mEngine = pEngine;
    } else {
        mEngine = new CheckForUpdatesEngine(pApplicationDate);

        mEngine->check();
    }

    // Retrieve and set some properties

    mGui->checkForUpdatesAtStartupCheckBox->setChecked(mSettings.value(SettingsCheckForUpdatesAtStartup, true).toBool());
    mGui->includeSnapshotsCheckBox->setChecked(mSettings.value(SettingsIncludeSnapshots, false).toBool());

    // Update our GUI

    updateGui();

    mInitialized = true;
}

//==============================================================================

CheckForUpdatesDialog::CheckForUpdatesDialog(const QString &pApplicationDate,
                                             QWidget *pParent) :
    CheckForUpdatesDialog(pApplicationDate, nullptr, pParent)
{
}

//==============================================================================

CheckForUpdatesDialog::CheckForUpdatesDialog(CheckForUpdatesEngine *pEngine) :
    CheckForUpdatesDialog({}, pEngine, nullptr)
{
}

//==============================================================================

CheckForUpdatesDialog::~CheckForUpdatesDialog()
{
    // Delete some internal objects

    delete mEngine;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CheckForUpdatesDialog::updateGui()
{
    // Determine the status of our check

    static const QString WhatIsNewUrl = "https://opencor.ws/user/whatIsNew.html?#";

    QString versionInformation = tr(R"(<a href="%1">%2 %3</a> is ready for you to <a href="https://opencor.ws/downloads/">download</a>.)");
    QString snapshotInformation = tr(R"(The <a href="%1">latest snapshot</a> (%2) is ready for you to <a href="https://opencor.ws/downloads/">download</a>.)");

    if (mEngine->status().isEmpty()) {
        if (mGui->includeSnapshotsCheckBox->checkState() == Qt::Checked) {
            if (mEngine->hasNewerVersion()) {
                // The user is after either an official version or a snapshot of
                // OpenCOR, so first look for a snapshot and go for an official
                // version, if no snapshot is available

                QString version;

                for (const auto &newerVersion : mEngine->newerVersions()) {
                    if (newerVersion.contains('-')) {
                        version = newerVersion;

                        break;
                    }
                }

                if (version.isEmpty()) {
                    version = mEngine->newerVersions().first();
                }

                if (version.contains('-')) {
                    mGui->statusLabel->setText(snapshotInformation.arg(WhatIsNewUrl+"latest",
                                                                       version));
                } else {
                    mGui->statusLabel->setText(versionInformation.arg(WhatIsNewUrl+version,
                                                                      qAppName(),
                                                                      version));
                }
            } else {
                mGui->statusLabel->setText(tr("No newer version or snapshot of %1 is available.").arg(qAppName()));
            }
        } else if (mEngine->hasNewerOfficialVersion()) {
            // The user is only after an official version of OpenCOR, so look
            // for the first one available

            QString version;

            for (const auto &newerVersion : mEngine->newerVersions()) {
                if (!newerVersion.contains('-')) {
                    version = newerVersion;

                    break;
                }
            }

            mGui->statusLabel->setText(versionInformation.arg(WhatIsNewUrl+version,
                                                              qAppName(),
                                                              version));
        } else {
            mGui->statusLabel->setText(tr("No newer version of %1 is available.").arg(qAppName()));
        }
    } else {
        mGui->statusLabel->setText(mEngine->status());
    }
}

//==============================================================================

void CheckForUpdatesDialog::statusLabelLinkActivated(const QString &pLink)
{
    // Open the link in the user's browser

    QDesktopServices::openUrl(pLink);
}

//==============================================================================

void CheckForUpdatesDialog::recheckButtonClicked()
{
    // Recheck for updates and then update our GUI accordingly

    mEngine->check();

    updateGui();
}

//==============================================================================

void CheckForUpdatesDialog::checkForUpdatesAtStartupCheckBoxToggled(bool pChecked)
{
    Q_UNUSED(pChecked)

    // Keep track of our property

    mSettings.setValue(SettingsCheckForUpdatesAtStartup,
                       mGui->checkForUpdatesAtStartupCheckBox->isChecked());
}

//==============================================================================

void CheckForUpdatesDialog::includeSnapshotsCheckBoxToggled(bool pChecked)
{
    Q_UNUSED(pChecked)

    // Keep track of our property

    mSettings.setValue(SettingsIncludeSnapshots,
                       mGui->includeSnapshotsCheckBox->isChecked());

    updateGui();
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
