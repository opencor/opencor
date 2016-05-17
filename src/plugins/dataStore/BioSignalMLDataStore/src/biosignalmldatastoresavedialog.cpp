/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Save as BioSignalML widget
//==============================================================================

#include "coreguiutils.h"
#include "biosignalmldatastoresavedialog.h"
#include "biosignalmldatastoreselectvars.h"

//==============================================================================

#include "ui_biosignalmldatastoresavedialog.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BioSignalMLSaveDialog::BioSignalMLSaveDialog(QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::BioSignalMLSaveDialog),
    mDefaultFileName(QString()),
    mVariableLabels(QVector<QString>()),
    mSelectedVariables(QVector<bool>()),
    mGotSignals(false),
    mAccepted(false)
{
    // Set up the GUI

    mGui->setupUi(this);
    QObject::connect(mGui->setFileName, SIGNAL(clicked(bool)),
                     this, SLOT(setFileName(bool)));
    QObject::connect(mGui->selectVariables, SIGNAL(clicked(bool)),
                     this, SLOT(selectVariables(bool)));
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

void BioSignalMLSaveDialog::setButtonStates(void) const
{
    if (mGui->fileNameValue->text() != "" && mGotSignals) {
        mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(true);
        mGui->cancel_ok->button(QDialogButtonBox::Ok)->setDefault(true);
    }
    else if (mGui->fileNameValue->text() == "") {
        mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(false);
        mGui->setFileName->setDefault(true);
    }
    else {
        mGui->cancel_ok->button(QDialogButtonBox::Ok)->setEnabled(false);
        mGui->selectVariables->setDefault(true);
    }
}

//==============================================================================

void BioSignalMLSaveDialog::setDefaultFileName(const QString &pFileName)
{
    mDefaultFileName = pFileName;
}

//==============================================================================

void BioSignalMLSaveDialog::setFileName(bool checked)
{
    Q_UNUSED(checked)

    QString biosignalmlFilter = QObject::tr("BioSignalML File")+" (*.biosignalml)";
    QString name = Core::getSaveFileName(QObject::tr("Enter BioSignalML file name"),
                                         mDefaultFileName,
                                         QStringList() << biosignalmlFilter,
                                         &biosignalmlFilter);
    if (!name.isEmpty()) {
        mGui->fileNameValue->setText(name);
        mGui->fileNameValue->setCursorPosition(name.length());
        mGui->setFileName->setDefault(false);
        mGui->shortNameValue->setFocus(Qt::TabFocusReason);
        setButtonStates();
    }
    this->activateWindow();
}

//==============================================================================

QString BioSignalMLSaveDialog::fileName(void) const
{
    return mGui->fileNameValue->text();
}

//==============================================================================

void BioSignalMLSaveDialog::setSelectedVariables(const DataStore::DataStoreVariables &pVariables)
{
    mVariableLabels.clear();
    auto variableBegin = pVariables.constBegin();
    auto variableEnd = pVariables.constEnd();
    for (auto variable = variableBegin; variable != variableEnd; ++variable)
        mVariableLabels.append((*variable)->label());
    mSelectedVariables = QVector<bool>(mVariableLabels.size(), true);
    mGui->selectedVariables->setText(QObject::tr("All variables"));
    mGotSignals = true;
}

//==============================================================================

void BioSignalMLSaveDialog::selectVariables(bool checked)
{
    Q_UNUSED(checked)

    // Set text to "All" "Selected" "None"
    // If None then OK disabled...

    BioSignalMLSelectVariables selection(this, mVariableLabels, mSelectedVariables);
    if (selection.exec() == QDialog::Accepted) {
        int selected = 0;
        for (auto i = 0 ;  i < mVariableLabels.size() ;  ++i) {
            mSelectedVariables[i] = selection.checked(i) ;
            if (mSelectedVariables[i]) ++selected;
            }
        mGui->selectedVariables->setText(
            selected == 0                     ? QObject::tr("No variables")
          : selected < mVariableLabels.size() ? QObject::tr("As specified")
                                              : QObject::tr("All variables"));
        mGotSignals = (selected > 0);
        setButtonStates();
    }

    this->activateWindow();
}

//==============================================================================

bool BioSignalMLSaveDialog::selectedVariable(const size_t pIndex) const
{
    return mSelectedVariables[pIndex];
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

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
