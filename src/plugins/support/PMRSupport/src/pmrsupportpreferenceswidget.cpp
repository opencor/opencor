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

    mGui->nameValue->setText(mSettings->value(SettingsPreferencesName).toString());
    mGui->emailValue->setText(mSettings->value(SettingsPreferencesEmail).toString());

    setFocusProxy(mGui->nameValue);
}

//==============================================================================

PmrSupportPreferencesWidget::~PmrSupportPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrSupportPreferencesWidget::resetPreferences()
{
    // Reset our preferences

    mGui->nameValue->setText(QString());
    mGui->emailValue->setText(QString());
}

//==============================================================================

void PmrSupportPreferencesWidget::savePreferences()
{
    // Save our preferences

    mSettings->setValue(SettingsPreferencesName, mGui->nameValue->text());
    mSettings->setValue(SettingsPreferencesEmail, mGui->emailValue->text());
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
