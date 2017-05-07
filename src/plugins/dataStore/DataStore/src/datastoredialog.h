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
// Data store dialog
//==============================================================================

#pragma once

//==============================================================================

#include "coreguiutils.h"
#include "datastoreglobal.h"
#include "datastoreinterface.h"

//==============================================================================

#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class DataStoreDialog;
}

//==============================================================================

class QStandardItem;
class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStore;

//==============================================================================

class DataItemDelegate : public QStyledItemDelegate
{
public:
    explicit DataItemDelegate(QObject *pParent);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class DATASTORE_EXPORT DataStoreDialog : public Core::Dialog
{
    Q_OBJECT

public:
    explicit DataStoreDialog(DataStore *pDataStore, const bool &pIncludeVoi,
                             QWidget *pParent);
    ~DataStoreDialog();

    void addWidget(QWidget *pWidget);

    DataStoreVariables selectedData() const;

private:
    Ui::DataStoreDialog *mGui;

    QStandardItemModel *mModel;

    QMap<QStandardItem *, DataStoreVariable*> mData;
    int mNbOfData;

    DataStoreVariables doSelectedData(QStandardItem *pItem) const;

    void updateDataSelectedState(QStandardItem *pItem,
                                 const Qt::CheckState &pCheckState);
    void checkDataSelectedState(QStandardItem *pItem, int &pNbOfselectedData);

private slots:
    void updateDataSelectedState(QStandardItem *pItem = 0);

    void on_allDataCheckBox_clicked();
};

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
