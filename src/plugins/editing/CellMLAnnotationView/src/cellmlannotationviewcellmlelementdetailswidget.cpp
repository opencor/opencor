//==============================================================================
// CellML annotation view CellML element details widget
//==============================================================================

#include "cellmlannotationviewcellmlelementdetailswidget.h"
#include "cellmlannotationviewwidget.h"

//==============================================================================

#include "ui_cellmlannotationviewcellmlelementdetailswidget.h"

//==============================================================================

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget::CellmlAnnotationViewCellmlElementDetailsWidget(CellmlAnnotationViewWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mParent(pParent),
    mGui(new Ui::CellmlAnnotationViewCellmlElementDetailsWidget),
    mItems(Items()),
    mWidget(0),
    mLayout(0),
    mCmetaIdValue(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    mWidget = new QWidget(this);
    mLayout = new QFormLayout(mWidget);

    mWidget->setLayout(mLayout);

    setWidget(mWidget);
}

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget::~CellmlAnnotationViewCellmlElementDetailsWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    updateGui(mItems);
}

//==============================================================================

QWidget * CellmlAnnotationViewCellmlElementDetailsWidget::focusProxyWidget() const
{
    // We want our cmeta:id widget to be a focus proxy widget

    return mCmetaIdValue;
}

//==============================================================================

CellmlAnnotationViewCellmlElementDetailsWidget::Item CellmlAnnotationViewCellmlElementDetailsWidget::item(const Type &pType,
                                                                                                          CellMLSupport::CellmlFileElement *pElement,
                                                                                                          const int &pNumber)
{
    // Return a formatted Item 'object'

    Item res;

    res.type    = pType;
    res.element = pElement;
    res.number  = pNumber;

    return res;
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::updateGui(const Items &pItems)
{
    // Hide ourselves (to avoid any flickering during the updaate)

    setVisible(false);

    // Keep track of the items

    mItems = pItems;

    // Remove everything from our form layout

    for (int i = 0, iMax = mLayout->count(); i < iMax; ++i) {
        QLayoutItem *item = mLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    mCmetaIdValue = 0;

    // Go through the different items which properties we want to add to the GUI

    for (int i = 0, iLast = pItems.count()-1; i <= iLast; ++i) {
        Item item = pItems.at(i);

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

        switch (item.type) {
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
        // Note: as long as all of the widgets' parent is our widget, then they
        //       will get automatically deleted, so no need to delete them in
        //       ~CellmlAnnotationViewCellmlElementDetailsWidget()...

        // Add a bold centered label as a header to let the user know what type
        // of item we are talking about

        QLabel *header = new QLabel(typeAsString(item.type), mWidget);

        QFont headerFont = header->font();

        headerFont.setBold(true);

        header->setAlignment(Qt::AlignCenter);
        header->setFont(headerFont);

        mLayout->addRow(header);

        // Show the item's cmeta:id, keeping in mind that we only want to allow
        // the editing of the cmeta:id of the very first item

        QString cmetaId = item.element->cmetaId();

        if (i == iLast) {
            // This is our 'main' current item, so we want to allow the user to
            // edit its cmeta:id

            mCmetaIdValue = new QLineEdit(cmetaId, mWidget);

            mLayout->addRow(new QLabel(tr("cmeta:id:"), mWidget),
                            mCmetaIdValue);
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

            QString name = ((item.type == Group) || (item.type == Connection))?
                               (item.number == -1)?
                                   "/":
                                   (item.type == Group)?
                                       tr("Group #%1").arg(item.number):
                                       tr("Connection #%1").arg(item.number):
                               static_cast<CellMLSupport::CellmlFileNamedElement *>(item.element)->name();

            addRowToCellmlFormLayout(tr("Name:"), name);
        }

        if (showXlinkHref)
            addRowToCellmlFormLayout(tr("xlink:href:"),
                                     static_cast<CellMLSupport::CellmlFileImport *>(item.element)->xlinkHref());

        if (showUnitReference)
            addRowToCellmlFormLayout(tr("Unit reference:"),
                                     static_cast<CellMLSupport::CellmlFileImportUnit *>(item.element)->unitReference());

        if (showComponentReference)
            addRowToCellmlFormLayout(tr("Component reference:"),
                                     static_cast<CellMLSupport::CellmlFileImportComponent *>(item.element)->componentReference());

        if (showUnit)
            addRowToCellmlFormLayout(tr("Unit:"),
                                     static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->unit());

        if (showInitialValue) {
            QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->initialValue();

            addRowToCellmlFormLayout(tr("Initial value:"),
                                     initialValue.isEmpty()?"/":initialValue);
        }

        if (showPublicInterface)
            addRowToCellmlFormLayout(tr("Public interface:"),
                                     static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->publicInterfaceAsString());

        if (showPrivateInterface)
            addRowToCellmlFormLayout(tr("Private interface:"),
                                     static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->privateInterfaceAsString());

        if (showRelationship)
            addRowToCellmlFormLayout(tr("Relationship:"),
                                     static_cast<CellMLSupport::CellmlFileRelationshipReference *>(item.element)->relationship());

        if (showRelationshipNamespace) {
            QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(item.element)->relationshipNamespace();

            addRowToCellmlFormLayout(tr("Relationship namespace:"),
                                     relationshipNamespace.isEmpty()?"/":relationshipNamespace);
        }

        if (showComponent)
            addRowToCellmlFormLayout(tr("Component:"),
                                     static_cast<CellMLSupport::CellmlFileComponentReference *>(item.element)->component());

        if (showFirstComponent)
            addRowToCellmlFormLayout(tr("First component:"),
                                     static_cast<CellMLSupport::CellmlFileMapComponents *>(item.element)->firstComponent());

        if (showSecondComponent)
            addRowToCellmlFormLayout(tr("Second component:"),
                                     static_cast<CellMLSupport::CellmlFileMapComponents *>(item.element)->secondComponent());

        if (showFirstVariable)
            addRowToCellmlFormLayout(tr("First variable:"),
                                     static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(item.element)->firstVariable());

        if (showSecondVariable)
            addRowToCellmlFormLayout(tr("Second variable:"),
                                     static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(item.element)->secondVariable());
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

    mCmetaIdValue->setFocus();
}

//==============================================================================

void CellmlAnnotationViewCellmlElementDetailsWidget::addRowToCellmlFormLayout(const QString &pLabel,
                                                                              const QString &pValue)
{
    // Add a row to our form layout

    mLayout->addRow(new QLabel(pLabel, mWidget), new QLabel(pValue, mWidget));
}

//==============================================================================

QString CellmlAnnotationViewCellmlElementDetailsWidget::typeAsString(const Type &pType) const
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

QLineEdit * CellmlAnnotationViewCellmlElementDetailsWidget::cmetaIdValue() const
{
    // Return our cmeta:id value widget

    return mCmetaIdValue;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
