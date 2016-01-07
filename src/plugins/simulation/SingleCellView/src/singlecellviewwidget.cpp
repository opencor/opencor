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

#include <QIcon>
#include <QLayout>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewWidget::SingleCellViewWidget(SingleCellViewPlugin *pPlugin,
                                           QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin),
    mSettingsGroup(QString()),
    mSimulationWidget(0),
    mSimulationWidgets(QMap<QString, SingleCellViewSimulationWidget *>())
{
}

//==============================================================================

void SingleCellViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the simulation widget's settings, but
    // mSimulationWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void SingleCellViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
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

void SingleCellViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the simulation widget associated with the given file, if any

    SingleCellViewSimulationWidget *oldSimulationWidget = mSimulationWidget;

    mSimulationWidget = mSimulationWidgets.value(pFileName);

    if (!mSimulationWidget) {
        // No simulation widget exists for the given file, so create one

        mSimulationWidget = new SingleCellViewSimulationWidget(mPlugin, pFileName, this);

        // Keep track of our editing widget and add it to ourselves

        mSimulationWidgets.insert(pFileName, mSimulationWidget);

        layout()->addWidget(mSimulationWidget);

        // Load our simulation widget's settings

        QSettings settings;

        settings.beginGroup(mSettingsGroup);
            mSimulationWidget->loadSettings(&settings);
        settings.endGroup();
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
        // There is a simulation widget for the given file name, so save its
        // settings

        QSettings settings;

        settings.beginGroup(mSettingsGroup);
            simulationWidget->saveSettings(&settings);
        settings.endGroup();

        // Now, we can delete it and remove it from our list

        delete simulationWidget;

        mSimulationWidgets.remove(pFileName);

        // Finally, reset our memory of the simulation widget, if needed

        if (simulationWidget == mSimulationWidget)
            mSimulationWidget = 0;
    }
}

//==============================================================================

QIcon SingleCellViewWidget::fileTabIcon(const QString &pFileName) const
{
    // Return, if possible, the tab icon for the given file

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        return simulationWidget->fileTabIcon();
    } else {
        static const QIcon NoIcon = QIcon();

        return NoIcon;
    }
}

//==============================================================================

bool SingleCellViewWidget::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // Save the given file, if possible

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget)
        return simulationWidget->saveFile(pOldFileName, pNewFileName);
    else
        return false;
}

//==============================================================================

void SingleCellViewWidget::fileOpened(const QString &pFileName)
{
    // Keep track, if possible, of the fact that a file has been opened

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->fileOpened(pFileName);
}

//==============================================================================

void SingleCellViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // Keep track, if possible, of the fact that a file has been un/locked

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->filePermissionsChanged();
}

//==============================================================================

void SingleCellViewWidget::fileModified(const QString &pFileName)
{
    // Keep track, if possible, of the fact that a file has been un/locked

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->fileModified(pFileName);
}

//==============================================================================

void SingleCellViewWidget::fileReloaded(const QString &pFileName)
{
    // Keep track, if possible, of the fact that a file has been reloaded

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
    // Keep track, if possible, of the fact that a file has been renamed

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget) {
        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }
}

//==============================================================================

void SingleCellViewWidget::fileClosed(const QString &pFileName)
{
    // Keep track, if possible, of the fact that a file has been closed

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->fileClosed(pFileName);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
