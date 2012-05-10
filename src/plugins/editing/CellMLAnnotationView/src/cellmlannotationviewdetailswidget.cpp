//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"

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
    mType(Empty)
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

    updateGui(CellmlAnnotationViewDetailsWidget::row(mType, mElement),
              true);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::Row CellmlAnnotationViewDetailsWidget::row(const Type &pType,
                                                                              CellMLSupport::CellmlFileElement *pElement)
{
    // Return a formatted Row 'object'

    Row res;

    res.type    = pType;
    res.element = pElement;

    return res;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const Row &pRow,
                                                  const bool &pNeedRetranslating)
{
    Q_ASSERT(   (((pRow.type == Empty) || (pRow.type == Metadata)) && !pRow.element)
             || ((pRow.type != Empty) && (pRow.type != Metadata) && pRow.element));

    // Keep track of the new type and element

    mType = pRow.type;
    mElement = pRow.element;

    // Determine which widget should be shown/hidden

    bool showCmetaId = (pRow.type != Metadata) && (pRow.type != Empty);
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

    switch (pRow.type) {
    case Model:
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
    case Unit:
        showName = true;

        break;
    case UnitElement:
        showName = true;

        break;
    case Component:
        showName = true;

        break;
    case Variable:
        showName = true;
        showUnit = true;
        showInitialValue = true;
        showPublicInterface = true;
        showPrivateInterface = true;

        break;
    case Group:
        break;
    case RelationshipReference:
        showRelationship = true;
        showRelationshipNamespace = true;

        break;
    case ComponentReference:
        showComponent = true;

        break;
    case Connection:
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

    // Hide ourselves (since we may potentially update ourselves quite a bit and
    // we want to avoid any flickering)
    // Note #1: one would normally use setUpdatesEnabled(), but it still results
    //          in bad flickering on Mac OS X, so...
    // Note #2: it's surprising that setVisible() doesn't cause any flickering
    //          on any of the platforms we are targetting, but let's not
    //          complain...

    setVisible(false);

    // Remove everything from our form layout

    for (int i = 0, iMax = mGui->formLayout->count(); i < iMax; ++i) {
        QLayoutItem *item = mGui->formLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    // Add whatever we need
    // Note: as long as all of the widgets' parent is ourselves, then they
    //       will get automatically deleted, so no need to delete them in
    //       ~CellmlAnnotationViewDetailsWidget()...

    if (showCmetaId)
        mGui->formLayout->addRow(new QLabel(tr("cmeta:id:"), this),
                                 new QLineEdit(pRow.element->cmetaId(), this));

    if (showName)
        addRowToFormLayout(tr("Name:"),
                           static_cast<CellMLSupport::CellmlFileNamedElement *>(pRow.element)->name());

    if (showXlinkHref)
        addRowToFormLayout(tr("xlink:href:"),
                           static_cast<CellMLSupport::CellmlFileImport *>(pRow.element)->xlinkHref());

    if (showUnitReference)
        addRowToFormLayout(tr("Unit reference:"),
                           static_cast<CellMLSupport::CellmlFileImportUnit *>(pRow.element)->unitReference());

    if (showComponentReference)
        addRowToFormLayout(tr("Component reference:"),
                           static_cast<CellMLSupport::CellmlFileImportComponent *>(pRow.element)->componentReference());

    if (showUnit)
        addRowToFormLayout(tr("Unit:"),
                           static_cast<CellMLSupport::CellmlFileVariable *>(pRow.element)->unit());

    if (showInitialValue) {
        QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(pRow.element)->initialValue();

        addRowToFormLayout(tr("Initial value:"),
                           initialValue.isEmpty()?"/":initialValue);
    }

    if (showPublicInterface)
        addRowToFormLayout(tr("Public interface:"),
                           static_cast<CellMLSupport::CellmlFileVariable *>(pRow.element)->publicInterfaceAsString());

    if (showPrivateInterface)
        addRowToFormLayout(tr("Private interface:"),
                           static_cast<CellMLSupport::CellmlFileVariable *>(pRow.element)->privateInterfaceAsString());

    if (showRelationship)
        addRowToFormLayout(tr("Relationship:"),
                           static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pRow.element)->relationship());

    if (showRelationshipNamespace) {
        QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pRow.element)->relationshipNamespace();

        addRowToFormLayout(tr("Relationship namespace:"),
                           relationshipNamespace.isEmpty()?"/":relationshipNamespace);
    }

    if (showComponent)
        addRowToFormLayout(tr("Component:"),
                           static_cast<CellMLSupport::CellmlFileComponentReference *>(pRow.element)->component());

    if (showFirstComponent)
        addRowToFormLayout(tr("First component:"),
                           static_cast<CellMLSupport::CellmlFileMapComponents *>(pRow.element)->firstComponent());

    if (showSecondComponent)
        addRowToFormLayout(tr("Second component:"),
                           static_cast<CellMLSupport::CellmlFileMapComponents *>(pRow.element)->secondComponent());

    if (showFirstVariable)
        addRowToFormLayout(tr("First variable:"),
                           static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pRow.element)->firstVariable());

    if (showSecondVariable)
        addRowToFormLayout(tr("Second variable:"),
                           static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pRow.element)->secondVariable());

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

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
