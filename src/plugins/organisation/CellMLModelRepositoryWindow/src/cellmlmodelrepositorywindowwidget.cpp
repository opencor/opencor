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

#include "cellmlmodelrepositorywindowwidget.h"

//==============================================================================

#include <QDesktopServices>
#include <QIODevice>
#include <QPaintEvent>
#include <QStandardItemModel>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

CellmlModelRepositoryWindowWidget::CellmlModelRepositoryWindowWidget(QWidget *pParent) :
    Core::TreeViewWidget(pParent)
{
    // Create an instance of the data model that we want to view

    mModel = new QStandardItemModel(this);

    // Set some properties

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFrameShape(QFrame::StyledPanel);
    setHeaderHidden(true);
    setModel(mModel);
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::NoSelection);

//---GRY---
//    connect(this, SIGNAL(resized(const QSize &, const QSize &)),
//            this, SLOT(recenterBusyWidget()));

    // Have links opened in the user's browser rather than in our list

//    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

//==============================================================================

QSize CellmlModelRepositoryWindowWidget::sizeHint() const
{
    // Suggest a default size for the CellML Model Repository widget
    // Note: this is critical if we want a docked widget, with a CellML Model
    //       Repository widget on it, to have a decent size when docked to the
    //       main window...

    return defaultSize(0.15);
}

//==============================================================================

void CellmlModelRepositoryWindowWidget::output(const QString &pMessage,
                                               const QMap<QString, QString> &pModelListing)
{
    // Set the page to contain pOutput using our output template
Q_UNUSED(pModelListing);

    mModel->clear();

    mModel->invisibleRootItem()->appendRow(new QStandardItem(pMessage));
}

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
