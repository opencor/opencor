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

#include "pmrsupportpreferenceswidget.h"

//==============================================================================

#include "ui_pmrsupportpreferenceswidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrSupportPreferencesWidget::PmrSupportPreferencesWidget(QWidget *pParent) :
    Preferences::PreferencesWidget(PluginName, pParent),
    mGui(new Ui::PmrSupportPreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->nameValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->emailValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    mPmrUrl = mSettings->value(SettingsPreferencesPmrUrl, SettingsPreferencesPmrUrlDefault).toString();
    mName = mSettings->value(SettingsPreferencesName).toString();
    mEmail = mSettings->value(SettingsPreferencesEmail).toString();

    mGui->pmrUrlValue->setCurrentText(mPmrUrl);
    mGui->nameValue->setText(mName);
    mGui->emailValue->setText(mEmail);

    setFocusProxy(mGui->pmrUrlValue);
}

//==============================================================================

PmrSupportPreferencesWidget::~PmrSupportPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

bool PmrSupportPreferencesWidget::preferencesChanged() const
{
    // Return whether our preferences have changed

    return    mGui->pmrUrlValue->currentText().compare(mPmrUrl)
           || mGui->nameValue->text().compare(mName)
           || mGui->emailValue->text().compare(mEmail);
}

//==============================================================================

void PmrSupportPreferencesWidget::resetPreferences()
{
    // Reset our preferences

    mPmrUrl = mGui->pmrUrlValue->itemText(0);
    mName = QString();
    mEmail = QString();

    mGui->pmrUrlValue->setCurrentText(mPmrUrl);
    mGui->nameValue->setText(mName);
    mGui->emailValue->setText(mEmail);
}

//==============================================================================

void PmrSupportPreferencesWidget::savePreferences()
{
    // Save our preferences

    mPmrUrl = mGui->pmrUrlValue->currentText();
    mName = mGui->nameValue->text();
    mEmail = mGui->emailValue->text();

    mSettings->setValue(SettingsPreferencesPmrUrl, mPmrUrl);
    mSettings->setValue(SettingsPreferencesName, mName);
    mSettings->setValue(SettingsPreferencesEmail, mEmail);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
