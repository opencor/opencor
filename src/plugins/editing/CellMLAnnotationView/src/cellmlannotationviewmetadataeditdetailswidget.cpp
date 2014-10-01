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
// CellML annotation view metadata edit details widget
//==============================================================================

#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewmetadatanormalviewdetailswidget.h"
#include "cellmlannotationviewmetadatawebviewwidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilerdftriple.h"
#include "cliutils.h"
#include "filemanager.h"
#include "guiutils.h"
#include "treeviewwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataeditdetailswidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QCursor>
#include <QFont>
#include <QFormLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QTimer>
#include <QVariant>
#include <QVBoxLayout>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

bool CellmlAnnotationViewMetadataEditDetailsWidget::Item::operator==(const Item &pItem) const
{
    // Return whether the current item is the same as the given one

    return    !name.compare(pItem.name)
           && !resource.compare(pItem.resource)
           && !id.compare(pItem.id);
}

//==============================================================================

bool CellmlAnnotationViewMetadataEditDetailsWidget::Item::operator<(const Item &pItem) const
{
    // Return whether the current item is lower than the given one

    int nameComparison     = name.compare(pItem.name);
    int resourceComparison = resource.compare(pItem.resource);
    int idComparison       = id.compare(pItem.id);

    return (nameComparison < 0)?
               true:
               (nameComparison > 0)?
                   false:
                   (resourceComparison < 0)?
                       true:
                       (resourceComparison > 0)?
                           false:
                           (idComparison < 0)?
                               true:
                               false;
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewEditingWidget *pParent) :
    Core::Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataEditDetailsWidget),
    mTermValue(0),
    mAddTermButton(0),
    mTerm(QString()),
    mTerms(QStringList()),
    mItemsCount(0),
    mErrorMessage(QString()),
    mLookUpTerm(false),
    mInformationType(None),
    mLookUpInformation(false),
    mItemsMapping(QMap<QString, Item>()),
    mEnabledItems(QMap<QString, bool>()),
    mCellmlFile(pParent->cellmlFile()),
    mElement(0),
    mUrls(QMap<QString, QString>()),
    mItemInformationSha1s(QStringList()),
    mItemInformationSha1(QString()),
    mLink(QString()),
    mTextContent(QString()),
    mNetworkReply(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a network access manager so that we can then retrieve a list of
    // CellML models from the CellML Model Repository

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when the download of our Internet file is
    // complete

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(termLookedUp(QNetworkReply *)));

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionCopy);

    // Create a form widget that will contain our qualifier and term fields

    QWidget *formWidget = new QWidget(this);
    QFormLayout *formWidgetLayout = new QFormLayout(formWidget);

    formWidget->setLayout(formWidgetLayout);

    // Create a widget that will contain both our qualifier value widget and a
    // button to look up the qualifier

    QWidget *qualifierWidget = new QWidget(formWidget);
    QHBoxLayout *qualifierWidgetLayout = new QHBoxLayout(qualifierWidget);

    qualifierWidgetLayout->setMargin(0);

    qualifierWidget->setLayout(qualifierWidgetLayout);

    // Create our qualifier value widget

    mQualifierValue = new QComboBox(qualifierWidget);

    mQualifierValue->addItems(CellMLSupport::CellmlFileRdfTriple::qualifiersAsStringList());

    connect(mQualifierValue, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(qualifierChanged(const QString &)));

    // Create our qualifier look up button widget

    mLookUpQualifierButton = new QPushButton(qualifierWidget);
    // Note #1: ideally, we could assign a QAction to our QPushButton, but this
    //          cannot be done, so instead we assign a few properties by hand...
    // Note #2: to use a QToolButton would allow us to assign a QAction to it,
    //          but a QToolButton doesn't look quite the same as a QPushButton
    //          on some platforms, so use a QPushButton instead...

    mLookUpQualifierButton->setCheckable(true);
    mLookUpQualifierButton->setIcon(QIcon(":/oxygen/categories/applications-internet.png"));
    mLookUpQualifierButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(mLookUpQualifierButton, SIGNAL(toggled(bool)),
            this, SLOT(lookUpQualifier()));

    // Add our qualifier value and qualifier look up button widgets to our
    // qualifier widget layout

    qualifierWidgetLayout->addWidget(mQualifierValue);
    qualifierWidgetLayout->addWidget(mLookUpQualifierButton);

    // Create a widget that will contain both our term widget and a button to
    // add it (if it is a direct term)

    QWidget *termWidget = new QWidget(formWidget);
    QHBoxLayout *termWidgetLayout = new QHBoxLayout(termWidget);

    termWidgetLayout->setMargin(0);

    termWidget->setLayout(termWidgetLayout);

    // Create our term value widget

    mTermValue = new QLineEdit(termWidget);

    connect(mTermValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(termChanged(const QString &)));

    // Create our add term button widget

    mAddTermButton = new QPushButton(termWidget);

    mAddTermButton->setEnabled(false);
    mAddTermButton->setIcon(QIcon(":/oxygen/actions/list-add.png"));
    mAddTermButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(mAddTermButton, SIGNAL(clicked(bool)),
            this, SLOT(addTerm()));

    // Add our term value and add term button widgets to our term widget layout

    termWidgetLayout->addWidget(mTermValue);
    termWidgetLayout->addWidget(mAddTermButton);

    // Add both our qualifier and term widgets to our form widget layout

    mQualifierLabel = new QLabel(formWidget);
    mTermLabel = new QLabel(formWidget);

    QFont newFont = mQualifierLabel->font();

    newFont.setBold(true);

    mQualifierLabel->setFont(newFont);
    mTermLabel->setFont(newFont);

    formWidgetLayout->addRow(mQualifierLabel, qualifierWidget);
    formWidgetLayout->addRow(mTermLabel, termWidget);

    // Reset the tab order from our parent's CellML list's tree view widget
    // Note: ideally, we would take advantage of Qt's signal/slot approach with
    //       the signal being emitted here and the slot being implemented in
    //       mParent, but this wouldn't work here since updateGui() gets called
    //       as part of the creation of this metadata details widget...

    setTabOrder(qobject_cast<QWidget *>(mParent->cellmlList()->treeViewWidget()),
                mQualifierValue);
    setTabOrder(mQualifierValue, mLookUpQualifierButton);
    setTabOrder(mLookUpQualifierButton, mTermValue);
    setTabOrder(mTermValue, mAddTermButton);

    // Create an output widget that will contain our output message and output
    // for ontological terms

    mOutput = new Core::Widget(this);

    mOutput->setLayout(new QVBoxLayout(mOutput));

    mOutput->layout()->setMargin(0);

    connect(mOutput, SIGNAL(resized(const QSize &, const QSize &)),
            this, SLOT(recenterBusyWidget()));

    // Create our output message (within a scroll area, in case the label is too
    // wide)

    mOutputMessageScrollArea = new QScrollArea(mOutput);

    mOutputMessageScrollArea->setFrameShape(QFrame::NoFrame);
    mOutputMessageScrollArea->setWidgetResizable(true);

    mOutputMessage = new Core::UserMessageWidget(mOutputMessageScrollArea);

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

    // Add our 'internal' widgets to our main layout

    mGui->layout->addWidget(formWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mOutput);

    // Update our GUI (incl. its enabled state)

    updateGui(mElement, true);
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::~CellmlAnnotationViewMetadataEditDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate various aspects of our form widget

    mQualifierLabel->setText(tr("Qualifier:"));
    mTermLabel->setText(tr("Term:"));

    mLookUpQualifierButton->setStatusTip(tr("Look up the qualifier"));
    mLookUpQualifierButton->setToolTip(tr("Look Up"));

    mAddTermButton->setStatusTip(tr("Add the term"));
    mAddTermButton->setToolTip(tr("Add"));

    // Retranslate our output message

    upudateOutputMessage(mLookUpTerm, mErrorMessage);

    // Retranslate our output headers

    updateOutputHeaders();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement,
                                                              const bool &pResetItemsGui)
{
    // Keep track of the CellML element

    mElement = pElement;

    // Enable/disable our add term button, depending on whether the direct term
    // is already associated with the CellML element

    bool fileReadableAndWritable = Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName());
    bool termIsDirect = isDirectTerm(mTermValue->text());

    if (termIsDirect) {
        QStringList termInformation = mTermValue->text().split("/");

        if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            mAddTermButton->setEnabled(    fileReadableAndWritable
                                       && !mCellmlFile->rdfTriple(mElement,
                                                                  CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                                                  termInformation[0],
                                                                  termInformation[1]));
        else
            mAddTermButton->setEnabled(    fileReadableAndWritable
                                       && !mCellmlFile->rdfTriple(mElement,
                                                                  CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierValue->currentIndex()-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                                  termInformation[0],
                                                                  termInformation[1]));
    } else {
        mAddTermButton->setEnabled(false);
    }

    // Reset our items' GUI, if needed and if our busy widget is not already
    // visible
    // Note: the reason for checking whether our busy widget is visible is that
    //       we come here every time the user modifies the term to look up. So,
    //       we don't want to call updateItemsGui() for no reasons. Indeed, if
    //       we were then our busy widget would get 'reset' every time, which
    //       doesn't look nice...

    if (   (pResetItemsGui && !mParent->parent()->isBusyWidgetVisible())
        || termIsDirect)
        updateItemsGui(Items(), !termIsDirect, QString());

    // Enable or disable the add buttons for our retrieved terms, depending on
    // whether the file is un/locked and whether they are already associated
    // with the CellML element

    QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();

    foreach (const QString &itemInformationSha1, mItemInformationSha1s) {
        Item item = mItemsMapping.value(itemInformationSha1);
        bool enabledButton;

        if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            enabledButton =     fileReadableAndWritable
                            && !mCellmlFile->rdfTriple(mElement,
                                                       CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                                       item.resource, item.id);
        else
            enabledButton =     fileReadableAndWritable
                            && !mCellmlFile->rdfTriple(mElement,
                                                       CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierValue->currentIndex()-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                       item.resource, item.id);

        if (enabledButton != mEnabledItems.value(itemInformationSha1)) {
            mEnabledItems.insert(itemInformationSha1, enabledButton);

            documentElement.findFirst(QString("td[id=button_%1]").arg(itemInformationSha1)).setStyleProperty("display", enabledButton?"table-cell":"none");
            documentElement.findFirst(QString("td[id=disabledButton_%1]").arg(itemInformationSha1)).setStyleProperty("display", !enabledButton?"table-cell":"none");
        }
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::upudateOutputMessage(const bool &pLookUpTerm,
                                                                         const QString &pErrorMessage,
                                                                         bool *pShowBusyWidget)
{
    // Update our output message

    if (pShowBusyWidget)
        *pShowBusyWidget = false;

    if (!Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName())) {
        mOutputMessage->setIconMessage(QString(), QString());

        if (pShowBusyWidget && pLookUpTerm && !mTermValue->text().isEmpty())
            *pShowBusyWidget = true;
    } else if (mTermValue->text().isEmpty()) {
        mOutputMessage->setIconMessage(":/oxygen/actions/help-hint.png",
                                       tr("Enter a term above..."));
    } else if (pLookUpTerm) {
        mOutputMessage->setIconMessage(QString(), QString());

        if (pShowBusyWidget)
            *pShowBusyWidget = true;
    } else if (pErrorMessage.isEmpty()) {
        if (isDirectTerm(mTermValue->text())) {
            if (mAddTermButton->isEnabled())
                mOutputMessage->setIconMessage(":/oxygen/actions/help-hint.png",
                                               tr("You can directly add the term <strong>%1</strong>...").arg(mTermValue->text()));
            else
                mOutputMessage->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("The term <strong>%1</strong> has already been added using the above qualifier...").arg(mTermValue->text()));
        } else {
            mOutputMessage->setIconMessage(":/oxygen/actions/help-about.png",
                                           tr("No terms were found for <strong>%1</strong>...").arg(mTerm));
        }
    } else {
        mOutputMessage->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                       Core::formatErrorMessage(pErrorMessage, false, true));
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateOutputHeaders()
{
    // Update our output headers

    QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();

    documentElement.findFirst("th[id=nameOrQualifier]").setInnerXml(tr("Name"));
    documentElement.findFirst("th[id=resource]").setInnerXml(tr("Resource"));
    documentElement.findFirst("th[id=id]").setInnerXml(tr("Id"));

    QWebElement countElement = documentElement.findFirst("th[id=count]");

    if (mItemsCount == 1)
        countElement.setInnerXml(tr("(1 term)"));
    else
        countElement.setInnerXml(tr("(%1 terms)").arg(QLocale().toString(mItemsCount)));
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateItemsGui(const Items &pItems,
                                                                   const bool &pLookUpTerm,
                                                                   const QString &pErrorMessage)
{
    // Keep track of some information

    mLookUpTerm = pLookUpTerm;
    mErrorMessage = pErrorMessage;

    // Reset various properties
    // Note: we might only do that before adding new items, but then again there
    //       is no need to waste memory...

    mUrls.clear();
    mItemInformationSha1s.clear();

    mItemsMapping.clear();
    mEnabledItems.clear();

    mOutputOntologicalTerms->setHtml(QString());

    // Populate our web view, but only if there is at least one item

    bool showBusyWidget = false;

    if (pItems.count()) {
        // Initialise our web view

        mOutputOntologicalTerms->setHtml(mOutputOntologicalTermsTemplate.arg(Core::iconDataUri(":/oxygen/actions/list-add.png", 16, 16),
                                                                             Core::iconDataUri(":/oxygen/actions/list-add.png", 16, 16, QIcon::Disabled)));

        // Add the items

        QString ontologicalTerms = QString();

        foreach (const Item &item, pItems) {
            // Keep track of some information

            QString itemInformation = item.resource+"|"+item.id;
            QString itemInformationSha1 = Core::sha1(itemInformation);
            QString resourceUrl = "http://identifiers.org/"+item.resource+"/?redirect=true";
            QString idUrl = "http://identifiers.org/"+item.resource+"/"+item.id+"/?profile=most_reliable&redirect=true";

            if (!mUrls.contains(item.resource))
                mUrls.insert(item.resource, resourceUrl);

            mUrls.insert(itemInformation, idUrl);

            mItemInformationSha1s << itemInformationSha1;

            // Add the item

            mItemsMapping.insert(itemInformationSha1, item);
            mEnabledItems.insert(itemInformationSha1, true);

            ontologicalTerms +=  "<tr id=\"item_"+itemInformationSha1+"\">\n"
                                +"    <td>\n"
                                +"        "+item.name+"\n"
                                +"    </td>\n"
                                +"    <td id=\"resource_"+itemInformationSha1+"\">\n"
                                +"        <a href=\""+itemInformation+"\">"+item.resource+"</a>\n"
                                +"    </td>\n"
                                +"    <td id=\"id_"+itemInformationSha1+"\">\n"
                                +"        <a href=\""+itemInformation+"\">"+item.id+"</a>\n"
                                +"    </td>\n"
                                +"    <td id=\"button_"+itemInformationSha1+"\">\n"
                                +"        <a class=\"noHover\" href=\""+itemInformationSha1+"\"><img class=\"button\"/></a>\n"
                                +"    </td>\n"
                                +"    <td id=\"disabledButton_"+itemInformationSha1+"\" style=\"display: none;\">\n"
                                +"        <img class=\"disabledButton\"/>\n"
                                +"    </td>\n"
                                +"</tr>\n";
        }

        mOutputOntologicalTerms->page()->mainFrame()->documentElement().findFirst("tbody").appendInside(ontologicalTerms);

        updateOutputHeaders();
    } else {
        // No items to show, so either there is no data available or an error
        // occurred, so update our output message

        upudateOutputMessage(pLookUpTerm, pErrorMessage, &showBusyWidget);

        // Pretend that we don't want to look anything up, if needed
        // Note: this is in case a resource or id used to be looked up, in which
        //       case we don't want it to be anymore...

        if (mLookUpInformation && (mInformationType != Qualifier))
            genericLookUp();
    }

    // Hide our busy widget (just to be on the safe side)

    mParent->parent()->hideBusyWidget();

    // Show/hide our output message and output for ontological terms

    mOutputMessageScrollArea->setVisible(!pItems.count());
    mOutputOntologicalTerms->setVisible(pItems.count());

    // Show our busy widget instead, if needed

    if (showBusyWidget)
        mParent->parent()->showBusyWidget(mOutput);
}

//==============================================================================

CellmlAnnotationViewMetadataEditDetailsWidget::Item CellmlAnnotationViewMetadataEditDetailsWidget::item(const QString &pName,
                                                                                                        const QString &pResource,
                                                                                                        const QString &pId)
{
    // Return a formatted Item 'object'

    Item res;

    res.name     = pName;
    res.resource = pResource;
    res.id       = pId;

    return res;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::genericLookUp(const QString &pItemInformation,
                                                                  const InformationType &pInformationType)
{
    // Retrieve the information

    QStringList itemInformation = pItemInformation.split("|");
    QString qualifier = (pInformationType != Qualifier)?QString():pItemInformation;
    QString resource = (pItemInformation.isEmpty() || (pInformationType == Qualifier))?QString():itemInformation[0];
    QString id = (pItemInformation.isEmpty() || (pInformationType == Qualifier))?QString():itemInformation[1];

    // Toggle the look up button, if needed
    // Note: we don't want nested generic look ups, hence we temporarily disable
    //       the handling of toggled() on mLookUpQualifierButton. If we don't do
    //       that then to toggle mLookUpQualifierButton may result in mLink and
    //       mTextContent being reset (see below), which may not be what we want
    //       (e.g. if we came here after clicking on a resource/id link)...

    if ((pInformationType != Qualifier) && mLookUpQualifierButton->isChecked()) {
        disconnect(mLookUpQualifierButton, SIGNAL(toggled(bool)),
                   this, SLOT(lookUpQualifier()));

        mLookUpQualifierButton->toggle();

        connect(mLookUpQualifierButton, SIGNAL(toggled(bool)),
                this, SLOT(lookUpQualifier()));
    }

    // Reset some internal properties, if needed

    if ((pInformationType != Resource) && (pInformationType != Id)) {
        mLink = QString();
        mTextContent = QString();
    }

    // (Un)highlight/(un)select our various items

    static const QString Highlighted = "highlighted";
    static const QString Selected = "selected";

    QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();
    QString itemInformationSha1 = mLink.isEmpty()?QString():Core::sha1(mLink);

    if (itemInformationSha1.compare(mItemInformationSha1)) {
        if (!mItemInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(mItemInformationSha1)).removeClass(Highlighted);

            if (mInformationType == Resource)
                documentElement.findFirst(QString("td[id=resource_%1]").arg(mItemInformationSha1)).removeClass(Selected);
            else if (mInformationType == Id)
                documentElement.findFirst(QString("td[id=id_%1]").arg(mItemInformationSha1)).removeClass(Selected);
        }

        if (!itemInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(itemInformationSha1)).addClass(Highlighted);

            if (pInformationType == Resource)
                documentElement.findFirst(QString("td[id=resource_%1]").arg(itemInformationSha1)).addClass(Selected);
            else if (pInformationType == Id)
                documentElement.findFirst(QString("td[id=id_%1]").arg(itemInformationSha1)).addClass(Selected);
        }

        mItemInformationSha1 = itemInformationSha1;
    } else if (!itemInformationSha1.isEmpty()) {
        if (pInformationType == Resource) {
            documentElement.findFirst(QString("td[id=resource_%1]").arg(itemInformationSha1)).addClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(itemInformationSha1)).removeClass(Selected);
        } else if (pInformationType == Id) {
            documentElement.findFirst(QString("td[id=resource_%1]").arg(itemInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(itemInformationSha1)).addClass(Selected);
        }
    }

    mInformationType = pInformationType;

    // Make sure that we have something to look up

    if (!mLookUpInformation)
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

void CellmlAnnotationViewMetadataEditDetailsWidget::disableLookUpInformation()
{
    // Disable the looking up of information

    mLookUpInformation = false;

    // Update the GUI by pretending to be interested in looking something up

    genericLookUp();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::qualifierChanged(const QString &pQualifier)
{
    // Look up the qualifier, if requested

    if (mLookUpQualifierButton->isChecked()) {
        // Enable the looking up of information

        mLookUpInformation = true;

        // Call our generic look up function

        genericLookUp(pQualifier, Qualifier);
    }

    // Update our GUI (incl. its enabled state)

    updateGui(mElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookUpQualifier()
{
    // Enable the looking up of information

    mLookUpInformation = true;

    // Call our generic look up function

    if (mLookUpQualifierButton->isChecked())
        // We want to look something up

        genericLookUp(mQualifierValue->currentText(), Qualifier);
    else
        // We don't want to look anything up anymore

        genericLookUp();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::linkClicked()
{
    // Retrieve some information about the link

    mOutputOntologicalTerms->retrieveLinkInformation(mLink, mTextContent);

    // Check whether we have clicked a resource/id link or a button link

    if (mTextContent.isEmpty()) {
        // We have clicked on a button link, so retrieve the item associated
        // with it

        Item item = mItemsMapping.value(mLink);

        // Add the ontological term to our CellML element as an RDF triple

        CellMLSupport::CellmlFileRdfTriple *rdfTriple;

        if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            rdfTriple = mCellmlFile->addRdfTriple(mElement,
                                                  CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                                  item.resource, item.id);
        else
            rdfTriple = mCellmlFile->addRdfTriple(mElement,
                                                  CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierValue->currentIndex()-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                  item.resource, item.id);

        // Disable the add button, now that we have added the ontological term

        mEnabledItems.insert(mLink, false);

        QWebElement documentElement = mOutputOntologicalTerms->page()->mainFrame()->documentElement();

        documentElement.findFirst(QString("td[id=button_%1]").arg(mLink)).setStyleProperty("display", "none");
        documentElement.findFirst(QString("td[id=disabledButton_%1]").arg(mLink)).setStyleProperty("display", "table-cell");

        // Ask our parent to update its GUI with the added RDF triple

        mParent->metadataDetails()->metadataViewDetails()->normalView()->addRdfTriple(rdfTriple);
    } else {
        // We have clicked on a resource/id link, so start by enabling the
        // looking up of information

        mLookUpInformation = true;

        // Call our generic look up function

        genericLookUp(mLink,
                      mUrls.contains(mTextContent)?
                          Resource:
                          Id);
    }
}

//==============================================================================

bool CellmlAnnotationViewMetadataEditDetailsWidget::isDirectTerm(const QString &pTerm) const
{
    // Return whether the given term is a direct one

    return    QRegularExpression("^"+CellMLSupport::ResourceRegExp+"/"+CellMLSupport::IdRegExp+"$").match(pTerm).hasMatch()
           && (pTerm.count("/") == 1);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::termChanged(const QString &pTerm)
{
    // Update our GUI (incl. its enabled state)

    updateGui(mElement, true);

    // Retrieve some ontological terms based on the given term, but only if the
    // term cannot be added directly and if it is not empty

    if (!isDirectTerm(pTerm) && !pTerm.isEmpty()) {
        // Add the term to our list of terms to look up

        mTerms << pTerm;

        // Retrieve some ontological terms, but after a short delay
        // Note: the delay is in case the term gets changed in between. Indeed,
        //       we can't cancel a request sent to PMR2, so we should try to
        //       send as few of them as possible...

        QTimer::singleShot(500, this, SLOT(lookUpTerm()));
    }
}

//==============================================================================

static const auto Pmr2RicordoUrl = QStringLiteral("https://models.physiomeproject.org/pmr2_ricordo/miriam_terms/");

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookUpTerm()
{
    // 'Cancel' the previous request, if any

    if (mNetworkReply) {
        mNetworkReply->close();

        mNetworkReply = 0;
    }

    // Now, retrieve some ontological terms

    QString term = mTerms.first();

    mTerms.removeFirst();

    if (mTerms.isEmpty())
        mNetworkReply = mNetworkAccessManager->get(QNetworkRequest(Pmr2RicordoUrl+term));
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::termLookedUp(QNetworkReply *pNetworkReply)
{
    // Ignore the network reply if it got cancelled

    if (pNetworkReply->error() == QNetworkReply::OperationCanceledError) {
        pNetworkReply->deleteLater();

        return;
    } else {
        mNetworkReply = 0;
    }

    // Keep track of the term we have just looked up

    mTerm = pNetworkReply->url().toString().remove(Pmr2RicordoUrl);

    // Retrieve the list of terms, should we have retrieved it without any
    // problem

    Items items = Items();
    QString errorMessage = QString();

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code

        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(pNetworkReply->readAll(), &jsonParseError);

        if (jsonParseError.error == QJsonParseError::NoError) {
            // Retrieve the list of terms

            QVariantMap termMap;
            QString name;
            QString resource;
            QString id;

            foreach (const QVariant &termsVariant, jsonDocument.object().toVariantMap()["results"].toList()) {
                termMap = termsVariant.toMap();
                name = termMap["name"].toString();

                if (   !name.isEmpty()
                    &&  CellMLSupport::CellmlFileRdfTriple::decodeTerm(termMap["identifiers_org_uri"].toString(), resource, id)) {
                    // We have a name and we could decode the term, so add the
                    // item to our list, should it not already be in it

                    Item newItem = item(name, resource, id);

                    if (!items.contains(newItem))
                        items << newItem;
                }
            }
        } else {
            errorMessage = jsonParseError.errorString();
        }
    } else {
        errorMessage = pNetworkReply->errorString();
    }

    // Update our GUI with the results of the look up after having sorted them
    // and kept track of it size

    std::sort(items.begin(), items.end());

    mItemsCount = items.count();

    updateItemsGui(items, false, errorMessage);

    // Update our GUI (incl. its enabled state)

    updateGui(mElement);

    // Delete (later) the network reply

    pNetworkReply->deleteLater();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::addTerm()
{
    // Add the term to our CellML element as an RDF triple

    CellMLSupport::CellmlFileRdfTriple *rdfTriple;
    QStringList termInformation = Core::stringFromPercentEncoding(mTermValue->text()).split("/");

    if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
        rdfTriple = mCellmlFile->addRdfTriple(mElement,
                                              CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                              termInformation[0], termInformation[1]);
    else
        rdfTriple = mCellmlFile->addRdfTriple(mElement,
                                              CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierValue->currentIndex()-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                              termInformation[0], termInformation[1]);

    // Disable the add term buton, now that we have added the term

    mAddTermButton->setEnabled(false);

    // Update our items' GUI

    updateItemsGui(Items(), !isDirectTerm(mTermValue->text()), QString());

    // Ask our parent to update its GUI with the added RDF triple

    mParent->metadataDetails()->metadataViewDetails()->normalView()->addRdfTriple(rdfTriple);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::showCustomContextMenu(const QPoint &pPosition)
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

void CellmlAnnotationViewMetadataEditDetailsWidget::on_actionCopy_triggered()
{
    // Copy the URL of the resource or id to the clipboard

    if (mUrls.contains(mTextContent))
        QApplication::clipboard()->setText(mUrls.value(mTextContent));
    else
        QApplication::clipboard()->setText(mUrls.value(mLink));
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::recenterBusyWidget()
{
    // Recenter our busy widget

    mParent->parent()->centerBusyWidget();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::filePermissionsChanged()
{
    // Update our GUI (incl. its enabled state)

    updateGui(mElement, !mItemsCount);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
