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
// Dock widget
//==============================================================================

#include "dockwidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

DockWidget::DockWidget(QWidget *pParent) :
    QDockWidget(pParent),
    CommonWidget(pParent)
{
#ifdef Q_OS_MAC
    // Remove the padding for the float and close buttons on OS X (indeed it
    // makes the title bar of a docked (i.e. non-floating) window insanely big,
    // so...)

    setStyleSheet("QDockWidget::float-button, QDockWidget::close-button {"
                  "    padding: 0px;"
                  "}");
#endif
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
