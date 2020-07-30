/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Mapping view widget
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "filemanager.h"
#include "mappingviewwidget.h"
#include "meshreader.h"
#include "cellmlfilemanager.h"
#include "zincwidget.h"

//==============================================================================

#include "ui_mappingviewwidget.h"

//==============================================================================

#include <QFile>
#include <QtGui>

#include<QDebug> //TODO no keep this one of course

//==============================================================================

#include "zincbegin.h" //TODO takeaway the useless
    #include "opencmiss/zinc/fieldconstant.hpp"
    #include "opencmiss/zinc/fieldmodule.hpp"
    #include "opencmiss/zinc/fieldvectoroperators.hpp"
    #include "opencmiss/zinc/status.h"
#include "zincend.h"

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

MappingViewWidget::MappingViewWidget(QWidget *pParent) :
    Core::SplitterWidget(pParent),
    mAxesFontPointSize(0)
{
    // Set our orientation

    setOrientation(Qt::Horizontal);

    // Keep track of our movement
    /*
    connect(this, &Core::SplitterWidget::splitterMoved,
            this, &MappingViewEditingWidget::splitterMoved);
    */

    mListWidgetVariables = new QListWidget();
    mListWidgetOutput = new QListWidget();

    addWidget(mListWidgetVariables);

    // Create a local copy of our .exnode and .exelem files

    QString applicationBaseFileName = QDir::tempPath()+"/mesh";

    mExNodeFileName = applicationBaseFileName+".exnode";
    mExElemFileName = applicationBaseFileName+".exelem";

    Core::writeResourceToFile(mExNodeFileName, QDir::currentPath()+"/../opencor/meshes/circulation.exnode");
    Core::writeResourceToFile(mExElemFileName, QDir::currentPath()+"/../opencor/meshes/circulation.exelem");
    //but nothing appear in /tmp/ !!!
    mListWidgetVariables->addItem(QDir::currentPath()+"/../opencor/meshes/circulation.exnode");
    mListWidgetVariables->addItem(mExElemFileName);

    // Create and add a Zinc widget

    mMappingViewZincWidget = new MappingViewZincWidget(this);

    //TODO change syntax
    connect(mMappingViewZincWidget, SIGNAL(devicePixelRatioChanged(const int &)),
            this, SLOT(devicePixelRatioChanged(const int &)));

    addWidget(mMappingViewZincWidget);
}

//==============================================================================

MappingViewWidget::~MappingViewWidget()
{
    QFile::remove(mExNodeFileName);
    QFile::remove(mExElemFileName);
}

//==============================================================================

void MappingViewWidget::retranslateUi()
{
    // Update ourself too since some widgets will have been reset following the
    // retranslation (e.g. mGui->fileNameValue)

    for(auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

void MappingViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (mEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new MappingViewEditingWidget(pFileName, this);

        mEditingWidgets.insert(pFileName, mEditingWidget);
    }

    //mListWidgetVariables->setModel(mEditingWidget->listViewModelVariables()); //TODO set only when charging the plugin ?
    //mGui->outputList->setModel(mEditingWidget->listViewModelOutput());
}

//==============================================================================

void MappingViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        // There is an editing widget for the given file name, so delete it and
        // remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editingWidget == mEditingWidget) {
            mEditingWidget = nullptr;
        }
    }
}

//==============================================================================

void MappingViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        editingWidget->filePermissionsChanged();
    }
}

//==============================================================================

void MappingViewWidget::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);
}

//==============================================================================

void MappingViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (mEditingWidgets.contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void MappingViewWidget::fileRenamed(const QString &pOldFileName, const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    MappingViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

void MappingViewWidget::devicePixelRatioChanged(const int &pDevicePixelRatio)
{
    Q_UNUSED(pDevicePixelRatio);
    //TODO to confirm
    // Update our scene using the given devide pixel ratio
    /*
    OpenCMISS::Zinc::Scene scene = mZincContext->getDefaultRegion().getScene();

    scene.beginChange();
        scene.createGraphicsPoints().getGraphicspointattributes().getFont().setPointSize(pDevicePixelRatio*mAxesFontPointSize);
    scene.endChange();
    */
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
