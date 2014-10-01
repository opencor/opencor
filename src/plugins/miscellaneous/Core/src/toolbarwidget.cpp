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
// Tool bar widget
//==============================================================================

#include "toolbarwidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ToolBarWidget::ToolBarWidget(QWidget *pParent) :
    QToolBar(pParent)
{
    // Remove the border which is normally drawn for a tool bar widget (indeed,
    // it doesn't look great when on a docked window)

    setStyleSheet("QToolBar {"
                  "    border: 0px;"
                  "}");

    // Force the size of the icons to be 20 by 20 pixels

    setIconSize(QSize(20, 20));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
