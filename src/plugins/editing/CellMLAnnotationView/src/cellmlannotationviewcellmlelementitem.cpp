//==============================================================================
// CellML annotation view CellML element item
//==============================================================================

#include "cellmlannotationviewcellmlelementitem.h"
#include "cellmlfile.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const bool &pError, const QString &pText) :
    QStandardItem(pText),
    mCategory(false),
    mType(pError?Error:Warning),
    mNumber(-1),
    mElement(0)
{
    // Constructor for either an error or a warning

    // Disable the item and use its text as a tooltip (in case it's too long and
    // doesn't fit within the allocated space we have)
    // Note: the item will get 're-enabled' by our item delegate...

    setEnabled(false);
    setToolTip(pText);

    // Set the icon for the item

    setIcon(mType);
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const Type &pType,
                                                                             const QString &pText) :
    QStandardItem(pText),
    mCategory(true),
    mType(pType),
    mNumber(-1),
    mElement(0)
{
    // Constructor for a category

    // Disable the item
    // Note: it will get 're-enabled' by our item delegate...

    setEnabled(false);

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

CellmlAnnotationViewCellmlElementItem::CellmlAnnotationViewCellmlElementItem(const Type &pType,
                                                                             CellMLSupport::CellmlFileElement *pElement,
                                                                             const int pNumber) :
    QStandardItem(),
    mCategory(false),
    mType(pType),
    mNumber(pNumber),
    mElement(pElement)
{
    static const QChar rightArrow = QChar(0x2192);

    // Set the text for some types

    switch (pType) {
    case Import:
        setText(static_cast<CellMLSupport::CellmlFileImport *>(pElement)->xlinkHref());

        break;
    case RelationshipReference:
        setText(static_cast<CellMLSupport::CellmlFileRelationshipReference *>(pElement)->relationship());

        break;
    case Group:
        setText(QObject::tr("Group #%1").arg(QString::number(pNumber)));

        break;
    case ComponentReference:
        setText(static_cast<CellMLSupport::CellmlFileComponentReference *>(pElement)->component());

        break;
    case Connection:
        setText(QObject::tr("Connection #%1").arg(QString::number(pNumber)));

        break;
    case ComponentMapping: {
        CellMLSupport::CellmlFileMapComponents *mapComponents = static_cast<CellMLSupport::CellmlFileMapComponents *>(pElement);

        setText(QString("%1 %2 %3").arg(mapComponents->firstComponent(),
                                        rightArrow,
                                        mapComponents->secondComponent()));

        break;
    }
    case VariableMapping: {
        CellMLSupport::CellmlFileMapVariablesItem *mapVariables = static_cast<CellMLSupport::CellmlFileMapVariablesItem *>(pElement);

        setText(QString("%1 %2 %3").arg(mapVariables->firstVariable(),
                                        rightArrow,
                                        mapVariables->secondVariable()));

        break;
    }
    default:
        // Another type of element which has a name

        setText(static_cast<CellMLSupport::CellmlFileNamedElement *>(pElement)->name());
    }

    // Set the icon for the item

    setIcon(pType);
}

//==============================================================================

void CellmlAnnotationViewCellmlElementItem::setIcon(const Type &pType)
{
    // Determine the icon to be used for the item

    switch (pType) {
    case Error:
        QStandardItem::setIcon(QIcon(":CellMLSupport_errorNode"));

        break;
    case Warning:
        QStandardItem::setIcon(QIcon(":CellMLSupport_warningNode"));

        break;
    case Model:
        QStandardItem::setIcon(QIcon(":CellMLSupport_modelNode"));

        break;
    case Import:
        QStandardItem::setIcon(QIcon(":CellMLSupport_importNode"));

        break;
    case ImportUnit:
    case Unit:
        QStandardItem::setIcon(QIcon(":CellMLSupport_unitNode"));

        break;
    case UnitElement:
        QStandardItem::setIcon(QIcon(":CellMLSupport_unitElementNode"));

        break;
    case ImportComponent:
    case Component:
    case ComponentMapping:
        QStandardItem::setIcon(QIcon(":CellMLSupport_componentNode"));

        break;
    case Variable:
    case VariableMapping:
        QStandardItem::setIcon(QIcon(":CellMLSupport_variableNode"));

        break;
    case Group:
        QStandardItem::setIcon(QIcon(":CellMLSupport_groupNode"));

        break;
    case RelationshipReference:
        QStandardItem::setIcon(QIcon(":CellMLSupport_relationshipReferenceNode"));

        break;
    case ComponentReference:
        QStandardItem::setIcon(QIcon(":CellMLSupport_componentReferenceNode"));

        break;
    case Connection:
        QStandardItem::setIcon(QIcon(":CellMLSupport_connectionNode"));

        break;
    default:
        // Type which doesn't require an icon, so...

        ;
    }
}

//==============================================================================

bool CellmlAnnotationViewCellmlElementItem::isCategory() const
{
    // Return whether the CellML element item is a category

    return mCategory;
}

//==============================================================================

int CellmlAnnotationViewCellmlElementItem::type() const
{
    // Return the CellML element item's type

    return mType;
}

//==============================================================================

int CellmlAnnotationViewCellmlElementItem::number() const
{
    // Return the CellML element item's number

    return mNumber;
}

//==============================================================================

CellMLSupport::CellmlFileElement * CellmlAnnotationViewCellmlElementItem::element() const
{
    // Return the CellML element item's element

    return mElement;
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
