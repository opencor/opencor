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

#include "biosignalmldatastoreselectvars.h"

//==============================================================================

#include "ui_biosignalmldatastoreselectvars.h"

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

BioSignalMLSelectVariables::BioSignalMLSelectVariables(QWidget *pParent,
                                                       const QVector<QString> & pLabels,
                                                       const QVector<bool> & pChecked) :
    QDialog(pParent),
    mGui(new Ui::BioSignalMLSelectVariables)
{
    // Set up the GUI

    mGui->setupUi(this);
    QTableWidget *varlist = mGui->selectedVariables;
    for (auto i = 0 ;  i < pLabels.size() ;  ++i) {
        varlist->insertRow(i);
        QTableWidgetItem *checked = new QTableWidgetItem();
        checked->setCheckState(pChecked[i] ? Qt::Checked : Qt::Unchecked);
        varlist->setItem(i, 0, checked);
        varlist->setItem(i, 1, new QTableWidgetItem(pLabels[i]));
    }

    QObject::connect(mGui->allVariables, SIGNAL(clicked(bool)),
                     this, SLOT(selectAllVariables(bool)));
}

//==============================================================================

BioSignalMLSelectVariables::~BioSignalMLSelectVariables()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void BioSignalMLSelectVariables::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

void BioSignalMLSelectVariables::selectAllVariables(bool checked)
{
    auto state = checked ? Qt::Checked : Qt::Unchecked;
    QTableWidget *varlist = mGui->selectedVariables;
    for (auto row = 0 ;  row < varlist->rowCount() ;  ++row)
      varlist->item(row, 0)->setCheckState(state);
}

//==============================================================================

bool BioSignalMLSelectVariables::checked(size_t pIndex)
{
    return (mGui->selectedVariables->item(pIndex, 0)->checkState() == Qt::Checked) ;
}

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
