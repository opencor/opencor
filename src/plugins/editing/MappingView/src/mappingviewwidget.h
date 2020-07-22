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

//==============================================================================

#include <QStringListModel>

//==============================================================================

namespace Ui {
    class MappingViewWidget;
} // namespace Ui

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

class MappingViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit MappingViewWidget(QWidget *pParent);
    ~MappingViewWidget() override;

    void retranslateUi() override;

    QWidget * widget(const QString &pFileName) override;

    void update(const QString &pFileName);

private:
    Ui::MappingViewWidget *mGui;

    CellMLSupport::CellmlFile *mCellmlFile;

    QStringListModel *mListViewModel;

    QString mFileName;

    void populateCellmlModel();

};

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
