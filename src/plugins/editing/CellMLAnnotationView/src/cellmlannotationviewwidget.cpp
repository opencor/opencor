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
// CellML Annotation view widget
//==============================================================================

#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QLayout>
#include <QMetaType>
#include <QScreen>
#include <QSettings>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(CellMLAnnotationViewPlugin *pPlugin,
                                                       QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin)
{
}

//==============================================================================

static const char *SettingsCellmlAnnotationViewEditingWidgetSizes         = "EditingWidgetSizes";
static const char *SettingsCellmlAnnotationViewMetadataDetailsWidgetSizes = "MetadataDetailsWidgetSizes";

//==============================================================================

void CellmlAnnotationViewWidget::loadSettings(QSettings &pSettings)
{
    // Retrieve the sizes of our editing widget and of its metadata details
    // Note: we would normally do this in CellmlAnnotationViewEditingWidget, but
    //       we have one instance of it per CellML file and we want to share
    //       some information between the different instances, so we have to do
    //       it here instead...

    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    QVariantList defaultEditingWidgetSizes = QVariantList() << 0.25*availableGeometry.width()
                                                            << 0.75*availableGeometry.width();
    QVariantList defaultMetadataDetailsWidgetSizes = QVariantList() << 0.25*availableGeometry.height()
                                                                    << 0.25*availableGeometry.height()
                                                                    << 0.50*availableGeometry.height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings.value(SettingsCellmlAnnotationViewEditingWidgetSizes, defaultEditingWidgetSizes).toList());
    mMetadataDetailsWidgetSizes = qVariantListToIntList(pSettings.value(SettingsCellmlAnnotationViewMetadataDetailsWidgetSizes, defaultMetadataDetailsWidgetSizes).toList());
}

//==============================================================================

void CellmlAnnotationViewWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of the sizes of our editing widget and of its metadata details

    pSettings.setValue(SettingsCellmlAnnotationViewEditingWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));
    pSettings.setValue(SettingsCellmlAnnotationViewMetadataDetailsWidgetSizes, qIntListToVariantList(mMetadataDetailsWidgetSizes));
}

//==============================================================================

void CellmlAnnotationViewWidget::retranslateUi()
{
    // Retranslate our editing widgets

    for (auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (mEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new CellmlAnnotationViewEditingWidget(mPlugin, pFileName, this, this);

        // Keep track of the sizes of our editing widget and those of its
        // metadata details

        connect(mEditingWidget, &CellmlAnnotationViewEditingWidget::splitterMoved,
                this, &CellmlAnnotationViewWidget::editingWidgetSplitterMoved);

        connect(mEditingWidget->metadataDetails(), &CellmlAnnotationViewMetadataDetailsWidget::splitterMoved,
                this, &CellmlAnnotationViewWidget::metadataDetailsWidgetSplitterMoved);

        // Keep track of our editing widget

        mEditingWidgets.insert(pFileName, mEditingWidget);
    }

    // Update the sizes of our new editing widget and those of its metadata
    // details

    mEditingWidget->setSizes(mEditingWidgetSizes);
    mEditingWidget->metadataDetails()->splitter()->setSizes(mMetadataDetailsWidgetSizes);

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

void CellmlAnnotationViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CellmlAnnotationViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

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

void CellmlAnnotationViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    CellmlAnnotationViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        editingWidget->filePermissionsChanged();
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so consider it reloaded, but only if it
    // has a corresponding widget that is invisible

    QWidget *crtWidget = widget(pFileName);

    if ((crtWidget != nullptr) && !crtWidget->isVisible()) {
        fileReloaded(pFileName);
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (mEditingWidgets.contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::fileRenamed(const QString &pOldFileName,
                                             const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CellmlAnnotationViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

QWidget * CellmlAnnotationViewWidget::widget(const QString &pFileName)
{
    // Return the requested (editing) widget

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

bool CellmlAnnotationViewWidget::saveFile(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    // Save (update) the CellML file to the given file

    CellmlAnnotationViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    return (editingWidget != nullptr)?editingWidget->cellmlFile()->update(pNewFileName):false;
}

//==============================================================================

void CellmlAnnotationViewWidget::editingWidgetSplitterMoved(const QIntList &pSizes)
{
    // The splitter of our editing widget has moved, so keep track of its new
    // sizes

    mEditingWidgetSizes = pSizes;
}

//==============================================================================

void CellmlAnnotationViewWidget::metadataDetailsWidgetSplitterMoved(const QIntList &pSizes)
{
    // The splitter of our editing widget's metadata details has moved, so keep
    // track of its new sizes

    mMetadataDetailsWidgetSizes = pSizes;
}

//==============================================================================

} // namespace CellMLAnnotationView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
