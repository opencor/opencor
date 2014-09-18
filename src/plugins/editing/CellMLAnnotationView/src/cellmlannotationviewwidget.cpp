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
// CellML annotation view widget
//==============================================================================

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QStackedWidget>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(CellMLAnnotationViewPlugin *pPluginParent,
                                                       QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::CellmlAnnotationViewWidget),
    mPluginParent(pPluginParent),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CellmlAnnotationViewEditingWidget *>()),
    mEditingWidgetSizes(QIntList()),
    mMetadataDetailsWidgetSizes(QIntList())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Add a stacked widget to our layout

    mContents = new QStackedWidget(this);

    layout()->addWidget(mContents);
}

//==============================================================================

CellmlAnnotationViewWidget::~CellmlAnnotationViewWidget()
{
    // Delete the GUI

    delete mGui;
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
    // Return whether we know about the given file, i.e. whether we have an
    // editing widget for it

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

void CellmlAnnotationViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (!mEditingWidget) {
        // No editing widget exists for the given file, so create one

        mEditingWidget = new CellmlAnnotationViewEditingWidget(mPluginParent, pFileName, this);

        // Keep track of the sizes of our editing widget and those of its
        // metadata details

        connect(mEditingWidget, SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(editingWidgetSplitterMoved(const QIntList &)));

        connect(mEditingWidget->metadataDetails(), SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(metadataDetailsWidgetSplitterMoved(const QIntList &)));

        // Keep track of our editing widget and add it to ourselves

        mEditingWidgets.insert(pFileName, mEditingWidget);

        layout()->addWidget(mEditingWidget);
    }

    // Update the sizes of our new editing widget and those of its metadata
    // details

    mEditingWidget->setSizes(mEditingWidgetSizes);
    mEditingWidget->metadataDetails()->splitter()->setSizes(mMetadataDetailsWidgetSizes);

    // Remove our previous editing widget and add ouew new one

    mContents->removeWidget(mContents->currentWidget());
    mContents->addWidget(mEditingWidget);

    // Set our focus proxy to our 'new' editing widget and make sure that the
    // latter immediately gets the focus
    // Note: if we were not to immediately give our 'new' editing widget the
    //       focus, then the central widget would give the focus to our 'old'
    //       editing widget (see CentralWidget::updateGui()), so...

    setFocusProxy(mEditingWidget);

    mEditingWidget->setFocus();
}

//==============================================================================

void CellmlAnnotationViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CellmlAnnotationViewEditingWidget *editingWidget  = mEditingWidgets.value(pFileName);

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

CellmlAnnotationViewEditingWidget * CellmlAnnotationViewWidget::editingWidget(const QString &pFileName) const
{
    // Return the requested editing widget

    return mEditingWidgets.value(pFileName);
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
