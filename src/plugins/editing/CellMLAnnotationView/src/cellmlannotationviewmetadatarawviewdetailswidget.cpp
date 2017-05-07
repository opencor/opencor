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

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatarawviewdetailswidget.h"

//==============================================================================

#include <QAbstractItemView>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataRawViewDetailsWidget::CellmlAnnotationViewMetadataRawViewDetailsWidget(CellMLSupport::CellmlFile *pCellmlFile,
                                                                                                   QWidget *pParent) :
    TreeViewWidget(pParent),
    mCellmlFile(pCellmlFile)
{
    // Customise ourselves

    mModel = new QStandardItemModel(this);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setModel(mModel);
    setRootIsDecorated(false);
}

//==============================================================================

void CellmlAnnotationViewMetadataRawViewDetailsWidget::retranslateUi()
{
    // Update our header labels

    mModel->setHorizontalHeaderLabels(QStringList() << tr("#")
                                                    << tr("Subject")
                                                    << tr("Predicate")
                                                    << tr("Object"));
}

//==============================================================================

void CellmlAnnotationViewMetadataRawViewDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement)
{
    if (!pElement)
        return;

    // Remove all previous RDF triples from our tree view widget

    while (mModel->hasChildren()) {
        foreach (QStandardItem *item, mModel->takeRow(0))
            delete item;
    }

    // Add the 'new' RDF triples to our tree view widget
    // Note: for the RDF triple's subject, we try to remove the CellML file's
    //       base URI, thus only leaving the equivalent of a CellML element
    //       cmeta:id which will speak more to the user than a possibly long URI
    //       reference...

    int rdfTripleCounter = 0;

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, mCellmlFile->rdfTriples(pElement)) {
        mModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(++rdfTripleCounter))
                                                                        << new QStandardItem((rdfTriple->subject()->type() == CellMLSupport::CellmlFileRdfTripleElement::UriReference)?
                                                                                                 rdfTriple->metadataId():
                                                                                                 rdfTriple->subject()->asString())
                                                                        << new QStandardItem(rdfTriple->predicate()->asString())
                                                                        << new QStandardItem(rdfTriple->object()->asString()));
    }

    // Make sure that all the columns have their contents fit

    resizeColumnToContents(0);
    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeColumnToContents(3);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
