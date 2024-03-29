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
// CellML Annotation view metadata normal view details widget
//==============================================================================

#include "cellmlannotationview.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "i18ninterface.h"
#include "usermessagewidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QLayout>
#include <QMenu>
#include <QRegularExpression>
#include <QString>
#include <QTimer>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataNormalViewDetailsWidget::CellmlAnnotationViewMetadataNormalViewDetailsWidget(CellMLSupport::CellmlFile *pCellmlFile,
                                                                                                         QWidget *pParent) :
    Core::Widget(pParent),
    mCellmlFile(pCellmlFile)
{
    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mCopyAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                  this);

    connect(mCopyAction, &QAction::triggered,
            this, &CellmlAnnotationViewMetadataNormalViewDetailsWidget::copy);

    mContextMenu->addAction(mCopyAction);

    // Create an output widget that will contain our output message and output
    // for ontological terms

    mOutput = new Core::Widget(this);

    // Create our output message

    mOutputMessage = new Core::UserMessageWidget(":/oxygen/actions/help-about.png", this);

    // Create our output for ontological terms

    Core::readFile(":/CellMLAnnotationView/ontologicalTerms.html", mOutputOntologicalTermsTemplate);

    mOutputOntologicalTerms = new WebViewerWidget::WebViewerWidget(mOutput);

    mOutputOntologicalTerms->webView()->setContextMenuPolicy(Qt::CustomContextMenu);
    mOutputOntologicalTerms->setOverrideCursor(true);
    mOutputOntologicalTerms->setZoomingEnabled(false);

    connect(mOutputOntologicalTerms->webView(), &QWebView::customContextMenuRequested,
            this, &CellmlAnnotationViewMetadataNormalViewDetailsWidget::showCustomContextMenu);

    mOutputOntologicalTerms->webView()->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(mOutputOntologicalTerms->webView()->page(), &QWebPage::linkClicked,
            this, &CellmlAnnotationViewMetadataNormalViewDetailsWidget::linkClicked);
    connect(mOutputOntologicalTerms->webView()->page(), &QWebPage::linkHovered,
            this, &CellmlAnnotationViewMetadataNormalViewDetailsWidget::linkHovered);

    // Add our output message and output for ontological terms to our output
    // widget

    QLayout *outputLayout = mOutput->createLayout();

    outputLayout->addWidget(mOutputMessage);
    outputLayout->addWidget(mOutputOntologicalTerms);

    // Add our output widget to our main layout

    createLayout()->addWidget(mOutput);
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::retranslateUi()
{
    // Retranslate our action

    I18nInterface::retranslateAction(mCopyAction, tr("Copy"),
                                     tr("Copy the URL to the clipboard"));

    // Retranslate our output message

    mOutputMessage->setMessage(tr("There is no metadata associated with the current CellML element..."));

    // Retranslate our output headers

    updateOutputHeaders();
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::updateOutputHeaders()
{
    // Update our output headers

    QWebElement documentElement = mOutputOntologicalTerms->webView()->page()->mainFrame()->documentElement();

    documentElement.findFirst("th[id=nameOrQualifier]").setInnerXml(tr("Qualifier"));
    documentElement.findFirst("th[id=resource]").setInnerXml(tr("Resource"));
    documentElement.findFirst("th[id=id]").setInnerXml(tr("Id"));

    QWebElement countElement = documentElement.findFirst("th[id=count]");

    if (mItemsCount == 1) {
        countElement.setInnerXml(tr("(1 term)"));
    } else {
        countElement.setInnerXml(tr("(%1 terms)").arg(QLocale().toString(mItemsCount)));
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::additionalGuiUpdates(const QString &pRdfTripleInformation,
                                                                               InformationType pInformationType,
                                                                               Information pLookUpRdfTripleInformation)
{
    // Update our output headers

    updateOutputHeaders();

    // Show/hide our output message and output for ontological terms

    mOutputMessage->setVisible(mItemsCount == 0);
    mOutputOntologicalTerms->setVisible(mItemsCount != 0);

    // Request for something to be looked up, if needed

    if (pLookUpRdfTripleInformation != Information::No) {
        if (mItemsCount != 0) {
            // Request for the first resource id, the last resource id or an
            // 'old' qualifier, resource or resource id to be looked up

            if (pLookUpRdfTripleInformation == Information::First) {
                // Look up the first resource id

                genericLookUp(mFirstRdfTripleInformation, InformationType::Id);
            } else if (pLookUpRdfTripleInformation == Information::Last) {
                // Look up the last resource id

                genericLookUp(mLastRdfTripleInformation, InformationType::Id);
            } else {
                // Look up any 'old' qualifier, resource or resource id

                genericLookUp(pRdfTripleInformation, pInformationType);
            }
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

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement,
                                                                    const QString &pRdfTripleInformation,
                                                                    InformationType pInformationType,
                                                                    Information pLookUpRdfTripleInformation)
{
    if (pElement == nullptr) {
        return;
    }

    // Keep track of the CellML element

    mElement = pElement;

    // Reset various properties
    // Note: we might only do that before adding new items, but then again there
    //       is no need to waste memory...

    mUrls.clear();
    mRdfTripleInformationSha1s.clear();

    mRdfTripleInformationSha1 = QString();

    mRdfTriplesMapping.clear();

    mItemsCount = 0;

    mFirstRdfTripleInformation = QString();
    mLastRdfTripleInformation = QString();

    // Populate our Web view, but only if there is at least one RDF triple

    const CellMLSupport::CellmlFileRdfTriples rdfTriples = mCellmlFile->rdfTriples(pElement);

    if (!rdfTriples.isEmpty()) {
        // Add the RDF triples

        for (auto rdfTriple : rdfTriples) {
            addRdfTriple(rdfTriple, false);
        }
    } else {
        mOutputOntologicalTerms->webView()->setHtml({});
    }

    // Do additional GUI updates

    additionalGuiUpdates(pRdfTripleInformation, pInformationType,
                         pLookUpRdfTripleInformation);
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::addRdfTriple(CellMLSupport::CellmlFileRdfTriple *pRdfTriple,
                                                                       bool pNeedAdditionalGuiUpdates)
{
    if (pRdfTriple == nullptr) {
        return;
    }

    // Initialise our Web view, if needed

    if (mItemsCount == 0) {
        int iconSize = int(16/qApp->devicePixelRatio());

        mOutputOntologicalTerms->webView()->setHtml(mOutputOntologicalTermsTemplate.arg(Core::iconDataUri(":/oxygen/actions/list-remove.png", iconSize, iconSize),
                                                                                        Core::iconDataUri(":/oxygen/actions/list-remove.png", iconSize, iconSize, QIcon::Disabled)));
    }

    // Add the item

    ++mItemsCount;

    QString qualifier = (pRdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelQualifier::ModelUnknown)?
                            pRdfTriple->modelQualifierAsString():
                            pRdfTriple->bioQualifierAsString();
    QString rdfTripleInformation = qualifier+"|"+pRdfTriple->resource()+"|"+pRdfTriple->id();
    QString rdfTripleInformationSha1 = Core::sha1(rdfTripleInformation);

    QString ontologicalTerm = R"(<tr id="item_)"+rdfTripleInformationSha1+R"(">)""\n"
                              R"(    <td id="qualifier_)"+rdfTripleInformationSha1+R"(">)""\n"
                              R"(        <a href=")"+rdfTripleInformation+R"(">)"+qualifier+"</a>\n"
                               "    </td>\n"
                              R"(    <td id="resource_)"+rdfTripleInformationSha1+R"(">)""\n"
                              R"(        <a href=")"+rdfTripleInformation+R"(">)"+pRdfTriple->resource()+"</a>\n"
                               "    </td>\n"
                              R"(    <td id="id_)"+rdfTripleInformationSha1+R"(">)""\n"
                              R"(        <a href=")"+rdfTripleInformation+R"(">)"+pRdfTriple->id()+"</a>\n"
                               "    </td>\n"
                              R"(    <td id="button_)"+rdfTripleInformationSha1+R"(">)""\n"
                              R"(        <a class="noHover" href=")"+rdfTripleInformationSha1+R"("><img class="button"></a>)""\n"
                               "    </td>\n"
                              R"(    <td id="disabledButton_)"+rdfTripleInformationSha1+R"(" style="display: none;">)""\n"
                              R"(        <img class="disabledButton">)""\n"
                               "    </td>\n"
                               "</tr>\n";

    if (mItemsCount == 1) {
        mOutputOntologicalTerms->webView()->page()->mainFrame()->documentElement().findFirst("tbody").appendInside(ontologicalTerm);
    } else {
        mOutputOntologicalTerms->webView()->page()->mainFrame()->documentElement().findFirst(QString("tr[id=item_%1]").arg(mRdfTripleInformationSha1s.last())).appendOutside(ontologicalTerm);
    }

    // Keep track of some information

    if (!mUrls.contains(pRdfTriple->resource())) {
        mUrls.insert(pRdfTriple->resource(), resourceUrl(pRdfTriple->resource()));
    }

    mUrls.insert(rdfTripleInformation, idUrl(pRdfTriple->id()));

    mRdfTripleInformationSha1s << rdfTripleInformationSha1;

    mRdfTriplesMapping.insert(rdfTripleInformationSha1, pRdfTriple);

    if (mFirstRdfTripleInformation.isEmpty()) {
        mFirstRdfTripleInformation = rdfTripleInformation;
    }

    mLastRdfTripleInformation = rdfTripleInformation;

    // Do some additional GUI updates, if needed

    if (pNeedAdditionalGuiUpdates) {
        mLookUpRdfTripleInformation = Information::Last;

        additionalGuiUpdates({}, InformationType::None, mLookUpRdfTripleInformation);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::genericLookUp(const QString &pRdfTripleInformation,
                                                                        InformationType pInformationType)
{
    // Keep track of the RDF triple information and type

    mRdfTripleInformation = pRdfTripleInformation;

    // Retrieve the RDF triple information

    QStringList rdfTripleInformation = pRdfTripleInformation.split('|');
    QString qualifier = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformation[0];
    QString resource = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformation[1];
    QString id = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformation[2];

    // (Un)highlight/(un)select our various RDF triple information

    static const QString Highlighted = "highlighted";
    static const QString Selected = "selected";

    QWebElement documentElement = mOutputOntologicalTerms->webView()->page()->mainFrame()->documentElement();
    QString rdfTripleInformationSha1 = pRdfTripleInformation.isEmpty()?QString():Core::sha1(pRdfTripleInformation);

    if (rdfTripleInformationSha1 != mRdfTripleInformationSha1) {
        if (!mRdfTripleInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(mRdfTripleInformationSha1)).removeClass(Highlighted);

            if (mInformationType == InformationType::Qualifier) {
                documentElement.findFirst(QString("td[id=qualifier_%1]").arg(mRdfTripleInformationSha1)).removeClass(Selected);
            } else if (mInformationType == InformationType::Resource) {
                documentElement.findFirst(QString("td[id=resource_%1]").arg(mRdfTripleInformationSha1)).removeClass(Selected);
            } else if (mInformationType == InformationType::Id) {
                documentElement.findFirst(QString("td[id=id_%1]").arg(mRdfTripleInformationSha1)).removeClass(Selected);
            }
        }

        if (!rdfTripleInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(rdfTripleInformationSha1)).addClass(Highlighted);

            if (pInformationType == InformationType::Qualifier) {
                documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            } else if (pInformationType == InformationType::Resource) {
                documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            } else if (pInformationType == InformationType::Id) {
                documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            }
        }

        mRdfTripleInformationSha1 = rdfTripleInformationSha1;
    } else if (!rdfTripleInformationSha1.isEmpty()) {
        if (pInformationType == InformationType::Qualifier) {
            documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
        } else if (pInformationType == InformationType::Resource) {
            documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
        } else if (pInformationType == InformationType::Id) {
            documentElement.findFirst(QString("td[id=qualifier_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=resource_%1]").arg(rdfTripleInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(rdfTripleInformationSha1)).addClass(Selected);
        }
    }

    mInformationType = pInformationType;

    // Check whether we have something to look up
    // Note: there is nothing do for Information::Any...

    if (mLookUpRdfTripleInformation == Information::First) {
        mOutputOntologicalTerms->webView()->page()->triggerAction(QWebPage::MoveToStartOfDocument);
    } else if (mLookUpRdfTripleInformation == Information::Last) {
        // Note #1: normally, we would use
        //             mOutputOntologicalTerms->page()->triggerAction(QWebPage::MoveToEndOfDocument);
        //          but this doesn't work...
        // Note #2: another option would be to use
        //              QWebFrame *outputFrame = mOutputOntologicalTerms->page()->mainFrame();
        //
        //              outputFrame->setScrollBarValue(Qt::Vertical, outputFrame->scrollBarMaximum(Qt::Vertical));
        //          but this doesn't get us exactly to the bottom of the page...

        QTimer::singleShot(0, this, &CellmlAnnotationViewMetadataNormalViewDetailsWidget::showLastRdfTriple);
    } else if (mLookUpRdfTripleInformation == Information::No) {
        return;
    }

    // Let people know that we want to look something up

    switch (pInformationType) {
    case InformationType::None:
        emit noLookUpRequested();

        break;
    case InformationType::Qualifier:
        emit qualifierLookUpRequested(qualifier);

        break;
    case InformationType::Resource:
        emit resourceLookUpRequested(resource);

        break;
    case InformationType::Id:
        emit idLookUpRequested(id);

        break;
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::disableLookUpInformation()
{
    // Disable the looking up of RDF triple information

    mLookUpRdfTripleInformation = Information::No;

    // Update the GUI by pretending to be interested in looking something up

    genericLookUp();
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::showLastRdfTriple()
{
    // Show our last RDF triple by scrolling to the end of the page

    QWebFrame *outputFrame = mOutputOntologicalTerms->webView()->page()->mainFrame();

    outputFrame->setScrollBarValue(Qt::Vertical, outputFrame->scrollBarMaximum(Qt::Vertical));
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::linkClicked()
{
    // Retrieve some information about the link

    mOutputOntologicalTerms->retrieveLinkInformation(mLink, mTextContent);

    // Check whether we have clicked a resource/id link or a button link

    if (mTextContent.isEmpty()) {
        // Update some information

        CellMLSupport::CellmlFileRdfTriple *rdfTriple = mRdfTriplesMapping.value(mLink);

        QString qualifier = (rdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelQualifier::ModelUnknown)?
                                rdfTriple->modelQualifierAsString():
                                rdfTriple->bioQualifierAsString();
        QString rdfTripleInformation = qualifier+"|"+rdfTriple->resource()+"|"+rdfTriple->id();

        mUrls.remove(rdfTripleInformation);

        mRdfTripleInformationSha1s.removeOne(mLink);

        mRdfTriplesMapping.remove(mLink);

        --mItemsCount;

        // Determine the 'new' RDF triple information to look up, based on
        // whether there are RDF triples left and whether the current RDF triple
        // is the one being highlighted

        QWebElement rdfTripleElement = mOutputOntologicalTerms->webView()->page()->mainFrame()->documentElement().findFirst(QString("tr[id=item_%1]").arg(mLink));

        if (mItemsCount == 0) {
            mRdfTripleInformation = QString();
            mInformationType = InformationType::None;
        } else if (mLink == mRdfTripleInformationSha1) {
            QWebElement newRdfTripleEment = rdfTripleElement.nextSibling();

            if (newRdfTripleEment.isNull()) {
                newRdfTripleEment = rdfTripleElement.previousSibling();
            }

            static const QRegularExpression ItemRegEx = QRegularExpression("^item_");

            CellMLSupport::CellmlFileRdfTriple *newRdfTriple = mRdfTriplesMapping.value(newRdfTripleEment.attribute("id").remove(ItemRegEx));
            QString newQualifier = (newRdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelQualifier::ModelUnknown)?
                                       newRdfTriple->modelQualifierAsString():
                                       newRdfTriple->bioQualifierAsString();

            mRdfTripleInformation = newQualifier+"|"+newRdfTriple->resource()+"|"+newRdfTriple->id();

            if (rdfTripleInformation == mFirstRdfTripleInformation) {
                mFirstRdfTripleInformation = mRdfTripleInformation;
            }

            if (rdfTripleInformation == mLastRdfTripleInformation) {
                mLastRdfTripleInformation = mRdfTripleInformation;
            }
        }

        // Remove the RDF triple from our GUI

        rdfTripleElement.removeFromDocument();

        // Do some additional GUI updates

        mLookUpRdfTripleInformation = Information::Any;

        if (mLink == mRdfTripleInformationSha1) {
            additionalGuiUpdates(mRdfTripleInformation, mInformationType, mLookUpRdfTripleInformation);
        } else {
            // The looked up information is the same, so no need to look it up
            // again
            // Note: indeed, to look it up again would result in the Web view
            //       flashing (since a 'new' Web page would be loaded)...

            additionalGuiUpdates(mRdfTripleInformation, mInformationType, Information::No);
        }

        // Remove the RDF triple from the CellML file

        mCellmlFile->rdfTriples().remove(rdfTriple);

        // Let people know that an RDF triple has been removed

        emit rdfTripleRemoved(rdfTriple);
    } else {
        // We have clicked on a qualifier/resource/id link, so start by enabling
        // the looking up of any RDF triple information

        mLookUpRdfTripleInformation = Information::Any;

        // Call our generic look up function

        QStringList rdfTripleInformation = mLink.split('|');

        genericLookUp(mLink,
                      (rdfTripleInformation[0] == mTextContent)?
                          InformationType::Qualifier:
                          (rdfTripleInformation[1] == mTextContent)?
                              InformationType::Resource:
                              InformationType::Id);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::linkHovered()
{
    // Retrieve some information about the link

    QString link;
    QString textContent;

    mOutputOntologicalTerms->retrieveLinkInformation(link, textContent);

    // Update our tool tip based on whether we are hovering a text or button
    // link
    // Note: this follows the approach used in linkClicked()...

    QString linkToolTip;

    if (!link.isEmpty()) {
        if (textContent.isEmpty()) {
            linkToolTip = tr("Remove Term");
        } else {
            QStringList rdfTripleInformation = link.split('|');

            linkToolTip = (rdfTripleInformation[0] == textContent)?
                              tr("Look Up Qualifier"):
                              (rdfTripleInformation[1] == textContent)?
                                  tr("Look Up Resource"):
                                  tr("Look Up Id");
        }
    }

    mOutputOntologicalTerms->setToolTip(linkToolTip);
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::showCustomContextMenu()
{
    // Retrieve some information about the link

    mOutputOntologicalTerms->retrieveLinkInformation(mLink, mTextContent);

    // Show our context menu to allow the copying of the URL of the resource or
    // id, but only if we are over a link, i.e. if both mLink and mTextContent
    // are not empty

    if (!mLink.isEmpty() && !mTextContent.isEmpty()) {
        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::copy()
{
    // Copy the qualifier or the URL of the resource or id to the clipboard

    QStringList rdfTripleInformation = mLink.split('|');

    if (rdfTripleInformation[0] == mTextContent) {
        QApplication::clipboard()->setText(mTextContent);
    } else if (rdfTripleInformation[1] == mTextContent) {
        QApplication::clipboard()->setText(mUrls.value(mTextContent));
    } else {
        QApplication::clipboard()->setText(mUrls.value(mLink));
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataNormalViewDetailsWidget::filePermissionsChanged()
{
    // Enable or disable the remove buttons for our RDF triples, depending on
    // whether the file is un/locked

    bool fileReadableAndWritable = Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName());
    QWebElement documentElement = mOutputOntologicalTerms->webView()->page()->mainFrame()->documentElement();

    for (const auto &rdfTripleInformationSha1 : qAsConst(mRdfTripleInformationSha1s)) {
        documentElement.findFirst(QString("td[id=button_%1]").arg(rdfTripleInformationSha1)).setStyleProperty("display", fileReadableAndWritable?"table-cell":"none");
        documentElement.findFirst(QString("td[id=disabledButton_%1]").arg(rdfTripleInformationSha1)).setStyleProperty("display", (!fileReadableAndWritable)?"table-cell":"none");
    }
}

//==============================================================================

} // namespace CellMLAnnotationView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
