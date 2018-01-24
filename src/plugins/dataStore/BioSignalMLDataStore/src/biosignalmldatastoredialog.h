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
