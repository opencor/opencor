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

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples(mParent->cellmlFile())),
    mRdfTripleInfo(QString()),
    mType(Unknown)
{
    // Set up the GUI

    mGui->setupUi(this);

    mWidget = new QWidget(this);

    QVBoxLayout *widgetLayout = new QVBoxLayout(mWidget);

    widgetLayout->setMargin(0);

    mWidget->setLayout(widgetLayout);

    QWidget *gridWidget = new QWidget(mWidget);

    mLayout = new QGridLayout(gridWidget);

    gridWidget->setLayout(mLayout);

    widgetLayout->addWidget(gridWidget);
    widgetLayout->addStretch();

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
    // Hide ourselves (to avoid any flickering during the updaate)

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

            QLabel *qualifier = mParent->newLabelLink(mWidget,
                                                      "<a href=\""+rdfTripleInfo+"\">"+qualifierAsString+"</a>",
                                                      false, 1.0, Qt::AlignCenter);

            connect(qualifier, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupQualifier(const QString &)));

            mLayout->addWidget(qualifier, ++row, 0);

            // Resource

            QLabel *resource = mParent->newLabelLink(mWidget,
                                                     "<a href=\""+rdfTripleInfo+"\">"+rdfTriple->resource()+"</a>",
                                                     false, 1.0, Qt::AlignCenter);

            connect(resource, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResource(const QString &)));

            mLayout->addWidget(resource, row, 1);

            // Id

            QLabel *id = mParent->newLabelLink(mWidget,
                                               "<a href=\""+rdfTripleInfo+"\">"+rdfTriple->id()+"</a>",
                                               false, 1.0, Qt::AlignCenter);

            connect(id, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResourceId(const QString &)));

            mLayout->addWidget(id, row, 2);

            // Keep track of the very first resource id

            if (row == 1)
                firstRdfTripleInfo = rdfTripleInfo;
        }

        // Request for something to be looked up, be it the first resource id or
        // an 'old' qualifier, resource or resource id

        if (pRdfTripleInfo.isEmpty() && (pType == Unknown))
            // Nothing 'old' to look up, so look up the first resource id

            lookupResourceId(firstRdfTripleInfo);
        else
            // Look up an 'old' qualifier, resource or resource id

            switch (pType) {
            case Qualifier:
                lookupQualifier(pRdfTripleInfo, pRetranslate);

                break;
            case Resource:
                lookupResource(pRdfTripleInfo, pRetranslate);

                break;
            case Id:
                lookupResourceId(pRdfTripleInfo, pRetranslate);

                break;
            default:
                // Unknown, so nothing to do...
                // Note: we can't reach this point, but it's to 'please' some
                //       compilers, so...

                ;
            }
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
    QString qualifierAsString = rdfTripleInfoAsStringList[0];
    QString resourceAsString = rdfTripleInfoAsStringList[1];
    QString idAsString = rdfTripleInfoAsStringList[2];

    // Keep track of the RDF triple information and type

    mRdfTripleInfo = pRdfTripleInfo;
    mType          = pType;

    // Make the row corresponding to the qualifier, resource or id bold
    // Note: to use mLayout->rowCount() to determine the number of rows isn't an
    //       option since no matter whether we remove rows (in updateGui()), the
    //       returned value will be the maximum number of rows that there has
    //       ever been, so...

    int row = 0;

    forever {
        if (mLayout->itemAtPosition(++row, 0)) {
            // Valid row, so check whether to make it bold (and italic in some
            // cases) or not

            QLabel *qualifier = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 0)->widget());
            QLabel *resource  = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 1)->widget());
            QLabel *id        = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 2)->widget());

            QFont font = id->font();

            font.setBold(   !qualifier->text().compare("<a href=\""+pRdfTripleInfo+"\">"+qualifierAsString+"</a>")
                         && !resource->text().compare("<a href=\""+pRdfTripleInfo+"\">"+resourceAsString+"</a>")
                         && !id->text().compare("<a href=\""+pRdfTripleInfo+"\">"+idAsString+"</a>"));
            font.setItalic(false);

            QFont italicFont = id->font();

            italicFont.setBold(font.bold());
            italicFont.setItalic(font.bold());

            qualifier->setFont((pType == Qualifier)?italicFont:font);
            resource->setFont((pType == Resource)?italicFont:font);
            id->setFont((pType == Id)?italicFont:font);
        } else {
            // No more rows, so...

            break;
        }
    }

    // Let people know that we want to lookup a resource id

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
        // Unknown, so nothing to do...
        // Note: we should never reach this point, but it's to 'please' some
        //       compilers, so...

        ;
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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
