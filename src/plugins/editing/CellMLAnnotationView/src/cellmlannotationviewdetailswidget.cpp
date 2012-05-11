//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"
#include "coreutils.h"

//==============================================================================

#include "ui_cellmlannotationviewdetailswidget.h"

//==============================================================================

#include <QLabel>
#include <QLineEdit>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewDetailsWidget::CellmlAnnotationViewDetailsWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::CellmlAnnotationViewDetailsWidget),
    mItems(Items()),
    mCmetaIdValue(0)
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

    updateGui(mItems);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::Item CellmlAnnotationViewDetailsWidget::item(const Type &pType,
                                                                                CellMLSupport::CellmlFileElement *pElement,
                                                                                const QString &pName)
{
    // Return a formatted Item 'object'

    Item res;

    res.type    = pType;
    res.element = pElement;
    res.name    = pName;

    return res;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const Items &pItems)
{
    // Hide ourselves (since we may potentially update ourselves quite a bit and
    // we want to avoid any flickering)
    // Note #1: one would normally use setUpdatesEnabled(), but it still results
    //          in bad flickering on Mac OS X, so...
    // Note #2: it's surprising that setVisible() doesn't cause any flickering
    //          on any of the platforms we are targetting, but let's not
    //          complain...

    setVisible(false);

    // Keep track of the items

    mItems = pItems;

    // Remove everything from our form layout

    for (int i = 0, iMax = mGui->formLayout->count(); i < iMax; ++i) {
        QLayoutItem *item = mGui->formLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    mCmetaIdValue = 0;

    // Go through the different items which properties we want to add to the GUI

    for (int i = 0, iLast = pItems.count()-1; i <= iLast; ++i) {
        Item item = pItems.at(i);

        Q_ASSERT(   (((item.type == Empty) || (item.type == Metadata)) && !item.element)
                 || ((item.type != Empty) && (item.type != Metadata) && item.element));

        // Determine which widget should be shown/hidden

        bool showCmetaId = (item.type != Metadata) && (item.type != Empty);
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
        case Metadata:
            break;
        default:
            // Empty, so...

            ;
        };

        // Add whatever we need
        // Note: as long as all of the widgets' parent is ourselves, then they
        //       will get automatically deleted, so no need to delete them in
        //       ~CellmlAnnotationViewDetailsWidget()...

        // Add a bold centered label as a header to let the user know what type
        // of item we are talking about

        QLabel *header = new QLabel(typeAsString(item.type), this);

        QFont headerFont = header->font();

        headerFont.setBold(true);

        header->setAlignment(Qt::AlignCenter);
        header->setFont(headerFont);

        mGui->formLayout->addRow(header);

        // Show the item's properties

        if (showCmetaId) {
            // We only want to allow the editing of the very first item

            QString cmetaId = item.element->cmetaId();

            if (i == iLast) {
                // This is our 'main' current item, so we want to allow the user
                // to edit its cmeta:id

                mCmetaIdValue = new QLineEdit(cmetaId, this);

                mCmetaIdValue->setFocus();

                mGui->formLayout->addRow(new QLabel(tr("cmeta:id:"), this),
                                         mCmetaIdValue);
            } else {
                // Not our 'main' current item, so just display its cmeta:id

                addRowToFormLayout(tr("cmeta:id:"),
                                   cmetaId.isEmpty()?"/":cmetaId);
            }
        }

        if (showName) {
            // Retrieve the name of the CellML element
            // Note: in the case of a group or a connection, there won't be a
            //       name, so we use the item's name, hoping one was provided...

            QString name = ((item.type == Group) || (item.type == Connection))?
                               item.name.isEmpty()?"/":item.name:
                               static_cast<CellMLSupport::CellmlFileNamedElement *>(item.element)->name();

            addRowToFormLayout(tr("Name:"), name);
        }

        if (showXlinkHref)
            addRowToFormLayout(tr("xlink:href:"),
                               static_cast<CellMLSupport::CellmlFileImport *>(item.element)->xlinkHref());

        if (showUnitReference)
            addRowToFormLayout(tr("Unit reference:"),
                               static_cast<CellMLSupport::CellmlFileImportUnit *>(item.element)->unitReference());

        if (showComponentReference)
            addRowToFormLayout(tr("Component reference:"),
                               static_cast<CellMLSupport::CellmlFileImportComponent *>(item.element)->componentReference());

        if (showUnit)
            addRowToFormLayout(tr("Unit:"),
                               static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->unit());

        if (showInitialValue) {
            QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->initialValue();

            addRowToFormLayout(tr("Initial value:"),
                               initialValue.isEmpty()?"/":initialValue);
        }

        if (showPublicInterface)
            addRowToFormLayout(tr("Public interface:"),
                               static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->publicInterfaceAsString());

        if (showPrivateInterface)
            addRowToFormLayout(tr("Private interface:"),
                               static_cast<CellMLSupport::CellmlFileVariable *>(item.element)->privateInterfaceAsString());

        if (showRelationship)
            addRowToFormLayout(tr("Relationship:"),
                               static_cast<CellMLSupport::CellmlFileRelationshipReference *>(item.element)->relationship());

        if (showRelationshipNamespace) {
            QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(item.element)->relationshipNamespace();

            addRowToFormLayout(tr("Relationship namespace:"),
                               relationshipNamespace.isEmpty()?"/":relationshipNamespace);
        }

        if (showComponent)
            addRowToFormLayout(tr("Component:"),
                               static_cast<CellMLSupport::CellmlFileComponentReference *>(item.element)->component());

        if (showFirstComponent)
            addRowToFormLayout(tr("First component:"),
                               static_cast<CellMLSupport::CellmlFileMapComponents *>(item.element)->firstComponent());

        if (showSecondComponent)
            addRowToFormLayout(tr("Second component:"),
                               static_cast<CellMLSupport::CellmlFileMapComponents *>(item.element)->secondComponent());

        if (showFirstVariable)
            addRowToFormLayout(tr("First variable:"),
                               static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(item.element)->firstVariable());

        if (showSecondVariable)
            addRowToFormLayout(tr("Second variable:"),
                               static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(item.element)->secondVariable());
    }

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::addRowToFormLayout(const QString &pLabel,
                                                           const QString &pValue)
{
    // Add a row to our form layout

    mGui->formLayout->addRow(new QLabel(pLabel, this),
                             new QLabel(pValue, this));
}

//==============================================================================

QString CellmlAnnotationViewDetailsWidget::typeAsString(const Type &pType) const
{
    switch (pType) {
    case Model:
        return tr("Model");
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
    case Metadata:
        return tr("Metadata");
    default:
        // Empty

        return QString();
    }
}

//==============================================================================

QWidget * CellmlAnnotationViewDetailsWidget::focusProxyWidget()
{
    // If anything, we want our cmeta:id value widget to be a focus proxy
    // widget, so...

    return mCmetaIdValue;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
