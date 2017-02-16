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
// Splitter widget
//==============================================================================

#include "splitterwidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void SplitterWidget::constructor(const Qt::Orientation &pOrientation)
{
    // Customise ourselves so that we are as thin as possible and without an
    // image for our handle

    setHandleWidth(2);
    setOrientation(pOrientation);
    setStyleSheet("QSplitter::handle {"
                  "    image: none;"
                  "}");
}

//==============================================================================

SplitterWidget::SplitterWidget(const Qt::Orientation &pOrientation,
                               QWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(this)
{
    // Construct ourselves

    constructor(pOrientation);
}

//==============================================================================

SplitterWidget::SplitterWidget(QWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(this)
{
    // Construct ourselves

    constructor(Qt::Horizontal);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
