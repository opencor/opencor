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
// Single cell view contents widget
//==============================================================================

#include "borderedwidget.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewgraphpanelswidget.h"
#include "singlecellviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewContentsWidget::SingleCellViewContentsWidget(SingleCellViewPlugin *pPlugin,
                                                           SingleCellViewSimulationWidget *pSimulationWidget,
                                                           QWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget()
{
    // Keep track of our movement

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(emitSplitterMoved()));

    // Create our information widget

    mInformationWidget = new SingleCellViewInformationWidget(pPlugin, pSimulationWidget, this);

    mInformationWidget->setObjectName("Information");

    // Create our graph panels widget

    mGraphPanelsWidget = new SingleCellViewGraphPanelsWidget(pSimulationWidget, this);

    mGraphPanelsWidget->setObjectName("GraphPanels");

    // Add our information and graph panels widgets to ourselves

    addWidget(new Core::BorderedWidget(mInformationWidget,
                                       false, false, true, true));
    addWidget(new Core::BorderedWidget(mGraphPanelsWidget,
                                       false, true, true, false));

    // Make our graph panels widget our focus proxy

    setFocusProxy(mGraphPanelsWidget);
}

//==============================================================================

void SingleCellViewContentsWidget::retranslateUi()
{
    // Retranslate our information and graph panels widgets

    mInformationWidget->retranslateUi();
    mGraphPanelsWidget->retranslateUi();
}

//==============================================================================

void SingleCellViewContentsWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our graph panels widgets

    pSettings->beginGroup(mGraphPanelsWidget->objectName());
        mGraphPanelsWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellViewContentsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our graph panels widgets

    pSettings->beginGroup(mGraphPanelsWidget->objectName());
        mGraphPanelsWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

SingleCellViewInformationWidget * SingleCellViewContentsWidget::informationWidget() const
{
    // Return our information widget

    return mInformationWidget;
}

//==============================================================================

SingleCellViewGraphPanelsWidget * SingleCellViewContentsWidget::graphPanelsWidget() const
{
    // Return our graph panels widget

    return mGraphPanelsWidget;
}

//==============================================================================

void SingleCellViewContentsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
