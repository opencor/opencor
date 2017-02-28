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
// Tree view widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "coreglobal.h"

//==============================================================================

#include <QTreeView>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT TreeViewWidget : public QTreeView, public CommonWidget
{
    Q_OBJECT

public:
    explicit TreeViewWidget(QWidget *pParent);

    void resizeColumnsToContents();

    void selectItem(const int &pRow = 0, const int &pColumn = 0);
    void selectFirstItem();

    bool isEditing() const;

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

    virtual QSize sizeHint() const;

    virtual void startDrag(Qt::DropActions pSupportedActions);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
