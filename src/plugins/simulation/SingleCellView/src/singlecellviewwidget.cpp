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
// Single cell view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "singlecellviewsimulationwidget.h"
#include "singlecellviewwidget.h"

//==============================================================================

#include "ui_singlecellviewwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewWidget::SingleCellViewWidget(SingleCellViewPlugin *pPlugin,
                                           QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin),
    mSimulationWidget(0),
    mSimulationWidgets(QMap<QString, SingleCellViewSimulationWidget *>())
{
    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);
}

//==============================================================================

void SingleCellViewWidget::retranslateUi()
{
    // Retranslate our simulation widgets

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets)
        simulationWidget->retranslateUi();
}

//==============================================================================

bool SingleCellViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mSimulationWidgets.contains(pFileName);
}

//==============================================================================

static const auto OutputTab  = QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;");
static const auto OutputGood = QStringLiteral(" style=\"color: green;\"");
static const auto OutputInfo = QStringLiteral(" style=\"color: navy;\"");
static const auto OutputBad  = QStringLiteral(" style=\"color: maroon;\"");
static const auto OutputBrLn = QStringLiteral("<br/>\n");

//==============================================================================

void SingleCellViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the simulation widget associated with the given file, if any

    SingleCellViewSimulationWidget *oldSimulationWidget = mSimulationWidget;

    mSimulationWidget = mSimulationWidgets.value(pFileName);

    if (!mSimulationWidget) {
        // No simulation widget exists for the given file, so create one

        mSimulationWidget = new SingleCellViewSimulationWidget(mPlugin, this);

        // Keep track of our editing widget and add it to ourselves

        mSimulationWidgets.insert(pFileName, mSimulationWidget);

        layout()->addWidget(mSimulationWidget);
    }

    // Hide our previous simulation widget and show our new one

    mSimulationWidget->show();

    if (oldSimulationWidget && (mSimulationWidget != oldSimulationWidget))
        oldSimulationWidget->hide();

    // Set our focus proxy to our 'new' simulation widget and make sure that the
    // latter immediately gets the focus

    setFocusProxy(mSimulationWidget);

    mSimulationWidget->setFocus();
}

//==============================================================================

void SingleCellViewWidget::finalize(const QString &pFileName)
{
    // Remove the simulation widget, should there be one for the given file

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        // There is a simulation widget for the given file name, so delete it
        // and remove it from our list

        delete simulationWidget;

        mSimulationWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (simulationWidget == mSimulationWidget)
            mSimulationWidget = 0;
    }
}

//==============================================================================

void SingleCellViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->filePermissionsChanged();
}

//==============================================================================

void SingleCellViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (contains(pFileName)) {
        finalize(pFileName);

        if (CellMLSupport::CellmlFileManager::instance()->isCellmlFile(pFileName))
            initialize(pFileName);
    }
}

//==============================================================================

void SingleCellViewWidget::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // The given file has been renamed, so update our simulating widgets mapping

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget) {
        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
