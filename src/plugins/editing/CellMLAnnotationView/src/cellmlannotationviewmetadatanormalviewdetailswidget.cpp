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
// CellML annotation view metadata normal view details widget
//==============================================================================

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatawebviewwidget.h"
#include "cliutils.h"
#include "filemanager.h"
#include "guiutils.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatanormalviewdetailswidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QMenu>
//#include <QPoint>
//#include <QRegularExpression>
#include <QScrollArea>
#include <QStackedWidget>
#include <QWebElement>
#include <QWebFrame>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataNormalViewDetailsWidget::CellmlAnnotationViewMetadataNormalViewDetailsWidget(CellmlAnnotationViewEditingWidget *pParent) :
    Core::Widget(pParent),
    mCellmlFile(pParent->cellmlFile()),
    mGui(new Ui::CellmlAnnotationViewMetadataNormalViewDetailsWidget),
    mItemsCount(0),
    mElement(0),
    mRdfTripleInformation(QString()),
    mInformationType(None),
    mLookUpInformation(First),
    mVerticalScrollBarPosition(0),
    mNeighbourRow(0),
    mRdfTriplesMapping(QMap<QObject *, CellMLSupport::CellmlFileRdfTriple *>()),
    mUrls(QMap<QString, QString>()),
    mRdfTripleInformationSha1s(QStringList()),
    mRdfTripleInformationSha1(QString()),
    mLink(QString()),
    mTextContent(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionCopy);

    // Create an output widget that will contain our output message and output
    // for ontological terms

    mOutput = new Core::Widget(this);

    mOutput->setLayout(new QVBoxLayout(mOutput));

    mOutput->layout()->setMargin(0);

    // Create our output message (within a scroll area, in case the label is too
    // wide)

    mOutputMessageScrollArea = new QScrollArea(mOutput);

    mOutputMessageScrollArea->setFrameShape(QFrame::NoFrame);
    mOutputMessageScrollArea->setWidgetResizable(true);

    mOutputMessage = new Core::UserMessageWidget(":/oxygen/actions/help-about.png",
                                                 mOutputMessageScrollArea);

    mOutputMessageScrollArea->setWidget(mOutputMessage);

    // Create our output for ontological terms

    Core::readTextFromFile(":/ontologicalTerms.html", mOutputOntologicalTermsTemplate);

    mOutputOntologicalTerms = new CellmlAnnotationViewMetadataWebViewWidget(mOutput);

    mOutputOntologicalTerms->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mOutputOntologicalTerms, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    mOutputOntologicalTerms->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(mOutputOntologicalTerms->page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));

    // Add our output message and output for ontological terms to our output
    // widget

    mOutput->layout()->addWidget(mOutputMessageScrollArea);
    mOutput->layout()->addWidget(mOutputOntologicalTerms);

    // Add our output widget to our main layout

    mGui->layout->addWidget(mOutput);
}

//==============================================================================

CellmlAnnotationViewMetadataNormalViewDetailsWidget::~CellmlAnnotationViewMetadataNormalViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our output message

    mOutputMessage->setMessage(tr("There is no metadata associated with the current CellML element..."));

    // Retranslate our output for ontological terms

    updateOutputOntologicalTerms();
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::updateOutputOntologicalTerms()
{
    // Update our output for ontological terms

    QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();

    documentElement.findFirst("th[id=nameOrQualifier]").setInnerXml(tr("Qualifier"));
    documentElement.findFirst("th[id=resource]").setInnerXml(tr("Resource"));
    documentElement.findFirst("th[id=id]").setInnerXml(tr("Id"));

    QWebElement countElement = documentElement.findFirst("th[id=count]");

    if (mItemsCount == 1)
        countElement.setInnerXml(tr("(1 term)"));
    else
        countElement.setInnerXml(tr("(%1 terms)").arg(QLocale().toString(mItemsCount)));
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement,
                                                                    const QString &pRdfTripleInformation,
                                                                    const InformationType &pInformationType,
                                                                    const Information &pLookUpInformation)
{
    if (!pElement)
        return;

    // Keep track of the CellML element

    mElement = pElement;

    // Reset various properties
    // Note: we might only do that before adding new items, but then again there
    //       is no need to waste memory, so...

    mUrls.clear();
    mRdfTripleInformationSha1s.clear();

    mOutputOntologicalTerms->setHtml(QString());

    // Populate mOutputOntologicalTerms, but only if there is at least one RDF
    // triple

    CellMLSupport::CellmlFileRdfTriples rdfTriples = mCellmlFile->rdfTriples(pElement);
    QString firstRdfTripleInformation = QString();
    QString lastRdfTripleInformation = QString();

    if (rdfTriples.count()) {
        // Number of terms

        mItemsCount = rdfTriples.count();

        // Add the RDF triples

        static const QString indent = "                ";
        QString ontologicalTerms = QString();

        bool firstRdfTriple = true;

        foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, rdfTriples) {
            // Keep track of the item information and its SHA-1 value, as well
            // as of the qualifier and of the URLs for the resource and id

            QString qualifier = (rdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelUnknown)?
                                    rdfTriple->modelQualifierAsString():
                                    rdfTriple->bioQualifierAsString();
            QString rdfTripleInformation = qualifier+"|"+rdfTriple->resource()+"|"+rdfTriple->id();
            QString rdfTripleInformationSha1 = Core::sha1(rdfTripleInformation);
            QString resourceUrl = "http://identifiers.org/"+rdfTriple->resource()+"/?redirect=true";
            QString idUrl = "http://identifiers.org/"+rdfTriple->resource()+"/"+rdfTriple->id()+"/?profile=most_reliable&redirect=true";

            if (!mUrls.contains(rdfTriple->resource()))
                mUrls.insert(rdfTriple->resource(), resourceUrl);

            mUrls.insert(rdfTripleInformation, idUrl);

            mRdfTripleInformationSha1s << rdfTripleInformationSha1;

            // Keep track of the first and last RDF triple information

            if (firstRdfTriple) {
                firstRdfTripleInformation = rdfTripleInformation;

                firstRdfTriple = false;
            }

            lastRdfTripleInformation = rdfTripleInformation;

            // Add the item

//            mItemsMapping.insert(rdfTripleInformationSha1, item);
//            mEnabledItems.insert(rdfTripleInformationSha1, true);

            ontologicalTerms +=  indent+"<tr id=\"item_"+rdfTripleInformationSha1+"\">\n"
                                +indent+"    <td>\n"
                                +indent+"        <a href=\""+qualifier+"\">"+qualifier+"</a>\n"
                                +indent+"    </td>\n"
                                +indent+"    <td id=\"resource_"+rdfTripleInformationSha1+"\">\n"
                                +indent+"        <a href=\""+rdfTripleInformation+"\">"+rdfTriple->resource()+"</a>\n"
                                +indent+"    </td>\n"
                                +indent+"    <td id=\"id_"+rdfTripleInformationSha1+"\">\n"
                                +indent+"        <a href=\""+rdfTripleInformation+"\">"+rdfTriple->id()+"</a>\n"
                                +indent+"    </td>\n"
                                +indent+"    <td id=\"button_"+rdfTripleInformationSha1+"\">\n"
                                +indent+"        <a class=\"noHover\" href=\""+rdfTripleInformationSha1+"\"><img class=\"button\"/></a>\n"
                                +indent+"    </td>\n"
                                +indent+"    <td id=\"disabledButton_"+rdfTripleInformationSha1+"\" style=\"display: none;\">\n"
                                +indent+"        <img class=\"disabledButton\"/>\n"
                                +indent+"    </td>\n"
                                +indent+"</tr>\n";
        }

        mOutputOntologicalTerms->setHtml(mOutputOntologicalTermsTemplate.arg(Core::iconDataUri(":/oxygen/actions/list-remove.png", 16, 16),
                                                                             Core::iconDataUri(":/oxygen/actions/list-remove.png", 16, 16, QIcon::Disabled),
                                                                             ontologicalTerms));

        updateOutputOntologicalTerms();
    }

    // Hide our old output widget and show our new one

    mOutputMessageScrollArea->setVisible(!rdfTriples.count());
    mOutputOntologicalTerms->setVisible(rdfTriples.count());

    // Request for something to be looked up, if needed

    if (pLookUpInformation != No) {
        if (rdfTriples.count()) {
            // Request for the first resource id, the last resource id or an
            // 'old' qualifier, resource or resource id to be looked up

            if (pLookUpInformation == First)
                // Look up the first resource id

                genericLookUp(firstRdfTripleInformation, Id);
            else if (pLookUpInformation == Last)
                // Look up the last resource id

                genericLookUp(lastRdfTripleInformation, Id);
            else
                // Look up any 'old' qualifier, resource or resource id

                genericLookUp(pRdfTripleInformation, pInformationType);
        } else {
            // No RDF triple left, so ask for 'nothing' to be looked up
            // Note: we do this to let people know that there is nothing to look
            //       up and that they can 'clean' whatever they use to show a
            //       look up to the user...

            genericLookUp();
        }
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple)
{
    if (!pRdfTriple)
        return;

    // Make sure that the given RDF triple will be visible, this by handling the
    // change in the range of our vertical scroll bar which will result in
    // showLastRdfTriple() being called

//    connect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
//            this, SLOT(showLastRdfTriple()));

    // Enable the looking up of the last information

    mLookUpInformation = Last;

    // Update the GUI to reflect the addition of the given RDF triple

    updateGui(mElement, QString(), None, mLookUpInformation);
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::genericLookUp(const QString &pRdfTripleInformation,
                                                                        const InformationType &pInformationType)
{
    // Retrieve the RDF triple information

    QStringList rdfTripleInformationAsStringList = pRdfTripleInformation.split("|");
    QString qualifierAsString = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformationAsStringList[0];
    QString resourceAsString = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformationAsStringList[1];
    QString idAsString = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformationAsStringList[2];

    // Keep track of the RDF triple information and type

    mRdfTripleInformation = pRdfTripleInformation;

    // (Un)highlight/(un)select our various RDF triple information

    static const QString Highlighted = "highlighted";
    static const QString Selected = "selected";

    QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();
    QString rdfTripleInformationSha1 = pRdfTripleInformation.isEmpty()?QString():Core::sha1(pRdfTripleInformation);

    if (rdfTripleInformationSha1.compare(mRdfTripleInformationSha1)) {
        if (!mRdfTripleInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(mRdfTripleInformationSha1)).removeClass(Highlighted);

            if (mInformationType == Qualifier)
                documentElement.findFirst(QString("td[id=qualifier_%1]").arg(mRdfTripleInformationSha1)).removeClass(Selected);
            else if (mInformationType == Resource)
                documentElement.findFirst(QString("td[id=resource_%1]").arg(mRdfTripleInformationSha1)).removeClass(Selected);
            else if (mInformationType == Id)
                documentElement.findFirst(QString("td[id=id_%1]").arg(mRdfTripleInformationSha1)).removeClass(Selected);
        }

        if (!rdfTripleInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(rdfTripleInformationSha1)).addClass(Highlighted);

            if (pInformationType == Qualifier)
                documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            else if (pInformationType == Resource)
                documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            else if (pInformationType == Id)
                documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
        }

        mRdfTripleInformationSha1 = rdfTripleInformationSha1;
    } else if (!rdfTripleInformationSha1.isEmpty()) {
        if (pInformationType == Qualifier) {
            documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
        } else if (pInformationType == Resource) {
            documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
        } else if (pInformationType == Id) {
            documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
        }
    }

    mInformationType = pInformationType;

    // Check whether we have something to look up

    if (mLookUpInformation == No)
        // Nothing to look up, so...

        return;

    // Let people know that we want to look something up

    switch (pInformationType) {
    case Qualifier:
        emit qualifierLookUpRequested(qualifierAsString);

        break;
    case Resource:
        emit resourceLookUpRequested(resourceAsString);

        break;
    case Id:
        emit idLookUpRequested(resourceAsString, idAsString);

        break;
    default:
        // None

        emit noLookUpRequested();
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation()
{
    // Disable the looking up of information

    mLookUpInformation = No;

    // Update the GUI by pretending to be interested in looking something up

    genericLookUp();
}

//==============================================================================

QString CellmlAnnotationViewMetadataNormalViewDetailsWidget::rdfTripleInformation(const int &pRow) const
{
Q_UNUSED(pRow);
    // Return the RDF triple information for the given row

    QString res = QString();

//    // Retrieve the item for the first label link from the given row

//    QLayoutItem *item = mGridLayout->itemAtPosition(pRow, 0);

//    if (!item)
//        // No item could be retrieved, so...

//        return res;

//    // Retrieve the text from the item's widget which is a QLabel

//    res = static_cast<QLabel *>(item->widget())->text();

//    // Extract the RDF triple information from the text

//    res.remove(QRegularExpression("^[^\"]*\""));
//    res.remove(QRegularExpression("\"[^\"]*$"));

    // We are done with retrieving the RDF triple information for the given row,
    // so...

    return res;
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::removeRdfTriple()
{
    // Retrieve the RDF triple associated with the remove button

    CellMLSupport::CellmlFileRdfTriple *rdfTriple = mRdfTriplesMapping.value(sender());

    mRdfTriplesMapping.remove(sender());

    // Remove the RDF triple from the CellML file and from our set of RDF
    // triples this widget uses

    mCellmlFile->rdfTriples().remove(rdfTriple);

    // Retrieve the number of the row we want to delete, as well as the total
    // number of rows
    // Note: should some RDF triples have been removed, then to call
    //       mGridLayout->rowCount() won't give us the correct number of rows,
    //       so...

//    int row = 0;
//    int rowMax = mGridLayout->rowCount();

//    for (int i = 1; i < rowMax; ++i) {
//        QLayoutItem *item = mGridLayout->itemAtPosition(i, 3);

//        if (!item) {
//            // The row doesn't exist anymore, so...

//            rowMax = i;

//            break;
//        }

//        if (item->widget() == sender())
//            // This is the row we want to remove

//            row = i;
//    }

    // Make sure that row and rowMax have meaningful values

//    Q_ASSERT(row > 0);
//    Q_ASSERT(rowMax > row);

    // Determine the neighbour row which we want to be visible

//    mNeighbourRow = (rowMax-1 > row)?row:row-1;

//    // Determine the 'new' RDF triple information to look up, depending on
//    // whether there is any RDF triple left

//    if (mRdfTriplesMapping.isEmpty()) {
//        // There are no RDF triples left, so...

//        mRdfTripleInformation = QString();
//        mType = None;
//    } else if (!rdfTripleInformation(row).compare(mRdfTripleInformation)) {
//        // The RDF triple information is related to the row we want to delete,
//        // so we need to find a new one

//        mRdfTripleInformation = rdfTripleInformation((rowMax-1 > row)?row+1:row-1);
//    }

    // Make sure that the neighbour of our removed RDF triple will be made
    // visible and we do this by handling the change in the range of our
    // vertical scroll bar which will result in showNeighbourRdfTriple() being
    // called

//    connect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
//            this, SLOT(showNeighbourRdfTriple()));

    // Update the GUI to reflect the removal of the RDF triple

    updateGui(mElement, mRdfTripleInformation, mInformationType, mLookUpInformation);

    // Let people know that an RDF triple has been removed

    emit rdfTripleRemoved(rdfTriple);
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::showNeighbourRdfTriple()
{
//    // No need to show our neighbour RDF triple, so...

//    disconnect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
//               this, SLOT(showNeighbourRdfTriple()));

//    // Make sure that the neighbour RDF triple is visible

//    ensureWidgetVisible(mGridLayout->itemAtPosition(mNeighbourRow, 0)->widget());
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::showLastRdfTriple()
{
//    // No need to show our last RDF triple, so...

//    disconnect(verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
//               this, SLOT(showLastRdfTriple()));

//    // Determine the number of rows in our grid layout
//    // Note: to use mGridLayout->rowCount() isn't an option since no matter
//    //       whether we remove rows (in updateGui()), the returned value will be
//    //       the maximum number of rows that there has ever been, so...

//    int row = 0;

//    while (mGridLayout->itemAtPosition(++row, 0))
//        ;

//    // Make sure that the last RDF triple is visible

//    ensureWidgetVisible(mGridLayout->itemAtPosition(row-1, 0)->widget());
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::linkClicked()
{
    // Retrieve some information about the link

    mOutputOntologicalTerms->retrieveLinkInformation(mLink, mTextContent);

    // Check whether we have clicked a resource/id link or a button link

    if (mTextContent.isEmpty()) {
        // We have clicked on a button link, so remove the RDF triple associated
        // with it

//---GRY--- TO BE DONE...
    } else {
        // We have clicked on a qualifier/resource/id link, so start by enabling
        // the looking up of any information

        mLookUpInformation = Any;

        // Call our generic look up function

        genericLookUp(mLink,
                      (!mLink.compare(mTextContent))?
                          Qualifier:
                          mUrls.contains(mTextContent)?
                              Resource:
                              Id);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::showCustomContextMenu(const QPoint &pPosition)
{
    Q_UNUSED(pPosition);

    // Retrieve some information about the link

    mOutputOntologicalTerms->retrieveLinkInformation(mLink, mTextContent);

    // Show our context menu to allow the copying of the URL of the resource or
    // id, but only if we are over a link, i.e. if both mLink and mTextContent
    // are not empty

    if (!mLink.isEmpty() && !mTextContent.isEmpty())
        mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::on_actionCopy_triggered()
{
    // Copy the qualifier or the URL of the resource or id to the clipboard

    if (!mLink.compare(mTextContent))
        QApplication::clipboard()->setText(mLink);
    else if (mUrls.contains(mTextContent))
        QApplication::clipboard()->setText(mUrls.value(mTextContent));
    else
        QApplication::clipboard()->setText(mUrls.value(mLink));
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
