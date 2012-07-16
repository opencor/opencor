//==============================================================================
// CellML annotation view metadata BioModels.Net view details widget
//==============================================================================

#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "coreutils.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"

//==============================================================================

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                                                                                           const bool &pEditingMode) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mCellmlFile(pParent->cellmlFile()),
    mGui(new Ui::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget),
    mGridWidget(0),
    mGridLayout(0),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples(mCellmlFile)),
    mRdfTripleInformation(QString()),
    mType(No),
    mLookupInformation(true),
    mEditingMode(pEditingMode),
    mRdfTriplesMapping(QMap<QObject *, CellMLSupport::CellmlFileRdfTriple *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a stacked widget which will contain our GUI

    mWidget = new QStackedWidget(this);

    // Add our stacked widget to our scroll area

    setWidget(mWidget);
}

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::~CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // For the rest of our GUI, it's easier to just update it, so...

    updateGui(mRdfTriples, mRdfTripleInformation, mType, true);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples,
                                                                             const QString &pRdfTripleInformation,
                                                                             const Type &pType,
                                                                             const bool &pRetranslate)
{
    // Note: we are using a grid layout to dislay the contents of our view, but
    //       this unfortunately results in some very bad flickering on Mac OS X.
    //       This can, however, be addressed using a stacked widget, so...

    // Prevent ourselves from being updated (to avoid any flickering)

    setUpdatesEnabled(false);

    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Create a new widget and layout

    QWidget *newGridWidget = new QWidget(mWidget);
    QGridLayout *newGridLayout = new QGridLayout(newGridWidget);

    newGridWidget->setLayout(newGridLayout);

    // Populate our new layout, but only if there is at least one RDF triple

    QString firstRdfTripleInformation = QString();

    if (pRdfTriples.count()) {
        // Create labels to act as headers

        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                tr("Qualifier"),
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
                                                (pRdfTriples.count() == 1)?
                                                    tr("(1 term)"):
                                                    tr("(%1 terms)").arg(QString::number(pRdfTriples.count())),
                                                1.0, false, true, Qt::AlignCenter),
                                 0, 3);

        // Add the RDF triples information to our layout
        // Note: for the RDF triple's subject, we try to remove the CellML
        //       file's URI base, thus only leaving the equivalent of a CellML
        //       element cmeta:id which will speak more to the user than a
        //       possibly long URI reference...

        int row = 0;

        foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, pRdfTriples) {
            // Qualifier

            QString qualifierAsString = (rdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelUnknown)?
                                            rdfTriple->modelQualifierAsString():
                                            rdfTriple->bioQualifierAsString();
            QString rdfTripleInformation = qualifierAsString+"|"+rdfTriple->resource()+"|"+rdfTriple->id();

            QLabel *qualifierLabel = Core::newLabelLink(newGridWidget,
                                                        "<a href=\""+rdfTripleInformation+"\">"+qualifierAsString+"</a>",
                                                        1.0, false, false, Qt::AlignCenter);

            connect(qualifierLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupQualifier(const QString &)));

            newGridLayout->addWidget(qualifierLabel, ++row, 0);

            // Resource

            QLabel *resourceLabel = Core::newLabelLink(newGridWidget,
                                                       "<a href=\""+rdfTripleInformation+"\">"+rdfTriple->resource()+"</a>",
                                                       1.0, false, false, Qt::AlignCenter);

            connect(resourceLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResource(const QString &)));

            newGridLayout->addWidget(resourceLabel, row, 1);

            // Id

            QLabel *idLabel = Core::newLabelLink(newGridWidget,
                                                 "<a href=\""+rdfTripleInformation+"\">"+rdfTriple->id()+"</a>",
                                                 1.0, false, false, Qt::AlignCenter);

            connect(idLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupId(const QString &)));

            newGridLayout->addWidget(idLabel, row, 2);

            // Remove button, if needed

            if (mEditingMode) {
                QPushButton *removeButton = new QPushButton(newGridWidget);
                // Note #1: ideally, we could assign a QAction to our
                //          QPushButton, but this cannot be done, so... we
                //          assign a few properties by hand...
                // Note #2: to use a QToolButton would allow us to assign a
                //          QAction to it, but a QToolButton doesn't look quite
                //          the same as a QPushButton on some platforms, so...

                removeButton->setIcon(QIcon(":/oxygen/actions/list-remove.png"));
                removeButton->setStatusTip(tr("Remove the metadata information"));
                removeButton->setToolTip(tr("Remove"));
                removeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

                mRdfTriplesMapping.insert(removeButton, rdfTriple);

                connect(removeButton, SIGNAL(clicked()),
                        this, SLOT(removeRdfTriple()));

                newGridLayout->addWidget(removeButton, row, 3, Qt::AlignCenter);
            }

            // Keep track of the very first resource id

            if (row == 1)
                firstRdfTripleInformation = rdfTripleInformation;
        }

        // Have all the rows take a minimum of vertical space

        newGridLayout->setRowStretch(++row, 1);

        // Have the remove buttons column take as little horizontal space as
        // possible compared to the other columns

        newGridLayout->setColumnStretch(0, 1);
        newGridLayout->setColumnStretch(1, 1);
        newGridLayout->setColumnStretch(2, 1);
    } else {
        // No RDF triples, so...

        newGridLayout->addWidget(Core::newLabel(newGridWidget,
                                                tr("No data available..."),
                                                1.25, false, false, Qt::AlignCenter),
                                 0, 0);
    }

    // Add our new widget to our stacked widget

    mWidget->addWidget(newGridWidget);

    // Get rid of our old widget and layout (and its contents)

    if (mGridWidget) {
        mWidget->removeWidget(mGridWidget);

        for (int i = 0, iMax = mGridLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mGridLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

        delete mGridWidget;
    }

    // Keep track of our new widget and layout

    mGridWidget = newGridWidget;
    mGridLayout = newGridLayout;

    // Allow ourselves to be updated again

    setUpdatesEnabled(true);

    // Request for something to be looked up, if needed

    if (mLookupInformation) {
        if (pRdfTriples.count()) {
            // Request for the first resource id or an 'old' qualifier, resource
            // or resource id to be looked up

            if (pRdfTripleInformation.isEmpty() && (pType == No))
                // Nothing 'old' to look up, so look up the first resource id

                genericLookup(firstRdfTripleInformation, Id, pRetranslate);
            else
                // Look up an 'old' qualifier, resource or resource id

                genericLookup(pRdfTripleInformation, pType, pRetranslate);
        } else {
            // No RDF triple left, so ask for 'nothing' to be looked up
            // Note: we do this to let people know that there is nothing to look
            //       up and that they can 'clean' whatever they use to show a
            //       lookup to the user...

            genericLookup();
        }
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::genericLookup(const QString &pRdfTripleInformation,
                                                                                 const Type &pType,
                                                                                 const bool &pRetranslate)
{
    // Retrieve the RDF triple information

    QStringList rdfTripleInformationAsStringList = pRdfTripleInformation.split("|");
    QString qualifierAsString = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformationAsStringList[0];
    QString resourceAsString = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformationAsStringList[1];
    QString idAsString = pRdfTripleInformation.isEmpty()?QString():rdfTripleInformationAsStringList[2];

    // Keep track of the RDF triple information and type

    mRdfTripleInformation = pRdfTripleInformation;
    mType = pType;

    // Make the row corresponding to the qualifier, resource or id bold
    // Note: to use mGridLayout->rowCount() to determine the number of rows
    //       isn't an option since no matter whether we remove rows (in
    //       updateGui()), the returned value will be the maximum number of rows
    //       that there has ever been, so...

    int row = 0;

    forever
        if (mGridLayout->itemAtPosition(++row, 0)) {
            // Valid row, so check whether to make it bold (and italic in some
            // cases) or not

            QLabel *qualifierLabel = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 0)->widget());
            QLabel *resourceLabel  = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 1)->widget());
            QLabel *idLabel        = qobject_cast<QLabel *>(mGridLayout->itemAtPosition(row, 2)->widget());

            QFont font = idLabel->font();

            font.setBold(   mLookupInformation
                         && !qualifierLabel->text().compare("<a href=\""+pRdfTripleInformation+"\">"+qualifierAsString+"</a>")
                         && !resourceLabel->text().compare("<a href=\""+pRdfTripleInformation+"\">"+resourceAsString+"</a>")
                         && !idLabel->text().compare("<a href=\""+pRdfTripleInformation+"\">"+idAsString+"</a>"));
            font.setItalic(false);

            QFont italicFont = idLabel->font();

            italicFont.setBold(font.bold());
            italicFont.setItalic(font.bold());

            qualifierLabel->setFont((pType == Qualifier)?italicFont:font);
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

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::disableLookupInformation()
{
    // Disable the looking up of information

    mLookupInformation = false;

    // Update the GUI by pretending to be interested in looking something up

    genericLookup();
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupQualifier(const QString &pRdfTripleInformation,
                                                                                   const bool &pRetranslate)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pRdfTripleInformation, Qualifier, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupResource(const QString &pRdfTripleInformation,
                                                                                  const bool &pRetranslate)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pRdfTripleInformation, Resource, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupId(const QString &pRdfTripleInformation,
                                                                            const bool &pRetranslate)
{
    // Enable the looking up of information

    mLookupInformation = true;

    // Call our generic lookup function

    genericLookup(pRdfTripleInformation, Id, pRetranslate);
}

//==============================================================================

QString CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::rdfTripleInformation(const int &pRow) const
{
    // Return the RDF triple information for the given row

    QString res = QString();

    // Retrieve the item for the first label link from the given row

    QLayoutItem *item = mGridLayout->itemAtPosition(pRow, 0);

    if (!item)
        // No item could be retrieved, so...

        return res;

    // Retrieve the text from the item's widget which is a QLabel

    res = static_cast<QLabel *>(item->widget())->text();

    // Extract the RDF triple information from the text

    res.remove(QRegExp("^[^\"]*\""));
    res.remove(QRegExp("\"[^\"]*$"));

    // We are done with retrieving the RDF triple information for the given row,
    // so...

    return res;
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::removeRdfTriple()
{
    // Retrieve the RDF triple associated with the remove button

    QObject *button = sender();

    CellMLSupport::CellmlFileRdfTriple *rdfTriple = mRdfTriplesMapping.value(button);

    // Remove the RDF triple from the CellML file and from our set of RDF
    // triples this widget uses

    mCellmlFile->rdfTriples()->remove(rdfTriple);
    mRdfTriples.remove(rdfTriple);

    // Retrieve the number of the row we want to delete, as well as the total
    // number of rows
    // Note: should some RDF triples have been removed, then to call
    //       mGridLayout->rowCount() won't give us the correct number of rows,
    //       so...

    int row = -1;
    int rowMax = mGridLayout->rowCount();

    for (int i = 1, iMax = mGridLayout->rowCount(); i < iMax; ++i) {
        QLayoutItem *item = mGridLayout->itemAtPosition(i, 3);

        if (!item) {
            // The row doesn't exist anymore, so...

            rowMax = i;

            break;
        }

        if (item->widget() == button)
            // This is the row we want to remove

            row = i;
    }

    // Make sure that row and rowMax have meaningful values

    Q_ASSERT(row > 0);
    Q_ASSERT(rowMax > row);

    // Determine the 'new' RDF triple information to look up

    if (mRdfTriples.isEmpty()) {
        mRdfTripleInformation = QString();
        mType = No;
    } else if (!rdfTripleInformation(row).compare(mRdfTripleInformation)) {
        // The RDF triple information is related to the row we want to delete,
        // so we need to find some new one

        mRdfTripleInformation = rdfTripleInformation((rowMax-1 > row)?row+1:row-1);
    }

    // Update the GUI to reflect the removal of the RDF triple

    updateGui(mRdfTriples, mRdfTripleInformation, mType);

    // Let people know that some metadata has been removed

    emit metadataUpdated();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
