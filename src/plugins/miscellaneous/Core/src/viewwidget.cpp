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
// View widget
//==============================================================================

#include "viewwidget.h"

//==============================================================================

#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ViewWidget::ViewWidget(QWidget *pParent, const bool &pNeedLayout) :
    Widget(pParent),
    BusySupportWidget()
{
    // Create and set our vertical layout, if needed

    if (pNeedLayout) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        layout->setMargin(0);
        layout->setSpacing(0);

        setLayout(layout);
    }
}

//==============================================================================

void ViewWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Widget::resizeEvent(pEvent);

    // (Re)size our busy widget

    resizeBusyWidget();
}

//==============================================================================

QList<QWidget *> ViewWidget::statusBarWidgets() const
{
    // No status bar widgets by default

    return QList<QWidget *>();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
