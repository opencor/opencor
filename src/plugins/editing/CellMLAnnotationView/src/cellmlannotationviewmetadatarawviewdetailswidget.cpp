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
// CellML annotation view metadata raw view details widget
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

CellmlAnnotationViewMetadataRawViewDetailsWidget::CellmlAnnotationViewMetadataRawViewDetailsWidget(CellmlAnnotationViewEditingWidget *pParent) :
    TreeViewWidget(pParent),
    mCellmlFile(pParent->cellmlFile())
{
    // Customise ourselves

    mModel = new QStandardItemModel(this);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setModel(mModel);
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
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

    // Prevent ourselves from being updated (to avoid flickering)

    setUpdatesEnabled(false);

    // Remove all previous RDF triples from our tree view widget

    while (mModel->rowCount())
        foreach (QStandardItem *item, mModel->takeRow(0))
            delete item;

    // Add the 'new' RDF triples to our tree view widget
    // Note: for the RDF triple's subject, we try to remove the CellML file's
    //       URI base, thus only leaving the equivalent of a CellML element
    //       cmeta:id which will speak more to the user than a possibly long URI
    //       reference...

    int rdfTripleCounter = 0;

    foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, mCellmlFile->rdfTriples(pElement))
        mModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << new QStandardItem(QString::number(++rdfTripleCounter))
                                                                        << new QStandardItem((rdfTriple->subject()->type() == CellMLSupport::CellmlFileRdfTripleElement::UriReference)?
                                                                                                 rdfTriple->metadataId():
                                                                                                 rdfTriple->subject()->asString())
                                                                        << new QStandardItem(rdfTriple->predicate()->asString())
                                                                        << new QStandardItem(rdfTriple->object()->asString()));

    // Make sure that all the columns have their contents fit

    resizeColumnToContents(0);
    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeColumnToContents(3);

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
