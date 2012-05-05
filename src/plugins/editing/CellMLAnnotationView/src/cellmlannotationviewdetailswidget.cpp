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

    updateGui(mType, mElement, mMathmlElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::update(const Type &pType,
                                               CellMLSupport::CellmlFileElement *pElement)
{
    Q_ASSERT(   (((pType == Empty) || (pType == Metadata)) && !pElement)
             || ((pType != Empty) && (pType != Metadata) && pElement));

    // Update the GUI

    updateGui(pType, pElement, 0);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::update(const Type &pType,
                                               CellMLSupport::CellmlFileMathmlElement *pMathmlElement)
{
    // Update the GUI

    updateGui(pType, 0, pMathmlElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::updateGui(const Type &pType,
                                                  CellMLSupport::CellmlFileElement *pElement,
                                                  CellMLSupport::CellmlFileMathmlElement *pMathmlElement)
{
    // Keep track of the new type and elements

    bool needUpdatingGui = mType != pType;

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

    // Show/hide the relevant widgets, but only if required

    if (needUpdatingGui) {
        mGui->field01Label->setVisible(showCmetaId);
        mGui->field01Value->setVisible(showCmetaId);

        bool showField02Field =    showName || showUri
                                || showRelationshipRef || showComponentRef;

        mGui->field02Label->setVisible(showField02Field);
        mGui->field02Value->setVisible(showField02Field);
    }

    // Update the value of the widgets which are shown

    if (showCmetaId) {
        mGui->field01Label->setText(tr("cmeta:id:"));
        mGui->field01Value->setText(pElement->cmetaId());
    }

    if (showName) {
        mGui->field02Label->setText(tr("Name:"));
        mGui->field02Value->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
    } else if (showUri) {
        mGui->field02Label->setText(tr("URI:"));
        mGui->field02Value->setText(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->uri());
    } else if (showRelationshipRef) {
        mGui->field02Label->setText(tr("Relationship reference:"));
        mGui->field02Value->setText(static_cast<CellMLSupport::CellmlFileRelationshipRef *>(pElement)->relationship());
    } else if (showComponentRef) {
        mGui->field02Label->setText(tr("Component reference:"));
        mGui->field02Value->setText(static_cast<CellMLSupport::CellmlFileComponentRef *>(pElement)->component());
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
