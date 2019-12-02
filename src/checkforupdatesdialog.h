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

#pragma once

//==============================================================================

#include "guiutils.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace Ui {
    class CheckForUpdatesDialog;
} // namespace Ui

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsCheckForUpdatesDialog    = QStringLiteral("CheckForUpdatesDialog");
static const auto SettingsCheckForUpdatesAtStartup = QStringLiteral("CheckForUpdatesAtStartup");
static const auto SettingsIncludeSnapshots         = QStringLiteral("IncludeSnapshots");

//==============================================================================

class CheckForUpdatesEngine : public QObject
{
    Q_OBJECT

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
    explicit CheckForUpdatesDialog(const QString &pApplicationDate,
                                   QWidget *pParent);
    explicit CheckForUpdatesDialog(CheckForUpdatesEngine *pEngine);
    ~CheckForUpdatesDialog() override;

private:
    Ui::CheckForUpdatesDialog *mGui;

    CheckForUpdatesEngine *mEngine;

    bool mInitialized;

    explicit CheckForUpdatesDialog(const QString &pApplicationDate,
                                   CheckForUpdatesEngine *pEngine,
                                   QWidget *pParent);

    void updateGui();

private slots:
    void statusLabelLinkActivated(const QString &pLink);
    void recheckButtonClicked();
    void checkForUpdatesAtStartupCheckBoxToggled(bool pChecked);
    void includeSnapshotsCheckBoxToggled(bool pChecked);
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
