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
// General preferences widget
//==============================================================================

#include "generalpreferenceswidget.h"

//==============================================================================

#include "ui_generalpreferenceswidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SettingsPreferences = QStringLiteral("Preferences");

//==============================================================================

GeneralPreferencesWidget::GeneralPreferencesWidget(PluginManager *pPluginManager,
                                                   QWidget *pParent) :
    QWidget(pParent),
    mGui(new Ui::GeneralPreferencesWidget),
    mPluginManager(pPluginManager)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

GeneralPreferencesWidget::~GeneralPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void GeneralPreferencesWidget::savePreferences()
{
    // Save our preferences

    QSettings settings;

    settings.beginGroup(SettingsPreferences);
//---ISSUE193--- TO BE DONE...
qDebug("[%s]", qPrintable(settings.group()));
int value = settings.value("TEST").toInt();
qDebug("  OLD: %d", value);
settings.setValue("TEST", ++value);
qDebug("  NEW: %d", settings.value("TEST").toInt());
    settings.endGroup();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
