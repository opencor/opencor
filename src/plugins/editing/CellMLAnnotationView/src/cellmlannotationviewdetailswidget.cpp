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

void CellmlAnnotationViewDetailsWidget::setType(const Type &pType)
{
    if (pType == mType)
        return;

    // Show/hide whatever widget is required / not required for the new type

//---GRY--- TO BE DONE...

    mType = pType;

    QColor color;

    switch (pType) {
    case Model:
        color = Qt::red;

        break;
    case Import:
        color = Qt::green;

        break;
    case Unit:
        color = Qt::blue;

        break;
    case UnitElement:
        color = Qt::cyan;

        break;
    case Component:
        color = Qt::magenta;

        break;
    case Variable:
        color = Qt::yellow;

        break;
    case MathmlElement:
        color = Qt::darkRed;

        break;
    case Group:
        color = Qt::darkGreen;

        break;
    case RelationshipRef:
        color = Qt::darkBlue;

        break;
    case ComponentRef:
        color = Qt::darkCyan;

        break;
    case Connection:
        color = Qt::darkMagenta;

        break;
    case Metadata:
        color = Qt::darkYellow;

        break;
    default:
        // Empty

        color = Qt::white;

        break;
    };

    QPalette pal(palette());

    pal.setColor(QPalette::Background, color);

    setAutoFillBackground(true);
    setPalette(pal);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setEmpty()
{
    // Empty the widget

    setType(Empty);
}

//==============================================================================

void CellmlAnnotationViewDetailsWidget::setModel()
{
    // Get the widget ready for some model

    setType(Model);
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
