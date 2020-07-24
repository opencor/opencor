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
    explicit MappingViewEditingWidget(const QString &pFileName,
                                               QWidget *pParent);

    void retranslateUi() override;

    CellMLSupport::CellmlFile * cellmlFile() const;

    QStringListModel *listViewModelVariables();

    QStringListModel *listViewModelOutput();

    void filePermissionsChanged();
private:

    CellMLSupport::CellmlFile *mCellmlFile;

    QStringListModel
        *mListViewModelVariables,//TODO temporary
        *mListViewModelOutput;//TODO temporary

    //QSqlRelationalTableModel *mTableModel;

    QString mFileName;
    QString mOutputFileName;

    void populateCellmlModel();
    void populateOutput();
};

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
