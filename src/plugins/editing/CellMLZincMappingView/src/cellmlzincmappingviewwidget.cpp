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
// CellML-Zinc Mapping view widget
//==============================================================================

#include "cellmlzincmappingvieweditingwidget.h"
#include "cellmlzincmappingviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QScreen>

//==============================================================================

namespace OpenCOR {
namespace CellMLZincMappingView {

//==============================================================================

CellMLZincMappingViewWidget::CellMLZincMappingViewWidget(QWidget *pParent) :
    Core::ViewWidget(pParent)
{
}

//==============================================================================

void CellMLZincMappingViewWidget::retranslateUi()
{
    // Retranslate our editing widgets

    for (auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

static const char *SettingsCellmlZincMappingViewDisplayedZincField           = "CellmlZincMappingViewDisplayedZincField";
static const char *SettingsCellmlZincMappingViewEditingWidgetHorizontalSizes = "CellmlZincMappingViewEditingWidgetHorizontalSizes";
static const char *SettingsCellmlZincMappingViewEditingWidgetVerticalSizes   = "CellmlZincMappingViewEditingWidgetVerticalSizes";
static const char *SettingsCellmlZincMappingViewMeshFileNames                = "CellmlZincMappingViewMeshFileNames";

//==============================================================================

void CellMLZincMappingViewWidget::loadSettings(QSettings &pSettings)
{
    // Retrieve the sizes of our editing widget and of the mesh file names

    static const QRect AvailableGeometry = qApp->primaryScreen()->availableGeometry();
    static const int AvailableGeometryHeight = AvailableGeometry.height();
    static const int AvailableGeometryWidth = AvailableGeometry.width();
    static const QVariantList DefaultEditingWidgetHorizontalSizes = { 0.20*AvailableGeometryWidth,
                                                                      0.80*AvailableGeometryWidth };
    static const QVariantList DefaultEditingWidgetVerticalSizes = { 0.87*AvailableGeometryHeight,
                                                                    0.10*AvailableGeometryHeight,
                                                                    0.13*AvailableGeometryHeight};
    static const QStringList DefaultMeshFileNames = {};

    static const QVariantList DefaultDisplayedZincFields = {true, true, true, true, true};

    mEditingWidgetHorizontalSizes = qVariantListToIntList(pSettings.value(SettingsCellmlZincMappingViewEditingWidgetHorizontalSizes, DefaultEditingWidgetHorizontalSizes).toList());
    mEditingWidgetVerticalSizes = qVariantListToIntList(pSettings.value(SettingsCellmlZincMappingViewEditingWidgetVerticalSizes, DefaultEditingWidgetVerticalSizes).toList());

    mMeshFileNames = pSettings.value(SettingsCellmlZincMappingViewMeshFileNames, DefaultMeshFileNames).toStringList();

    mDisplayedZincFields.clear();

    for (QVariant variant : pSettings.value(SettingsCellmlZincMappingViewDisplayedZincField, DefaultDisplayedZincFields).toList()) {
        mDisplayedZincFields.append(variant.toBool());
    }
}

//==============================================================================

void CellMLZincMappingViewWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of the sizes of our editing widget and of the mesh file names

    pSettings.setValue(SettingsCellmlZincMappingViewEditingWidgetHorizontalSizes, qIntListToVariantList(mEditingWidgetHorizontalSizes));
    pSettings.setValue(SettingsCellmlZincMappingViewEditingWidgetVerticalSizes, qIntListToVariantList(mEditingWidgetVerticalSizes));

    pSettings.setValue(SettingsCellmlZincMappingViewMeshFileNames, mMeshFileNames);

    QVariantList toVariant;
    for (bool value : mDisplayedZincFields) {
        toVariant.append(QVariant(value));
    }

    pSettings.setValue(SettingsCellmlZincMappingViewDisplayedZincField, toVariant);
}

//==============================================================================

void CellMLZincMappingViewWidget::initialize(const QString &pFileName)
{

    // Try to get back the last state of display

    if (mEditingWidget != nullptr) {
        mDisplayedZincFields = mEditingWidget->getZincWidget()->getCheckedAction();
    }


    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (mEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new CellMLZincMappingViewEditingWidget(pFileName, mMeshFileNames, this, this);

        // Keep track of the sizes of our editing widget

        connect(mEditingWidget, &CellMLZincMappingViewEditingWidget::horizontalSplitterMoved,
                this, &CellMLZincMappingViewWidget::EditingWidgetHorizontalSplitterMoved);
        connect(mEditingWidget, &CellMLZincMappingViewEditingWidget::verticalSplitterMoved,
                this, &CellMLZincMappingViewWidget::EditingWidgetVerticalSplitterMoved);

        // Keep track of our editing widget

        mEditingWidgets.insert(pFileName, mEditingWidget);
    }

    // load its settings

    mEditingWidget->getZincWidget()->setCheckedAction(mDisplayedZincFields);

    // Update the sizes of our new editing widget

    mEditingWidget->setSizes(mEditingWidgetHorizontalSizes,
                             mEditingWidgetVerticalSizes);

    // Set our focus proxy to our 'new' editing widget and make sure that the
    // latter immediately gets the focus
    // Note: if we were not to immediately give the focus to our 'new' editor,
    //       then the central widget would give the focus to our 'old' editor
    //       (see CentralWidget::updateGui()), which is clearly not what we
    //       want...

    setFocusProxy(mEditingWidget);

    mEditingWidget->setFocus();
}

//==============================================================================

void CellMLZincMappingViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CellMLZincMappingViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {

        mDisplayedZincFields = editingWidget->getZincWidget()->getCheckedAction();

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

void CellMLZincMappingViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (mEditingWidgets.contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void CellMLZincMappingViewWidget::fileRenamed(const QString &pOldFileName,
                                              const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CellMLZincMappingViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

QWidget * CellMLZincMappingViewWidget::widget(const QString &pFileName)
{
    // Return the requested (editing) widget

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

void CellMLZincMappingViewWidget::setDefaultMeshFiles(const QStringList &pFileNames)
{
      mMeshFileNames = pFileNames;
}

//==============================================================================

void CellMLZincMappingViewWidget::EditingWidgetHorizontalSplitterMoved(const QIntList &pSizes)
{
    // The horizontal splitter of our editing widget has moved, so keep track of
    // its new sizes

    mEditingWidgetHorizontalSizes = pSizes;
}

//==============================================================================

void CellMLZincMappingViewWidget::EditingWidgetVerticalSplitterMoved(const QIntList &pSizes)
{
    // The vertical splitter of our editing widget has moved, so keep track of
    // its new sizes

    mEditingWidgetVerticalSizes = pSizes;
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
