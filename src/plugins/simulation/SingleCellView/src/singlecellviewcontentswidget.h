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
// Single cell view contents widget
//==============================================================================

#ifndef SINGLECELLVIEWCONTENTSWIDGET_H
#define SINGLECELLVIEWCONTENTSWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelsWidget;
class SingleCellViewInformationWidget;

//==============================================================================

class SingleCellViewContentsWidget : public QSplitter, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewContentsWidget(QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellViewInformationWidget * informationWidget();
    SingleCellViewGraphPanelsWidget * graphPanelsWidget();

private:
    QIntList mSplitterSizes;

    SingleCellViewInformationWidget *mInformationWidget;
    SingleCellViewGraphPanelsWidget *mGraphPanelsWidget;

private Q_SLOTS:
    void splitterMoved();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
