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
// Mapping view widget
//==============================================================================

#pragma once

//==============================================================================

#include "viewwidget.h"
#include "cellmlfile.h"
#include "splitterwidget.h"

//==============================================================================

#include <QStringListModel> //TODO remove when over
#include <QTableView>
#include <QStandardItemModel>

//==============================================================================

namespace Ui {
    class MappingViewWidget;
} // namespace Ui

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

class MappingViewEditingWidget : public Core::SplitterWidget
{
    Q_OBJECT

public:

    explicit MappingViewEditingWidget(const QString &pCellmlFileName,
                                      const QString &pMeshFileName,
                                               QWidget *pParent);

    void retranslateUi() override;

    CellMLSupport::CellmlFile * cellmlFile() const;

    QStringListModel *listViewModelVariables();

    QStringListModel *listViewModelOutput();

    QStandardItemModel *getTreeViewModel();

    void filePermissionsChanged();

    QString getVariableOfNode(int pNodeId);

    void AddVariableToNode(QString pVariable, int pNodeId);

private:

    CellMLSupport::CellmlFile *mCellmlFile;

    //TODO : * or not ?
    QMap<int, QString> mMapMatch;

    QStandardItemModel *mTreeViewModel;

    QStringListModel
        *mListViewModelVariables = nullptr,//TODO temporary
        *mListViewModelOutput = nullptr;//TODO temporary

    void populateCellmlModel();
    void populateOutput(const QString &pMeshFileName);
};

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
