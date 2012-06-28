//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "cellmlannotationviewcellmldetailswidget.h"
#include "cellmlannotationviewcellmllistwidget.h"
#include "cellmlannotationviewlistswidget.h"
#include "cellmlannotationviewdetailswidget.h"
#include "cellmlannotationviewmetadatadetailswidget.h"
#include "cellmlannotationviewmetadatalistwidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

#include <QLabel>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(QWidget *pParent,
                                                       CellMLAnnotationViewPlugin *pPluginParent,
                                                       const QString &pFileName) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mGui(new Ui::CellmlAnnotationViewWidget),
    mPluginParent(pPluginParent),
    mListsWidget(0),
    mDetailsWidget(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Retrieve and load, in case it's necessary, the requested CellML file

    mCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    mCellmlFile->load();

    // Customise our GUI which consists of two main parts:
    //
    //  1) A couple of lists (for CellML elements and metadata, resp.); and
    //  2) Some details (for a CellML element or metadata).
    //
    // These two main parts are widgets of ourselves and moving the splitter
    // will result in the splitter of other CellML files' view to be moved too

    // Create our two main parts

    mListsWidget   = new CellmlAnnotationViewListsWidget(this);
    mDetailsWidget = new CellmlAnnotationViewDetailsWidget(this);

    // Populate ourselves

    addWidget(mListsWidget);
    addWidget(mDetailsWidget);

    // Keep track of our splitter being moved

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitSplitterMoved()));

    // A connection to let our details widget know when some/all metadata
    // has/have removed

    connect(mListsWidget->metadataList(), SIGNAL(metadataUpdated()),
            mDetailsWidget->cellmlDetails(), SLOT(metadataUpdated()));
    connect(mListsWidget->metadataList(), SIGNAL(metadataUpdated()),
            mDetailsWidget->metadataDetails(), SLOT(metadataUpdated()));

    // Make our lists widget our focus proxy

    setFocusProxy(mListsWidget);

    // Select the first CellML node from our CellML list

    mListsWidget->cellmlList()->treeView()->selectFirstNode();
    mListsWidget->metadataList()->treeView()->selectFirstNode();
}

//==============================================================================

CellmlAnnotationViewWidget::~CellmlAnnotationViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewWidget::retranslateUi()
{
    // Retranslate our lists and details widgets

    mListsWidget->retranslateUi();
    mDetailsWidget->retranslateUi();
}

//==============================================================================

CellMLAnnotationViewPlugin * CellmlAnnotationViewWidget::pluginParent() const
{
    // Return our pointer to the plugin parent

    return mPluginParent;
}

//==============================================================================

CellMLSupport::CellmlFile * CellmlAnnotationViewWidget::cellmlFile() const
{
    // Return our pointer to the CellML file

    return mCellmlFile;
}

//==============================================================================

CellmlAnnotationViewListsWidget * CellmlAnnotationViewWidget::listsWidget() const
{
    // Return our lists widget

    return mListsWidget;
}

//==============================================================================

CellmlAnnotationViewDetailsWidget * CellmlAnnotationViewWidget::detailsWidget() const
{
    // Return our details widget

    return mDetailsWidget;
}

//==============================================================================

void CellmlAnnotationViewWidget::updateSizes(const QList<int> &pSizes)
{
    // The splitter of another CellmlAnnotationViewWidget object has been moved,
    // so update our sizes

    setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewWidget::emitSplitterMoved()
{
    // Let whoever know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

QLabel * CellmlAnnotationViewWidget::newLabel(QWidget *pParent,
                                              const QString &pText,
                                              const bool &pBold,
                                              const double &pFontPercentage,
                                              const Qt::Alignment &pAlignment)
{
    // Create and return a label, allowing to set some of its properties

    QLabel *res = new QLabel(pText, pParent);

    QFont font = res->font();

    font.setBold(pBold);
    font.setPointSize(pFontPercentage*font.pointSize());

    res->setAlignment(pAlignment);
    res->setFont(font);

    return res;
}

//==============================================================================

QLabel * CellmlAnnotationViewWidget::newLabelLink(QWidget *pParent,
                                                  const QString &pText,
                                                  const bool &pBold,
                                                  const double &pFontPercentage,
                                                  const Qt::Alignment &pAlignment)
{
    // Create a label link, allowing to set set some of its properties

    QLabel *res = newLabel(pParent, pText, pBold, pFontPercentage, pAlignment);

    // Prevent Qt from associating a context menu with the label (something
    // which it does automatically when a label is a link...)

    res->setContextMenuPolicy(Qt::NoContextMenu);

    // Return our label link

    return res;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
