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

    update(mType, mElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::update(const Type &pType,
                                               CellMLSupport::CellmlFileElement *pElement)
{
    Q_ASSERT(   (((pType == Empty) || (pType == Metadata)) && !pElement)
             || ((pType != Empty) && (pType != Metadata) && pElement));

    // Keep track of the new type and element

    mType = pType;
    mElement = pElement;

    // Determine which widget should be shown/hidden

    bool showId = false;
    bool showName = false;
    bool showUri = false;
    bool showRelationshipRef = false;
    bool showComponentRef = false;

    switch (pType) {
    case Model:
        showId = true;
        showName = true;

        break;
    case Import:
        showId = true;
        showUri = true;

        break;
    case Unit:
        showId = true;
        showName = true;

        break;
    case UnitElement:
        showId = true;
        showName = true;

        break;
    case Component:
        showId = true;
        showName = true;

        break;
    case Variable:
        showId = true;
        showName = true;

        break;
    case MathmlElement:
        showId = true;

        break;
    case Group:
        showId = true;

        break;
    case RelationshipRef:
        showId = true;
        showRelationshipRef = true;

        break;
    case ComponentRef:
        showId = true;
        showComponentRef = true;

        break;
    case Connection:
        showId = true;

        break;
    case Metadata:
        showId = true;

        break;
    default:
        // Empty

        break;
    };

    // Show/hide the relevant widgets

    mUi->idLabel->setVisible(showId);
    mUi->idValue->setVisible(showId);

    bool showNameField =    showName || showUri
                         || showRelationshipRef || showComponentRef;

    mUi->nameLabel->setVisible(showNameField);
    mUi->nameValue->setVisible(showNameField);

    // Update the value of the widgets which are shown

    if (showId)
        mUi->idValue->setText(pElement->cmetaId());

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
