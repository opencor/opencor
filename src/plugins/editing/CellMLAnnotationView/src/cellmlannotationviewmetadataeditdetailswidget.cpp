//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilerdftriple.h"
#include "coreutils.h"
#include "treeviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataeditdetailswidget.h"

//==============================================================================

#include <QClipboard>
#include <QComboBox>
#include <QFormLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollBar>
#include <QStackedWidget>
#include <QVariant>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

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

CellmlAnnotationViewMetadataEditDetailsWidget::CellmlAnnotationViewMetadataEditDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mCellmlFile(pParent->cellmlFile()),
    mGui(new Ui::CellmlAnnotationViewMetadataEditDetailsWidget),
    mMainWidget(0),
    mMainLayout(0),
    mFormWidget(0),
    mFormLayout(0),
    mItemsScrollArea(0),
    mGridWidget(0),
    mGridLayout(0),
    mQualifierValue(0),
    mLookupQualifierButton(0),
    mQualifierIndex(0),
    mLookupQualifierButtonIsChecked(false),
    mTermValue(0),
    mAddTermButton(0),
    mTerm(QString()),
    mTermIsDirect(false),
    mTermUrl(QString()),
    mOtherTermUrl(QString()),
    mItems(Items()),
    mErrorMsg(QString()),
    mLookupTerm(false),
    mInformation(QString()),
    mType(No),
    mLookupInformation(false),
    mItemsMapping(QMap<QObject *, Item>()),
    mItemsVerticalScrollBarPosition(0),
    mCellmlElement(0),
    mCurrentResourceOrIdLabel(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a stacked widget which will contain our GUI

    mWidget = new QStackedWidget(this);

    // Add our stacked widget to our scroll area

    setWidget(mWidget);

    // Create a network access manager so that we can then retrieve a list of
    // CellML models from the CellML Model Repository

    mNetworkAccessManager = new QNetworkAccessManager(this);

    // Make sure that we get told when the download of our Internet file is
    // complete

    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(termLookedUp(QNetworkReply *)) );
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

    // For the rest of our GUI, it's easier to just update it, so...

    updateGui(mItems, mErrorMsg, mLookupTerm, mItemsVerticalScrollBarPosition, true);

    // Update the enabled state of our various add buttons

    updateGui(mCellmlElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(CellMLSupport::CellmlFileElement *pCellmlElement)
{
    // Keep track of the CellML element

    mCellmlElement = pCellmlElement;

    // Update the add term button, depending on whether the direct term is
    // already associated with the CellML element

    if (mTermIsDirect) {
        QStringList termInformation = mTerm.split("/");

        if (mQualifierIndex < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            mAddTermButton->setEnabled(!mCellmlElement->hasMetadata(CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierIndex+1),
                                                                    termInformation[0], termInformation[1]));
        else
            mAddTermButton->setEnabled(!mCellmlElement->hasMetadata(CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierIndex-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                                    termInformation[0], termInformation[1]));
    } else {
        mAddTermButton->setEnabled(false);
    }

    // Enable or disable the add buttons for our retrieved terms, depending on
    // whether they are already associated with the CellML element

    int row = 0;

    forever {
        if (!mGridLayout->itemAtPosition(++row, 0))
            break;

        QPushButton *addButton = qobject_cast<QPushButton *>(mGridLayout->itemAtPosition(row, 3)->widget());

        Item item = mItemsMapping.value(addButton);

        if (mQualifierIndex < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
            addButton->setEnabled(!mCellmlElement->hasMetadata(CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierIndex+1),
                                                               item.resource, item.id));
        else
            addButton->setEnabled(!mCellmlElement->hasMetadata(CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierIndex-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                               item.resource, item.id));
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(const Items &pItems,
                                                              const QString &pErrorMsg,
                                                              const bool &pLookupTerm,
                                                              const int &pItemsVerticalScrollBarPosition,
                                                              const bool &pRetranslate)
{
    // Note: we are using certain layouts to dislay the contents of our view,
    //       but this unfortunately results in some very bad flickering on OS X.
    //       This can, however, be addressed using a stacked widget, so...

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Create a widget which will contain our GUI

    QWidget *newMainWidget = new QWidget(this);
    QVBoxLayout *newMainLayout = new QVBoxLayout(newMainWidget);

    newMainLayout->setMargin(0);

    newMainWidget->setLayout(newMainLayout);

    // Create a form widget which will contain our qualifier and term fields

    QWidget *newFormWidget = new QWidget(newMainWidget);
    QFormLayout *newFormLayout = new QFormLayout(newFormWidget);

    newFormWidget->setLayout(newFormLayout);

    // Add our qualifier field

    // Create a widget which will contain both our qualifier value widget and a
    // button to look up the qualifier

    QWidget *qualifierWidget = new QWidget(newFormWidget);

    QHBoxLayout *qualifierWidgetLayout = new QHBoxLayout(qualifierWidget);

    qualifierWidgetLayout->setMargin(0);

    qualifierWidget->setLayout(qualifierWidgetLayout);

    // Create our qualifier value widget

    mQualifierValue = new QComboBox(qualifierWidget);

    mQualifierValue->addItems(CellMLSupport::CellmlFileRdfTriple::qualifiersAsStringList());

    mQualifierValue->setCurrentIndex(mQualifierIndex);

    connect(mQualifierValue, SIGNAL(currentIndexChanged(int)),
            this, SLOT(qualifierChanged(const int &)));
    connect(mQualifierValue, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(qualifierChanged(const QString &)));

    // Create our qualifier lookup button widget

    mLookupQualifierButton = new QPushButton(qualifierWidget);
    // Note #1: ideally, we could assign a QAction to our QPushButton, but this
    //          cannot be done, so... we assign a few properties by hand...
    // Note #2: to use a QToolButton would allow us to assign a QAction to it,
    //          but a QToolButton doesn't look quite the same as a QPushButton
    //          on some platforms, so...

    mLookupQualifierButton->setCheckable(true);
    mLookupQualifierButton->setChecked(mLookupQualifierButtonIsChecked);
    mLookupQualifierButton->setIcon(QIcon(":/oxygen/categories/applications-internet.png"));
    mLookupQualifierButton->setStatusTip(tr("Look up the qualifier"));
    mLookupQualifierButton->setToolTip(tr("Look Up"));
    mLookupQualifierButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(mLookupQualifierButton, SIGNAL(clicked()),
            this, SLOT(lookupQualifier()));

    // Add our QComboBox and QPushButton to our cmeta:id widget

    qualifierWidgetLayout->addWidget(mQualifierValue);
    qualifierWidgetLayout->addWidget(mLookupQualifierButton);

    // Add our qualifier widget to our main layout

    newFormLayout->addRow(Core::newLabel(tr("Qualifier:"), 1.0, true, newFormWidget),
                          qualifierWidget);

    // Add our term field

    // Create a widget which will contain both our qualifier value widget and a
    // button to look up the qualifier

    QWidget *termWidget = new QWidget(newFormWidget);

    QHBoxLayout *termWidgetLayout = new QHBoxLayout(termWidget);

    termWidgetLayout->setMargin(0);

    termWidget->setLayout(termWidgetLayout);

    // Create our term value widget

    mTermValue = new QLineEdit(termWidget);

    mTermValue->setText(mTerm);
    // Note: we set the text to whatever term was previously being looked up and
    //       this before tracking changes to the term since we don't want to
    //       trigger a call to termChanged(). Indeed, we might come here as a
    //       result of a retranslation so we shouldn't look up for the term and,
    //       instead, we should call updateItemsGui() which we do at the end of
    //       this procedure...

    connect(mTermValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(termChanged(const QString &)));

    // Create our add term button widget

    mAddTermButton = new QPushButton(termWidget);

    mAddTermButton->setEnabled(false);
    mAddTermButton->setIcon(QIcon(":/oxygen/actions/list-add.png"));
    mAddTermButton->setStatusTip(tr("Add the term"));
    mAddTermButton->setToolTip(tr("Add"));
    mAddTermButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(mAddTermButton, SIGNAL(clicked()),
            this, SLOT(addTerm()));

    // Add our QComboBox and QPushButton to our cmeta:id widget

    termWidgetLayout->addWidget(mTermValue);
    termWidgetLayout->addWidget(mAddTermButton);

    // Add our term widget to our main layout

    newFormLayout->addRow(Core::newLabel(tr("Term:"), 1.0, true, newFormWidget),
                          termWidget);

    // Reset the tab order from our parent's CellML list's tree view widget
    // Note: ideally, we would take advantage of Qt's signal/slot approach with
    //       the signal being emitted here and the slot being implemented in
    //       mParent, but this wouldn't work here since updateGui() gets called
    //       as part of the creation of this metadata details widget, so...

    setTabOrder(qobject_cast<QWidget *>(mParent->cellmlList()->treeViewWidget()),
                mQualifierValue);
    setTabOrder(mQualifierValue, mLookupQualifierButton);
    setTabOrder(mLookupQualifierButton, mTermValue);
    setTabOrder(mTermValue, mAddTermButton);

    // Create a stacked widget (within a scroll area, so that only the items get
    // scrolled, not the whole metadata edit details widget) which will contain
    // a grid with the results of our terms lookup

    QScrollArea *newItemsScrollArea = new QScrollArea(newMainWidget);

    newItemsScrollArea->setFrameShape(QFrame::NoFrame);
    newItemsScrollArea->setWidgetResizable(true);

    // Add our 'internal' widgets to our new main widget

    newMainLayout->addWidget(newFormWidget);
    newMainLayout->addWidget(new Core::BorderedWidget(newItemsScrollArea,
                                                      true, false, false, false));

    // Keep track of the position of our items vertical scroll bar
    // Note: this is required to make sure that the position doesn't get reset
    //       as a result of retranslating the GUI...

    connect(newItemsScrollArea->verticalScrollBar(), SIGNAL(sliderMoved(int)),
            this, SLOT(trackItemsVerticalScrollBarPosition(const int &)));

    // Add our new widget to our stacked widget

    mWidget->addWidget(newMainWidget);

    // Remove the contents of our old form layout

    if (mFormWidget)
        for (int i = 0, iMax = mFormLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mFormLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

    // Reset the widget of our old items scroll area and stop tracking the
    // position of its vertical scroll bar
    // Note: the resetting this will automatically delete our old grid widget...

    if (mItemsScrollArea) {
        mItemsScrollArea->setWidget(0);

        disconnect(mItemsScrollArea->verticalScrollBar(), SIGNAL(sliderMoved(int)),
                   this, SLOT(trackItemsVerticalScrollBarPosition(const int &)));
    }

    // Get rid of our old main widget and layout (and its contents)

    if (mMainWidget) {
        mWidget->removeWidget(mMainWidget);

        for (int i = 0, iMax = mMainLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mMainLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

        delete mMainWidget;
    }

    // Keep track of our new main widgets and layouts

    mMainWidget = newMainWidget;
    mMainLayout = newMainLayout;

    mFormWidget = newFormWidget;
    mFormLayout = newFormLayout;

    mItemsScrollArea = newItemsScrollArea;

    mGridWidget = 0;   // Note: this will be set by our
    mGridLayout = 0;   //       other updateGui() function...

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);

    // Update our items GUI

    updateItemsGui(pItems, pErrorMsg, pLookupTerm);

    // Request for something to be looked up, if needed

    if (mLookupInformation)
        // Look up an 'old' qualifier, resource or resource id

        genericLookup(mInformation, mType, pRetranslate);

    // Set the position of our vertical scroll bar

    mItemsScrollArea->verticalScrollBar()->setValue(pItemsVerticalScrollBarPosition);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateItemsGui(const Items &pItems,
                                                                   const QString &pErrorMsg,
                                                                   const bool &pLookupTerm)
{
    Q_ASSERT(mItemsScrollArea);

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Keep track of some information

    mItems = pItems;
    mErrorMsg = pErrorMsg;
    mLookupTerm = pLookupTerm;

    // Create a new widget and layout

    QWidget *newGridWidget = new QWidget(mItemsScrollArea);
    QGridLayout *newGridLayout = new QGridLayout(newGridWidget);

    newGridWidget->setLayout(newGridLayout);

    // Populate our new layout, but only if there is at least one item

    if (pItems.count()) {
        // Create labels to act as headers

        newGridLayout->addWidget(Core::newLabel(tr("Name"),
                                                1.25, true, false,
                                                Qt::AlignCenter,
                                                newGridWidget),
                                 0, 0);
        newGridLayout->addWidget(Core::newLabel(tr("Resource"),
                                                1.25, true, false,
                                                Qt::AlignCenter,
                                                newGridWidget),
                                 0, 1);
        newGridLayout->addWidget(Core::newLabel(tr("Id"),
                                                1.25, true, false,
                                                Qt::AlignCenter,
                                                newGridWidget),
                                 0, 2);

        // Number of terms

        newGridLayout->addWidget(Core::newLabel((pItems.count() == 1)?
                                                    tr("(1 term)"):
                                                    tr("(%1 terms)").arg(QString::number(pItems.count())),
                                                1.0, false, true,
                                                Qt::AlignCenter,
                                                newGridWidget),
                                 0, 3);

        // Add the items

        int row = 0;

        foreach (const Item &item, pItems) {
            // Name

            newGridLayout->addWidget(Core::newLabel(item.name,
                                                    1.0, false, false,
                                                    Qt::AlignCenter,
                                                    newGridWidget),
                                     ++row, 0);

            // Resource

            QString itemInformation = item.resource+"|"+item.id;

            QLabel *resourceLabel = Core::newLabel("<a href=\""+itemInformation+"\">"+item.resource+"</a>",
                                                   1.0, false, false,
                                                   Qt::AlignCenter,
                                                   newGridWidget);

            resourceLabel->setAccessibleDescription("http://identifiers.org/"+item.resource+"/?redirect=true");
            resourceLabel->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(resourceLabel, SIGNAL(customContextMenuRequested(const QPoint &)),
                    this, SLOT(showCustomContextMenu(const QPoint &)));
            connect(resourceLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResource(const QString &)));

            newGridLayout->addWidget(resourceLabel, row, 1);

            // Id

            QLabel *idLabel = Core::newLabel("<a href=\""+itemInformation+"\">"+item.id+"</a>",
                                             1.0, false, false,
                                             Qt::AlignCenter,
                                             newGridWidget);

            idLabel->setAccessibleDescription("http://identifiers.org/"+item.resource+"/"+item.id+"/?profile=most_reliable&redirect=true");
            idLabel->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(idLabel, SIGNAL(customContextMenuRequested(const QPoint &)),
                    this, SLOT(showCustomContextMenu(const QPoint &)));
            connect(idLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupId(const QString &)));

            newGridLayout->addWidget(idLabel, row, 2);

            // Add button

            QPushButton *addButton = new QPushButton(newGridWidget);
            // Note #1: ideally, we could assign a QAction to our
            //          QPushButton, but this cannot be done, so... we
            //          assign a few properties by hand...
            // Note #2: to use a QToolButton would allow us to assign a
            //          QAction to it, but a QToolButton doesn't look quite
            //          the same as a QPushButton on some platforms, so...

            addButton->setIcon(QIcon(":/oxygen/actions/list-add.png"));
            addButton->setStatusTip(tr("Add the term"));
            addButton->setToolTip(tr("Add"));
            addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            mItemsMapping.insert(addButton, item);

            connect(addButton, SIGNAL(clicked()),
                    this, SLOT(addRetrievedTerm()));

            newGridLayout->addWidget(addButton, row, 3, Qt::AlignCenter);
        }

        // Have the remove buttons column take as little horizontal space as
        // possible compared to the other columns

        newGridLayout->setColumnStretch(0, 1);
        newGridLayout->setColumnStretch(1, 1);
        newGridLayout->setColumnStretch(2, 1);

        // Have all the rows take a minimum of vertical space

        newGridLayout->setRowStretch(++row, 1);
    } else {
        // No items to show, so either there is no data available or an error
        // occurred

        QString labelText;

        if (mTerm.isEmpty()) {
            labelText = tr("Please enter a term to search above...");
        } else if (pLookupTerm) {
            labelText = tr("Please wait while we are retrieving possible terms for <strong>%1</strong>...").arg(mTerm);
        } else if (pErrorMsg.isEmpty()) {
            if (mTermIsDirect)
                labelText = tr("<strong>Information:</strong> you can directly add <strong>%1</strong>...").arg(mTerm);
            else
                labelText = tr("Sorry, but no terms were found for <strong>%1</strong>...").arg(mTerm);
        } else {
            QString errorMsg = pErrorMsg[0].toLower()+pErrorMsg.right(pErrorMsg.size()-1);
            QString dots = (errorMsg[errorMsg.size()-1] == '.')?"..":"...";

            labelText = tr("<strong>Error:</strong> ")+errorMsg+dots;
        }

        newGridLayout->addWidget(Core::newLabel(labelText,
                                                1.25, false, false,
                                                Qt::AlignCenter,
                                                newGridWidget),
                                 0, 0);

        // Pretend that we want to look nothing up, if needed
        // Note: this is in case a resource or id used to be looked up, in which
        //       case we don't want it to be anymore...

        if (mLookupInformation && (mType != Qualifier))
            genericLookup();
    }

    // Set our new grid widget as the widget for our scroll area
    // Note: this will automatically delete the old grid widget...

    mItemsScrollArea->setWidget(newGridWidget);

    // Keep track of our new grid widgets and layouts

    mGridWidget = newGridWidget;
    mGridLayout = newGridLayout;

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);
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

void CellmlAnnotationViewMetadataEditDetailsWidget::genericLookup(const QString &pItemInformation,
                                                                  const Type &pType,
                                                                  const bool &pRetranslate)
{
    // Retrieve the information

    QStringList itemInformationAsStringList = pItemInformation.split("|");
    QString qualifierAsString = (pType != Qualifier)?QString():pItemInformation;
    QString resourceAsString = (pItemInformation.isEmpty() || (pType == Qualifier))?QString():itemInformationAsStringList[0];
    QString idAsString = (pItemInformation.isEmpty() || (pType == Qualifier))?QString():itemInformationAsStringList[1];

    // Keep track of the information

    mInformation = pItemInformation;
    mType = pType;

    // Toggle the lookup button, if needed

    if ((mType != Qualifier) && mLookupQualifierButton->isChecked())
        mLookupQualifierButton->toggle();

    // Make the row corresponding to the resource or id bold (and italic in some
    // cases)
    // Note: to use mGridLayout->rowCount() to determine the number of rows
    //       isn't an option since the returned value will be the maximum number
    //       of rows that there has ever been, so...

    int row = 0;

    forever {
        if (!mGridLayout->itemAtPosition(++row, 0))
            break;

        QLabel *nameLabel     = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 0)->widget());
        QLabel *resourceLabel = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 1)->widget());
        QLabel *idLabel       = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 2)->widget());

        QFont font = idLabel->font();

        font.setBold(   mLookupInformation
                     && !resourceLabel->text().compare("<a href=\""+pItemInformation+"\">"+resourceAsString+"</a>")
                     && !idLabel->text().compare("<a href=\""+pItemInformation+"\">"+idAsString+"</a>"));
        font.setItalic(false);

        QFont italicFont = idLabel->font();

        italicFont.setBold(font.bold());
        italicFont.setItalic(font.bold());

        nameLabel->setFont(font);
        resourceLabel->setFont((pType == Resource)?italicFont:font);
        idLabel->setFont((pType == Id)?italicFont:font);
    }

    // Check that we have something to look up

    if (!mLookupInformation)
        // Nothing to look up, so...

        return;

    // Let people know that we want to look something up

    switch (pType) {
    case Qualifier:
        emit qualifierLookupRequested(qualifierAsString, pRetranslate);

        break;
    case Resource:
        emit resourceLookupRequested(resourceAsString, pRetranslate);

        break;
    case Id:
        emit idLookupRequested(resourceAsString, idAsString, pRetranslate);

        break;
    default:
        // No

        emit noLookupRequested();
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::disableLookupInformation()
{
    // Disable the looking up of information

    mLookupInformation = false;

    // Update the GUI by pretending to be interested in looking something up

    genericLookup();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::qualifierChanged(const int &pQualifierIndex)
{
    // Keep track of the qualifier index

    mQualifierIndex = pQualifierIndex;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::qualifierChanged(const QString &pQualifier)
{
    // Lookup the qualifier, if requested

    if (mLookupQualifierButton->isChecked()) {
        // Enable the looking up of information

        mLookupInformation = true;

        // Call our generic lookup function

        genericLookup(pQualifier, Qualifier);
    }

    // Update the enabled state of our various add buttons

    updateGui(mCellmlElement);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupQualifier()
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Keep track of the checked status of our lookup button

    mLookupQualifierButtonIsChecked = mLookupQualifierButton->isChecked();

    // Call our generic lookup function

    if (mLookupQualifierButton->isChecked())
        // We want to look something up, so...

        genericLookup(mQualifierValue->currentText(), Qualifier);
    else
        // We don't want to look anything up anymore, so...

        genericLookup();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupResource(const QString &pItemInformation)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pItemInformation, Resource);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupId(const QString &pItemInformation)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pItemInformation, Id);
}

//==============================================================================

static const QString SemanticSbmlUrlStart = "http://www.semanticsbml.org/semanticSBML/annotate/search.json?q=";
static const QString SemanticSbmlUrlEnd   = "&full_info=1";

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::termChanged(const QString &pTerm)
{
    // Keep track of the term to look up

    mTerm = pTerm;

    // Check whether the term could be directly added, resulting in the add term
    // button being enabled/disabled, depending on the case

    mTermIsDirect = QRegularExpression("^"+CellMLSupport::ResourceRegExp+"/"+CellMLSupport::IdRegExp+"$").match(pTerm).hasMatch();

    // Update our items' GUI

    updateItemsGui(Items(), QString(), !mTermIsDirect);

    // Update the enabled state of our various add buttons

    updateGui(mCellmlElement);

    // Retrieve some possible terms based on the given term, but only if the
    // term cannot be added directly

    if (!mTermIsDirect) {
        // We are not dealing with a direct term, so retrieve some possible
        // terms

        QString termUrl = SemanticSbmlUrlStart+pTerm+SemanticSbmlUrlEnd;

        if (mTermUrl.isEmpty()) {
            // No other term is being looked up, so keep track of the given term
            // and look it up

            mTermUrl = termUrl;

            mNetworkAccessManager->get(QNetworkRequest(termUrl));
        } else {
            // Another term is already being looked up, so keep track of the
            // given term for later

            mOtherTermUrl = termUrl;
        }
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::termLookedUp(QNetworkReply *pNetworkReply)
{
    // We are done looking up the term, so...

    mTermUrl = QString();

    // Look up another term, should there be one to look up, or update the GUI
    // with the results of the lookup

    if (!mOtherTermUrl.isEmpty()) {
        // There is another term to look up, so...

        mNetworkAccessManager->get(QNetworkRequest(mOtherTermUrl));

        mOtherTermUrl = QString();
    } else {
        // No other term to look up, so make sure that the network reply we got
        // corresponds to that of the current term

        if (mTerm.compare(pNetworkReply->url().toString()
                                              .remove(QRegularExpression("^"+QRegularExpression::escape(SemanticSbmlUrlStart)))
                                              .remove(QRegularExpression(QRegularExpression::escape(SemanticSbmlUrlEnd)+"$")))) {
            // Not the correct term, so... delete (later) the network reply and
            // leave

            pNetworkReply->deleteLater();

            return;
        }

        // The network reply is for the current term, so retrieve the list of
        // terms, should we have retrieved it without any problem

        Items items = Items();
        QString errorMsg = QString();

        if (pNetworkReply->error() == QNetworkReply::NoError) {
            // Parse the JSON code

            QJsonParseError jsonParseError;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(pNetworkReply->readAll(), &jsonParseError);

            if (jsonParseError.error == QJsonParseError::NoError) {
                // Retrieve the list of terms

                QVariantMap resultMap = jsonDocument.object().toVariantMap();

                foreach (const QVariant &termsVariant, resultMap["result"].toList()) {
                    QVariantList termVariant = termsVariant.toList();

                    for (int i = 0, iMax = termVariant.count(); i < iMax; ++i) {
                        // At this stage, we have a term (in the form of either
                        // a MIRIAM URN or an identifiers.org URI) and a name
                        // (as well as a URL, but we don't care about it), so we
                        // need to decode the term to retrieve the corresponding
                        // resource and id

                        QVariantMap termMap = termVariant[i].toMap();

                        QString resource = QString();
                        QString id = QString();

                        if (!CellMLSupport::CellmlFileRdfTriple::decodeTerm(termMap["uri"].toString(),
                                                                            resource, id)) {
                            // The term couldn't be decoded, so...

                            items = Items();

                            errorMsg = tr("the search returned invalid results");

                            break;
                        } else {
                            // The term could be decoded, so add it to our list

                            items << item(termMap["name"].toString(), resource, id);
                        }
                    }

                    if (!errorMsg.isEmpty())
                        break;
                }
            } else {
                // Something went wrong, so...

                mErrorMsg = jsonParseError.errorString();
            }
        } else {
            // Something went wrong, so...

            errorMsg = pNetworkReply->errorString();
        }

        // Update our GUI with the results of the lookup after having sorted
        // them

        qSort(items.begin(), items.end());

        updateItemsGui(items, errorMsg, false);

        // Update the enabled state of our various add buttons

        updateGui(mCellmlElement);
    }

    // Delete (later) the network reply

    pNetworkReply->deleteLater();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::addTerm()
{
    // Add the term to our CellML element as an RDF triple

    CellMLSupport::CellmlFileRdfTriple *rdfTriple;
    QStringList termInformation = mTerm.replace("%3A", ":").split("/");

    if (mQualifierIndex < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
        rdfTriple = mCellmlElement->addMetadata(CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierIndex+1),
                                                termInformation[0], termInformation[1]);
    else
        rdfTriple = mCellmlElement->addMetadata(CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierIndex-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                termInformation[0], termInformation[1]);

    // Disable the add term buton, now that we have added the term

    mAddTermButton->setEnabled(false);

    // Let people know that we have added an RDF triple

    emit rdfTripleAdded(rdfTriple);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::addRetrievedTerm()
{
    // Retrieve the add button

    QPushButton *addButton = qobject_cast<QPushButton *>(sender());

    // Retrieve the item associated with the add button

    Item item = mItemsMapping.value(addButton);

    // Add the retrieved term to our CellML element as an RDF triple

    CellMLSupport::CellmlFileRdfTriple *rdfTriple;

    if (mQualifierIndex < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
        rdfTriple = mCellmlElement->addMetadata(CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierIndex+1),
                                                item.resource, item.id);
    else
        rdfTriple = mCellmlElement->addMetadata(CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierIndex-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                item.resource, item.id);

    // Disable the add button, now that we have added the retrieved term

    addButton->setEnabled(false);

    // Let people know that we have added an RDF triple

    emit rdfTripleAdded(rdfTriple);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::trackItemsVerticalScrollBarPosition(const int &pPosition)
{
    // Keep track of the new position of our vertical scroll bar

    mItemsVerticalScrollBarPosition = pPosition;
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::showCustomContextMenu(const QPoint &pPosition)
{
    Q_UNUSED(pPosition);

    // Keep track of the resource or id

    mCurrentResourceOrIdLabel = qobject_cast<QLabel *>(qApp->widgetAt(QCursor::pos()));

    // Create a custom context menu to allow the copying of the URL of the
    // resource or id

    QMenu menu;

    menu.addAction(mGui->actionCopy);

    menu.exec(QCursor::pos());
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::on_actionCopy_triggered()
{
    // Copy the URL of the resource or id to the clipboard

    QApplication::clipboard()->setText(mCurrentResourceOrIdLabel->accessibleDescription());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
