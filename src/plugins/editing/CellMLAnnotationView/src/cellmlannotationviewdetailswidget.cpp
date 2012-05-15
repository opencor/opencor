//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"
#include "treeview.h"

//==============================================================================

#include "ui_cellmlannotationviewdetailswidget.h"

//==============================================================================

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewDetailsWidget::CellmlAnnotationViewDetailsWidget(QWidget *pParent) :
    QScrollArea(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::CellmlAnnotationViewDetailsWidget),
    mCellmlItems(CellmlItems()),
    mRdfTriples(CellMLSupport::CellmlFileRdfTriples()),
    mCellmlWidget(0),
    mCellmlFormLayout(0),
    mCellmlCmetaIdValue(0),
    mMetadataWidget(0),
    mMetadataFormLayout(0),
    mMetadataTreeView(0),
    mMetadataDataModel(0)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::~CellmlAnnotationViewDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Update the GUI (since some labels get reinitialised as a result of the
    // retranslation)

    updateGui(mCellmlItems, mRdfTriples);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::CellmlItem CellmlAnnotationViewDetailsWidget::cellmlItem(const Type &pType,
                                                                                            CellMLSupport::CellmlFileElement *pElement,
                                                                                            const QString &pName)
{
    // Return a formatted CellmlItem 'object'

    CellmlItem res;

    res.type    = pType;
    res.element = pElement;
    res.name    = pName;

    return res;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlItems &pCellmlItems,
                                                  const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Hide ourselves (since we may potentially update ourselves quite a bit and
    // we want to avoid any flickering)
    // Note #1: one would normally use setUpdatesEnabled(), but it still results
    //          in bad flickering on Mac OS X, so...
    // Note #2: it's surprising that setVisible() doesn't cause any flickering
    //          on any of the platforms we are targetting, but let's not
    //          complain...

    setVisible(false);

    // Keep track of the CellML items and metadata group name

    mCellmlItems = pCellmlItems;
    mRdfTriples  = pRdfTriples;

    // Check whether we are dealing with some metadata or not

    if (pCellmlItems.count()) {
        // We are not dealing with some metadata, so enable the CellML side of
        // the GUI

        if (!widget() || (widget() != mCellmlWidget)) {
            mCellmlWidget = new QWidget(this);
            mCellmlFormLayout = new QFormLayout(mCellmlWidget);

            mCellmlWidget->setLayout(mCellmlFormLayout);

            setWidget(mCellmlWidget);
        }

        // Remove everything from our form layout

        for (int i = 0, iMax = mCellmlFormLayout->count(); i < iMax; ++i) {
            QLayoutItem *item = mCellmlFormLayout->takeAt(0);

            delete item->widget();
            delete item;
        }

        mCellmlCmetaIdValue = 0;

        // Go through the different items which properties we want to add to the
        // GUI

        for (int i = 0, iLast = pCellmlItems.count()-1; i <= iLast; ++i) {
            CellmlItem cellmlItem = pCellmlItems.at(i);

            // Determine which widget should be shown/hidden

            bool showName = false;
            bool showXlinkHref = false;
            bool showUnitReference = false;
            bool showComponentReference = false;
            bool showUnit = false;
            bool showInitialValue = false;
            bool showPublicInterface = false;
            bool showPrivateInterface = false;
            bool showRelationship = false;
            bool showRelationshipNamespace = false;
            bool showComponent = false;
            bool showFirstComponent = false;
            bool showSecondComponent = false;
            bool showFirstVariable = false;
            bool showSecondVariable = false;

            switch (cellmlItem.type) {
            case Model:
            case Unit:
            case UnitElement:
            case Component:
            case Group:
            case Connection:
                showName = true;

                break;
            case Import:
                showXlinkHref = true;

                break;
            case ImportUnit:
                showName = true;
                showUnitReference = true;

                break;
            case ImportComponent:
                showName = true;
                showComponentReference = true;

                break;
            case Variable:
                showName = true;
                showUnit = true;
                showInitialValue = true;
                showPublicInterface = true;
                showPrivateInterface = true;

                break;
            case RelationshipReference:
                showRelationship = true;
                showRelationshipNamespace = true;

                break;
            case ComponentReference:
                showComponent = true;

                break;
            case ComponentMapping:
                showFirstComponent = true;
                showSecondComponent = true;

                break;
            case VariableMapping:
                showFirstVariable = true;
                showSecondVariable = true;

                break;
            };

            // Add whatever we need
            // Note: as long as all of the widgets' parent is our scroll area's
            //       widget, then they will get automatically deleted, so no need to
            //       delete them in ~CellmlAnnotationViewDetailsWidget()...

            // Add a bold centered label as a header to let the user know what type
            // of item we are talking about

            QLabel *header = new QLabel(typeAsString(cellmlItem.type),
                                        mCellmlWidget);

            QFont headerFont = header->font();

            headerFont.setBold(true);

            header->setAlignment(Qt::AlignCenter);
            header->setFont(headerFont);

            mCellmlFormLayout->addRow(header);

            // Show the item's cmeta:id, keeping in mind that we only want to
            // allow the editing of the cmeta:id of the very first item

            QString cmetaId = cellmlItem.element->cmetaId();

            if (i == iLast) {
                // This is our 'main' current item, so we want to allow the user
                // to edit its cmeta:id

                mCellmlCmetaIdValue = new QLineEdit(cmetaId, mCellmlWidget);

                mCellmlFormLayout->addRow(new QLabel(tr("cmeta:id:"), mCellmlWidget),
                                          mCellmlCmetaIdValue);
            } else {
                // Not our 'main' current item, so just display its cmeta:id

                addRowToCellmlFormLayout(tr("cmeta:id:"),
                                         cmetaId.isEmpty()?"/":cmetaId);
            }

            // Show the item's remaining properties

            if (showName) {
                // Retrieve the name of the CellML element
                // Note: in the case of a group or a connection, there won't be a
                //       name, so we use the item's name, hoping one was provided...

                QString name = ((cellmlItem.type == Group) || (cellmlItem.type == Connection))?
                                   cellmlItem.name.isEmpty()?"/":cellmlItem.name:
                                   static_cast<CellMLSupport::CellmlFileNamedElement *>(cellmlItem.element)->name();

                addRowToCellmlFormLayout(tr("Name:"), name);
            }

            if (showXlinkHref)
                addRowToCellmlFormLayout(tr("xlink:href:"),
                                         static_cast<CellMLSupport::CellmlFileImport *>(cellmlItem.element)->xlinkHref());

            if (showUnitReference)
                addRowToCellmlFormLayout(tr("Unit reference:"),
                                         static_cast<CellMLSupport::CellmlFileImportUnit *>(cellmlItem.element)->unitReference());

            if (showComponentReference)
                addRowToCellmlFormLayout(tr("Component reference:"),
                                         static_cast<CellMLSupport::CellmlFileImportComponent *>(cellmlItem.element)->componentReference());

            if (showUnit)
                addRowToCellmlFormLayout(tr("Unit:"),
                                         static_cast<CellMLSupport::CellmlFileVariable *>(cellmlItem.element)->unit());

            if (showInitialValue) {
                QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(cellmlItem.element)->initialValue();

                addRowToCellmlFormLayout(tr("Initial value:"),
                                         initialValue.isEmpty()?"/":initialValue);
            }

            if (showPublicInterface)
                addRowToCellmlFormLayout(tr("Public interface:"),
                                         static_cast<CellMLSupport::CellmlFileVariable *>(cellmlItem.element)->publicInterfaceAsString());

            if (showPrivateInterface)
                addRowToCellmlFormLayout(tr("Private interface:"),
                                         static_cast<CellMLSupport::CellmlFileVariable *>(cellmlItem.element)->privateInterfaceAsString());

            if (showRelationship)
                addRowToCellmlFormLayout(tr("Relationship:"),
                                         static_cast<CellMLSupport::CellmlFileRelationshipReference *>(cellmlItem.element)->relationship());

            if (showRelationshipNamespace) {
                QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(cellmlItem.element)->relationshipNamespace();

                addRowToCellmlFormLayout(tr("Relationship namespace:"),
                                         relationshipNamespace.isEmpty()?"/":relationshipNamespace);
            }

            if (showComponent)
                addRowToCellmlFormLayout(tr("Component:"),
                                         static_cast<CellMLSupport::CellmlFileComponentReference *>(cellmlItem.element)->component());

            if (showFirstComponent)
                addRowToCellmlFormLayout(tr("First component:"),
                                         static_cast<CellMLSupport::CellmlFileMapComponents *>(cellmlItem.element)->firstComponent());

            if (showSecondComponent)
                addRowToCellmlFormLayout(tr("Second component:"),
                                         static_cast<CellMLSupport::CellmlFileMapComponents *>(cellmlItem.element)->secondComponent());

            if (showFirstVariable)
                addRowToCellmlFormLayout(tr("First variable:"),
                                         static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(cellmlItem.element)->firstVariable());

            if (showSecondVariable)
                addRowToCellmlFormLayout(tr("Second variable:"),
                                         static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(cellmlItem.element)->secondVariable());
        }
    } else {
        // We are dealing with some metadata, so enable the metadata ML side of
        // the GUI

        if (!widget() || (widget() != mMetadataWidget)) {
            mMetadataWidget = new QWidget(this);
            mMetadataFormLayout = new QVBoxLayout(mMetadataWidget);

            mMetadataFormLayout->setMargin(0);

            mMetadataWidget->setLayout(mMetadataFormLayout);

            mMetadataTreeView  = new Core::TreeView(mMetadataWidget);
            mMetadataDataModel = new QStandardItemModel(mMetadataTreeView);

            mMetadataTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            mMetadataTreeView->setFrameShape(QFrame::NoFrame);
            mMetadataTreeView->setModel(mMetadataDataModel);
            mMetadataTreeView->setRootIsDecorated(false);
            mMetadataTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

            mMetadataDataModel->setHorizontalHeaderLabels(QStringList() << tr("Subject")
                                                                        << tr("Predicate")
                                                                        << tr("Object"));

            mMetadataFormLayout->addWidget(mMetadataTreeView);

            setWidget(mMetadataWidget);
        }

        // Remove all previous triples from our tree view

        while (mMetadataDataModel->rowCount())
            foreach (QStandardItem *item, mMetadataDataModel->takeRow(0))
                delete item;

        // Add the 'new' triples to our tree view

        foreach (CellMLSupport::CellmlFileRdfTriple *rdfTriple, pRdfTriples)
            mMetadataDataModel->invisibleRootItem()->appendRow(QList<QStandardItem *>() << new QStandardItem(rdfTriple->subject()->asString())
                                                                                        << new QStandardItem(rdfTriple->predicate()->asString())
                                                                                        << new QStandardItem(rdfTriple->object()->asString()));
    }

    // Re-show ourselves

    setVisible(true);

    // Scroll down to the bottom of ourselves, just in case things don't fit
    // within the viewport
    // Note: for this, we need to be up-to-date, hence we make a call to
    //       qApp->processEvents() and this can only be done when we are once
    //       again visible...

    qApp->processEvents();

    verticalScrollBar()->setValue(verticalScrollBar()->maximum());

    // Give the focus to the cmeta:id value field
    // Note: indeed, to have the cmeta:id value field as a focus proxy widget
    //       for CellmlAnnotationViewWidget isn't good enough to have it get the
    //       focus after selecting a 'new' CellML element in
    //       CellmlAnnotationViewWidget (while it's when we switch from one
    //       CellML file to another), so...

    if ((widget() == mCellmlWidget) && mCellmlCmetaIdValue)
        mCellmlCmetaIdValue->setFocus();
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellmlItems &pCellmlItems)
{
    // Call our generic updateGui()

    updateGui(pCellmlItems, CellMLSupport::CellmlFileRdfTriples());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const CellMLSupport::CellmlFileRdfTriples &pRdfTriples)
{
    // Call our generic updateGui()

    updateGui(CellmlItems(), pRdfTriples);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::addRowToCellmlFormLayout(const QString &pLabel,
                                                                 const QString &pValue)
{
    // Add a row to our form layout

    mCellmlFormLayout->addRow(new QLabel(pLabel, mCellmlWidget),
                              new QLabel(pValue, mCellmlWidget));
}

//==============================================================================

QString CellmlAnnotationViewDetailsWidget::typeAsString(const Type &pType) const
{
    switch (pType) {
    case Import:
        return tr("Import");
    case ImportUnit:
        return tr("Imported unit");
    case ImportComponent:
        return tr("Imported component");
    case Unit:
        return tr("Unit");
    case UnitElement:
        return tr("Unit element");
    case Component:
        return tr("Component");
    case Variable:
        return tr("Variable");
    case Group:
        return tr("Group");
    case RelationshipReference:
        return tr("Relationshop reference");
    case ComponentReference:
        return tr("Component reference");
    case Connection:
        return tr("Connection");
    case ComponentMapping:
        return tr("Component mapping");
    case VariableMapping:
        return tr("Variable mapping");
    default:
        // Model

        return tr("Model");
    }
}

//==============================================================================

QWidget * CellmlAnnotationViewDetailsWidget::focusProxyWidget()
{
    // If anything, we want our cmeta:id value widget to be a focus proxy widget

    return (widget() == mCellmlWidget)?mCellmlCmetaIdValue:0;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
