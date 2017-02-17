/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Check for updates dialog
//==============================================================================

#pragma once

//==============================================================================

#include "guiutils.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace Ui {
    class CheckForUpdatesDialog;
}

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsCheckForUpdatesDialog    = QStringLiteral("CheckForUpdatesDialog");
static const auto SettingsCheckForUpdatesAtStartup = QStringLiteral("CheckForUpdatesAtStartup");
static const auto SettingsIncludeSnapshots         = QStringLiteral("IncludeSnapshots");

//==============================================================================

class CheckForUpdatesEngine
{
public:
    explicit CheckForUpdatesEngine(const QString &pApplicationDate);

    void check();

    QString status() const;

    QStringList newerVersions() const;

    bool hasNewerVersion() const;
    bool hasNewerOfficialVersion() const;

private:
    QString mApplicationDate;

    QString mStatus;

    QStringList mNewerVersions;
};

//==============================================================================

class CheckForUpdatesDialog : public Dialog
{
    Q_OBJECT

public:
    explicit CheckForUpdatesDialog(QSettings *pSettings,
                                   const QString &pApplicationDate,
                                   QWidget *pParent);
    explicit CheckForUpdatesDialog(QSettings *pSettings,
                                   CheckForUpdatesEngine *pEngine);
    ~CheckForUpdatesDialog();

private:
    Ui::CheckForUpdatesDialog *mGui;

    CheckForUpdatesEngine *mEngine;

    bool mInitialized;

    void constructor(const QString &pApplicationDate,
                     CheckForUpdatesEngine *pEngine);

    void updateGui();

private slots:
    void on_buttonBox_accepted();
    void on_statusLabel_linkActivated(const QString &pLink);
    void on_recheckButton_clicked();
    void on_checkForUpdatesAtStartupCheckBox_toggled(bool pChecked);
    void on_includeSnapshotsCheckBox_toggled(bool pChecked);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
