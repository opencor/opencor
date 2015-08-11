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
    mAccepted(false)
{
    // Set up the GUI

    mGui->setupUi(this);
    QObject::connect(mGui->setFileName, SIGNAL(clicked(bool)),
                     this, SLOT(setFileName(bool)));
    QObject::connect(mGui->cancel_ok, SIGNAL(accepted()),
                     this, SLOT(accepted()));
    this->setVisible(false);
}

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

void BioSignalMLSaveDialog::accepted(void)
{
    mAccepted = true;
}

//==============================================================================

bool BioSignalMLSaveDialog::run(void)
{
    mAccepted = false;
    mGui->fileNameValue->clear();
    mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(false) ;
    this->setVisible(true);

    this->open() ;
    QEventLoop loop;
    connect(this, SIGNAL(finished(int)), & loop, SLOT(quit()));
    mGui->setFileName->clicked(true);
    loop.exec();

    this->setVisible(false);
    return mAccepted;
}

//==============================================================================

void BioSignalMLSaveDialog::setFileName(bool checked)
{
    Q_UNUSED(checked)

    QString bsmlFiles = QObject::tr("BioSignalML")+" (*.bsml)";
    QString name = Core::getSaveFileName(QObject::tr("Enter BioSignalML file name"),
                                         QString(),
                                         bsmlFiles, &bsmlFiles);
    if (!name.isEmpty()) {
        mGui->fileNameValue->setText(name);
        mGui->fileNameValue->setCursorPosition(name.length());
        mGui->setFileName->setDefault(false);
        mGui->shortNameValue->setFocus(Qt::TabFocusReason);
        mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(true) ;
        mGui->cancel_ok->button(QDialogButtonBox::Ok)->setDefault(true) ;
    }
    this->activateWindow();
}

//==============================================================================

QString BioSignalMLSaveDialog::fileName(void) const
{
    return mGui->fileNameValue->text();
}

//==============================================================================

QString BioSignalMLSaveDialog::shortName(void) const
{
    return mGui->shortNameValue->text();
}

//==============================================================================

QString BioSignalMLSaveDialog::description(void) const
{
    return mGui->descriptionValue->toPlainText();
}

//==============================================================================

QString BioSignalMLSaveDialog::author(void) const
{
    return mGui->authorValue->text();
}

//==============================================================================

void BioSignalMLSaveDialog::setComment(const QString & pComment)
{
   mGui->commentValue->setText(pComment);
}

//==============================================================================

}   // namespace BSMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
