//==============================================================================
// CellML annotation view metadata edit details widget
//==============================================================================

#include "cellmlannotationviewmetadataeditdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "coreutils.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadataeditdetailswidget.h"

//==============================================================================

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QStackedWidget>
#include <QVariant>
#include <QVBoxLayout>

//==============================================================================

#include <QJsonParser>

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
    mLookupButton(0),
    mLookupButtonIsChecked(false),
    mTerm(QString()),
    mTermUrl(QString()),
    mOtherTermUrl(QString()),
    mItems(Items()),
    mErrorMsg(QString()),
    mInformation(QString()),
    mType(No),
    mLookupInformation(false),
    mItemsMapping(QMap<QObject *, Item>())
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
            this, SLOT(termLookupFinished(QNetworkReply *)) );
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

    updateGui(mItems, mErrorMsg, true);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateGui(const Items &pItems,
                                                              const QString &pErrorMsg,
                                                              const bool &pRetranslate)
{
    // Note: we are using certain layouts to dislay the contents of our view,
    //       but this unfortunately results in some very bad flickering on Mac
    //       OS X. This can, however, be addressed using a stacked widget, so...

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

    connect(mQualifierValue, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(qualifierChanged(const QString &)));

    // Create our qualifier lookup button widget

    mLookupButton = new QPushButton(qualifierWidget);
    // Note #1: ideally, we could assign a QAction to our QPushButton, but this
    //          cannot be done, so... we assign a few properties by hand...
    // Note #2: to use a QToolButton would allow us to assign a QAction to it,
    //          but a QToolButton doesn't look quite the same as a QPushButton
    //          on some platforms, so...

    mLookupButton->setCheckable(true);
    mLookupButton->setChecked(mLookupButtonIsChecked);
    mLookupButton->setIcon(QIcon(":/oxygen/categories/applications-internet.png"));
    mLookupButton->setStatusTip(tr("Look up the qualifier"));
    mLookupButton->setToolTip(tr("Look Up"));
    mLookupButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(mLookupButton, SIGNAL(clicked()),
            this, SLOT(lookupQualifier()));

    // Add our QComboBox and QPushButton to our cmeta:id widget

    qualifierWidgetLayout->addWidget(mQualifierValue);
    qualifierWidgetLayout->addWidget(mLookupButton);

    // Add our cmeta:id widget to our main layout

    newFormLayout->addRow(Core::newLabel(newFormWidget, tr("Qualifier:"), 1.0, true),
                          qualifierWidget);

    // Add our term field

    QLineEdit *termValue = new QLineEdit(newFormWidget);

    termValue->setText(mTerm);
    // Note: we set the text to whatever term was previously being looked up and
    //       this before tracking changes to the term since we don't want to
    //       trigger a call to lookupTerm(). Indeed, we might come here as a
    //       result of a retranslation so we shouldn't look up for the term and,
    //       instead, we should call updateItemsGui() which we do at the end of
    //       this procedure...

    connect(termValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(lookupTerm(const QString &)));

    newFormLayout->addRow(Core::newLabel(newFormWidget, tr("Term:"), 1.0, true),
                          termValue);

    // Let people know that the GUI has been populated

    emit guiPopulated(mQualifierValue, mLookupButton, termValue);

    // Create a stacked widget (within a scroll area, so that only the items get
    // scrolled, not the whole metadata edit details widget) which will contain
    // a grid with the results of our ontological terms lookup

    QScrollArea *newItemsScrollArea = new QScrollArea(newMainWidget);

    newItemsScrollArea->setFrameShape(QFrame::NoFrame);
    newItemsScrollArea->setWidgetResizable(true);

    // Add our 'internal' widgets to our new main widget

    newMainLayout->addWidget(newFormWidget);
    newMainLayout->addWidget(Core::newLineWidget(newMainWidget));
    newMainLayout->addWidget(newItemsScrollArea);

    // Add our new widget to our stacked widget

    mWidget->addWidget(newMainWidget);

    // Remove the contents of our old form layout

    if (mFormWidget)
        for (int i = 0, iMax = mFormLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mFormLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

    // Reset the widget of our old items scroll area
    // Note: this will automatically delete the old grid widget...

    if (mItemsScrollArea)
        mItemsScrollArea->setWidget(0);

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

    updateItemsGui(pItems, pErrorMsg);

    // Request for something to be looked up, if needed

    if (mLookupInformation)
        // Look up an 'old' qualifier, resource or resource id

        genericLookup(mInformation, mType, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::updateItemsGui(const Items &pItems,
                                                                   const QString &pErrorMsg)
{
    Q_ASSERT(mItemsScrollArea);

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Keep track of the items and error message

    mItems = pItems;
    mErrorMsg = pErrorMsg;

    // Create a new widget and layout

    QWidget *newGridWidget = new QWidget(mItemsScrollArea);
    QGridLayout *newGridLayout = new QGridLayout(newGridWidget);

    newGridWidget->setLayout(newGridLayout);

    // Populate our new layout, but only if there is at least one item

    if (pItems.isEmpty()) {
        // No items to show, so either there is no data available or an error
        // occurred

        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                pErrorMsg.isEmpty()?tr("No data available..."):pErrorMsg,
                                                1.25, false, false, Qt::AlignCenter),
                                 0, 0);

        // Pretend that we want to look nothing up, if needed
        // Note: this is in case a resource or id used to be looked up, in which
        //       case we don't want it to be anymore...

        if (mLookupInformation && (mType != Qualifier))
            genericLookup();
    } else {
        // Create labels to act as headers

        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                tr("Name"),
                                                1.25, true, false, Qt::AlignCenter),
                                 0, 0);
        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                tr("Resource"),
                                                1.25, true, false, Qt::AlignCenter),
                                 0, 1);
        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                tr("Id"),
                                                1.25, true, false, Qt::AlignCenter),
                                 0, 2);

        // Number of terms

        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                (pItems.count() == 1)?
                                                    tr("(1 term)"):
                                                    tr("(%1 terms)").arg(QString::number(pItems.count())),
                                                1.0, false, true, Qt::AlignCenter),
                                 0, 3);

        // Add the items

        int row = 0;

        foreach (const Item &item, pItems) {
            // Name

            newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                    item.name,
                                                    1.0, false, false, Qt::AlignCenter),
                                     ++row, 0);

            // Resource

            QString information = item.resource+"|"+item.id;

            QLabel *resourceLabel = Core::newLabelLink(newGridWidget,
                                                       "<a href=\""+information+"\">"+item.resource+"</a>",
                                                       1.0, false, false, Qt::AlignCenter);

            connect(resourceLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResource(const QString &)));

            newGridLayout->addWidget(resourceLabel, row, 1);

            // Id

            QLabel *idLabel = Core::newLabelLink(newGridWidget,
                                                 "<a href=\""+information+"\">"+item.id+"</a>",
                                                 1.0, false, false, Qt::AlignCenter);

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
            addButton->setStatusTip(tr("Add the metadata information"));
            addButton->setToolTip(tr("Add"));
            addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            mItemsMapping.insert(addButton, item);

            connect(addButton, SIGNAL(clicked()),
                    this, SLOT(addRdfTriple()));

            newGridLayout->addWidget(addButton, row, 3, Qt::AlignCenter);
        }

        // Have the remove buttons column take as little horizontal space as
        // possible compared to the other columns

        newGridLayout->setColumnStretch(0, 1);
        newGridLayout->setColumnStretch(1, 1);
        newGridLayout->setColumnStretch(2, 1);
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

void CellmlAnnotationViewMetadataEditDetailsWidget::genericLookup(const QString &pInformation,
                                                                  const Type &pType,
                                                                  const bool &pRetranslate)
{
    // Retrieve the information

    QStringList informationAsStringList = pInformation.split("|");
    QString qualifierAsString = (pType != Qualifier)?QString():pInformation;
    QString resourceAsString = (pInformation.isEmpty() || (pType == Qualifier))?QString():informationAsStringList[0];
    QString idAsString = (pInformation.isEmpty() || (pType == Qualifier))?QString():informationAsStringList[1];

    // Keep track of the information

    mInformation = pInformation;
    mType = pType;

    // Toggle the lookup button, if needed

    if ((mType != Qualifier) && mLookupButton->isChecked())
        mLookupButton->toggle();

    // Make the row corresponding to the resource or id bold
    // Note: to use mGridLayout->rowCount() to determine the number of rows
    //       isn't an option since the returned value will be the maximum number
    //       of rows that there has ever been, so...

    int row = 0;

    forever
        if (mGridLayout->itemAtPosition(++row, 0)) {
            // Valid row, so check whether to make it bold (and italic in some
            // cases) or not

            QLabel *nameLabel     = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 0)->widget());
            QLabel *resourceLabel = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 1)->widget());
            QLabel *idLabel       = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 2)->widget());

            QFont font = idLabel->font();

            font.setBold(   mLookupInformation
                         && !resourceLabel->text().compare("<a href=\""+pInformation+"\">"+resourceAsString+"</a>")
                         && !idLabel->text().compare("<a href=\""+pInformation+"\">"+idAsString+"</a>"));
            font.setItalic(false);

            QFont italicFont = idLabel->font();

            italicFont.setBold(font.bold());
            italicFont.setItalic(font.bold());

            nameLabel->setFont(font);
            resourceLabel->setFont((pType == Resource)?italicFont:font);
            idLabel->setFont((pType == Id)?italicFont:font);
        } else {
            // No more rows, so...

            break;
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

void CellmlAnnotationViewMetadataEditDetailsWidget::qualifierChanged(const QString &pQualifier)
{
    // Lookup the qualifier, if requested

    if (mLookupButton->isChecked()) {
        // Enable the looking up of information

        mLookupInformation = true;

        // Call our generic lookup function

        genericLookup(pQualifier, Qualifier);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupQualifier()
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Keep track of the checked status of our lookup button

    mLookupButtonIsChecked = mLookupButton->isChecked();

    // Call our generic lookup function

    if (mLookupButton->isChecked())
        // We want to look something up, so...

        genericLookup(mQualifierValue->currentText(), Qualifier);
    else
        // We don't want to look anything up anymore, so...

        genericLookup();
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupResource(const QString &pInformation)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pInformation, Resource);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupId(const QString &pInformation)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pInformation, Id);
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::lookupTerm(const QString &pTerm)
{
    // Keep track of the term to look up

    mTerm = pTerm;

    // Retrieve some possible ontological terms based on the given term

    QString termUrl = QString("http://www.semanticsbml.org/semanticSBML/annotate/search.json?q=%1&full_info=1").arg(pTerm);

    if (mTermUrl.isEmpty()) {
        // No other term is being looked up, so keep track of the given term and
        // look it up

        mTermUrl = termUrl;

        mNetworkAccessManager->get(QNetworkRequest(termUrl));
    } else {
        // Another term is already being looked up, so keep track of the given
        // term for later

        mOtherTermUrl = termUrl;
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::termLookupFinished(QNetworkReply *pNetworkReply)
{
    // Retrieve the list of ontological terms, should we have retrieved it
    // without any problem

    Items items = Items();
    QString errorMsg = QString();

    if (pNetworkReply->error() == QNetworkReply::NoError) {
        // Parse the JSON code

        QJson::Parser jsonParser;
        bool parsingOk;

        QVariantMap resultMap = jsonParser.parse(pNetworkReply->readAll(), &parsingOk).toMap();

        if (parsingOk) {
            // Retrieve the list of ontological terms

            foreach (const QVariant &ontologicalsTermVariant, resultMap["result"].toList()) {
                QVariantList ontologicalTermVariant = ontologicalsTermVariant.toList();

                for (int i = 0, iMax = ontologicalTermVariant.count(); i < iMax; ++i) {
                    // At this stage, we have an ontological term in the form of
                    // a MIRIAM URN and a name (as well as a URL, but we don't
                    // care about it), so we need to decode the MIRIAM URN to
                    // retrieve the corresponding resource and id

                    QVariantMap ontologicalTermMap = ontologicalTermVariant[i].toMap();

                    QString resource = QString();
                    QString id = QString();

                    CellMLSupport::CellmlFileRdfTriple::decodeMiriamUrn(ontologicalTermMap["uri"].toString(),
                                                                        resource, id);

                    // Add the ontological term to our list

                    items << item(ontologicalTermMap["name"].toString(),
                                  resource, id);
                }
            }
        } else {
            // Something went wrong, so...

            errorMsg = jsonParser.errorString();
        }
    } else {
        // Something went wrong, so...

        errorMsg = pNetworkReply->errorString();
    }

    // We are done looking up the term, so...

    mTermUrl = QString();

    // Look up another term, should there be one to look up, or update the GUI
    // with the results of the lookup

    if (!mOtherTermUrl.isEmpty()) {
        // There is another term to look up, so...

        mNetworkAccessManager->get(QNetworkRequest(mOtherTermUrl));

        mOtherTermUrl = QString();
    } else {
        // No other term to look up, so we can update our GUI with the results
        // of the lookup after having sorted them

        qSort(items.begin(), items.end());

        updateItemsGui(items, errorMsg);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataEditDetailsWidget::addRdfTriple()
{
    // Retrieve the item associated with the add button

    QObject *addButton = sender();

    Item item = mItemsMapping.value(addButton);

    // Determine what the subject of the item's corresponding RDF triple should
    // be

    QString rdfTripleSubject = QUrl::fromLocalFile(mCellmlFile->fileName()).toString()+"#"+mParent->currentMetadataId();

    // Add the item as a RDF triple to the CellML file

    CellMLSupport::CellmlFileRdfTriple *rdfTriple;

    if (mQualifierValue->currentIndex() < CellMLSupport::CellmlFileRdfTriple::LastBioQualifier)
        // We want to use a biology qualifier, so...

        rdfTriple = new CellMLSupport::CellmlFileRdfTriple(rdfTripleSubject,
                                                           CellMLSupport::CellmlFileRdfTriple::BioQualifier(mQualifierValue->currentIndex()+1),
                                                           item.resource, item.id);
    else
        // We want to use a model qualifier, so...

        rdfTriple = new CellMLSupport::CellmlFileRdfTriple(rdfTripleSubject,
                                                           CellMLSupport::CellmlFileRdfTriple::ModelQualifier(mQualifierValue->currentIndex()-CellMLSupport::CellmlFileRdfTriple::LastBioQualifier+1),
                                                           item.resource, item.id);

    mCellmlFile->rdfTriples()->add(rdfTriple);

    // Let people know that some metadata has been added

    emit metadataAdded(rdfTriple);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
