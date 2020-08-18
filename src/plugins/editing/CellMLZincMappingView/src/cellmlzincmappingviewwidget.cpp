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

#include "cellmlzincmappingviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QMenu>
#include <QScreen>

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

CellMLZincMappingViewWidget::CellMLZincMappingViewWidget(QWidget *pParent) :
    Core::ViewWidget(pParent)
{

    //TODO
    mMeshFileName = "/home/tuareg/Documents/OpenCOR/opencor/meshes/trilinearCube.exfile";

    static const QRect AvailableGeometry = qApp->primaryScreen()->availableGeometry();
    static const int AvailableGeometryHeight = AvailableGeometry.height();
    static const int AvailableGeometryWidth = AvailableGeometry.width();

    static const QVariantList DefaultEditingWidgetHorizontalSizes = { 0.20*AvailableGeometryWidth,
                                                               0.80*AvailableGeometryWidth };
    static const QVariantList DefaultEditingWidgetVerticalSizes = { 0.87*AvailableGeometryHeight,
                                                               0.1*AvailableGeometryHeight,
                                                               0.12*AvailableGeometryHeight};

    mEditingWidgetHorizontalSizes = qVariantListToIntList(DefaultEditingWidgetHorizontalSizes);
    mEditingWidgetVerticalSizes = qVariantListToIntList(DefaultEditingWidgetVerticalSizes);

}

//==============================================================================

CellMLZincMappingViewWidget::~CellMLZincMappingViewWidget()
{
}

//==============================================================================

void CellMLZincMappingViewWidget::retranslateUi()
{
    // Update ourself too since some widgets will have been reset following the
    // retranslation (e.g. mGui->fileNameValue)

    for(auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

void CellMLZincMappingViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (mEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new CellMLZincMappingViewEditingWidget(pFileName, mMeshFileName,this, this);

        mEditingWidgets.insert(pFileName, mEditingWidget);

        mEditingWidget->setSizes(mEditingWidgetHorizontalSizes,mEditingWidgetVerticalSizes);
    }

    // Set our focus proxy to our 'new' simulation widget and make sure that the
    // latter immediately gets the focus

    setFocusProxy(mEditingWidget);

    mEditingWidget->setFocus();
}

//==============================================================================

void CellMLZincMappingViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CellMLZincMappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

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

CellMLZincMappingViewEditingWidget* CellMLZincMappingViewWidget::editingWidget(const QString &pFileName) const
{
    // Return the requested simulation widget

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

QWidget * CellMLZincMappingViewWidget::widget(const QString &pFileName)
{
    // Return the requested (simulation) widget

    return editingWidget(pFileName);
}

//==============================================================================

void CellMLZincMappingViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    CellMLZincMappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        editingWidget->filePermissionsChanged();
    }
}

//==============================================================================

void CellMLZincMappingViewWidget::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);
}

//==============================================================================

void CellMLZincMappingViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (mEditingWidgets.contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void CellMLZincMappingViewWidget::fileRenamed(const QString &pOldFileName, const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CellMLZincMappingViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

void CellMLZincMappingViewWidget::setDefaultMeshFile(const QString &pFileName)
{
      mMeshFileName = pFileName;
}


//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
