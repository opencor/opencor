//==============================================================================
// CellML annotation view metadata details widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadataviewdetailswidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewmetadatadetailswidget.h"

//==============================================================================

#include <QLabel>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::CellmlAnnotationViewMetadataDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    Widget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewMetadataDetailsWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our unsupported metadata message widget

    mUnsupportedMetadataMsg = new QLabel(pParent);

    QFont unsupportedMetadataMsgFont = mUnsupportedMetadataMsg->font();

    unsupportedMetadataMsgFont.setPointSize(1.5*unsupportedMetadataMsgFont.pointSize());

    mUnsupportedMetadataMsg->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mUnsupportedMetadataMsg->setFont(unsupportedMetadataMsgFont);
    mUnsupportedMetadataMsg->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Expanding);
    mUnsupportedMetadataMsg->setWordWrap(true);

    mBorderedUnsupportedMetadataMsg = new Core::BorderedWidget(mUnsupportedMetadataMsg,
                                                               false, true, true, false);

    mBorderedUnsupportedMetadataMsg->setVisible(false);
    // Note: we don't initially want to see it, so...

    // Create our details widget

    mMetadataViewDetails    = new CellmlAnnotationViewMetadataViewDetailsWidget(pParent);

    // Add our widgets to our layout

    mGui->layout->addWidget(mBorderedUnsupportedMetadataMsg);
    mGui->layout->addWidget(new Core::BorderedWidget(mMetadataViewDetails,
                                                     false, true, false, false));

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

CellmlAnnotationViewMetadataDetailsWidget::~CellmlAnnotationViewMetadataDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    mMetadataViewDetails->retranslateUi();

    // Update our unsupported metadata message

    mUnsupportedMetadataMsg->setText(tr("Sorry, but the <strong>%1</strong> view does not support this type of metadata...").arg(mParent->pluginParent()->viewName()));
}

//==============================================================================

void CellmlAnnotationViewMetadataDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Show/hide our unsupported metadata message depending on whether the type
    // of the RDF triples is known or not

    mBorderedUnsupportedMetadataMsg->setVisible(pRdfTriples.type() == CellMLSupport::CellmlFileRdfTriple::Unknown);

    // Update our Metadata view details GUI

    mMetadataViewDetails->updateGui(pRdfTriples);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
