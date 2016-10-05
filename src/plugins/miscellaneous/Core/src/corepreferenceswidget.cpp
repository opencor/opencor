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
// Core preferences widget
//==============================================================================

#include "corepreferenceswidget.h"

//==============================================================================

#include "ui_corepreferenceswidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CorePreferencesWidget::CorePreferencesWidget(QSettings *pSettings,
                                             QWidget *pParent) :
    Preferences::PreferencesWidget(pSettings, pParent),
    mGui(new Ui::CorePreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

CorePreferencesWidget::~CorePreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CorePreferencesWidget::savePreferences()
{
//---ISSUE193--- TO BE DONE...
qDebug("[%s]", qPrintable(mSettings->group()));
int value = mSettings->value("TEST").toInt();
qDebug("  OLD: %d", value);
mSettings->setValue("TEST", ++value);
qDebug("  NEW: %d", mSettings->value("TEST").toInt());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
