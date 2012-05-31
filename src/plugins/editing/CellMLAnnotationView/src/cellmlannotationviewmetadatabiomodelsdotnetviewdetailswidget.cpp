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

        QString firstMiriamUrn = QString();
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

            QString miriamUrn = "urn:miriam:"+rdfTriple->resource()+":"+rdfTriple->id();
            QLabel *id = mParent->newLabel(mWidget,
                                           "<a href=\""+miriamUrn+"\">"+rdfTriple->id()+"</a>",
                                           false, 1.0, Qt::AlignCenter);

            id->setContextMenuPolicy(Qt::NoContextMenu);
            // Note: the above remove the context menu automatically added by Qt
            //       when a label is a link...

            connect(id, SIGNAL(linkActivated(const QString &)),
                    this, SLOT(lookupMiriamUrn(const QString &)));

            mLayout->addWidget(id, row, 2);

            if (row == 1)
                firstMiriamUrn = miriamUrn;
        }

        // Request for the first MIRIAM URN to be looked up

        lookupMiriamUrn(firstMiriamUrn);
    }

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

void CellmlAnnotationViewMetadataBioModelsDotNetViewDetailsWidget::lookupMiriamUrn(const QString &pMiriamUrn) const
{
    // Make the row corresponding to the MIRIAM URN bold

    for (int i = 1, iMax = mLayout->rowCount(); i < iMax; ++i) {
        QLabel *qualifier = qobject_cast<QLabel *>(mLayout->itemAtPosition(i, 0)->widget());
        QLabel *resource  = qobject_cast<QLabel *>(mLayout->itemAtPosition(i, 1)->widget());
        QLabel *id        = qobject_cast<QLabel *>(mLayout->itemAtPosition(i, 2)->widget());

        QFont font = id->font();

        font.setBold(id->text().contains(pMiriamUrn));

        qualifier->setFont(font);
        resource->setFont(font);
        id->setFont(font);
    }

    // Let people know that we want to lookup a MIRIAM URN

    emit miriamUrnLookupRequested(pMiriamUrn);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
