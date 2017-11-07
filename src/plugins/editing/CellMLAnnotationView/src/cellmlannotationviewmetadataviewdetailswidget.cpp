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
// CellML Annotation view metadata view details widget
//==============================================================================

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatarawviewdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataViewDetailsWidget::CellmlAnnotationViewMetadataViewDetailsWidget(CellMLSupport::CellmlFile *pCellmlFile,
                                                                                             QWidget *pParent) :
    QStackedWidget(pParent),
    Core::CommonWidget(this),
    mCellmlFile(pCellmlFile)
{
    // Create our different metadata views

    mRawView = new CellmlAnnotationViewMetadataRawViewDetailsWidget(pCellmlFile, this);
    mNormalView = new CellmlAnnotationViewMetadataNormalViewDetailsWidget(pCellmlFile, this);

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
    case CellMLSupport::CellmlFileRdfTriple::Unknown:
        removeWidget(mNormalView);
        addWidget(mRawView);

        mRawView->updateGui(pElement);

        break;
    case CellMLSupport::CellmlFileRdfTriple::BioModelsDotNetQualifier:
    case CellMLSupport::CellmlFileRdfTriple::Empty:
        removeWidget(mRawView);
        addWidget(mNormalView);

        mNormalView->updateGui(pElement);

        break;
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
