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

    if (showCmetaId) {
        QLabel *cmetaIdLabel = new QLabel(tr("cmeta:id:"), this);
        QLineEdit *cmetaIdValue = new QLineEdit(pElement->cmetaId(), this);

        mGui->formLayout->addRow(cmetaIdLabel, cmetaIdValue);
    }

    if (showName) {
        QLabel *nameLabel = new QLabel(tr("Name:"), this);
        QLabel *nameValue = new QLabel(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name(),
                                       this);

        mGui->formLayout->addRow(nameLabel, nameValue);
    }

    if (showXlinkHref) {
        QLabel *xlinkHrefLabel = new QLabel(tr("xlink:href:"), this);
        QLabel *xlinkHrefValue = new QLabel(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->xlinkHref(),
                                            this);

        mGui->formLayout->addRow(xlinkHrefLabel, xlinkHrefValue);
    }

    if (showUnitReference) {
        QLabel *unitReferenceLabel = new QLabel(tr("Unit reference:"), this);
        QLabel *unitReferenceValue = new QLabel(static_cast<CellMLSupport::CellmlFileImportUnit *>(pElement)->unitReference(),
                                                this);

        mGui->formLayout->addRow(unitReferenceLabel, unitReferenceValue);
    }

    if (showComponentReference) {
        QLabel *componentReferenceLabel = new QLabel(tr("Component reference:"), this);
        QLabel *componentReferenceValue = new QLabel(static_cast<CellMLSupport::CellmlFileImportComponent *>(pElement)->componentReference(),
                                                     this);

        mGui->formLayout->addRow(componentReferenceLabel, componentReferenceValue);
    }

    if (showUnit) {
        QLabel *unitLabel = new QLabel(tr("Unit:"), this);
        QLabel *unitValue = new QLabel(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->unit(),
                                       this);

        mGui->formLayout->addRow(unitLabel, unitValue);
    }

    if (showInitialValue) {
        QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->initialValue();

        QLabel *initialValueLabel = new QLabel(tr("Initial value:"), this);
        QLabel *initialValueValue = new QLabel(initialValue.isEmpty()?"/":initialValue, this);

        mGui->formLayout->addRow(initialValueLabel, initialValueValue);
    }

    if (showPublicInterface) {
        QLabel *publicInterfaceLabel = new QLabel(tr("Public interface:"), this);
        QLabel *publicInterfaceValue = new QLabel(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->publicInterfaceAsString(),
                                                  this);

        mGui->formLayout->addRow(publicInterfaceLabel, publicInterfaceValue);
    }

    if (showPrivateInterface) {
        QLabel *privateInterfaceLabel = new QLabel(tr("Private interface:"), this);
        QLabel *privateInterfaceValue = new QLabel(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->privateInterfaceAsString(),
                                                   this);

        mGui->formLayout->addRow(privateInterfaceLabel, privateInterfaceValue);
    }

    if (showRelationship) {
        QLabel *relationshipLabel = new QLabel(tr("Relationship:"), this);
        QLabel *relationshipValue = new QLabel(static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationship(),
                                               this);

        mGui->formLayout->addRow(relationshipLabel, relationshipValue);
    }

    if (showRelationshipNamespace) {
        QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationshipNamespace();

        QLabel *relationshipNamespaceLabel = new QLabel(tr("Relationship namespace:"), this);
        QLabel *relationshipNamespaceValue = new QLabel(relationshipNamespace.isEmpty()?"/":relationshipNamespace,
                                                        this);

        mGui->formLayout->addRow(relationshipNamespaceLabel, relationshipNamespaceValue);
    }

    if (showComponent) {
        QLabel *componentLabel = new QLabel(tr("Component:"), this);
        QLabel *componentValue = new QLabel(static_cast<CellMLSupport::CellmlFileComponentReference *>(pElement)->component(),
                                            this);

        mGui->formLayout->addRow(componentLabel, componentValue);
    }

    if (showFirstComponent) {
        QLabel *firstComponentLabel = new QLabel(tr("First component:"), this);
        QLabel *firstComponentValue = new QLabel(static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement)->firstComponent(),
                                                 this);

        mGui->formLayout->addRow(firstComponentLabel, firstComponentValue);
    }

    if (showSecondComponent) {
        QLabel *secondComponentLabel = new QLabel(tr("Second component:"), this);
        QLabel *secondComponentValue = new QLabel(static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement)->secondComponent(),
                                                  this);

        mGui->formLayout->addRow(secondComponentLabel, secondComponentValue);
    }

    if (showFirstVariable) {
        QLabel *firstVariableLabel = new QLabel(tr("First variable:"), this);
        QLabel *firstVariableValue = new QLabel(static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement)->firstVariable(),
                                                this);

        mGui->formLayout->addRow(firstVariableLabel, firstVariableValue);
    }

    if (showSecondVariable) {
        QLabel *secondVariableLabel = new QLabel(tr("Second variable:"), this);
        QLabel *secondVariableValue = new QLabel(static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement)->secondVariable(),
                                                 this);

        mGui->formLayout->addRow(secondVariableLabel, secondVariableValue);
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
