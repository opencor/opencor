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
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setType(const Type &pType)
{
    if (pType == mType)
        return;

    // Show/hide whatever widget is required / not required for the new type

    mType = pType;

    bool showId = false;
    bool showName = false;

    switch (pType) {
    case Model:
        showId = true;
        showName = true;

        break;
    case Import:
        showId = true;

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

        break;
    case ComponentRef:
        showId = true;

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

    mUi->idLabel->setVisible(showId);
    mUi->idValue->setVisible(showId);

    mUi->nameLabel->setVisible(showName);
    mUi->nameValue->setVisible(showName);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setEmpty()
{
    // Empty the widget

    setType(Empty);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setModel(CellMLSupport::CellmlFileElement *pElement)
{
    // Get the widget ready for some model

    setType(Model);

    mUi->idValue->setText(pElement->cmetaId());
    mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setImport(CellMLSupport::CellmlFileElement *pElement)
{
    // Get the widget ready for some import

    setType(Import);

    mUi->idValue->setText(pElement->cmetaId());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setUnit(CellMLSupport::CellmlFileElement *pElement)
{
    // Get the widget ready for some unit

    setType(Unit);

    mUi->idValue->setText(pElement->cmetaId());
    mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setUnitElement(CellMLSupport::CellmlFileElement *pElement)
{
    // Get the widget ready for some unit element

    setType(UnitElement);

    mUi->idValue->setText(pElement->cmetaId());
    mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setComponent(CellMLSupport::CellmlFileElement *pElement)
{
    // Get the widget ready for some component

    setType(Component);

    mUi->idValue->setText(pElement->cmetaId());
    mUi->nameValue->setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setVariable()
{
    // Get the widget ready for some variable

    setType(Variable);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setMathmlElement()
{
    // Get the widget ready for some MathML element

    setType(MathmlElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setGroup()
{
    // Get the widget ready for some group

    setType(Group);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setRelationshipRef()
{
    // Get the widget ready for some relationship ref(erence)

    setType(RelationshipRef);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setComponentRef()
{
    // Get the widget ready for some component ref(erence)

    setType(ComponentRef);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setConnection()
{
    // Get the widget ready for some connection

    setType(Connection);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setMetadata()
{
    // Get the widget ready for some metadata

    setType(Metadata);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
