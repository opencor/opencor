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
// CellML annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::CellmlAnnotationViewMetadataViewDetailsWidget(CellmlAnnotationViewEditingWidget *pParent) :
    QStackedWidget(pParent),
    Core::CommonWidget(pParent),
    mCellmlFile(pParent->cellmlFile())
{
    // Create our different metadata views

    mRawView    = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pParent);
    mNormalView = new CellmlAnnotationViewMetadataNormalViewDetailsWidget(pParent);

    // Make our raw view the default widget
    // Note: for the GUI to be properly initialised, we must add and immediately
    //       remove the views that we don't yet need. Not to do that may mess
    //       things up in our parent...

    addWidget(mRawView);

    addWidget(mNormalView);
    removeWidget(mNormalView);
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mRawView->retranslateUi();
    mNormalView->retranslateUi();
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement)
{
    if (!pElement)
        return;

    // Decide on which view to use and update it, if needed

    switch (mCellmlFile->rdfTriples(pElement).type()) {
    case CellMLSupport::CellmlFileRdfTriple::BioModelsDotNetQualifier:
    case CellMLSupport::CellmlFileRdfTriple::Empty:
        removeWidget(mRawView);
        addWidget(mNormalView);

        mNormalView->updateGui(pElement);

        break;
    default:
        // Unknown type

        removeWidget(mNormalView);
        addWidget(mRawView);

        mRawView->updateGui(pElement);
    }
}

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget * CellmlAnnotationViewMetadataViewDetailsWidget::rawView() const
{
    // Return our raw view

    return mRawView;
}

//==============================================================================

CellmlAnnotationViewMetadataNormalViewDetailsWidget * CellmlAnnotationViewMetadataViewDetailsWidget::normalView() const
{
    // Return our normal view

    return mNormalView;
}

//==============================================================================

void CellmlAnnotationViewMetadataViewDetailsWidget::filePermissionsChanged()
{
    // Let our normal view widget know that the file has been un/locked
    // Note: we don't need to let our raw view widget know about it since it
    //       only presents information to the user...

    mNormalView->filePermissionsChanged();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
