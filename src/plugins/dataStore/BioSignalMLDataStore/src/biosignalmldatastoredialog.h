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
// BioSignalML data store dialog
//==============================================================================

#pragma once

//==============================================================================

#include "datastoredialog.h"

//==============================================================================

class QLabel;
class QTextEdit;

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

class BiosignalmlDataStoreDialog : public DataStore::DataStoreDialog
{
    Q_OBJECT

public:
    explicit BiosignalmlDataStoreDialog(DataStore::DataStore *pDataStore,
                                        QWidget *pParent);

    QString name() const;
    QString author() const;
    QString description() const;

private:
    QLineEdit *mNameValue;
    QLineEdit *mAuthorValue;
    QTextEdit *mDescriptionValue;

    QLabel * boldLabel(const QString &pText);
};

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
