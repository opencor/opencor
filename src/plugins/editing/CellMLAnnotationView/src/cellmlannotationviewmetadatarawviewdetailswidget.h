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
// CellML annotation view metadata raw view details widget
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
