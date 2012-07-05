//==============================================================================
// CellML annotation view metadata BioModels.Net view details widget
//==============================================================================

#include "cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"
#include "cellmlannotationviewwidget.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatabiomodelsdotnetviewdetailswidget.h"

//==============================================================================

#include <QGridLayout>
#include <QLabel>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(CellmlAnnotationViewWidget *pParent,
                                                                                                                           const bool &pEditingMode) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples(mParent->cellmlFile())),
    mRdfTripleInfo(QString()),
    mType(Unknown),
    mEditingMode(pEditingMode),
    mRdfTriplesMapping(QMap<QObject *, CellMLSupport::CellmlFileRdfTriple *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create the widget (and its layout) which will contain our GUI

    mWidget = new QWidget(this);

    QVBoxLayout *widgetLayout = new QVBoxLayout(mWidget);

    widgetLayout->setMargin(0);

    mWidget->setLayout(widgetLayout);

    // Create another (grid) widget (and its layout) which will contain the
    // BioModels.Net information and add it to our main widget together with
    // some stretch
    // Note: the stretch is to ensure that the contents of our grid widget
    //       doesn't take all the vertical space, but only the vertical space
    //       which is actually needed to display its contents...

    QWidget *gridWidget = new QWidget(mWidget);

    mLayout = new QGridLayout(gridWidget);

    gridWidget->setLayout(mLayout);

    widgetLayout->addWidget(gridWidget);
    widgetLayout->addStretch();

    // Add our widget to our scroll area

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

    updateGui(mRdfTriples, mRdfTripleInfo, mType, true);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples,
                                                                             const QString &pRdfTripleInfo,
                                                                             const Type &pType,
                                                                             const bool &pRetranslate)
{
    // Hide ourselves (to avoid any flickering during the update)

    setVisible(false);

    // Keep track of the RDF triples

    mRdfTriples = pRdfTriples;

    // Remove everything from our form layout

    for (int i = 0, iMax = mLayout->count(); i < iMax; ++i) {
        QLayoutItem *item = mLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    // Update the GUI itself, but only if there is at least one RDF triple

    if (pRdfTriples.count()) {
        // Create labels to act as headers

        mLayout->addWidget(mParent->newLabel(mWidget, tr("Qualifier"), true, 1.25,
                                             Qt::AlignCenter),
                           0, 0);
        mLayout->addWidget(mParent->newLabel(mWidget, tr("Resource"), true, 1.25,
                                             Qt::AlignCenter),
                           0, 1);
        mLayout->addWidget(mParent->newLabel(mWidget, tr("Id"), true, 1.25,
                                             Qt::AlignCenter),
                           0, 2);

        // Add the RDF triples information to our layout
        // Note: for the RDF triple's subject, we try to remove the CellML
        //       file's URI base, thus only leaving the equivalent of a CellML
        //       element cmeta:id which will speak more to the user than a
        //       possibly long URI reference...

        QString firstRdfTripleInfo = QString();
        int row = 0;

        foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, pRdfTriples) {
            // Qualifier

            QString qualifierAsString = (rdfTriple->modelQualifier() != CellMLSupport::CellmlFileRdfTriple::ModelUnknown)?
                                            rdfTriple->modelQualifierAsString():
                                            rdfTriple->bioQualifierAsString();
            QString rdfTripleInfo = qualifierAsString+"|"+rdfTriple->resource()+"|"+rdfTriple->id();

            QLabel *qualifierLabel = mParent->newLabelLink(mWidget,
                                                           "<a href=\""+rdfTripleInfo+"\">"+qualifierAsString+"</a>",
                                                           false, 1.0, Qt::AlignCenter);

            connect(qualifierLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupQualifier(const QString &)));

            mLayout->addWidget(qualifierLabel, ++row, 0);

            // Resource

            QLabel *resourceLabel = mParent->newLabelLink(mWidget,
                                                          "<a href=\""+rdfTripleInfo+"\">"+rdfTriple->resource()+"</a>",
                                                          false, 1.0, Qt::AlignCenter);

            connect(resourceLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResource(const QString &)));

            mLayout->addWidget(resourceLabel, row, 1);

            // Id

            QLabel *idLabel = mParent->newLabelLink(mWidget,
                                                    "<a href=\""+rdfTripleInfo+"\">"+rdfTriple->id()+"</a>",
                                                    false, 1.0, Qt::AlignCenter);

            connect(idLabel, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResourceId(const QString &)));

            mLayout->addWidget(idLabel, row, 2);

            // Remove button, if needed

            if (mEditingMode) {
                QToolButton *removeButton = new QToolButton(mWidget);

                removeButton->setDefaultAction(mGui->actionRemoveMetadataInformation);

                mRdfTriplesMapping.insert(removeButton, rdfTriple);

                connect(removeButton, SIGNAL(clicked()),
                        this, SLOT(removeRdfTriple()));
                // Note: we don't rely on mGui->actionRemoveMetadataInformation
                //       and its triggered event since we need sender() to give
                //       us the remove button which was clicked...

                mLayout->addWidget(removeButton, row, 3);
            }

            // Keep track of the very first resource id

            if (row == 1)
                firstRdfTripleInfo = rdfTripleInfo;
        }

        // Request for something to be looked up, be it the first resource id or
        // an 'old' qualifier, resource or resource id

        if (pRdfTripleInfo.isEmpty() && (pType == Unknown))
            // Nothing 'old' to lookup, so lookup the first resource id

            genericLookup(firstRdfTripleInfo, Id, pRetranslate);
        else
            // Lookup an 'old' qualifier, resource or resource id

            genericLookup(pRdfTripleInfo, pType, pRetranslate);
    } else {
        // No RDF triple left, so ask for an 'unknown' to be looked up
        // Note: we do this to let people know that there is nothing to lookup
        //       and that they can 'clean' whatever they use to show a lookup to
        //       the user...

        genericLookup();
    }

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::genericLookup(const QString &pRdfTripleInfo,
                                                                                 const Type &pType,
                                                                                 const bool &pRetranslate)
{
    // Retrieve the RDF triple information

    QStringList rdfTripleInfoAsStringList = pRdfTripleInfo.split("|");
    QString qualifierAsString = pRdfTripleInfo.isEmpty()?QString():rdfTripleInfoAsStringList[0];
    QString resourceAsString = pRdfTripleInfo.isEmpty()?QString():rdfTripleInfoAsStringList[1];
    QString idAsString = pRdfTripleInfo.isEmpty()?QString():rdfTripleInfoAsStringList[2];

    // Keep track of the RDF triple information and type

    mRdfTripleInfo = pRdfTripleInfo;
    mType          = pType;

    // Make the row corresponding to the qualifier, resource or id bold
    // Note: to use mLayout->rowCount() to determine the number of rows isn't an
    //       option since no matter whether we remove rows (in updateGui()), the
    //       returned value will be the maximum number of rows that there has
    //       ever been, so...

    int row = 0;

    forever
        if (mLayout->itemAtPosition(++row, 0)) {
            // Valid row, so check whether to make it bold (and italic in some
            // cases) or not

            QLabel *qualifierLabel = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 0)->widget());
            QLabel *resourceLabel  = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 1)->widget());
            QLabel *idLabel        = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 2)->widget());

            QFont font = idLabel->font();

            font.setBold(   !qualifierLabel->text().compare("<a href=\""+pRdfTripleInfo+"\">"+qualifierAsString+"</a>")
                         && !resourceLabel->text().compare("<a href=\""+pRdfTripleInfo+"\">"+resourceAsString+"</a>")
                         && !idLabel->text().compare("<a href=\""+pRdfTripleInfo+"\">"+idAsString+"</a>"));
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

    // Let people know that we want to lookup something

    switch (pType) {
    case Qualifier:
        emit qualifierLookupRequested(qualifierAsString, pRetranslate);

        break;
    case Resource:
        emit resourceLookupRequested(resourceAsString, pRetranslate);

        break;
    case Id:
        emit resourceIdLookupRequested(resourceAsString, idAsString,
                                       pRetranslate);

        break;
    default:
        // Unknown

        emit unknownLookupRequested();
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupQualifier(const QString &pRdfTripleInfo,
                                                                                   const bool &pRetranslate)
{
    // Call our generic lookup function

    genericLookup(pRdfTripleInfo, Qualifier, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupResource(const QString &pRdfTripleInfo,
                                                                                  const bool &pRetranslate)
{
    // Call our generic lookup function

    genericLookup(pRdfTripleInfo, Resource, pRetranslate);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupResourceId(const QString &pRdfTripleInfo,
                                                                                    const bool &pRetranslate)
{
    // Call our generic lookup function

    genericLookup(pRdfTripleInfo, Id, pRetranslate);
}

//==============================================================================

QString CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::rdfTripleInfo(const int &pRow) const
{
    // Return the RDF triple information for the given row

    QString res = QString();

    // Retrieve the item for the first label link from the given row

    QLayoutItem *item = mLayout->itemAtPosition(pRow, 0);

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

    mParent->cellmlFile()->removeRdfTriple(rdfTriple);
    mRdfTriples.remove(rdfTriple);

    // Retrieve the number of the row we want to delete, as well as the total
    // number of rows
    // Note: should some RDF triples have been removed, then to call
    //       mLayout->rowCount() won't give us the correct number of rows, so...

    int row = -1;
    int rowMax = mLayout->rowCount();

    for (int i = 1, iMax = mLayout->rowCount(); i < iMax; ++i) {
        QLayoutItem *item = mLayout->itemAtPosition(i, 3);

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

    // Determine the 'new' RDF triple information to lookup

    if (mRdfTriples.isEmpty()) {
        mRdfTripleInfo = QString();
        mType = Unknown;
    } else if (!rdfTripleInfo(row).compare(mRdfTripleInfo)) {
        // The RDF triple information is related to the row we want to delete,
        // so we need to find some new one

        mRdfTripleInfo = rdfTripleInfo((rowMax-1 > row)?row+1:row-1);
    }

    // Update the GUI to reflect the removal of the RDF triple

    updateGui(mRdfTriples, mRdfTripleInfo, mType);

    // Let people know that some metadata has been removed

    emit metadataUpdated();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
