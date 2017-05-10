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
// CellML Annotation view metadata raw view details widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "treeviewwidget.h"

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewMetadataRawViewDetailsWidget : public Core::TreeViewWidget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataRawViewDetailsWidget(CellMLSupport::CellmlFile *pCellmlFile,
                                                              QWidget *pParent);

    virtual void retranslateUi();

    void updateGui(iface::cellml_api::CellMLElement *pElement);

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    QStandardItemModel *mModel;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
