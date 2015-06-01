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
// CellML Model Repository widget
//==============================================================================

#ifndef CELLMLMODELREPOSITORYWINDOWWIDGET_H
#define CELLMLMODELREPOSITORYWINDOWWIDGET_H

//==============================================================================

#include "treeviewwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
}   // namespace Core

//==============================================================================

namespace CellMLModelRepositoryWindow {

//==============================================================================

class CellmlModelRepositoryWindowWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindowWidget(QWidget *pParent);

    void output(const QString &pMessage,
                const QMap<QString, QString> &pModelListing);

protected:
    virtual QSize sizeHint() const;

private:
    Core::TreeViewWidget *mModelListing;
};

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
