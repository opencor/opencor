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

    bool needUpdatingGui = (mType != pType) || pNeedRetranslating;

    mType = pType;
    mElement = pElement;

    // Determine which widget should be shown/hidden

    bool showCmetaId = false;
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
        showCmetaId = true;
        showName = true;

        break;
    case Import:
        showCmetaId = true;
        showXlinkHref = true;

        break;
    case ImportUnit:
        showCmetaId = true;
        showName = true;
        showUnitReference = true;

        break;
    case ImportComponent:
        showCmetaId = true;
        showName = true;
        showComponentReference = true;

        break;
    case Unit:
        showCmetaId = true;
        showName = true;

        break;
    case UnitElement:
        showCmetaId = true;
        showName = true;

        break;
    case Component:
        showCmetaId = true;
        showName = true;

        break;
    case Variable:
        showCmetaId = true;
        showName = true;
        showUnit = true;
        showInitialValue = true;
        showPublicInterface = true;
        showPrivateInterface = true;

        break;
    case Group:
        showCmetaId = true;

        break;
    case RelationshipReference:
        showCmetaId = true;
        showRelationship = true;
        showRelationshipNamespace = true;

        break;
    case ComponentReference:
        showCmetaId = true;
        showComponent = true;

        break;
    case Connection:
        showCmetaId = true;

        break;
    case ComponentMapping:
        showCmetaId = true;
        showFirstComponent = true;
        showSecondComponent = true;

        break;
    case VariableMapping:
        showCmetaId = true;
        showFirstVariable = true;
        showSecondVariable = true;

        break;
    case Metadata:
        break;
    default:
        // Empty, so...

        ;
    };

    // Show/hide the relevant widgets, but only if required

    if (needUpdatingGui) {
        // Remove everything from our form layout
        // Note: ideally, we wouldn't have to remove everything and add whatever
        //       widgets we need. Instead, and ideally, we would have all the
        //       rows of widgets that we will ever need and would just show/hide
        //       whichever rows we need / don't need. Now, we can show/hide
        //       indvidual widgets, but not rows and this is where the problem
        //       lies since if we hide all the widgets that make up a row, then
        //       we will still have the layout will still show the space which
        //       exists (and which we want) between two rows, so it will look
        //       odd, hence everytime we are dealing with a new type of element,
        //       we remove everything and add whatever we need...

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
            mCmetaIdLabel = new QLabel(tr("cmeta:id:"), this);
            mCmetaIdValue = new QLineEdit(this);

            mGui->formLayout->addRow(mCmetaIdLabel, mCmetaIdValue);
        } else {
            mCmetaIdLabel = 0;
            mCmetaIdValue = 0;
        }

        if (showName) {
            mNameLabel = new QLabel(tr("Name:"), this);
            mNameValue = new QLabel(this);

            mGui->formLayout->addRow(mNameLabel, mNameValue);
        } else {
            mNameLabel = 0;
            mNameValue = 0;
        }

        if (showXlinkHref) {
            mXlinkHrefLabel = new QLabel(tr("xlink:href:"), this);
            mXlinkHrefValue = new QLabel(this);

            mGui->formLayout->addRow(mXlinkHrefLabel, mXlinkHrefValue);
        } else {
            mXlinkHrefLabel = 0;
            mXlinkHrefValue = 0;
        }

        if (showUnitReference) {
            mUnitReferenceLabel = new QLabel(tr("Unit reference:"), this);
            mUnitReferenceValue = new QLabel(this);

            mGui->formLayout->addRow(mUnitReferenceLabel, mUnitReferenceValue);
        } else {
            mUnitReferenceLabel = 0;
            mUnitReferenceValue = 0;
        }

        if (showComponentReference) {
            mComponentReferenceLabel = new QLabel(tr("Component reference:"), this);
            mComponentReferenceValue = new QLabel(this);

            mGui->formLayout->addRow(mComponentReferenceLabel, mComponentReferenceValue);
        } else {
            mComponentReferenceLabel = 0;
            mComponentReferenceValue = 0;
        }

        if (showUnit) {
            mUnitLabel = new QLabel(tr("Unit:"), this);
            mUnitValue = new QLabel(this);

            mGui->formLayout->addRow(mUnitLabel, mUnitValue);
        } else {
            mUnitLabel = 0;
            mUnitValue = 0;
        }

        if (showInitialValue) {
            mInitialValueLabel = new QLabel(tr("Initial value:"), this);
            mInitialValueValue = new QLabel(this);

            mGui->formLayout->addRow(mInitialValueLabel, mInitialValueValue);
        } else {
            mInitialValueLabel = 0;
            mInitialValueValue = 0;
        }

        if (showPublicInterface) {
            mPublicInterfaceLabel = new QLabel(tr("Public interface:"), this);
            mPublicInterfaceValue = new QLabel(this);

            mGui->formLayout->addRow(mPublicInterfaceLabel, mPublicInterfaceValue);
        } else {
            mPublicInterfaceLabel = 0;
            mPublicInterfaceValue = 0;
        }

        if (showPrivateInterface) {
            mPrivateInterfaceLabel = new QLabel(tr("Private interface:"), this);
            mPrivateInterfaceValue = new QLabel(this);

            mGui->formLayout->addRow(mPrivateInterfaceLabel, mPrivateInterfaceValue);
        } else {
            mPrivateInterfaceLabel = 0;
            mPrivateInterfaceValue = 0;
        }

        if (showRelationship) {
            mRelationshipLabel = new QLabel(tr("Relationship:"), this);
            mRelationshipValue = new QLabel(this);

            mGui->formLayout->addRow(mRelationshipLabel, mRelationshipValue);
        } else {
            mRelationshipLabel = 0;
            mRelationshipValue = 0;
        }

        if (showRelationshipNamespace) {
            mRelationshipNamespaceLabel = new QLabel(tr("Relationship namespace:"), this);
            mRelationshipNamespaceValue = new QLabel(this);

            mGui->formLayout->addRow(mRelationshipNamespaceLabel, mRelationshipNamespaceValue);
        } else {
            mRelationshipNamespaceLabel = 0;
            mRelationshipNamespaceValue = 0;
        }

        if (showComponent) {
            mComponentLabel = new QLabel(tr("Component:"), this);
            mComponentValue = new QLabel(this);

            mGui->formLayout->addRow(mComponentLabel, mComponentValue);
        } else {
            mComponentLabel = 0;
            mComponentValue = 0;
        }

        if (showFirstComponent) {
            mFirstComponentLabel = new QLabel(tr("First component:"), this);
            mFirstComponentValue = new QLabel(this);

            mGui->formLayout->addRow(mFirstComponentLabel, mFirstComponentValue);
        } else {
            mFirstComponentLabel = 0;
            mFirstComponentValue = 0;
        }

        if (showSecondComponent) {
            mSecondComponentLabel = new QLabel(tr("Second component:"), this);
            mSecondComponentValue = new QLabel(this);

            mGui->formLayout->addRow(mSecondComponentLabel, mSecondComponentValue);
        } else {
            mSecondComponentLabel = 0;
            mSecondComponentValue = 0;
        }

        if (showFirstVariable) {
            mFirstVariableLabel = new QLabel(tr("First variable:"), this);
            mFirstVariableValue = new QLabel(this);

            mGui->formLayout->addRow(mFirstVariableLabel, mFirstVariableValue);
        } else {
            mFirstVariableLabel = 0;
            mFirstVariableValue = 0;
        }

        if (showSecondVariable) {
            mSecondVariableLabel = new QLabel(tr("Second variable:"), this);
            mSecondVariableValue = new QLabel(this);

            mGui->formLayout->addRow(mSecondVariableLabel, mSecondVariableValue);
        } else {
            mSecondVariableLabel = 0;
            mSecondVariableValue = 0;
        }
    }

    // Update the value of the widgets which are shown

    if (showCmetaId)
        mCmetaIdValue->setText(pElement->cmetaId());

    if (showName)
        mNameValue->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());

    if (showXlinkHref)
        mXlinkHrefValue->setText(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->xlinkHref());

    if (showUnitReference)
        mUnitReferenceValue->setText(static_cast<CellMLSupport::CellmlFileImportUnit *>(pElement)->unitReference());

    if (showComponentReference)
        mComponentReferenceValue->setText(static_cast<CellMLSupport::CellmlFileImportComponent *>(pElement)->componentReference());

    if (showUnit)
        mUnitValue->setText(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->unit());

    if (showInitialValue) {
        QString initialValue = static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->initialValue();

        mInitialValueValue->setText(initialValue.isEmpty()?"/":initialValue);
    }

    if (showPublicInterface)
        mPublicInterfaceValue->setText(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->publicInterfaceAsString());

    if (showPrivateInterface)
        mPrivateInterfaceValue->setText(static_cast<CellMLSupport::CellmlFileVariable *>(pElement)->privateInterfaceAsString());

    if (showRelationship)
        mRelationshipValue->setText(static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationship());

    if (showRelationshipNamespace) {
        QString relationshipNamespace = static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationshipNamespace();

        mRelationshipNamespaceValue->setText(relationshipNamespace.isEmpty()?"/":relationshipNamespace);
    }

    if (showComponent)
        mComponentValue->setText(static_cast<CellMLSupport::CellmlFileComponentReference *>(pElement)->component());

    if (showFirstComponent)
        mFirstComponentValue->setText(static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement)->firstComponent());

    if (showSecondComponent)
        mSecondComponentValue->setText(static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement)->secondComponent());

    if (showFirstVariable)
        mFirstVariableValue->setText(static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement)->firstVariable());

    if (showSecondVariable)
        mSecondVariableValue->setText(static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement)->secondVariable());
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
