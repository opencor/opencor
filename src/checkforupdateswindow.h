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

#ifndef CHECKFORUPDATESWINDOW_H
#define CHECKFORUPDATESWINDOW_H

//==============================================================================

#include <QDialog>
#include <QString>

//==============================================================================

namespace Ui {
    class CheckForUpdatesWindow;
}

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsCheckForUpdatesAtStartup = QStringLiteral("CheckForUpdatesAtStartup");
static const auto SettingsIncludeSnapshots         = QStringLiteral("IncludeSnapshots");

//==============================================================================

class MainWindow;

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

class CheckForUpdatesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CheckForUpdatesWindow(const QString &pApplicationDate,
                                   MainWindow *pMainWindow = 0);
    explicit CheckForUpdatesWindow(CheckForUpdatesEngine *pEngine,
                                   MainWindow *pMainWindow = 0);
    ~CheckForUpdatesWindow();

    void loadSettings(QSettings *pSettings);
    void saveSettings(QSettings *pSettings) const;

private:
    Ui::CheckForUpdatesWindow *mGui;

    CheckForUpdatesEngine *mEngine;

    void constructor(const QString &pApplicationDate,
                     CheckForUpdatesEngine *pEngine);

    void updateGui();

private Q_SLOTS:
    void on_buttonBox_accepted();
    void on_statusLabel_linkActivated(const QString &pLink);
    void on_recheckButton_clicked();
    void on_includeSnapshotsCheckBox_toggled(bool pChecked);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
