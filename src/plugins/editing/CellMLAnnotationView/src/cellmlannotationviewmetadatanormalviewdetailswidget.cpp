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
#include <QScrollArea>
#include <QStackedWidget>
#include <QString>
#include <QTimer>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>

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
    mLookUpRdfTripleInformation(First),
    mVerticalScrollBarPosition(0),
    mNeighbourRow(0),
    mRdfTriplesMapping(QMap<QString, CellMLSupport::CellmlFileRdfTriple *>()),
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
                                                                    const Information &pLookUpRdfTripleInformation)
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

    mRdfTripleInformationSha1 = QString();

    mRdfTriplesMapping.clear();


    // Populate mOutputOntologicalTerms, but only if there is at least one RDF
    // triple

    CellMLSupport::CellmlFileRdfTriples rdfTriples = mCellmlFile->rdfTriples(pElement);
    QString firstRdfTripleInformation = QString();
    QString lastRdfTripleInformation = QString();

    if (rdfTriples.count()) {
        // Number of terms

        mItemsCount = rdfTriples.count();

        // Add the RDF triples

        QString ontologicalTerms = QString();

        bool firstRdfTriple = true;

        foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, rdfTriples) {
            // Keep track of some information

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

            mRdfTriplesMapping.insert(rdfTripleInformationSha1, rdfTriple);

            // Keep track of the first and last RDF triple information

            if (firstRdfTriple) {
                firstRdfTripleInformation = rdfTripleInformation;

                firstRdfTriple = false;
            }

            lastRdfTripleInformation = rdfTripleInformation;

            // Add the item

            ontologicalTerms +=  "<tr id=\"item_"+rdfTripleInformationSha1+"\">\n"
                                +"    <td id=\"qualifier_"+rdfTripleInformationSha1+"\">\n"
                                +"        <a href=\""+rdfTripleInformation+"\">"+qualifier+"</a>\n"
                                +"    </td>\n"
                                +"    <td id=\"resource_"+rdfTripleInformationSha1+"\">\n"
                                +"        <a href=\""+rdfTripleInformation+"\">"+rdfTriple->resource()+"</a>\n"
                                +"    </td>\n"
                                +"    <td id=\"id_"+rdfTripleInformationSha1+"\">\n"
                                +"        <a href=\""+rdfTripleInformation+"\">"+rdfTriple->id()+"</a>\n"
                                +"    </td>\n"
                                +"    <td id=\"button_"+rdfTripleInformationSha1+"\">\n"
                                +"        <a class=\"noHover\" href=\""+rdfTripleInformationSha1+"\"><img class=\"button\"/></a>\n"
                                +"    </td>\n"
                                +"    <td id=\"disabledButton_"+rdfTripleInformationSha1+"\" style=\"display: none;\">\n"
                                +"        <img class=\"disabledButton\"/>\n"
                                +"    </td>\n"
                                +"</tr>\n";
        }

        mOutputOntologicalTerms->setHtml(mOutputOntologicalTermsTemplate.arg(Core::iconDataUri(":/oxygen/actions/list-remove.png", 16, 16),
                                                                             Core::iconDataUri(":/oxygen/actions/list-remove.png", 16, 16, QIcon::Disabled),
                                                                             ontologicalTerms));

        updateOutputOntologicalTerms();
    } else {
        mOutputOntologicalTerms->setHtml(QString());
    }

    // Hide our old output widget and show our new one

    mOutputMessageScrollArea->setVisible(!rdfTriples.count());
    mOutputOntologicalTerms->setVisible(rdfTriples.count());

    // Request for something to be looked up, if needed

    if (pLookUpRdfTripleInformation != No) {
        if (rdfTriples.count()) {
            // Request for the first resource id, the last resource id or an
            // 'old' qualifier, resource or resource id to be looked up

            if (pLookUpRdfTripleInformation == First)
                // Look up the first resource id

                genericLookUp(firstRdfTripleInformation, Id);
            else if (pLookUpRdfTripleInformation == Last)
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

    // Enable the looking up of the last RDF triple information

    mLookUpRdfTripleInformation = Last;

    // Update the GUI to reflect the addition of the given RDF triple

    updateGui(mElement, QString(), None, mLookUpRdfTripleInformation);
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::genericLookUp(const QString &pRdfTripleInformation,
                                                                        const InformationType &pInformationType)
{
    // Retrieve the RDF triple information

    QStringList rdfTripleInformation = pRdfTripleInformation.split("|");
    QString qualifier = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformation[0];
    QString resource = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformation[1];
    QString id = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformation[2];

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
    // Note: there is nothing nothing do for Any...

    if (mLookUpRdfTripleInformation == First)
        mOutputOntologicalTerms->page()->triggerAction(QWebPage::MoveToStartOfDocument);
    else if (mLookUpRdfTripleInformation == Last)
        // Note #1: normally, we would use
        //             mOutputOntologicalTerms->page()->triggerAction(QWebPage::MoveToEndOfDocument);
        //          but this doesn't work...
        // Note #2: another option would be to use
        //              QWebFrame *outputFrame = mOutputOntologicalTerms->page()->mainFrame();
        //
        //              outputFrame->setScrollBarValue(Qt::Vertical, outputFrame->scrollBarMaximum(Qt::Vertical));
        //          but this doesnt' get us exactly to the bottom of the page...

        QTimer::singleShot(1, this, SLOT(showLastRdfTriple()));
    else if (mLookUpRdfTripleInformation == No)
        return;

    // Let people know that we want to look something up

    switch (pInformationType) {
    case Qualifier:
        emit qualifierLookUpRequested(qualifier);

        break;
    case Resource:
        emit resourceLookUpRequested(resource);

        break;
    case Id:
        emit idLookUpRequested(resource, id);

        break;
    default:
        // None

        emit noLookUpRequested();
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation()
{
    // Disable the looking up of RDF triple information

    mLookUpRdfTripleInformation = No;

    // Update the GUI by pretending to be interested in looking something up

    genericLookUp();
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
    // Show our last RDF triple by scrolling to the end of the page

    QWebFrame *outputFrame = mOutputOntologicalTerms->page()->mainFrame();

    outputFrame->setScrollBarValue(Qt::Vertical, outputFrame->scrollBarMaximum(Qt::Vertical));
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::linkClicked()
{
    // Retrieve some information about the link

    mOutputOntologicalTerms->retrieveLinkInformation(mLink, mTextContent);

    // Check whether we have clicked a resource/id link or a button link

    if (mTextContent.isEmpty()) {
        // We have clicked on a button link, so retrieve the RDF triple
        // associated with it

        CellMLSupport::CellmlFileRdfTriple *rdfTriple = mRdfTriplesMapping.value(mLink);

        mRdfTriplesMapping.remove(mLink);

        // Remove the RDF triple from the CellML file and from our set of RDF
        // triples this widget uses

        mCellmlFile->rdfTriples().remove(rdfTriple);

        // Determine the 'new' RDF triple information to look up, depending on
        // whether there is any RDF triple left

        if (mRdfTriplesMapping.isEmpty()) {
            mRdfTripleInformation = QString();
            mInformationType = None;
        } else if (!mLink.compare(mRdfTripleInformationSha1)) {
            QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();
            QWebElement currentRdfTripleElement = documentElement.findFirst(QString("tr[id=item_%1]").arg(mLink));
            QWebElement neighbouringRdfTripleEment = currentRdfTripleElement.nextSibling();

            if (neighbouringRdfTripleEment.isNull())
                neighbouringRdfTripleEment = currentRdfTripleElement.previousSibling();

            CellMLSupport::CellmlFileRdfTriple *neighbouringRdfTriple = mRdfTriplesMapping.value(neighbouringRdfTripleEment.attribute("id").remove(QRegularExpression("^item_")));
            QString qualifier = (neighbouringRdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelUnknown)?
                                    neighbouringRdfTriple->modelQualifierAsString():
                                    neighbouringRdfTriple->bioQualifierAsString();

            mRdfTripleInformation = qualifier+"|"+neighbouringRdfTriple->resource()+"|"+neighbouringRdfTriple->id();
        }

        // Update the GUI to reflect the removal of the RDF triple

        QWebFrame *outputFrame = mOutputOntologicalTerms->page()->mainFrame();
        int horizontalScrollBarValue = outputFrame->scrollBarValue(Qt::Horizontal);
        int verticalScrollBarValue = outputFrame->scrollBarValue(Qt::Vertical);

        updateGui(mElement, mRdfTripleInformation, mInformationType, mLookUpRdfTripleInformation);

        outputFrame->setScrollBarValue(Qt::Horizontal, horizontalScrollBarValue);
        outputFrame->setScrollBarValue(Qt::Vertical, verticalScrollBarValue);

        // Let people know that an RDF triple has been removed

        emit rdfTripleRemoved(rdfTriple);
    } else {
        // We have clicked on a qualifier/resource/id link, so start by enabling
        // the looking up of any RDF triple information

        mLookUpRdfTripleInformation = Any;

        // Call our generic look up function

        QStringList rdfTripleInformation = mLink.split("|");

        genericLookUp(mLink,
                      (!rdfTripleInformation[0].compare(mTextContent))?
                          Qualifier:
                          !rdfTripleInformation[1].compare(mTextContent)?
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

    QStringList rdfTripleInformation = mLink.split("|");

    if (!rdfTripleInformation[0].compare(mTextContent))
        QApplication::clipboard()->setText(mTextContent);
    else if (!rdfTripleInformation[1].compare(mTextContent))
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
