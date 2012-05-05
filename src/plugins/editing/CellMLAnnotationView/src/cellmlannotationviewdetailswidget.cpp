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

    bool showName = false;

    switch (pType) {
    case Model:
        showName = true;

        break;
    case Import:
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

        break;
    case MathmlElement:

        break;
    case Group:

        break;
    case RelationshipRef:

        break;
    case ComponentRef:

        break;
    case Connection:

        break;
    case Metadata:

        break;
    default:
        // Empty

        break;
    };

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

void CellmlAnnotationViewDetailsWidget::setModel(CellMLSupport::CellmlFileModel *pModel)
{
    // Get the widget ready for some model

    setType(Model);

    mUi->nameValue->setText(pModel->name());
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setImport()
{
    // Get the widget ready for some import

    setType(Import);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setUnit()
{
    // Get the widget ready for some unit

    setType(Unit);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setUnitElement()
{
    // Get the widget ready for some unit element

    setType(UnitElement);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setComponent()
{
    // Get the widget ready for some component

    setType(Component);
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
