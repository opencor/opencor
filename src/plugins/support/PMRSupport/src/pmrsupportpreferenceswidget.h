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
// PMR support preferences widget
//==============================================================================

#pragma once

//==============================================================================

#include "preferencesinterface.h"

//==============================================================================

namespace Ui {
    class PmrSupportPreferencesWidget;
} // namespace Ui

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

static const auto SettingsPreferencesPmrUrl = QStringLiteral("PmrUrl");
static const auto SettingsPreferencesName   = QStringLiteral("Name");
static const auto SettingsPreferencesEmail  = QStringLiteral("Email");

//==============================================================================

static const auto SettingsPreferencesPmrUrlDefault = QStringLiteral("https://models.physiomeproject.org");
static const auto SettingsPreferencesNameDefault   = QString("");
static const auto SettingsPreferencesEmailDefault  = QString("");

//==============================================================================

class PmrSupportPreferencesWidget : public Preferences::PreferencesWidget
{
    Q_OBJECT

public:
    explicit PmrSupportPreferencesWidget(QWidget *pParent);
    ~PmrSupportPreferencesWidget() override;

    bool preferencesChanged() const override;

    void resetPreferences() override;
    void savePreferences() override;

private:
    Ui::PmrSupportPreferencesWidget *mGui;

    QString mPmrUrl;
    QString mName;
    QString mEmail;

private slots:
    void pmrUrlValueCurrentTextChanged(const QString &pCurrentText);
    void noteValueLinkActivated(const QString &pLink);
};

//==============================================================================

} // namespace PMRSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
