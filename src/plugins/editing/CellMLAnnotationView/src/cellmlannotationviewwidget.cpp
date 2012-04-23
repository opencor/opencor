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

    mUi->verticalLayout->addWidget(Core::newLineWidget(this));
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

    mDebugOutput->append(QString("    [Information] Model name: %1 [%2]").arg(cellmlFile->model()->name(),
                                                                              cellmlFile->model()->cmetaId()));

    // Retrieve the model's imports

    if (cellmlFile->imports().isEmpty()) {
        mDebugOutput->append(QString("    [Information] No imports"));
    } else {
        mDebugOutput->append(QString("    [Information] Imports:"));

        foreach (CellMLSupport::CellmlFileImport *cellmlFileImport,
                 cellmlFile->imports()) {
            mDebugOutput->append(QString("        From %1 [%2]:").arg(cellmlFileImport->uri(),
                                                                 cellmlFileImport->cmetaId()));

            foreach (CellMLSupport::CellmlFileImportUnit *unit,
                     cellmlFileImport->units())
                mDebugOutput->append(QString("            Unit: %1 ---> %2 [%3]").arg(unit->name(), unit->referenceName(), unit->cmetaId()));

            foreach (CellMLSupport::CellmlFileImportComponent *component,
                     cellmlFileImport->components())
                mDebugOutput->append(QString("            Component: %1 ---> %2 [%3]").arg(component->name(), component->referenceName(), component->cmetaId()));
        }
    }

    // Retrieve the model's unit definitions

    if (cellmlFile->units().isEmpty()) {
        mDebugOutput->append(QString("    [Information] No units"));
    } else {
        mDebugOutput->append(QString("    [Information] Units:"));

        foreach (CellMLSupport::CellmlFileUnit *cellmlFileUnit,
                 cellmlFile->units()) {
            mDebugOutput->append(QString("        %1 [%2]").arg(cellmlFileUnit->name(),
                                                                cellmlFileUnit->cmetaId()));
            mDebugOutput->append(QString("            Base unit: %1").arg(cellmlFileUnit->isBaseUnit()?"yes":"no"));

            foreach (CellMLSupport::CellmlFileUnitElement *cellmlFileUnitElement,
                     cellmlFileUnit->unitElements()) {
                mDebugOutput->append(QString("            %1 [%2]").arg(cellmlFileUnitElement->name(),
                                                                        cellmlFileUnitElement->cmetaId()));
                mDebugOutput->append(QString("                Prefix: %1").arg(cellmlFileUnitElement->prefix()));
                mDebugOutput->append(QString("                Multiplier: %1").arg(cellmlFileUnitElement->multiplier()));
                mDebugOutput->append(QString("                Offset: %1").arg(cellmlFileUnitElement->offset()));
                mDebugOutput->append(QString("                Exponent: %1").arg(cellmlFileUnitElement->exponent()));
            }
        }
    }
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
