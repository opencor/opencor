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

#include "checkforupdateswindow.h"

//==============================================================================

#include "ui_checkforupdateswindow.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

CheckForUpdatesWindow::CheckForUpdatesWindow() :
    QWidget(0),
    mGui(new Ui::CheckForUpdatesWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

}

//==============================================================================

CheckForUpdatesWindow::~CheckForUpdatesWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
