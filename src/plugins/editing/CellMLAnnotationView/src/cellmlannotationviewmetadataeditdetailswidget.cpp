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
#include "cellmlannotationviewmetadataeditdetailswidget.h"
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
#include <QLayout>
#include <QLayoutItem>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedWidget>
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
    mLookUpResource(true),
    mLink(QString()),
    mTextContent(QString()),
    mItemInformation(QString()),
    mItemResourceOrId(QString()),
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
    //          cannot be done, so... we assign a few properties by hand...
    // Note #2: to use a QToolButton would allow us to assign a QAction to it,
    //          but a QToolButton doesn't look quite the same as a QPushButton
    //          on some platforms, so...

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

    connect(mAddTermButton, SIGNAL(clicked()),
            this, SLOT(addTerm()));

    // Add our term value and add term button widgets to our term widget layout

    termWidgetLayout->addWidget(mTermValue);
    termWidgetLayout->addWidget(mAddTermButton);

    // Add both our qualifier and term widgets to our form widget layout

    mQualifierLabel = Core::newLabel(QString(), 1.0, true, formWidget);
    mTermLabel = Core::newLabel(QString(), 1.0, true, formWidget);

    formWidgetLayout->addRow(mQualifierLabel, qualifierWidget);
    formWidgetLayout->addRow(mTermLabel, termWidget);

    // Reset the tab order from our parent's CellML list's tree view widget
    // Note: ideally, we would take advantage of Qt's signal/slot approach with
    //       the signal being emitted here and the slot being implemented in
    //       mParent, but this wouldn't work here since updateGui() gets called
    //       as part of the creation of this metadata details widget, so...

    setTabOrder(qobject_cast<QWidget *>(mParent->cellmlList()->treeViewWidget()),
                mQualifierValue);
    setTabOrder(mQualifierValue, mLookUpQualifierButton);
    setTabOrder(mLookUpQualifierButton, mTermValue);
    setTabOrder(mTermValue, mAddTermButton);

    // Create an output widget that will contain our output message and ourput
    // for possible ontological terms

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

    // Create our output for possible ontological terms

    Core::readTextFromFile(":/possibleOntologicalTerms.html", mOutputPossibleOntologicalTermsTemplate);

    mOutputPossibleOntologicalTerms = new CellmlAnnotationViewMetadataWebViewWidget(mOutput);

    mOutputPossibleOntologicalTerms->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mOutputPossibleOntologicalTerms, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    mOutputPossibleOntologicalTerms->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(mOutputPossibleOntologicalTerms->page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)),
            this, SLOT(linkHovered(const QString &, const QString &, const QString &)));
    connect(mOutputPossibleOntologicalTerms->page(), SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(linkClicked()));

    // Add our output message and ourput for possible ontological terms to our
    // output widget

    mOutput->layout()->addWidget(mOutputMessageScrollArea);
    mOutput->layout()->addWidget(mOutputPossibleOntologicalTerms);

    // Add our 'internal' widgets to our main layout

    mGui->layout->addWidget(formWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mOutput);

    // Update the enabled state of our various add buttons

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

    // Retranslate our output for possible ontological terms

    updateOutputPossibleOntologicalTerms();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(iface::cellml_api::CellMLElement *pElement,
                                                              const bool &pResetItemsGui)
{
    // Keep track of the CellML element

    mElement = pElement;

    // Enable/disable some of our user fields

    bool fileReadableAndWritable = Core::FileManager::instance()->isReadableAndWritable(mCellmlFile->fileName());

    mQualifierValue->setEnabled(fileReadableAndWritable);
    mLookUpQualifierButton->setEnabled(fileReadableAndWritable);

    mTermValue->setEnabled(fileReadableAndWritable);

    // Enable/disable our add term button, depending on whether the direct term
    // is already associated with the CellML element

    bool termIsDirect = isDirectTerm(mTermValue->text());

    if (termIsDirect) {
        QStringList termInformation = mTermValue->text().split("/");

        if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            mAddTermButton->setEnabled(    fileReadableAndWritable
                                       && !mCellmlFile->rdfTripleExists(mElement,
                                                                        CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                                                        termInformation[0], termInformation[1]));
        else
            mAddTermButton->setEnabled(    fileReadableAndWritable
                                       && !mCellmlFile->rdfTripleExists(mElement,
                                                                        CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierValue->currentIndex()-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                                        termInformation[0], termInformation[1]));
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
    // whether they are already associated with the CellML element

    QWebElement documentElement = mOutputPossibleOntologicalTerms->page()->mainFrame()->documentElement();

    foreach (const QString &itemInformationSha1, mItemInformationSha1s) {
        Item item = mItemsMapping.value(itemInformationSha1);
        bool enabledButton;

        if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            enabledButton =     fileReadableAndWritable
                            && !mCellmlFile->rdfTripleExists(mElement,
                                                             CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                                             item.resource, item.id);
        else
            enabledButton =     fileReadableAndWritable
                            && !mCellmlFile->rdfTripleExists(mElement,
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

void CellmlAnnotationViewMetadataEditDetailsWidget::updateOutputPossibleOntologicalTerms()
{
    // Update our output for possible ontological terms

    QWebElement documentElement = mOutputPossibleOntologicalTerms->page()->mainFrame()->documentElement();

    documentElement.findFirst("th[id=name]").setInnerXml(tr("Name"));
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
    //       is no need to waste memory, so...

    mUrls.clear();
    mItemInformationSha1s.clear();

    mOutputPossibleOntologicalTerms->setHtml(QString());

    // Populate our new layout, but only if there is at least one item

    bool showBusyWidget = false;

    if (pItems.count()) {
        // Add the items

        static const QString indent = "                ";
        QString possibleOntologicalTerms = QString();

        foreach (const Item &item, pItems) {
            // Keep track of the URLs for the resource and id, and of the item
            // information

            QString itemInformation = item.resource+"|"+item.id;
            QString itemInformationSha1 = Core::sha1(itemInformation);
            QString resourceUrl = "http://identifiers.org/"+item.resource+"/?redirect=true";
            QString idUrl = "http://identifiers.org/"+item.resource+"/"+item.id+"/?profile=most_reliable&redirect=true";

            if (!mUrls.contains(item.resource))
                mUrls.insert(item.resource, resourceUrl);

            mUrls.insert(itemInformation, idUrl);

            mItemInformationSha1s << itemInformationSha1;

            // Add button

            mItemsMapping.insert(itemInformationSha1, item);
            mEnabledItems.insert(itemInformationSha1, true);

            possibleOntologicalTerms +=  indent+"<tr id=\"item_"+itemInformationSha1+"\">\n"
                                        +indent+"    <td>\n"
                                        +indent+"        "+item.name+"\n"
                                        +indent+"    </td>\n"
                                        +indent+"    <td id=\"resource_"+itemInformationSha1+"\">\n"
                                        +indent+"        <a href=\""+itemInformation+"\">"+item.resource+"</a>\n"
                                        +indent+"    </td>\n"
                                        +indent+"    <td id=\"id_"+itemInformationSha1+"\">\n"
                                        +indent+"        <a href=\""+itemInformation+"\">"+item.id+"</a>\n"
                                        +indent+"    </td>\n"
                                        +indent+"    <td id=\"button_"+itemInformationSha1+"\">\n"
                                        +indent+"        <a class=\"noHover\" href=\""+itemInformationSha1+"\"><img class=\"button\"/></a>\n"
                                        +indent+"    </td>\n"
                                        +indent+"    <td id=\"disabledButton_"+itemInformationSha1+"\" style=\"display: none;\">\n"
                                        +indent+"        <img class=\"disabledButton\"/>\n"
                                        +indent+"    </td>\n"
                                        +indent+"</tr>\n";
        }

        mOutputPossibleOntologicalTerms->setHtml(mOutputPossibleOntologicalTermsTemplate.arg(Core::iconDataUri(":/oxygen/actions/list-add.png", 16, 16),
                                                                                             Core::iconDataUri(":/oxygen/actions/list-add.png", 16, 16, QIcon::Disabled),
                                                                                             possibleOntologicalTerms));

        updateOutputPossibleOntologicalTerms();
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

    // Hide our old output widget and show our new one

    mOutputMessageScrollArea->setVisible(!pItems.count());
    mOutputPossibleOntologicalTerms->setVisible(pItems.count());

    // show our busy widget instead, if needed

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
                                                                  const InformationType &pInformationType,
                                                                  const bool &pRetranslate)
{
    // Retrieve the information

    QStringList itemInformationAsStringList = pItemInformation.split("|");
    QString qualifierAsString = (pInformationType != Qualifier)?QString():pItemInformation;
    QString resourceAsString = (pItemInformation.isEmpty() || (pInformationType == Qualifier))?QString():itemInformationAsStringList[0];
    QString idAsString = (pItemInformation.isEmpty() || (pInformationType == Qualifier))?QString():itemInformationAsStringList[1];

    // Keep track of the type of information we are looking up

    mInformationType = pInformationType;

    // Toggle the look up button, if needed

    if ((pInformationType != Qualifier) && mLookUpQualifierButton->isChecked())
        mLookUpQualifierButton->toggle();

    // (Un)highlight/(un)select our various items

    static const QString Highlighted = "highlighted";
    static const QString Selected = "selected";

    QWebElement documentElement = mOutputPossibleOntologicalTerms->page()->mainFrame()->documentElement();
    QString itemInformationSha1 = Core::sha1(mLink);
    bool lookUpResource = mUrls.contains(mTextContent);

    if (itemInformationSha1.compare(mItemInformationSha1)) {
        if (!mItemInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(mItemInformationSha1)).removeClass(Highlighted);

            if (mLookUpResource)
                documentElement.findFirst(QString("td[id=resource_%1]").arg(mItemInformationSha1)).removeClass(Selected);
            else
                documentElement.findFirst(QString("td[id=id_%1]").arg(mItemInformationSha1)).removeClass(Selected);
        }

        if (!itemInformationSha1.isEmpty()) {
            documentElement.findFirst(QString("tr[id=item_%1]").arg(itemInformationSha1)).addClass(Highlighted);

            if (lookUpResource)
                documentElement.findFirst(QString("td[id=resource_%1]").arg(itemInformationSha1)).addClass(Selected);
            else
                documentElement.findFirst(QString("td[id=id_%1]").arg(itemInformationSha1)).addClass(Selected);
        }

        mItemInformationSha1 = itemInformationSha1;
    } else {
        if (lookUpResource) {
            documentElement.findFirst(QString("td[id=resource_%1]").arg(itemInformationSha1)).addClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(itemInformationSha1)).removeClass(Selected);
        } else {
            documentElement.findFirst(QString("td[id=resource_%1]").arg(itemInformationSha1)).removeClass(Selected);
            documentElement.findFirst(QString("td[id=id_%1]").arg(itemInformationSha1)).addClass(Selected);
        }
    }

    mLookUpResource = lookUpResource;

    // Check that we have something to look up

    if (!mLookUpInformation)
        // Nothing to look up, so...

        return;

    // Let people know that we want to look something up

    switch (pInformationType) {
    case Qualifier:
        emit qualifierLookUpRequested(qualifierAsString, pRetranslate);

        break;
    case Resource:
        emit resourceLookUpRequested(resourceAsString, pRetranslate);

        break;
    case Id:
        emit idLookUpRequested(resourceAsString, idAsString, pRetranslate);

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

    // Update the enabled state of our various add buttons

    updateGui(mElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookUpQualifier()
{
    // Enable the looking up of information

    mLookUpInformation = true;

    // Call our generic look up function

    if (mLookUpQualifierButton->isChecked())
        // We want to look something up, so...

        genericLookUp(mQualifierValue->currentText(), Qualifier);
    else
        // We don't want to look anything up anymore, so...

        genericLookUp();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::linkHovered(const QString &pLink,
                                                                const QString &pTitle,
                                                                const QString &pTextContent)
{
    Q_UNUSED(pTitle);

    // Keep track of the link and text content

    mLink = pLink;
    mTextContent = pTextContent;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::linkClicked()
{
    // Manually get the link and text content values for the link that just got
    // clicked
    // Note: normally, we get that information by handling the linkHovered()
    //       signal (see above), but it may be wrong when the user actually
    //       clicks on a link. Indeed, say that you are over a link and then
    //       scroll the list using your mouse wheel, and you end up over another
    //       link. Now, because your mouse didn't move, no linkHovered() message
    //       was emitted, whcih means that both mLink and mTextContent contain
    //       the wrong information...

    QWebHitTestResult hitTestResult = mOutputPossibleOntologicalTerms->page()->mainFrame()->hitTestContent(mOutputPossibleOntologicalTerms->mapFromGlobal(QCursor::pos()));
    QWebElement element = hitTestResult.element();

    if (!element.tagName().compare("IMG"))
        mLink = element.parent().attribute("href");
    else
        mLink = element.attribute("href");

    mTextContent = hitTestResult.linkText();

    // Check whether we have clicked a resource/id link or a button link

    if (mTextContent.isEmpty()) {
        // We have clicked on a button link, so retrieve the item associated
        // with the add button

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

        QWebElement documentElement = mOutputPossibleOntologicalTerms->page()->mainFrame()->documentElement();

        documentElement.findFirst(QString("td[id=button_%1]").arg(mLink)).setStyleProperty("display", "none");
        documentElement.findFirst(QString("td[id=disabledButton_%1]").arg(mLink)).setStyleProperty("display", "table-cell");

        // Let people know that we have added an RDF triple

        emit rdfTripleAdded(rdfTriple);
    } else {
        // We have clicked on a resource/id link, so start by enabling the
        // looking up of information

        mLookUpInformation = true;

        // Call our generic look up function

        genericLookUp(mLink, mUrls.contains(mTextContent)?Resource:Id);
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
    // Update the enabled state of our various add buttons

    updateGui(mElement, true);

    // Retrieve some possible ontological terms based on the given term, but
    // only if the term cannot be added directly and if it is not empty

    if (!isDirectTerm(pTerm) && !pTerm.isEmpty()) {
        // Add the term to our list of terms to look up

        mTerms << pTerm;

        // Retrieve some possible ontological terms, but after a short delay
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

    // Now, retrieve some possible ontological terms

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
            // Something went wrong, so...

            errorMessage = jsonParseError.errorString();
        }
    } else {
        // Something went wrong, so...

        errorMessage = pNetworkReply->errorString();
    }

    // Update our GUI with the results of the look up after having sorted them
    // and kept track of it size

    std::sort(items.begin(), items.end());

    mItemsCount = items.count();

    updateItemsGui(items, false, errorMessage);

    // Update the enabled state of our various add buttons

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

    // Let people know that we have added an RDF triple

    emit rdfTripleAdded(rdfTriple);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::showCustomContextMenu(const QPoint &pPosition)
{
    Q_UNUSED(pPosition);

    // Show our context menu to allow the copying of the URL of the resource or
    // id, but only if we are over a link, i.e. if both mLink and mTextContent
    // are not empty

    if (!mLink.isEmpty() && !mTextContent.isEmpty()) {
        // Keep track of both mLink and mTextContent
        // Note: indeed, as soon as we show our context menu linkHovered() will
        //       get called (since we won't be hovering the link anymore),
        //       resulting in both mLink and mTextContent becoming empty...

        mItemInformation = mLink;
        mItemResourceOrId = mTextContent;

        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::on_actionCopy_triggered()
{
    // Copy the URL of the resource or id to the clipboard

    if (mUrls.contains(mItemResourceOrId))
        QApplication::clipboard()->setText(mUrls.value(mItemResourceOrId));
    else
        QApplication::clipboard()->setText(mUrls.value(mItemInformation));
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::fileReloaded()
{
    // The file has been reloaded, so we need to reset our various user fields

    mQualifierValue->setCurrentIndex(0);
    mLookUpQualifierButton->setChecked(false);

    mTermValue->setText(QString());
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::recenterBusyWidget()
{
    // Recenter our busy widget

    mParent->parent()->centerBusyWidget();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
