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
// Save as BioSignalML widget
//==============================================================================

#include "coreguiutils.h"
#include "bsmldatastoresavedialog.h"
#include "i18ninterface.h"
#include "filemanager.h"

//==============================================================================

#include "ui_bsmldatastoresavedialog.h"

//==============================================================================

namespace OpenCOR {
namespace BSMLDataStore {

//==============================================================================

BioSignalMLSaveDialog::BioSignalMLSaveDialog(QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::BioSignalMLSaveDialog),
    mFileName(QString())
{
    // Set up the GUI

    mGui->setupUi(this);
    QObject::connect(mGui->setFileName, SIGNAL(clicked(bool)),
                     this, SLOT(setFileName(bool)));
    QObject::connect(mGui->descriptionValue, SIGNAL(textChanged()),
                     this, SLOT(setDescription()));

    mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(false) ;

    mGui->fileNameList->setFocus(Qt::TabFocusReason);
    mGui->setFileName->setDefault(true);
}


/**

static const auto SettingsGraphPanelSizes = QStringLiteral("GraphPanelSizes");

//==============================================================================

void SingleCellViewGraphPanelsWidget::loadSettings(QSettings *pSettings)
{
    QIntList splitterSizes = qVariantListToIntList(pSettings->value(SettingsGraphPanelSizes).toList());
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the size of each graph panel

    pSettings->setValue(SettingsGraphPanelSizes, qIntListToVariantList(mSplitterSizes));
}

**/

//==============================================================================

BioSignalMLSaveDialog::~BioSignalMLSaveDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void BioSignalMLSaveDialog::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

}

//==============================================================================

void BioSignalMLSaveDialog::setFileName(bool checked)
{
    Q_UNUSED(checked)

    QString bsmlFiles = QObject::tr("BioSignalML")+" (*.bsml)";

    QString name = Core::getSaveFileName(QObject::tr("Enter BioSignalML file name"),
                                         QString(),
                                         bsmlFiles, &bsmlFiles);
    if (name != "") {
      mFileName = name ;
      mGui->fileNameList->insertItem(0, mFileName);
      mGui->fileNameList->setCurrentIndex(0);
      mGui->setFileName->setDefault(false);
      mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(true) ;
      mGui->cancel_ok->button(QDialogButtonBox::Ok)->setDefault(true) ;
      mGui->shortNameValue->setFocus(Qt::TabFocusReason);
      }
}

//==============================================================================

QString BioSignalMLSaveDialog::fileName(void) const
{
    return mFileName;
}

//==============================================================================

void BioSignalMLSaveDialog::setDescription()
{
   mDescription = mGui->descriptionValue->toPlainText();
}

//==============================================================================

QString BioSignalMLSaveDialog::description(void) const
{
    return mDescription;
}

}   // namespace BSMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
