/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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

namespace Ui {
    class DataStoreDialog;
} // namespace Ui

//==============================================================================

class QStandardItem;
class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataItemDelegate : public Core::StyledItemDelegate
{
public:
    explicit DataItemDelegate(QObject *pParent);

    void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
               const QModelIndex &pIndex) const override;
};

//==============================================================================

class DataStore;

//==============================================================================

class DATASTORE_EXPORT DataStoreDialog : public Core::Dialog
{
    Q_OBJECT

public:
    explicit DataStoreDialog(const QString &pDataStoreName,
                             DataStore *pDataStore, bool pIncludeVoi,
                             const QMap<int, QIcon> &pIcons, QWidget *pParent);
    ~DataStoreDialog() override;

    void addWidget(QWidget *pWidget);

    DataStoreVariables selectedData() const;

private:
    Ui::DataStoreDialog *mGui;

    QStandardItemModel *mModel;

    QMap<QStandardItem *, DataStoreVariable *> mData;
    int mNbOfData = 0;

    DataStoreVariables selectedData(QStandardItem *pItem) const;

    void checkDataSelectedState(QStandardItem *pItem, int &pNbOfselectedData);

    void updateDataSelectedState(QStandardItem *pItem,
                                 const Qt::CheckState &pCheckState);

private slots:
    void allDataCheckBoxClicked();

    void updateDataSelectedState(QStandardItem *pItem);
};

//==============================================================================

} // namespace DataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
