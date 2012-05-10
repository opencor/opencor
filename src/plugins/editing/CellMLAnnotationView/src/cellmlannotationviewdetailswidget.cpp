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

    updateGui(mType, mElement, true);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const Type &pType,
                                                  CellMLSupport::CellmlFileElement *pElement,
                                                  const bool &pNeedRetranslating)
{
    Q_ASSERT(   (((pType == Empty) || (pType == Metadata)) && !pElement)
             || ((pType != Empty) && (pType != Metadata) && pElement));

    // Keep track of the new type and element

    mType = pType;
    mElement = pElement;

    // Determine which widget should be shown/hidden

    bool showCmetaId = (pType != Metadata) && (pType != Empty);
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

    switch (pType) {
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
    // Note: one would normally use setUpdatesEnabled(), but it still results in
    //       bad flickering on Mac OS X, so...

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
    //       CellmlAnnotationViewDetailsWidget's destructor...

    if (showCmetaId)
        mGui->formLayout->addRow(new QLabel(tr("cmeta:id:"), this),
                                 new QLineEdit(pElement->cmetaId(), this));

    if (showName)
        mGui->formLayout->addRow(new QLabel(tr("Name:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name(),
                                            this));

    if (showXlinkHref)
        mGui->formLayout->addRow(new QLabel(tr("xlink:href:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->xlinkHref(),
                                            this));

    if (showUnitReference)
        mGui->formLayout->addRow(new QLabel(tr("Unit reference:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileImportUnit *>(pElement)->unitReference(),
                                            this));

    if (showComponentReference)
        mGui->formLayout->addRow(new QLabel(tr("Component reference:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileImportComponent *>(pElement)->componentReference(),
                                            this));

    if (showUnit)
        mGui->formLayout->addRow(new QLabel(tr("Unit:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->unit(),
                                            this));

    if (showInitialValue) {
        QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->initialValue();

        mGui->formLayout->addRow(new QLabel(tr("Initial value:"), this),
                                 new QLabel(initialValue.isEmpty()?"/":initialValue, this));
    }

    if (showPublicInterface)
        mGui->formLayout->addRow(new QLabel(tr("Public interface:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->publicInterfaceAsString(),
                                            this));

    if (showPrivateInterface)
        mGui->formLayout->addRow(new QLabel(tr("Private interface:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->privateInterfaceAsString(),
                                            this));

    if (showRelationship)
        mGui->formLayout->addRow(new QLabel(tr("Relationship:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationship(),
                                            this));

    if (showRelationshipNamespace) {
        QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationshipNamespace();

        mGui->formLayout->addRow(new QLabel(tr("Relationship namespace:"), this),
                                 new QLabel(relationshipNamespace.isEmpty()?"/":relationshipNamespace,
                                            this));
    }

    if (showComponent)
        mGui->formLayout->addRow(new QLabel(tr("Component:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileComponentReference *>(pElement)->component(),
                                            this));

    if (showFirstComponent)
        mGui->formLayout->addRow(new QLabel(tr("First component:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement)->firstComponent(),
                                            this));

    if (showSecondComponent)
        mGui->formLayout->addRow(new QLabel(tr("Second component:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement)->secondComponent(),
                                            this));

    if (showFirstVariable)
        mGui->formLayout->addRow(new QLabel(tr("First variable:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement)->firstVariable(),
                                            this));

    if (showSecondVariable)
        mGui->formLayout->addRow(new QLabel(tr("Second variable:"), this),
                                 new QLabel(static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement)->secondVariable(),
                                            this));

    // Re-show ourselves

    setVisible(true);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
