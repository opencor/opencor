//==============================================================================
// CellML annotation view details widget
//==============================================================================

#include "cellmlannotationviewdetailswidget.h"

//==============================================================================

#include "ui_cellmlannotationviewdetailswidget.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewDetailsWidget::CellmlAnnotationViewDetailsWidget(QWidget *pParent) :
    Widget(pParent),
    mUi(new Ui::CellmlAnnotationViewDetailsWidget),
    mType(Empty)
{
    // Set up the UI

    mUi->setupUi(this);
}

//==============================================================================

CellmlAnnotationViewDetailsWidget::~CellmlAnnotationViewDetailsWidget()
{
    // Delete the UI

    delete mUi;
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::retranslateUi()
{
    // Retranslate our UI

    mUi->retranslateUi(this);

    // Update the UI (since some labels get reinitialised as a result of the
    // retranslation)

    updateUi(mType, mElement, mMathmlElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::update(const Type &pType,
                                               CellMLSupport::CellmlFileElement *pElement)
{
    Q_ASSERT(   (((pType == Empty) || (pType == Metadata)) && !pElement)
             || ((pType != Empty) && (pType != Metadata) && pElement));

    // Update the UI

    updateUi(pType, pElement, 0);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::update(const Type &pType,
                                               CellMLSupport::CellmlFileMathmlElement *pMathmlElement)
{
    // Update the UI

    updateUi(pType, 0, pMathmlElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateUi(const Type &pType,
                                                 CellMLSupport::CellmlFileElement *pElement,
                                                 CellMLSupport::CellmlFileMathmlElement *pMathmlElement)
{
    // Keep track of the new type and elements

    mType = pType;

    mElement = pElement;
    mMathmlElement = pMathmlElement;

    // Determine which widget should be shown/hidden

    bool showCmetaId = false;
    bool showName = false;
    bool showUri = false;
    bool showRelationshipRef = false;
    bool showComponentRef = false;

    switch (pType) {
    case Model:
        showCmetaId = true;
        showName = true;

        break;
    case Import:
        showCmetaId = true;
        showUri = true;

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

        break;
    case MathmlElement:
        break;
    case Group:
        showCmetaId = true;

        break;
    case RelationshipRef:
        showCmetaId = true;
        showRelationshipRef = true;

        break;
    case ComponentRef:
        showCmetaId = true;
        showComponentRef = true;

        break;
    case Connection:
        showCmetaId = true;

        break;
    case ComponentMapping:
        showCmetaId = true;

        break;
    case VariableMapping:
        showCmetaId = true;

        break;
    case Metadata:
        showCmetaId = true;

        break;
    default:
        // Empty

        break;
    };

    // Show/hide the relevant widgets

    mUi->cmetaIdLabel->setVisible(showCmetaId);
    mUi->cmetaIdValue->setVisible(showCmetaId);

    bool showNameField =    showName || showUri
                         || showRelationshipRef || showComponentRef;

    mUi->nameLabel->setVisible(showNameField);
    mUi->nameValue->setVisible(showNameField);

    // Update the value of the widgets which are shown

    if (showCmetaId)
        mUi->cmetaIdValue->setText(pElement->cmetaId());

    if (showName) {
        mUi->nameLabel->setText(tr("Name:"));
        mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
    } else if (showUri) {
        mUi->nameLabel->setText(tr("URI:"));
        mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->uri());
    } else if (showRelationshipRef) {
        mUi->nameLabel->setText(tr("Relationship reference:"));
        mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileRelationshipRef *>(pElement)->relationship());
    } else if (showComponentRef) {
        mUi->nameLabel->setText(tr("Component reference:"));
        mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileComponentRef *>(pElement)->component());
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
