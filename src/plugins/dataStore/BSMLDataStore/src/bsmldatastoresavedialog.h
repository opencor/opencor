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

#ifndef BSMLDATASTORESAVEDIALOG_H
#define BSMLDATASTORESAVEDIALOG_H

//==============================================================================

#include "bsmldatastoreglobal.h"

//==============================================================================

#include <QDialog>

//==============================================================================

namespace Ui {
    class BioSignalMLSaveDialog;
}

//==============================================================================

namespace OpenCOR {
namespace BSMLDataStore {

//==============================================================================

class BSMLDATASTORE_EXPORT BioSignalMLSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BioSignalMLSaveDialog(QWidget * pParent);
    ~BioSignalMLSaveDialog();

    virtual void retranslateUi();

    bool run(void);

    QString fileName(void) const;
    QString shortName(void) const;
    QString description(void) const;
    QString author(void) const;
    void setComment(const QString & pComment);

private slots:
    void accepted(void);
    void setFileName(bool checked = false);

private:
    Ui::BioSignalMLSaveDialog *mGui;

    bool mAccepted;
};

//==============================================================================

}   // namespace BSMLDataStore
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
