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
// Widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT Widget : public QWidget, public CommonWidget
{
    Q_OBJECT

public:
    enum LayoutType {
        VerticalLayout,
        HorizontalLayout,
        FormLayout,
        GridLayout,
        StackedLayout
    };

    explicit Widget(QWidget *pParent);
    explicit Widget(const QSize &pSizeHint, QWidget *pParent);

    void createLayout(const LayoutType &pLayoutType = VerticalLayout);

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

    virtual QSize sizeHint() const;

private:
    QSize mSizeHint;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
