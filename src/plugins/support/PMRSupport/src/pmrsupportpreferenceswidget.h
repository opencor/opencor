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
// PMR support preferences widget
//==============================================================================

#pragma once

//==============================================================================

#include "preferencesinterface.h"

//==============================================================================

namespace Ui {
    class PmrSupportPreferencesWidget;
}

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

static const auto PluginName = QStringLiteral("PMRSupport");

//==============================================================================

static const auto SettingsPreferencesPmrUrl = QStringLiteral("PmrUrl");
static const auto SettingsPreferencesName   = QStringLiteral("Name");
static const auto SettingsPreferencesEmail  = QStringLiteral("Email");

//==============================================================================

static const auto SettingsPreferencesPmrUrlDefault = QStringLiteral("https://models.physiomeproject.org");

//==============================================================================

class PmrSupportPreferencesWidget : public Preferences::PreferencesWidget
{
    Q_OBJECT

public:
    explicit PmrSupportPreferencesWidget(QWidget *pParent);
    ~PmrSupportPreferencesWidget();

    virtual bool preferencesChanged() const;

    virtual void resetPreferences();
    virtual void savePreferences();

private:
    Ui::PmrSupportPreferencesWidget *mGui;

    QString mPmrUrl;
    QString mName;
    QString mEmail;

private slots:
    void on_pmrUrlValue_currentTextChanged(const QString &pCurrentText);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
