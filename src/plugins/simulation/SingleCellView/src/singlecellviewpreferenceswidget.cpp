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
// Single Cell view preferences widget
//==============================================================================

#include "singlecellviewpreferenceswidget.h"

//==============================================================================

#include "ui_singlecellviewpreferenceswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewPreferencesWidget::SingleCellViewPreferencesWidget(QSettings *pSettings,
                                                                 QWidget *pParent) :
    Preferences::PreferencesWidget(pSettings, pParent),
    mGui(new Ui::SingleCellViewPreferencesWidget)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

SingleCellViewPreferencesWidget::~SingleCellViewPreferencesWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellViewPreferencesWidget::savePreferences()
{
//---ISSUE193--- TO BE DONE...
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
