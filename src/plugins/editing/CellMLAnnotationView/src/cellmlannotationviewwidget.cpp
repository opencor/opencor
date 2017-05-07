/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
#include <QDesktopWidget>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(CellMLAnnotationViewPlugin *pPlugin,
                                                       QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CellmlAnnotationViewEditingWidget *>()),
    mEditingWidgetSizes(QIntList()),
    mMetadataDetailsWidgetSizes(QIntList())
{
}

//==============================================================================

static const auto SettingsCellmlAnnotationViewEditingWidgetSizes         = QStringLiteral("EditingWidgetSizes");
static const auto SettingsCellmlAnnotationViewMetadataDetailsWidgetSizes = QStringLiteral("MetadataDetailsWidgetSizes");

//==============================================================================

void CellmlAnnotationViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the sizes of our editing widget and of its metadata details
    // Note: we would normally do this in CellmlAnnotationViewEditingWidget, but
    //       we have one instance of it per CellML file and we want to share
    //       some information between the different instances, so we have to do
    //       it here instead...

    QVariantList defaultEditingWidgetSizes = QVariantList() << 0.25*qApp->desktop()->screenGeometry().width()
                                                            << 0.75*qApp->desktop()->screenGeometry().width();
    QVariantList defaultMetadataDetailsWidgetSizes = QVariantList() << 0.25*qApp->desktop()->screenGeometry().height()
                                                                    << 0.25*qApp->desktop()->screenGeometry().height()
                                                                    << 0.50*qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsCellmlAnnotationViewEditingWidgetSizes, defaultEditingWidgetSizes).toList());
    mMetadataDetailsWidgetSizes = qVariantListToIntList(pSettings->value(SettingsCellmlAnnotationViewMetadataDetailsWidgetSizes, defaultMetadataDetailsWidgetSizes).toList());
}

//==============================================================================

void CellmlAnnotationViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the sizes of our editing widget and of its metadata details

    pSettings->setValue(SettingsCellmlAnnotationViewEditingWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));
    pSettings->setValue(SettingsCellmlAnnotationViewMetadataDetailsWidgetSizes, qIntListToVariantList(mMetadataDetailsWidgetSizes));
}

//==============================================================================

void CellmlAnnotationViewWidget::retranslateUi()
{
    // Retranslate our editing widgets

    foreach (CellmlAnnotationViewEditingWidget *editingWidget, mEditingWidgets)
        editingWidget->retranslateUi();
}

//==============================================================================

bool CellmlAnnotationViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mEditingWidgets.contains(pFileName);
}

//==============================================================================

void CellmlAnnotationViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (!mEditingWidget) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new CellmlAnnotationViewEditingWidget(mPlugin, pFileName, this, this);

        // Keep track of the sizes of our editing widget and those of its
        // metadata details

        connect(mEditingWidget, SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(editingWidgetSplitterMoved(const QIntList &)));

        connect(mEditingWidget->metadataDetails(), SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(metadataDetailsWidgetSplitterMoved(const QIntList &)));

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

    if (editingWidget) {
        // There is an editing widget for the given file name, so delete it and
        // remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editingWidget == mEditingWidget)
            mEditingWidget = 0;
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so enable/disable parts of our GUI,
    // should the given file be managed

    CellmlAnnotationViewEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget)
        editingWidget->filePermissionsChanged();
}

//==============================================================================

void CellmlAnnotationViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (contains(pFileName)) {
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

    if (editingWidget) {
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
    // Ask our CellML annotation view widget to save the given file

    CellmlAnnotationViewEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    return editingWidget?editingWidget->cellmlFile()->save(pNewFileName):false;
}

//==============================================================================

QString CellmlAnnotationViewWidget::resourceUrl(const QString &pResource)
{
    // Return the URL for the given resource

    return "http://identifiers.org/"+pResource+"/?redirect=true";
}

//==============================================================================

QString CellmlAnnotationViewWidget::idUrl(const QString &pResource,
                                          const QString &pId)
{
    // Return the URL for the given resource

    return "http://identifiers.org/"+pResource+"/"+pId+"/?profile=most_reliable&redirect=true";
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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
