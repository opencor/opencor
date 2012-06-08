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
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples())
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

    updateGui(mRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
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

        QString firstResourceId = QString();
        int row = 0;

        foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, pRdfTriples) {
            mLayout->addWidget(mParent->newLabel(mWidget,
                                                 (rdfTriple->modelQualifierType() != CellMLSupport::CellmlFileRdfTriple::ModelUnknown)?
                                                     rdfTriple->modelQualifierTypeAsString():
                                                     rdfTriple->bioQualifierTypeAsString(),
                                                 false, 1.0, Qt::AlignCenter),
                               ++row, 0);
            mLayout->addWidget(mParent->newLabel(mWidget, rdfTriple->resource(),
                                                 false, 1.0, Qt::AlignCenter),
                               row, 1);

            QString resourceId = rdfTriple->resource()+"|"+rdfTriple->id();

            QLabel *id = mParent->newLabel(mWidget,
                                           "<a href=\""+resourceId+"\">"+rdfTriple->id()+"</a>",
                                           false, 1.0, Qt::AlignCenter);

            id->setContextMenuPolicy(Qt::NoContextMenu);
            // Note: the above remove the context menu automatically added by Qt
            //       when a label is a link...

            connect(id, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupResourceId(const QString &)));

            mLayout->addWidget(id, row, 2);

            if (row == 1)
                firstResourceId = resourceId;
        }

        // Request for the first resource id to be looked up

        lookupResourceId(firstResourceId);
    }

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupResourceId(const QString &pResourceId) const
{
    // Retrieve the resource and id

    QStringList resourceIdAsStringList = pResourceId.split("|");
    QString resourceAsString = resourceIdAsStringList[0];
    QString idAsString = resourceIdAsStringList[1];

    // Make the row corresponding to the resource id bold
    // Note: to use mLayout->rowCount() to determine the number of rows isn't an
    //       option since no matter whether we remove rows (in updateGui()), the
    //       returned value will be the maximum number of rows that there has
    //       ever been, so...

    int row = 0;

    forever {
        if (mLayout->itemAtPosition(++row, 0)) {
            // Valid row, so check whether to make it bold or not

            QLabel *qualifier = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 0)->widget());
            QLabel *resource  = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 1)->widget());
            QLabel *id        = qobject_cast<QLabel *>(mLayout->itemAtPosition(row, 2)->widget());

            QFont font = id->font();

            font.setBold(   !resource->text().compare(resourceAsString)
                         && !id->text().compare("<a href=\""+pResourceId+"\">"+idAsString+"</a>"));

            qualifier->setFont(font);
            resource->setFont(font);
            id->setFont(font);
        } else {
            // No more rows, so...

            break;
        }
    }

    // Let people know that we want to lookup a resource id

    emit resourceIdLookupRequested(resourceAsString, idAsString);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
