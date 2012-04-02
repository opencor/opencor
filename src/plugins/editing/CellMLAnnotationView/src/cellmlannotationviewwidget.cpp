//==============================================================================
// CellML annotation view widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "coreutils.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewwidget.h"

//==============================================================================

#include <QSplitter>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewWidget::CellmlAnnotationViewWidget(QWidget *pParent,
                                                       const QString &pFileName) :
    Widget(pParent),
    mUi(new Ui::CellmlAnnotationViewWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create our horizontal splitter

    mHorizontalSplitter = new QSplitter(Qt::Horizontal, this);

    // Create and customise a tree view which will contain all of the units,
    // components, groups and connections from the CellML file

    mTreeView = new Core::TreeView(this);

    mTreeView->setFrameShape(QFrame::NoFrame);
    mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeView->header()->setVisible(false);

    // Populate our horizontal splitter with the aforementioned tree view, as
    // well as with a dummy (for now) widget
    //---GRY--- THE DUMMY WIDGET WILL EVENTUALLY GET REPLACED BY SOMETHING THAT
    //          WILL ALLOW THE USER TO EDIT METADATA, MAKE USE OF RICORDO, ETC.

    mHorizontalSplitter->addWidget(new Core::BorderedWidget(mTreeView,
                                                            Core::BorderedWidget::Right));
    mHorizontalSplitter->addWidget(new Core::BorderedWidget(new QWidget(this),
                                                            Core::BorderedWidget::Left));

    mUi->verticalLayout->addWidget(mHorizontalSplitter);

    // Keep track of our horizontal splitter being moved

    connect(mHorizontalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(emitHorizontalSplitterMoved()));

    // Create a (temporary) debug output
    //---GRY--- THIS WIDGET IS ONLY FOR US WHILE WE ARE WORKING ON THIS
    //          PLUGIN...

    mDebugOutput = new QTextEdit(this);

    mDebugOutput->setFrameStyle(QFrame::NoFrame);
    mDebugOutput->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    mUi->verticalLayout->addWidget(Core::newRealLineWidget(this));
    mUi->verticalLayout->addWidget(mDebugOutput);

    // Initialise our tree view

    initTreeView(pFileName);
}

//==============================================================================

QList<int> CellmlAnnotationViewWidget::horizontalSplitterSizes() const
{
    // Return our horizontal splitter's sizes

    return mHorizontalSplitter->sizes();
}

//==============================================================================

void CellmlAnnotationViewWidget::initTreeView(const QString &pFileName)
{
    // Initialise our tree view with the units, components, groups and
    // connections from the CellML file

    // Retrieve our CellML file object and load the CellML file

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    cellmlFile->load();

    mDebugOutput->append(QString("File name: %1").arg(pFileName));

    if (cellmlFile->issues().count()) {
        // Something went wrong while trying to load the CellML file, so report
        // it and leave

        mDebugOutput->append(QString("    [Error] %1").arg(cellmlFile->issues().first().formattedMessage()));

        return;
    }

    // Output the name of the CellML model

    mDebugOutput->append(QString("    [Information] Model name: %1").arg(QString::fromStdWString(cellmlFile->model()->name())));
}

//==============================================================================

void CellmlAnnotationViewWidget::updateHorizontalSplitter(const QList<int> &pSizes)
{
    // Update our horizontal splitter after a the horizontal splitter of another
    // CellmlAnnotationViewWidget object has been moved

    mHorizontalSplitter->setSizes(pSizes);
}

//==============================================================================

void CellmlAnnotationViewWidget::emitHorizontalSplitterMoved()
{
    // Let whoever know that our horizontal splitter has been moved

    emit horizontalSplitterMoved(mHorizontalSplitter->sizes());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
