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

    mDebugOutput->append(QString("    Model name: %1 [%2]").arg(cellmlFile->model()->name(),
                                                                cellmlFile->model()->cmetaId()));

    // Retrieve the model's imports

    if (cellmlFile->imports().isEmpty()) {
        mDebugOutput->append(QString("    No imports"));
    } else {
        mDebugOutput->append(QString("    Imports:"));

        foreach (CellMLSupport::CellmlFileImport *import,
                 cellmlFile->imports()) {
            mDebugOutput->append(QString("        From %1 [%2]:").arg(import->uri(),
                                                                      import->cmetaId()));

            foreach (CellMLSupport::CellmlFileImportUnit *unit,
                     import->units())
                mDebugOutput->append(QString("            Unit: %1 ---> %2 [%3]").arg(unit->name(), unit->referenceName(), unit->cmetaId()));

            foreach (CellMLSupport::CellmlFileImportComponent *component,
                     import->components())
                mDebugOutput->append(QString("            Component: %1 ---> %2 [%3]").arg(component->name(), component->referenceName(), component->cmetaId()));
        }
    }

    // Retrieve the model's unit definitions

    if (cellmlFile->units().isEmpty()) {
        mDebugOutput->append(QString("    No units"));
    } else {
        mDebugOutput->append(QString("    Units:"));

        foreach (CellMLSupport::CellmlFileUnit *unit, cellmlFile->units()) {
            mDebugOutput->append(QString("        %1 [%2]").arg(unit->name(),
                                                                unit->cmetaId()));
            mDebugOutput->append(QString("            Base unit: %1").arg(unit->isBaseUnit()?"yes":"no"));

            if (unit->unitElements().isEmpty()) {
                mDebugOutput->append(QString("            No unit elements"));
            } else {
                mDebugOutput->append(QString("            Unit elements:"));

                foreach (CellMLSupport::CellmlFileUnitElement *unitElement,
                         unit->unitElements()) {
                    mDebugOutput->append(QString("            %1 [%2]").arg(unitElement->name(),
                                                                            unitElement->cmetaId()));
                    mDebugOutput->append(QString("                Prefix: %1").arg(unitElement->prefix()));
                    mDebugOutput->append(QString("                Multiplier: %1").arg(unitElement->multiplier()));
                    mDebugOutput->append(QString("                Offset: %1").arg(unitElement->offset()));
                    mDebugOutput->append(QString("                Exponent: %1").arg(unitElement->exponent()));
                }
            }
        }
    }

    // Retrieve the model's groups

    if (cellmlFile->groups().isEmpty()) {
        mDebugOutput->append(QString("    No groups"));
    } else {
        mDebugOutput->append(QString("    Groups:"));

        foreach (CellMLSupport::CellmlFileGroup *group, cellmlFile->groups()) {
            mDebugOutput->append(QString("        Group [%1]").arg(group->cmetaId()));

            if (group->relationshipRefs().isEmpty()) {
                mDebugOutput->append(QString("            No relationship ref(erence)s"));
            } else {
                mDebugOutput->append(QString("            Relationship ref(erence)s:"));

                foreach (CellMLSupport::CellmlFileRelationshipRef *relationshipRef,
                         group->relationshipRefs()) {
                    mDebugOutput->append(QString("                %1 [%2]").arg(relationshipRef->name(),
                                                                                relationshipRef->cmetaId()));
                    mDebugOutput->append(QString("                    Relationship: %1").arg(relationshipRef->relationship()));
                    mDebugOutput->append(QString("                    Relationship namespace: %1").arg(relationshipRef->relationshipNamespace()));
                }
            }

            if (group->componentRefs().isEmpty()) {
                mDebugOutput->append(QString("            No component ref(erence)s"));
            } else {
                mDebugOutput->append(QString("            Component ref(erence)s:"));

                foreach (CellMLSupport::CellmlFileComponentRef *componentRef,
                         group->componentRefs())
                    initComponentRefTreeView("                ", componentRef);
            }
        }
    }
}

//==============================================================================

void CellmlAnnotationViewWidget::initComponentRefTreeView(const QString &pLeadingSpace,
                                                          CellMLSupport::CellmlFileComponentRef *pComponentRef)
{
    mDebugOutput->append(QString("%1%2 [%3]").arg(pLeadingSpace,
                                                  pComponentRef->name(),
                                                  pComponentRef->cmetaId()));

    foreach (CellMLSupport::CellmlFileComponentRef *componentRef,
             pComponentRef->componentRefs())
        initComponentRefTreeView(pLeadingSpace+"    ", componentRef);
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
